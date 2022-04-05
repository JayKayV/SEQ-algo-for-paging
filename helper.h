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