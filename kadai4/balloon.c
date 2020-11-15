#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>


#define MAX_BALLOONS 40
#define MAX_T 100

/**********************
 * board の情報
 */

typedef struct balloon {
	int time;
	int pos;
} balloon_t, * balloon_tp;
balloon_t balloons[MAX_BALLOONS];

void setupBalloons(FILE * in, int n) {
    int i=0;
    for(i=0; i< n; i++) {
    	int time, pos;
    	fscanf(in, "%d %d", &pos, &time);
    	balloons[i].time = time;
    	balloons[i].pos = pos;
    }
}

typedef struct result {
	int isOK;
	int num;
} result_t, *result_tp;

result_t solve(int n) {
	result_t result = { true, 0 };
	return result;
}

/*******
 * こちらで用意したmain 関数。
 * 問題準備してから、solve() をよび、正解比較もおこなう。
 */
int main(int argc, char* argv[]) {
    struct {
        char * in, * ans;
    } filepairs[] = { {"kadai4/sample.in", "kadai4/sample.ans"},
                      { "kadai4/B.in", "kadai4/B.ans"}};

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
            int n, ans; char isOK[10];
            result_t result;
            fscanf(in, "%d", &n);
            if(n==0) break;
            setupBalloons(in, n);
            result = solve(n);
            fscanf(ansIn, "%s %d", isOK, &ans);
            if((strcmp("OK", isOK) ==0) != result.isOK ||
            		ans != result.num) {
                failCount++;
                printf("You failed problem No. %d (result: %s %d, ans: %s, %d)\n",
                		totalCount, result.isOK? "OK": "NG", result.num, isOK, ans);
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

