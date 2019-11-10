#include <stdio.h>
#include <assert.h>
#include <math.h>


#define MAX_N 100
#define MAX_T 100

/**********************
 * board の情報
 */

typedef enum { W, L, B } state_t;
state_t board[MAX_N+1];

void setupBoard(FILE * in, int n, int l, int b) {
    int i=0;
    for(i=0; i< n+1; i++) board[i]=W;

    for(i=0; i< l; i++) {
        int d;
        fscanf(in, "%d", &d);
        board[d] = L;
    }
    for(i=0; i<b; i++) {
        int d;
        fscanf(in, "%d", &d);
        board[d] = B;
    }
}
void printBoard(int n) {
  int i;
  for(i=0; i< n; i++) {
          printf("%d:%c,", i, board[i]==L? 'L': (board[i]==B? 'B': '_'));
  }
  printf("%d:G", n);
}

double prob[MAX_T+2][MAX_N+1];

double solve(int n, int t) {
        int i,j;

        for(i=0; i<t+1; i++)
                for(j=0; j<n+1; j++)
                        prob[i][j]=0.0;
        prob[0][0] = 1.0;
        for(i=0; i<t; i++) {
                for(j=0; j<n; j++) {
                        int k;
                        for(k=1;k<=6;k++) {
                                int nPos = (j+k>n)? 2*n-k-j: j+k;
                                int nStep = (board[nPos]==L)? i+2: i+1;
                                if(board[nPos]==B) nPos = 0;
                                prob[nStep][nPos] += prob[i][j] / 6.0;
                /*      printf("prob[%d][%d]: %lf (+%lf)\n", nStep, nPos, prob[nStep][nPos], prob[i][j]/6.0);*/
                        }
                }
        }
        {
                double result = 0.0;
                for(i=0; i<=t; i++) result += prob[i][n];
                return result;
        }
}

/*******
 * こちらで用意したmain 関数。
 * 問題準備してから、solve() をよび、正解比較もおこなう。
 */
int main(int argc, char* argv[]) {
    struct {
        char * in, * ans;
    } filepairs[] = { {"dp/sample.in", "dp/sample.ans"},
                      { "dp/C.in", "dp/C.ans"}};

    int i;
    for(i=0; i<2; i++) {
        char* inFile = filepairs[i].in;
        char* ansFile = filepairs[i].ans;
        FILE * in = fopen(inFile, "r");
        FILE * ansIn = fopen(ansFile, "r");
        if(in==NULL) {
            printf("Can't open file: %s.\n", inFile);
            return 0;
        }
        if(ansIn==NULL) {
            printf("Can't open file: %s.\n", ansFile);
            return 0;
        }
        int failCount = 0;
        int totalCount = 0;
        printf("Processing input: %s\n", inFile);
        while(1) {
            int n, t, l, b; double result, ans;
            fscanf(in, "%d %d %d %d", &n, &t, &l, &b);
            if(n==0) break;
            setupBoard(in, n, l, b);
            result = solve(n, t);
            fscanf(ansIn, "%lf", &ans);
            double diff = ans-result;
            double diffx = fabs(diff);
            if(diffx > 0.00001) {
                failCount++;
                printf("! You failed problem No. %d (result: %lf, ans: %lf, diff:%lf, %lf)\n", totalCount, result, ans,diff,diffx);
            }
            totalCount++;
        }
        if(failCount==0) {
            printf("Congratulation! You passed all problems (%d) in %s!\n", totalCount, inFile);
        } else {
            printf("Im sorry you missed %d/%d in %s!\n", failCount, totalCount, inFile);
            return 0;
        }
    }
    return 0;
}

