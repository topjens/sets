#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INIT_SIZE 10
#define BUF_SIZE 256

char **arr1 = NULL;
char **arr2 = NULL;

int arr_len(char **arr);
void delete(char **arr);
void do_intersection(char **arr1, char **arr2);
void do_union(char **arr1, char **arr2);
void dump_array(char **arr);
int partition(char **arr, int low, int high);
char **readfile(const char *filename);
void sort(char **arr, int low, int high);
void swap(char **a, char **b);

int arr_len(char **arr)
{
		int count;

		if(!arr)
				return 0;

		for(count = 0; arr[count]; count++){}

		return count;
}

void delete(char **arr)
{
		if(!arr)
				return;
    
		for(int i = 0; arr[i] != NULL; i++)
				free(arr[i]);

		free(arr);
}

void do_intersection(char **arr1, char **arr2)
{
		int i = 0, j = 0;

		while(arr1[i] != NULL && arr2[j] != NULL) {
				int cmp = strcmp(arr1[i], arr2[j]);
				if(cmp < 0)
						i++;
				else if(cmp > 0)
						j++;
				else {
						printf(" %s ", arr2[j++]);
						i++;
				}
		}
}

void do_union(char **arr1, char **arr2)
{
		int i = 0, j = 0;

		while(arr1[i] != NULL && arr2[j] != NULL) {
				int cmp = strcmp(arr1[i], arr2[j]);
				if(cmp < 0)
						printf(" %s ", arr1[i++]);
				else if(cmp > 0)
						printf(" %s ", arr2[j++]);
				else {
						printf(" %s ", arr2[j++]);
						i++;
				}
		}

		/* print remaining elements if one array is bigger than the other */
		while(arr1[i] != NULL)
				printf(" %s ", arr1[i++]);
		while(arr2[j] != NULL)
				printf(" %s ", arr2[j++]);
}

void dump_array(char **arr)
{
		if(!arr)
				return;
		
		for(int i = 0; arr[i]; i++)
				printf("%d: %s\n", i, arr[i]);

}

int partition(char **arr, int low, int high)
{
		char *pivot = arr[high];
		int i = (low - 1);

		for(int j = low; j <= high - 1; j++) {
				if(strcmp(arr[j], pivot) < 0) {
						i++;
						swap(&arr[i], &arr[j]);
				}
		}
		swap(&arr[i + 1], &arr[high]);
		return (i + 1);
}

char **readfile(const char *filename)
{
		FILE *fp = fopen(filename, "r");
		if(!fp)
				return NULL;
    
		size_t size = INIT_SIZE;
		size_t count = 0;
		char buf[BUF_SIZE];

		char **ret = malloc(size * sizeof *ret);

		/* read the file line by line and store the lines into the array,
		   double the size as necessary */
		while(fgets(buf, BUF_SIZE, fp) != NULL) {
				if(count >= size) {
						size *= 2;
						ret = realloc(ret, size * sizeof *ret);
				}
				ret[count] = strdup(buf);
				count++;
		}

		/* we want arr to be the exact size, as to not disturb our sort
		   algortihm or fill up our results with NULLs */
		ret = realloc(ret, (count + 1) * sizeof *ret);

		/* always make array NULL-terminated */
		ret[count] = NULL;

		return ret;
}

void sort(char **arr, int low, int high)
{
		if(low < high) {
				int pi = partition(arr, low, high);

				sort(arr, low, pi - 1);
				sort(arr, pi + 1, high);
		}
}

void strip_newlines(char **arr)
{
		if(!arr)
				return;

		for(int i = 0; arr[i]; i++) {
				for(char *p = arr[i]; *p; p++) {
						if(*p == '\n')
								*p = '\0';
						else if(*p == '\r')
								*p = '\0';
				}
		}
}

void swap(char **a, char **b)
{
		char *t = *a;
		*a = *b;
		*b = t;
}
		
int main(int argc, char *argv[])
{
		/*char *arr1[] = {"Calvo2018", "Baumann2018", NULL};
		  char *arr2[] = {"Top2019", "Top2020", "Calvo2018", NULL};*/

		if(argc < 3)
				return 0;

		setbuf(stdout, NULL);

	    arr1 = readfile(argv[1]);
	    arr2 = readfile(argv[2]);

		strip_newlines(arr1);
		strip_newlines(arr2);

		/*dump_array(arr1);
		dump_array(arr2);*/

		/* -2 to accomodate for the NULL and because C counts from 0 */
		sort(arr1, 0, arr_len(arr1) - 2);
		sort(arr2, 0, arr_len(arr2) - 2);

		/*dump_array(arr1);
		  dump_array(arr2);*/
		
		printf("Union: ");
		do_union(arr1, arr2);
		printf("\nIntersection: ");
		do_intersection(arr1, arr2);
		printf("\n");

		delete(arr1);
		delete(arr2);
    
		return 0;
}
