#define _CRT_SECURE_NO_WARNINGS   
#include <stdio.h>
#define MAX_N      20000
#define MAX_VALUE 100000      
#define DEFAULT_SEED 12345


static unsigned long rngState = DEFAULT_SEED;
static void rng_seed(unsigned long seed) { rngState = seed; }
static int rng_next(int bound) {
    rngState = rngState * 1103515245UL + 12345UL;
    return (int)((rngState >> 16) % (unsigned long)bound);
}


static int original[MAX_N], work[MAX_N];

static void make_random(int a[], int n) {
    int i;
    for (i = 0; i < n; i++) a[i] = rng_next(MAX_VALUE);
}

static void copy_array(const int src[], int dst[], int n) {
    int i;
    for (i = 0; i < n; i++) dst[i] = src[i];
}

static int is_sorted(const int a[], int n) {
    int i;
    for (i = 1; i < n; i++) if (a[i - 1] > a[i]) return 0;
    return 1;
}


static long long insertion_sort(int a[], int n) {
    long long cmp = 0;
    int i, j, cur;

    for (i = 1; i < n; i++) {
        cur = a[i];
        j = i - 1;
        while (j >= 0) {
            cmp++;                              
            if (a[j] > cur) {
                a[j + 1] = a[j];
                j--;
            }
            else {
                break;                          
            }
        }
        a[j + 1] = cur;
    }
    return cmp;
}

static long long shell_sort_original(int a[], int n) {
    long long cmp = 0;
    int gap, i, j, cur;

    for (gap = n / 2; gap > 0; gap /= 2) {
        for (i = gap; i < n; i++) {
            cur = a[i];
            j = i;
            while (j >= gap) {
                cmp++;                          
                if (a[j - gap] > cur) {
                    a[j] = a[j - gap];          
                    j -= gap;
                }
                else {
                    break;
                }
            }
            a[j] = cur;
        }
    }
    return cmp;
}


static long long shell_sort_knuth(int a[], int n) {
    long long cmp = 0;
    int gap = 1, i, j, cur;

    while (gap * 3 + 1 < n) gap = gap * 3 + 1;  

    while (gap > 0) {
        for (i = gap; i < n; i++) {
            cur = a[i];
            j = i;
            while (j >= gap) {
                cmp++;
                if (a[j - gap] > cur) {
                    a[j] = a[j - gap];
                    j -= gap;
                }
                else {
                    break;
                }
            }
            a[j] = cur;
        }
        gap = (gap - 1) / 3;                    
    }
    return cmp;
}


static unsigned long ask_seed(void) {
    char line[64];
    unsigned long value = 0;
    int i = 0, digits = 0;

    printf("난수 시드를 입력하세요 (그냥 Enter를 누르면 %d): ", DEFAULT_SEED);
    if (fgets(line, sizeof(line), stdin) == NULL) return DEFAULT_SEED;
    while (line[i] == ' ' || line[i] == '\t') i++;
    while (line[i] >= '0' && line[i] <= '9') {
        value = value * 10 + (unsigned long)(line[i] - '0');
        digits++; i++;
    }
    return (digits > 0) ? value : DEFAULT_SEED;
}


int main(void) {
    int sizes[] = { 1000, 2000, 4000, 8000, 16000, 20000 };
    int caseCount = (int)(sizeof(sizes) / sizeof(sizes[0]));
    long long insCmp[8], oriCmp[8], knuCmp[8];
    int ok = 1, k, n;

    rng_seed(ask_seed());

    for (k = 0; k < caseCount; k++) {
        n = sizes[k];
        make_random(original, n);               

        copy_array(original, work, n);
        insCmp[k] = insertion_sort(work, n);
        if (!is_sorted(work, n)) ok = 0;

        copy_array(original, work, n);
        oriCmp[k] = shell_sort_original(work, n);
        if (!is_sorted(work, n)) ok = 0;

        copy_array(original, work, n);
        knuCmp[k] = shell_sort_knuth(work, n);
        if (!is_sorted(work, n)) ok = 0;

        printf("\nData Size : %d\n\n", n);
        printf("Insertion Sort       : %lld comparisons\n", insCmp[k]);
        printf("Shell Sort (Original): %lld comparisons\n", oriCmp[k]);
        printf("Shell Sort (Knuth)   : %lld comparisons\n", knuCmp[k]);
    }

    printf("\n------------------------------------\n");
    printf("\n[요약]\n");
    printf("     n   삽입 정렬      쉘(기본)     쉘(Knuth)   삽입/기본   삽입/Knuth   기본/Knuth\n");
    for (k = 0; k < caseCount; k++) {
        printf("%6d  %11lld  %11lld  %11lld   %8.1f배  %9.1f배  %9.2f배\n",
            sizes[k], insCmp[k], oriCmp[k], knuCmp[k],
            (double)insCmp[k] / (double)oriCmp[k],
            (double)insCmp[k] / (double)knuCmp[k],
            (double)oriCmp[k] / (double)knuCmp[k]);
    }

    printf("\n[데이터가 2배가 될 때 비교 횟수의 증가 배율]\n");
    printf("        n         삽입 정렬   쉘(기본)   쉘(Knuth)\n");
    for (k = 1; k < caseCount; k++) {
        printf("%6d -> %6d   %7.2f배  %7.2f배  %7.2f배\n",
            sizes[k - 1], sizes[k],
            (double)insCmp[k] / (double)insCmp[k - 1],
            (double)oriCmp[k] / (double)oriCmp[k - 1],
            (double)knuCmp[k] / (double)knuCmp[k - 1]);
    }

    printf("\n정렬 결과 확인 : %s\n", ok ? "모든 경우 오름차순 정렬 성공" : "실패한 경우 있음");
    return 0;
}