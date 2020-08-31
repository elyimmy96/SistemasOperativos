#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <ncurses.h>
#include <time.h>

int num_cpus();
void* sorter(void *param);
int compare(const void *a, const void *b);
void merge(int arr[], int l, int m, int r);
void mergeSort(int arr[], int l, int r);

int *arr;

int main() {
  int cpus = num_cpus();
  int highlight = 1;
  int choice = 0;
	int c;
  int tam;

  initscr();
	clear();
	noecho();
	cbreak();
  keypad(stdscr, TRUE);

  printw("Hay %d hilos disponibles, cuantos hilos quieres utilizar?\n", cpus);
  for(int i = 1; i <= cpus; ++i) {
    if(highlight == i) {
      attron(A_REVERSE);
      mvprintw(i, 5, "%d\n", i);
      attroff(A_REVERSE);
    } else {
      mvprintw(i, 5, "%d\n", i);
    }
  }
  do {
    c = getch();
    switch(c)
		{	case KEY_UP:
				if(highlight == 0)
					highlight = cpus;
				else
					--highlight;
				break;
			case KEY_DOWN:
				if(highlight == cpus)
					highlight = 1;
				else
					++highlight;
				break;
			case 10:
				choice = highlight;
				break;
		}
    if(choice != 0)
			break;
    for(int i = 1; i <= cpus; ++i) {
      if(highlight == i) {
        attron(A_REVERSE);
        mvprintw(i, 5, "%d\n", i);
        attroff(A_REVERSE);
      } else {
        mvprintw(i, 5, "%d\n", i);
      }
    }
    clrtoeol();
    refresh();
  } while (c != 'q');
  printw("Seleccionados %d hilos.\n", choice);
  echo();
  keypad(stdscr, FALSE);
  printw("De que tamaño sera el arreglo? ");
  scanw("%d\n", &tam);
  printw("Creando arreglo de %d elementos\n", tam);
  refresh();
  endwin();
  // int x, y;
  // getyx(stdscr, y, x);
  // move(y, 0);
  arr = malloc(sizeof(int)*tam);
  srand(time(NULL));
  for(int i = 0; i < tam; i++)
    arr[i] = rand() % 5000;
  // for(int i = 0; i < tam; i++) {
  //   printf("%d", arr[i]);
  //   if((i+1) % 10 == 0){
  //      printf("\n");
  //   } else {
  //     printf("\t");
  //   }
  // }
  // printf("\n");
  clock_t tic =  0.0;
	clock_t toc = 0.0;
  clock_t tic2 =  0.0;
	clock_t toc2 = 0.0;
  tic = clock();
  int lim[choice][3];
  pthread_t threads[choice];
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  for(int i = 0; i < choice; i++) {
    int div = tam%choice == 0 ? tam/choice : tam/choice+1;
    lim[i][0] = div*i;
    lim[i][1] = div*(i+1)-1 <= tam-1 ? div*(i+1)-1 : tam-1;
    lim[i][2] = i+1;
    // printw("\n\nThread %d\tInf: %d\tSup: %d\n", i, lim[i][0], lim[i][1]);
  }
  for (int i = 0; i < choice; i++) {
      pthread_create(&threads[i], &attr, sorter, &lim[i]);
  }
  for (int i = 0; i < choice; i++) {
      pthread_join(threads[i], NULL);
  }
  tic2 = clock();
  // mergeSort(arr, 0, tam - 1);
  qsort(arr, tam, sizeof(int), compare);
  toc2 = clock();
  double time_2 = (double)(toc2-tic2) * 1000.0 / CLOCKS_PER_SEC;
  printf("Tiempo sort final: %lf seconds.\n", time_2);
  toc = clock();
	double elapsedTime = (double)(toc-tic) * 1000.0 / CLOCKS_PER_SEC;
  // for(int i = 0; i < tam; i++) {
  //   printf("%d", arr[i]);
  //   if((i+1) % 10 == 0){
  //      printf("\n");
  //   } else {
  //     printf("\t");
  //   }
  // }
  printf("\nElapsed time: %lf seconds\n", elapsedTime);
  return 0;
}

int num_cpus() {

	unsigned  int	eax=11,ebx=0,ecx=1,edx=0;
	asm
	volatile
	( " cpuid "
	:  "=a"  ( eax ) ,
	"=b"  ( ebx ) ,
	"=c"  ( ecx ) ,
	"=d"  ( edx )
	:  "0"  ( eax ) ,  "2"  ( ecx )
	:  ) ;

	return eax*ebx;

}
void* sorter(void *param) {
  clock_t tic =  0.0;
	clock_t toc = 0.0;
	int *par= (int *)param;
	int lower=par[0];
	int i, upper = par[1];
  tic = clock();
	qsort(arr+lower, upper-lower+1, sizeof(int), compare);
  toc = clock();
  // mergeSort(arr, lower, upper);
  double elapsedTime = (double)(toc-tic) * 1000.0 / CLOCKS_PER_SEC;
  printf("Tiempo thread %d: %lf seconds.\n", par[2], elapsedTime);
  pthread_exit(0);
}

int compare(const void *a, const void *b) {
  return (*(int*)a - *(int*)b);
}

void merge(int arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    /* create temp arrays */
    int L[n1], R[n2];

    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    /* Copy the remaining elements of L[], if there
       are any */
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    /* Copy the remaining elements of R[], if there
       are any */
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

/* l is for left index and r is right index of the
   sub-array of arr to be sorted */
void mergeSort(int arr[], int l, int r)
{
    if (l < r) {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l + (r - l) / 2;

        // Sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        merge(arr, l, m, r);
    }
}
