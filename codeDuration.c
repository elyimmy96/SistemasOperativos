#include <stdio.h>
#include <time.h>

int main(void) {
	clock_t tic =  0.0;
	clock_t toc = 0.0;
	tic = clock();
	/*Function that wants to be measured */

	toc = clock();
	double elapsedTime = (double)(toc-tic) * 1000.0 / CLOCKS_PER_SEC;

	printf("Elapsed time: %lf seconds\n", elapsedTime);
}
