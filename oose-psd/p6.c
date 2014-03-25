// Program for performing Russian Peasant Multiplication
#include <stdio.h>

int main () {
	unsigned a, b;
	unsigned sum;
	sum = 0;

	scanf ("%u %u", &a, &b);

	while (a > 0) {
		if (a & 1) {
			sum += b;
		}
		a >>= 1;
		b <<= 1;
	}

	printf ("%u\n", sum);

	return 0;
}
