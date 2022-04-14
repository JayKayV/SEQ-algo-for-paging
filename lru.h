void lru(int frame_size, int* ar, int n) {
    int arr[16][16];

    int stack[16], stack_sz = 0;
    int faults[16], cnt[16];
    fillzeros(stack, 16); //assign 0 to all elements in stack
    fillzeros(faults, 16);
    fillzeros(cnt, 16);

    for (int i = 0; i < n; ++i) {
        int v = ar[i];

        if (stack_sz == 0)
            push(stack, &stack_sz, v);
        else {
            int ind = test_avail(stack, stack_sz, v);
            if (ind < 0) {
                if (stack_sz < frame_size)
                    push(stack, &stack_sz, v);
                else {
                    int m_i = 0, m = cnt[m_i];
                    for (int j = 1; j < frame_size; ++j)
                        if (cnt[j] > m && cnt[j] > 0) {
                            m_i = j;
                            m = cnt[m_i];
                        } 
                    stack[m_i] = v;               
                    cnt[m_i] = 0;
                    faults[i] = 1;
                }
            } else {
                cnt[ind] = 0;
            }
        }       
        inc_all(cnt, stack, frame_size);
        for (int k = 0; k < frame_size; ++k) {
            arr[k][i] = stack[k];
        }
    }
    print_table(arr, ar, faults, frame_size, n);
}
