#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

struct Point {
	int x, y;
	int type;

	Point () : x(0), y(0), type(0)
	{	}

	Point (int xx, int yy) : x(xx), y(yy), type(0)
	{	}
};

struct Metric {
	int type;
	float dist;

	Metric () : type (0), dist(0)
	{	}
};

bool operator < (const Metric& a, const Metric& b) {
    return a.dist < b.dist;
}

bool read_file (string fname, vector<Point>& points) {
	int x, y;
	ifstream ifs(fname);

	if (!ifs) {
		cerr<<"Error opening file \""<<fname<<"\"!\n";
		return false;
	}

	while (ifs>>x>>y) {
		points.push_back (Point (x, y));
	}

	return true;
}

float distance (Point p, Point q) {
	return sqrt ((p.x-q.x)*(p.x-q.x) + (p.y-q.y)*(p.y-q.y));
}

void k_min_distances (const vector <Point>& points, const int size, const Point& centre, vector <Metric>& k_min) {
	for (int i = 0; i < size; ++i) {
		k_min[i].dist = distance (points[i], centre);
		k_min[i].type = points[i].type;
	}
	sort (k_min.begin(), k_min.begin()+size);
}

void calc_type (Point& p, const vector <Metric>& k_min, int k) {
	map<int, float> m;
	for (int i = 0; i < k; ++i) {
		m[k_min[i].type] += 1.0f / k_min[i].dist;
	}
	float maximum = m.begin()->second;
	int type = m.begin()->first;
	for (map<int, float>::iterator iter = m.begin(); iter != m.end(); ++iter)
		if (iter->second > maximum) {
			maximum = iter->second;
			type = iter->first;
		}
	p.type = type;
}

void k_nearest_neighbour (vector <Point>& points, int k) {
	if (k > points.size()) {
		cerr<<"k parameter greater than sample size!\nNothing to do!\n";
		return;
	}

	for (int i = 0; i < k; ++i)
		points[i].type = i+1;

	vector <Metric> k_min(points.size()-1);
	do {
		for (int i = k; i < points.size(); ++i) {
			k_min_distances (points, i, points[i], k_min);
			calc_type (points[i], k_min, k);
		}
	} while (0);
}

void print_list (const vector <Point>& points) {
	for (int i = 0; i < points.size(); ++i)
		cout<<char('A'+i)<<' '<<points[i].x<<' '<<points[i].y<<' '<<points[i].type<<'\n';
}

int main () {
	vector <Point> points;

	if (read_file ("test", points)) {
		k_nearest_neighbour (points, 3);
		print_list (points);
	}

	return 0;
}
