#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <float.h>
#include <string.h>
#include <math.h>

/****************************
 * 問題情報
 */

#define NOPATH INT_MAX
#define INF DBL_MAX
#define MAX_CITY 30
#define MAX_TICKETS 8

int tickets[MAX_TICKETS];
int road[MAX_CITY][MAX_CITY];

/* setup用の関数、皆さんは見る必要はない */
void setupBoard(FILE * in, int n, int m, int p) {
    for(int i=0; i< m; i++) {
        for (int j = 0; j < m; j++) {
            road[i][j] = NOPATH;
        }
    }
    for (int i = 0; i < n; i++) {
        int t;
        fscanf(in, "%d", &t);
        tickets[i] = t;
    }
    for (int i = 0; i < p; i++) {
        int x, y, z;
        fscanf(in, "%d %d %d", &x, &y, &z);
        road[x-1][y-1] = z;
        road[y-1][x-1] = z;
    }
}

/************************************************
 * 探索ノード (準備済み)
 */

typedef struct searchNode {
    double time; /* 対象都市までの経路長 */
    int used;  /* 利用チケット数 */
    int city; /* 対象都市 */
} searchNode_t, * searchNode_tp;

void printElem(searchNode_tp elem) {
    printf("(time: %f, used %d, city %d)\n", elem->time, elem->used, elem->city);
}

/***********************************************
 * priority queue (汎用のつもり)
 */

#define BUFSIZE 10000
/* ELEM は要素の型を表す。利用用途に合わせて型名を変えてください
 * ちなみに、今回は searchNode_t を利用 */
#define ELEM searchNode_t

 /***
  * 比較機(comparator)のプロトタイプ宣言
  * 比較演算はデータ構造に合わせて定義すること
  * a,b: 要素へのポインタ
  * 返り値： aとb が同じ大きさなら0,
  *          [a, b] の順で並べるべきなら負の値、
  *          [b, a] の順で並べるべきなら正の値を返す
  * 例：要素が整数値なら、 return *a - *b; で OK.
  */
int compare(ELEM * a, ELEM * b);

typedef struct priorityQ {
  int size;
  ELEM buf[BUFSIZE];
} priorityQ_t, * priorityQ_tp;

void reset(priorityQ_tp q) {
  assert(q != NULL);
  q->size = 0;
}

int qSize(priorityQ_tp q) {
 assert(q != NULL);
 return q->size;
}

void enqueue(priorityQ_tp q, ELEM data) {
  assert(q != NULL);
  assert(q->size < BUFSIZE);
  { /* upheap */
      int target=q->size++;
      while(target!=0) {
          int parent= (target-1)/2;
          ELEM * pref = &q->buf[parent];
          if(compare(pref, &data)>0) {
              q->buf[target] = *pref;
              target = parent;
          } else {
              break;
          }
      }
      q->buf[target] = data;
  }
}

ELEM dequeue(priorityQ_tp q) {
  assert(q != NULL);
  assert(q->size > 0);
  {
      ELEM result = q->buf[0];
      q->size--;
      ELEM * moved = &q->buf[q->size];
      {   /* downheap */
          int target=0;
          while(1) {
              int child = target*2+1;
              if(child >= q->size) break;
              if(child +1 !=  q->size &&
                  compare(&q->buf[child],
                          &q->buf[child+1]) > 0) {
                  child = child + 1;
              }
              if(compare(moved, &q->buf[child])>0) {
                  q->buf[target] = q->buf[child];
                  target = child;
              } else {
                  break;
              }
          }
          q->buf[target] = *moved;
          return result;
      }
  }
}
void printQueueInside(priorityQ_tp p) {
    int i;
    printf("[priorityQ, size: %d, body:{", p->size);
    for(i=0; i<p->size; i++) {
        if(i!=0) printf(", ");
        printElem(&p->buf[i]);
    }
    printf("}]\n");
}
/*********************
 * 上記で宣言された 比較機
 * 今回用に実装済み
 */
int compare(searchNode_tp a, searchNode_tp b) {
    /* a, b の順でよければ 負の値 */
    if (a->time < b->time) return -1;
    if (a->time == b->time) return 0;
    return 1;
}

/****************************************************
 * solver 系
 */

priorityQ_t Q;

/**********************
 * 入力： `n(Tickets)`, `m(Cities)`, `p(Paths)` は問題のとおり。
 * 　変数名を間違わないように、（）内の文字を補足してます。
 * 　好みに応じて変更してもらっても構わない。
 *   a, b （開始都市、ゴール都市)については、もとの問題では
 *   1..m の値をとるが、 0始まり 0..(m-1) になるよう
 *   -1 した値を与えている。
 * 返り値: 到達可能な場合は、最短所要時間を答えよ。
 * 到達不能な場合は `DBL_MAX` (`INF`) を返すこと。
 */
double solve(int nTickets, int mCities, int pPaths, int a, int b) {
    reset(&Q);
    searchNode_t start = { 0.0, 0, a };
    enqueue(&Q, start);
 
    while(qSize(&Q) > 0) {
        searchNode_t here = dequeue(&Q);
        /* printElem(&here);printf("\n");fflush(stdout); */
        // TODO
        // 各自で完成させてみましょう。
    }
    return INF;
}

int main(void) {
    int optVersion = 0;
    struct {
        const char *in, *ans, *ansOpt;
    } filepairs[] =
        { {"kadai3/sample.in", "kadai3/sampleE.ans", "kadai3/sample.ans"},
          {"kadai3/D1.in", "kadai3/D1E.ans", "kadai3/D1.ans"}}; 
//        {{"kadai3/sample.in", "kadai3/sample.ans"}, {"kadai2/D1.in", "kadai2/D1.ans"}};

    for (int i = 0; i < 2; i++){
        const char *inFile = filepairs[i].in;
        const char *ansFile = optVersion ? filepairs[i].ansOpt : filepairs[i].ans;
        FILE *in = fopen(inFile, "r");
        FILE *ansIn = fopen(ansFile, "r");
        if (in == NULL) {
            printf("Can't open file: %s.\n", inFile);
            return 0;
        }
        if (ansIn == NULL) {
            printf("Can't open file: %s.\n", ansFile);
            return 0;
        }

        int totalCount = 0, failCount = 0;
        while(1) {
            int n, m, p, a, b;
            double result, ans;
            fscanf(in, "%d %d %d %d %d", &n, &m, &p, &a, &b);
            if(n==0) break;
            setupBoard(in, n, m, p);
            result = solve(n, m, p, a-1, b-1);
/*
            if (result == INF)
            {
                printf("Impossible\n");
            }
            else
            {
                printf("%f\n", result);
            }
*/
            char strAns[100];
            fscanf(ansIn, "%s", strAns);
            if (strcmp(strAns, "Impossible")==0) {
                ans = INF;
            } else { 
                sscanf(strAns, "%lf", &ans);
            }
            if(fabs(ans-result) > 0.001) {
                failCount++;
                if(result==INF) {
                    printf("You failed data No. %d (result: DBL_MAX (INF), ans: %s)\n", totalCount, strAns);
                } else {
                    printf("You failed data No. %d (result: %f, ans: %s)\n", totalCount, result, strAns);
                }
            } else {
                printf("You passed data No. %d (ans: %s)\n", totalCount, strAns);
            }
            totalCount++;
        }
        if (failCount == 0) {
            printf("!! Congratulations! You passed all data (%d) in %s!\n", totalCount, inFile);
        } else {
            printf("! I'm sorry you missed %d/%d in %s!\n", failCount, totalCount, inFile);
            return 0;
        }
    }
    return 0;
}

/* Debug 時は、問題がたくさんあると大変ですよね？
 * 下記を main() 関数にして（本来の main は mainX にでもして）、
 * 間違えたデータセットを、`budeg.in` ファイルに配置してデバッグすると
 * よいでしょう。
 */
int mainDebug(void)
{
    const char *inFile = "kadai3/debug.in";
    FILE *in = fopen(inFile, "r");
    if (in == NULL) {
        printf("Can't open file: %s.\n", inFile);
        return 0;
    }
    int n, m, p, a, b;
    double result, ans;
    fscanf(in, "%d %d %d %d %d", &n, &m, &p, &a, &b);
    setupBoard(in, n, m, p);
    result = solve(n, m, p, a - 1, b - 1);
    if (result == INF) {
        printf("Impossible\n");
    } else {
        printf("%f\n", result);
    }
    return 0;
}
