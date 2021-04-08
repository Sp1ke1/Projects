
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
int cmpfunc (const void * a, const void * b) {
    return ( *(int*)a - *(int*)b );
}
int deleteDups (int * arr, int size) {

	int prev = 0; 
	for ( int i = 0; i < size; i++) {
		if (arr[i] != arr[prev]) {
			arr[++prev] = arr[i];
		}
	}

	int count = prev + 1;
	return count; 
}

int main (int argc, char *argv[]) {


    int min, max;


    printf("Intervaly:\n");

    while ( scanf("%d %d", &min, &max) == 2 ) {

        if ( min < 1 || max < 1 || min > max){
            printf("Nespravny vstup.\n");
            return 1;
        }

        int maxPower;
        int minPower;
        int numbers[200000];
        int counter = 0;

        // printf("Interval: <%d;%d>\n", min, max);

        if (min == 1) {numbers[counter] = 1; counter++;}

        int maxNumber = floor(sqrt(max));
        for (int j = 2; j <= maxNumber; j++) {
            maxPower = floor(log(max)/log(j));
            //printf("max power for j = %d is %d\n", j, maxPower);
            minPower = ceil(log(min)/log(j));
            //printf("min power for j = %d is %d\n", j, minPower);
            for (int i = minPower; i <= maxPower; i++) {
                    if (i == 1) {continue;}
                    int current = (int)pow(j,i);
                    numbers[counter] = current;
                    //printf("for j = %d and i = %d counter is %d\n", j, i, counter);
                    counter++;

                }
            }

        qsort(numbers, counter, sizeof(int), cmpfunc);
        if (counter > 1) {
        counter = deleteDups (numbers, counter);
    	}	

        /*
        printf("The numbers in the array:\n");
        for (int g = 0; g < counter; g++) {
            printf("%d ", numbers[g]);
        }
        printf("\n");
        */
        

        // printf("Deleting duplicates\n");
     
        /*
        printf("The numbers in the array:\n");
        for (int g = 0; g < n; g++) {
            printf("%d ", numbers[g]);
        }
        */
       // printf("counter is: %d no dups is: %d", counter, n );

        // printf("\n");
       // printf("final counter is %d", counter);
        if ((max == min) && (counter != 0)) {printf("<%d;%d> -> 0\n", min, max);}
        else if (counter == 1) {printf("<%d;%d> -> 1\n", min, max);}
        else {
            int result = (max - min) - counter + 1;
            printf("<%d;%d> -> %d\n", min, max, result);
        }

        for (int i = 0; i < counter; i++) {
            numbers[i] = 0;
        }


    }
    if ( min < 1 || max < 1 || min > max) {
            printf("Nespravny vstup.\n");
            return 1;
        }

    if (!feof(stdin)) {
        printf("Nespravny vstup.\n");
        return 1;
    }

    return 0;
}
