#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

unsigned int ns[] = { 1000, 3000, 1500};

void fill_increasing(int *t, unsigned int n) {
    srand (time(NULL));
    int min = 0;
    int max = 5;
    for (unsigned int i = 0; i < n; i++ ) {
        t[i] = (rand() % (max - min)) + min;
        min = max;
        max += 5;
    }
}

void fill_decreasing(int *t, unsigned int n) {
    srand (time(NULL));
    int min = 0;
    int max = 5;
    unsigned int i = 0;
    while(n > i) {
        t[--n] = (rand() % (max - min)) + min;
        min = max;
        max += 5;
    }
}

void fill_vshape(int *t, unsigned int n) {
    int start_index = n/2; //srodek tablicy
    int l_index = start_index; //lewy
    int r_index = --start_index; //prawy
    // t[start_index] = 0;
    int l_start = 1;
    int r_start = 0;

    for(int i = 0; i<=n/2; i++) {
        t[l_index] = l_start;
        l_start+=2;
        t[r_index] = r_start;
        r_start+=2;
        // if(r_index > n) {
        //     break;
        // }
        l_index--;
        r_index++;
    }
}

int swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void selection_sort(int *t, unsigned int n) {
    for (int i = 0; i < n; i++)
    {
        int minimal = i;
        for (int j = i+1; j < n; j++)  {
        if (t[j] < t[minimal]) {
            minimal = j;
            }
        }
        swap(&t[minimal], &t[i]);
    }
}

void insertion_sort(int *t, unsigned int n) {
     for(int j = 1; j<n; j++) {
        int key = t[j];
        int i = j - 1;

        while ((i >= 0) && (t[i] > key)) {
        t[i + 1] = t[i];
        i--;
        }
        t[i + 1] = key;

    }
}

int partition(int tab[], int p, int r)
{
    int x = tab[r];
    int i = p - 1;
    for(int j = p; j < r; j++)
    {
        if(tab[j] <= x) {
            i = i + 1;
            swap(&tab[i], &tab[j]);
        }
    }
    i++;
    swap(&tab[i], &tab[r]);
    return i;
}

int random_partition(int tab[], int p, int r)
{
    int i = rand() % (r + 1 - p) + p;
    swap(&tab[i], &tab[r]);
    return partition(tab, p, r);
}
int quickSort(int tab[], int p, int r) {
    if(p < r)
    {
        int q = partition(tab, p, r);
        quickSort(tab, p, q - 1);
        quickSort(tab, q + 1, r);
    }
}

void quick_sort(int *t, unsigned int n) {
    quickSort(t, 0, n - 1);
}

void heap_max(int tab[], int n, int i)
{
    int max = i;
    int l = 2*i + 1;
    int r = 2*i + 2;

    if (l < n && tab[l] > tab[max])
        max = l;

    if (r < n && tab[r] > tab[max])
        max = r;

    if (max != i)
    {
        swap(&tab[i], &tab[max]);

        heap_max(tab, n, max);
    }
}

void heap_sort(int *t, unsigned int n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heap_max(t, n, i);

    for (int i=n-1; i>=0; i--)
    {
        swap(&t[0], &t[i]);

        heap_max(t, i, 0);
    }
}

void fill_random(int *t, unsigned int n) {
    for (unsigned int i = 0; i < n; i++) {
        t[i] = rand();
    }
}

void is_random(int *t, unsigned int n) {
    return;
}

void is_increasing(int *t, unsigned int n) {
    for (unsigned int i = 1; i < n; i++) {
        assert(t[i] > t[i - 1]);
    }
}

void is_decreasing(int *t, unsigned int n) {
    for (unsigned int i = 1; i < n; i++) {
        assert(t[i] < t[i - 1]);
    }
}

void is_vshape(int *t, unsigned int n) {
    int *begin = t;
    int *end = t + n - 1;

    while (end - begin > 1) {
        assert(*begin > *end);
        begin++;
        assert(*end > *begin);
        end--;
    }
}

void is_sorted(int *t, unsigned int n) {
    for (unsigned int i = 1; i < n; i++) {
        assert(t[i] >= t[i - 1]);
    }
}

void (*fill_functions[])(int *, unsigned int) = { fill_random, fill_increasing, fill_decreasing, fill_vshape};
void (*check_functions[])(int *, unsigned int) = { is_random, is_increasing, is_decreasing, is_vshape};
void (*sort_functions[])(int *, unsigned int) = { selection_sort, insertion_sort, quick_sort, heap_sort };

char *fill_names[] = { "Random", "Increasing", "Decreasing", "V-shape" }; //vshape
char *sort_names[] = { "SelectionSort", "InsertionSort", "QuickSort", "HeapSort" };

int main() {
    for (unsigned int i = 0; i < sizeof(sort_functions) / sizeof(*sort_functions); i++) {
        void (*sort)(int *, unsigned int) = sort_functions[i];

        for (unsigned int j = 0; j < sizeof(fill_functions) / sizeof(*fill_functions); j++) {
            void (*fill)(int *, unsigned int) = fill_functions[j];
            void (*check)(int *, unsigned int) = check_functions[j];

            for (unsigned int k = 0; k < sizeof(ns) / sizeof(*ns); k++) {
                unsigned int n = ns[k];
                int *t = (int *) malloc(n * sizeof(*t));

                fill(t, n);
                check(t, n);

                clock_t begin = clock();
                sort(t, n);
                clock_t end = clock();
                is_sorted(t, n);

                printf("%s\t%s\t%u\t%f\n", sort_names[i], fill_names[j], n, (double)(end - begin) / (double) CLOCKS_PER_SEC);
                free(t);
            }
        }
    }
    return 0;
}
