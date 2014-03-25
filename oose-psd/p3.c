// Program for finding the minimum length unsorted array, setting which makes the complete array sorted.
#include <stdio.h>
#define MAX 100

void foo (int arr[], int n) {
	int i, beg, end, min, max;

	for (beg = 1; beg < n; ++beg) {
		if (arr[beg] < arr[beg-1])
			break;
	}

	for (end = n-2; end >= beg; --end) {
		if (arr[end] > arr[end+1])
			break;
	}

	if (beg-1 < end+1) {
		--beg; ++end;
		min = max = arr[beg];
		for (i = beg; i <= end; ++i) {
			if (arr[i] > max)
				max = arr[i];
			if (arr[i] < min)
				min = arr[i];
		}
		for (i = 0; i < beg; ++i)
			if (arr[i] >= min) {
				beg = i;
				break;
			}
		for (i = end+1; i < n; ++i)
			if (arr[i] <= max) {
				end = i;
			}

		printf ("Unsorted array [%d:%d]:", beg, end);
		for (; beg <= end; ++beg)
			printf (" %d", arr[beg]);
		putchar ('\n');
	}
	else
		printf ("Array is sorted\n");
}

int main () {
	int arr[MAX];
	int n, i, ele;
	i = 0;
	while (scanf("%d", &ele) > 0)
		arr[i++] = ele;
	n = i;
	foo (arr, n);

	return 0;
}
