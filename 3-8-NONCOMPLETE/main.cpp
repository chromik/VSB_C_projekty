#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int N=1000000;

void swap(long int array[], long int left, long int right)
{
	long int tmp = array[right];
	array[right] = array[left];
	array[left] = tmp;        
}

void quicksort(long int array[], long int left, long int right)
{
	if(left < right){
		long int boundary = left;
		for(long int i = left + 1; i < right; i++){
			if(array[i] < array[left]){
				swap(array, i, ++boundary);
			}

		}
		swap(array, left, boundary);
		quicksort(array, left, boundary);
		quicksort(array, boundary + 1, right);
	}    
}


int main(int argc, char **argv)
{
	FILE *a = fopen(argv[1],"r");
	FILE *b = fopen(argv[2],"r");

	long int *aa = (long int *)malloc(N*sizeof(long int));
	long int *bb = (long int *)malloc(N*sizeof(long int));

	for (long int i = 0; i < N; ++i) {
		fscanf(a,"%li\n",&aa[i]);
		fscanf(b,"%li\n",&bb[i]);
	}

	quicksort(aa,0,N);
	quicksort(bb,0,N);

	for (int i=0; i<100;++i) {
		printf("%li\n", aa[i]);
	}

	long int uk1 = 0, uk2 = 0;
	while (uk1 != N && uk2 != N) {
		if (aa[uk1]==bb[uk2]){
			printf("%li\n",aa[uk1]);
			++uk1;
			++uk2;
		}
		else if (aa[uk1]<bb[uk2])
			++uk1;
		else
			++uk2;
	}
}