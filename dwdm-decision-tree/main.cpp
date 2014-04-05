#include "parse_arff.cpp"

bool operator == (const Field& a, const Field& b) {
	if ((a.type & (BAD|MISSING)) || (b.type & (BAD|MISSING)))
		return false;
	else if (a.type == b.type)
		switch (a.type) {
			case NOMINAL: case NUMERIC:
				return a.value.nval == b.value.nval;
			case REAL:
				return a.value.rval == b.value.rval;
		}
	return false;
}

float calc_entropy_1 (const vector <int>& data) {
	float e = 0;
	float sum = accumulate (data.begin(), data.end(), 0.0f);
	for (int i = 0; i < data.size(); ++i) {
		if (data[i] == 0) continue;
		e += data[i] / sum * log2f (data[i]/sum);
	}
	return -e;
}

float calc_entropy_2 (const vector <vector<int> >& data) {
	float e = 0.0f, tsum = 0.0f, sum = 0.0f;
	for (int i = 0; i < data.size(); ++i) {
		e += (tsum = accumulate (data[i].begin(), data[i].end(), 0.0f)) * calc_entropy_1 (data[i]);
		sum += tsum;
	}
	return e / sum;
}

vector <pair<Field*, int> >* calc_occurrences (const Table& tab, const Attribute& attr, int index1, const Field& f, int index2, bool flag) {
	if (attr.type != NOMINAL) {
		cerr<<"Error: "<<__func__<<": Attribute type is not NOMINAL\n";
		return nullptr;
	}
	vector <pair<Field*, int> >* counter = new vector <pair<Field*, int> >(attr.domain.size());
	for (vector <Row>::const_iterator iter = tab.data.begin(); iter != tab.data.end(); ++iter) {
		if (flag || iter->fields[index2] == f) {
			(*counter)[iter->fields[index1].value.nval].first = (Field*)&iter->fields[index1];
			++(*counter)[iter->fields[index1].value.nval].second;
		}
	}
	return counter;
}

void to_vector (const vector <pair<Field*, int> >& m, vector <vector <int> >& vec) {
	vec.push_back (vector <int>());
	for (vector <pair<Field*, int> >::const_iterator iter = m.begin(); iter != m.end(); ++iter) {
		(vec.back()).push_back(iter->second);
	}
}

int main () {
	Table* tab = init();
	if (tab == nullptr) {
		cerr<<"Aborting...\n";
		return -1;
	}
	cout <<"Calculating entropy for all NOMINAL pairs...\n";
	int target_index = tab->header.size()-1;
	Field dummy;
	vector <vector<int> > data;
	vector <pair<Field*, int> >* curr = nullptr, *temp = nullptr;
	for (int i = 1; i < tab->header.size()-1; ++i) {
		data.clear();
		if ((curr=calc_occurrences (*tab, tab->header[i], i, dummy, 0, true)) != nullptr)
			for (vector <pair<Field*, int> >::iterator iter = curr->begin(); iter != curr->end(); ++iter)
				if ((temp=calc_occurrences (*tab, tab->header[target_index], target_index, *(iter->first), i, false)) != nullptr)
					to_vector (*temp, data);
			cout<<tab->header[i].name<<" and "<<tab->header[target_index].name<<" entropy \t =\t"<<calc_entropy_2 (data)<<'\n';
	}
	return 0;
}
