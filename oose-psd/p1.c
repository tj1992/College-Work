// program for displaying Pascal's triange

#include <stdio.h>
#define MAX 80

int main () {
	unsigned N, i, j;
	unsigned pascal_triangle[MAX][MAX] = { 1 };

	scanf ("%u", &N);
	if (N > MAX)
		N = MAX;

	for (i = 1; i < N; ++i) {
		pascal_triangle[i][0] = 1;
		for (j = 1; j < i; ++j)
			pascal_triangle[i][j] = pascal_triangle[i-1][j-1] + pascal_triangle[i-1][j];
		pascal_triangle[i][j] = 1;
	}

	for (i = 0; i < N; ++i) {
		for (j = 0; j < i+1; ++j)
			printf ("%u ", pascal_triangle[i][j]);
		putchar ('\n');
	}

	return 0;
}
