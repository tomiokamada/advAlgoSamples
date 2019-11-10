#include <stdio.h>
#include <stdlib.h>

void setupBoard() {
    /* 未実装 */
}
int solve(int amounts) {
    /* 未実装 */
    return 0;
}

int main(int argc, char* argv[]) {
    struct {
        char * in, *ans;
    } filepairs[] = { { "kadai4/sample.in", "kadai4/sample.ans" } };

    int i;
    for (i = 0; i < 1; i++) {
        char* inFile = filepairs[i].in;
        char* ansFile = filepairs[i].ans;
        FILE * in = fopen(inFile, "r");
        FILE * ansIn = fopen(ansFile, "r");
        if (in == NULL) {
            printf("Can't open file: %s.\n", inFile);
            return 0;
        }
        if (ansIn == NULL) {
            printf("Can't open file: %s.\n", ansFile);
            return 0;
        }
        int failCount = 0;
        int totalCount = 0;
        printf("Processing input: %s\n", inFile);

        setupBoard(); /* setupBoard() の呼び出し */
        while (1) {
            int amounts, result, ans;
            fscanf(in, "%d", &amounts);
            if (amounts == 0)
                break;
            result = solve(amounts);  /* solve() の呼び出し */
            fscanf(ansIn, "%d", &ans);
            if (ans != result) {
                failCount++;
                printf("! You failed data No. %d (result: %d, ans: %d)\n",
                        totalCount, result, ans);
            }
            totalCount++;
        }
        if (failCount == 0) {
            printf("!! Congratulation! You passed all data (%d) in %s!\n",
                    totalCount, inFile);
        } else {
            printf("! Im sorry you missed %d/%d in %s!\n", failCount,
                    totalCount, inFile);
            return 0;
        }
    }
    return 0;
}
