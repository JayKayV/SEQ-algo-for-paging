#include <stdlib.h>
#include <stdio.h>
#include "helper.h"
#include "lru.h"

//define L, N, M for seq algorithm here
//L, N, M should sastify 0 <= M < N <= L (L >= 2)
#define L 2
#define N 2
#define M 1      

int dbg = 0;

typedef struct seq {
    int low;
    int high;
    int dir;

    //we only need N elements to keep track of Nth most page fault
    //pf_time should be saved as queue to optimize space
    int pf_time[50];
}seq;

//for simulating stack
int pop(int*, int*, int);
void push(int*, int*, int);

//for seq algorithm
struct seq seperate(struct seq*, int);
void removeSeq(struct seq* , int*);
void assign(struct seq*, int, int, int);
int length(struct seq);
int find_seq_idx(struct seq*, int);
int find_pf(struct seq);
void removeByIdx(struct seq* cnt, int*, int);
short can_overlap(struct seq, int);
short can_extend(struct seq, int);
void extend(struct seq*, int, int);
void print_seq(struct seq, int);

void run_seq(int frame_size, int* ar, int n) {
    //sequences
    struct seq seqs[16];
    int seq_cnt = 0;

    //lru
    int lru[16];
    fillzeros(lru, 16);

    //frame content
    int frame[16];
    fillzeros(frame, frame_size);

    //faults
    int faults[30];
    fillzeros(faults, n);

    //table
    int table[16][30];

    //init first values
    assign(&seqs[0], 0, 0, 1);
    seqs[0].pf_time[0] = 0;
    lru[0] = seq_cnt = 1;
    frame[0] = ar[0];
    for (int k = 0; k < frame_size; ++k) 
            table[k][0] = frame[k];
    
    //fill until frame is full
    int i = 1, c = 1;
    while (c < frame_size && i < n) {
        int idx = test_avail(frame, c, ar[i]);
        if (idx < 0) {
            frame[c] = ar[i];
            extend(&seqs[0], c, i);
            ++c; 
        } else {
            lru[idx] = 0;
        }

        inc_all(lru, frame, frame_size);
        for (int k = 0; k < frame_size; ++k) {
            table[k][i] = frame[k];
        }
        ++i;
    }

    while (i < n) {
        //printf("%d\n", ++dbg);
        /*
        for (int o = 0; o < seq_cnt; ++o)
            print_seq(seqs[o], o);
        */
        int u;
        if ((u=test_avail(frame, frame_size, ar[i])) >= 0) {
            lru[u] = 0;
            
            inc_all(lru, frame, frame_size);
            for (int k = 0; k < frame_size; ++k) {
                table[k][i] = frame[k];
            }
            ++i;
            continue;
        }
        faults[i] = 1;
        int idx = find_seq_idx(seqs, seq_cnt), pf;
        if (idx >= 0) 
            pf = find_pf(seqs[idx]);
        else {
            //perform lru
            pf = 0;
            int m = lru[pf];
            for (int j = 1; j < frame_size; ++j)
                if (lru[j] > m && lru[j] > 0) {
                    pf = j;
                    m = lru[pf];
                } 
        }

        //update seqs and lru accordingly
        int idxs[2] = {-1, -1};
        //check extend
        for (int j = 0; j < seq_cnt; ++j)
            if (can_extend(seqs[j], pf)) {
                if (idxs[0] < 0)
                    idxs[0] = j;
                else
                    idxs[1] = j;
            }
        
        //if there is a sequence can be extended
        if (idxs[0] >= 0) {
            int idx_to_extend;
            if (idxs[1] >= 0) {
                if (seqs[idxs[0]].pf_time[0] > seqs[idxs[1]].pf_time[0]) 
                    idx_to_extend = idxs[0];
                else 
                    idx_to_extend = idxs[1];                  
            } else 
                idx_to_extend = idxs[0];
            
            //check if extending sequence can lead to overlap with another sequence
            for (int j = 0; j < seq_cnt;) 
                if (can_overlap(seqs[j], pf)) {
                    removeByIdx(seqs, &seq_cnt, j);
                } else ++j;
            extend(&seqs[idx_to_extend], pf, i);
        } else {
            //if no sequence can be extended, proceed to replace in middle of a sequence 
            seqs[seq_cnt++] = seperate(&seqs[idx], pf);
            removeSeq(seqs, &seq_cnt);
            seqs[seq_cnt - 1].pf_time[0] = i;
        }
        /*
        for (int o = 0; o < frame_size; ++o)
            printf("%d ", frame[o]);
        puts("");
        */
        

        frame[pf] = ar[i];

        //keep a record of lru for fall handling
        lru[pf] = 0;
        inc_all(lru, frame, frame_size);

        //save record
        for (int k = 0; k < frame_size; ++k) {
            table[k][i] = frame[k];
        }
        ++i;
    }

    print_table(table, ar, faults, frame_size, n);
}

int main() {
    int seqlen = -1, frame_size = -1; //sequence length, frame size
    int ar[30];

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
        printf("Enter the number of elements need to be simulated (max: 30): ");
        scanf("%d", &seqlen); 

        if (test_value(seqlen, 1, 30) == 0)
            break;
        else {
            printf("Sequence length must be between 0 and 30\n"); 
            seqlen = -1;
        }
    }
    printf("Enter the sequence of numbers: ");
    for (int i = 0; i < seqlen; ++i)
        scanf("%d", &ar[i]);

    printf("Input received: seqlen-%d fr-sz-%d\n", seqlen, frame_size);

    printf("\nLRU Table: \n");
    lru(frame_size, ar, seqlen);

    printf("\nSEQ Table (L = %d,N = %d,M = %d): \n", L, N, M);
    run_seq(frame_size, ar, seqlen);
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

//seq algo helpers
struct seq seperate(struct seq* a, int j) {
    if (a->dir == -1)
        a->low=j+1;
    else if (a->dir == 1)
        a->high=j-1;

    struct seq u;
    u.low=u.high=j;
    u.dir=0;
    return u;
}

void assign(struct seq* s, int l, int h, int d) {
    s->low = l;
    s->high = h;
    s->dir = d;
}

int find_pf(struct seq a) {
    if (a.dir == 1)
        return a.high-M;
    return a.low+M;
}

int length(struct seq a) {
    return a.high - a.low + 1;
}

//pf: page fault, pf_t: time of this page fault
void extend(struct seq* a, int pf, int pf_t) {
    if (a->dir > 0 && a->high == pf - 1)
        a->high = pf; 
    else if (a->dir < 0 && a->low == pf + 1)
        a->low = pf; 
    else {
        if (a->high == pf-1) {
            a->high = pf;
            a->dir = 1;
        } else {
            a->low = pf;
            a->dir = -1;
        }
    }

    a->pf_time[1] = a->pf_time[0];
    a->pf_time[0] = pf_t;
}

short can_extend(struct seq a, int v) {
    return (a.dir >= 0 && a.high == v - 1) || (a.dir <= 0 && a.low == v + 1);
}

short can_overlap(struct seq a, int v) {
    return (a.low <= v && v <= a.high); 
}

int find_seq_idx(struct seq* a, int cnt) {
    int m = -1, idx = -1;
    for (int i = 0; i < cnt; ++i)
        if (length(a[i]) >= L && a[i].pf_time[1] > m) {
            m = a[i].pf_time[1];
            idx = i;
        } 
    return idx;
}

void removeByIdx(struct seq* a, int* seq_cnt, int idx) {
    for (int i = idx; i < *seq_cnt - 1; ++i)
        a[i] = a[i+1];
    *seq_cnt -= 1;
}

void update_time(struct seq* a, int cnt, int pf, int pf_t) {
    for (int i = 0; i < cnt; ++i)
        if (can_overlap(a[i], pf)) {
            a[i].pf_time[1] = a[i].pf_time[0];
            a[i].pf_time[0] = pf_t;
            break;
        }
}

void print_seq(struct seq a, int i) {
    if (i >= 0)
        printf("i-%d ", i);
    printf("seq dbg: %d %d %d\n", a.low, a.high, a.dir);
}

void removeSeq(struct seq* seqs, int* cnt) {
    for (int i = 0; i < *cnt;) {
        if (seqs[i].low > seqs[i].high) {
            for (int j = i; j < *cnt - 1; ++j) {
                seqs[j] = seqs[j+1];
            }
            *cnt -= 1;
            break;
        } 
        ++i;
    }
}
//end seq algo helpers