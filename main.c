#include <stdlib.h>
#include <stdio.h>
#include "helper.h"

//for simulating stack
int pop(int*, int*, int);
void push(int*, int*, int);

void print_table(int ar[16][16], int n, int m) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            printf("%2d ", ar[i][j]);
        }
        puts(""); 
    }
}

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
        inc_all(cnt, stack, 16);
        for (int k = 0; k < frame_size; ++k) {
            arr[k][i] = stack[k];
        }
    }
    for (int i = 0; i < n; ++i)
        printf("%2d ", ar[i]);
    printf("\n");
    print_table(arr, frame_size, n);
    for (int i = 0; i < n; ++i)
        if (faults[i]) 
            printf(" F ");
        else
            printf("   ");
}

void seq(int frame_size, int* ar, int n) {
    int arr[16][16], stack[16];

    int seqs[100][5], c = 0, stack_sz = 0;
    int cnt[16], faults[16];

    fillzeros(cnt, 16);
    fillzeros(faults, 16);
    for (int i = 0; i < n; ++i) {
        int v = ar[i];

        if (stack_sz == 0) {
            push(stack, &stack_sz, v); 
            c += 1;
            seqs[0][0] = seqs[0][1] = v;
            seqs[0][2] = seqs[0][3] = seqs[0][4] = 0;
        }
        else {
            int ind = test_avail(stack, stack_sz, v);
            if (ind < 0) {
                if (stack_sz < frame_size) {
                    push(stack, &stack_sz, v); 
                    for (int u = 0; u < c; ++u) {
                        if (seqs[u][4] == 0) 
                    }
                }
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
        
        inc_all(cnt, stack, 16);
        for (int k = 0; k < frame_size; ++k) {
            arr[k][i] = stack[k];
        }
    }

    for (int i = 0; i < n; ++i)
        printf("%2d ", ar[i]);
    printf("\n");
    print_table(arr, frame_size, n);
    for (int i = 0; i < n; ++i)
        if (faults[i]) 
            printf(" F ");
        else
            printf("   ");
}

int main() {
    int seqlen = -1, frame_size = -1; //sequence length, frame size
    int ar[16];

    while (frame_size < 0) {
        printf("Enter the size of the frame (max: 16): ");
        scanf("%d", &frame_size);

        if (test_value(frame_size, 1, 16) == 0)
            break;
        else {
            printf("Frame size must be between 0 and 16\n"); 
            frame_size = -1;
        }
    }

    while (seqlen < 0) {
        printf("Enter the number of elements need to be simulated (max: 16): ");
        scanf("%d", &seqlen); 

        if (test_value(seqlen, 1, 16) == 0)
            break;
        else {
            printf("Sequence length must be between 0 and 16\n"); 
            seqlen = -1;
        }
    }
    printf("Enter the sequence of numbers (all numbers must be positive): ");
    for (int i = 0; i < seqlen; ++i)
        scanf("%d", &ar[i]);

    printf("Input received: seqlen-%d fr-sz-%d\n", frame_size, seqlen);

    printf("\nLRU Table: \n");
    lru(frame_size, ar, seqlen);

    printf("\nSEQ Table: \n");
    seq(frame_size, ar, seqlen);
    return 0;
}

//start simulating stack
int pop(int* stack, int* n, int index) {
    int a = stack[index];
    for (int i = a; i < *n - 1; ++i)
        stack[i]=stack[i+1];
    *n -= 1;
    return a;
}

void push(int* stack, int* n, int v) {
    stack[*n] = v;
    *n += 1;
}
//end simulating stack