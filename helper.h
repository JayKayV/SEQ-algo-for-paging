short test_value(int a, int lwr, int upr) {
    if (lwr <= a && a <= upr)
        return 0;
    return -1;
}

int test_avail(int* a, int n, int v) {
    for (int i = 0; i < n; ++i)
        if (a[i] == v)
            return i;
    return -1;
}

void fillzeros(int* ar, int n) {
    for (int i = 0; i < n; ++i)
        ar[i] = 0;
}

void inc_all(int* ar, int* ar2, int n) {
    for (int i = 0; i < n; ++i)
        if (ar2[i] > 0)
            ++ar[i];
}

//start table output
void print_framecontent(int ar[16][16], int n, int m) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            printf("%2d ", ar[i][j]);
        }
        puts(""); 
    }
}

void print_table(int arr[16][16], int* ar, int* faults, int frame_size, int n) {
    for (int i = 0; i < n; ++i)
        printf("%2d ", ar[i]);
    printf("\n");
    print_framecontent(arr, frame_size, n);
    for (int i = 0; i < n; ++i)
        if (faults[i]) 
            printf(" F ");
        else
            printf("   ");
}
//end table output