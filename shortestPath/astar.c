#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <float.h>

/***********************************************
 * 盤面の管理
 */


#define ONE_HOP 10.00001
typedef struct point { /* 座標 */
    int x, y;
} point_t, * point_tp;

#define MAX_N_POINTS 110
point_t points[MAX_N_POINTS];

/********************
 * ２点間を移動可能な場合はその距離を返す関数。
 * a, b: 対象となる２点
 * 返り値： 上述
 */
double distance(point_t a, point_t b) {
    int xdiff = a.x-b.x;
    int ydiff = a.y-b.y;
    double result = sqrt(xdiff*xdiff+ydiff*ydiff);
    return result;
}

/* 盤面 setup用の関数、皆さんは見る必要はない */
void setupBoard(FILE * in, int n) {
    int i;
    for(i=0; i< n; i++) {
        fscanf(in, "%d %d", &points[i].x, &points[i].y);
    }
}

/************************************************
 * 探索ノード
 */

typedef struct searchNode {
    double pathLen; /* 対象ノードまでの経路長 */
    double plusEstimation; /* path + ゴールまでの直線距離 */
    int index;        /* 対象ノード */
} searchNode_t, * searchNode_tp;

void printElem(searchNode_tp elem) {
    int i = elem->index;
    printf("(%d[%d,%d], %lf, %lf)\n", i, points[i].x, points[i].y, elem->pathLen, elem->plusEstimation);
}

/***********************************************
 * priority queue (汎用のつもり)
 */

#define BUFSIZE 90000
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
 * 実装しましょう。
 */
int compare(searchNode_tp a, searchNode_tp b) {
    /* a, b の順でよければ 負の値 */
    if(a->plusEstimation < b->plusEstimation) return -1;
    if(a->plusEstimation == b->plusEstimation) return 0;
    return 1;
}


/****************************************************
 * solver 系
 */

priorityQ_t Q;
double cost[MAX_N_POINTS];

double solve(int n) {
    int i; 
    int count = 0;
    reset(&Q);
    for(i=0; i<n;i++) { // setup
        cost[i] = DBL_MAX;
    }
    searchNode_t start = { 0.0, 0 };
    cost[0] = 0.0;
    enqueue(&Q, start);

    point_t goal = points[n-1];

    while(qSize(&Q) > 0) {
        int j;
        searchNode_t here = dequeue(&Q);
        /* printElem(&here);printf("\n");fflush(stdout); */
        /* ↓　複数のパスから同一ノードにたどり着く
           ケースがあるので、既に訪問済みか確認 */      
        if(cost[here.index] < here.pathLen) continue;
        cost[here.index] = here.pathLen;
        if(here.index == n-1) break;
        for(j=1; j<n; j++) { /* １ステップで到達できる点を探す */
            if(here.index == j) continue;
            double dist = distance(points[here.index], points[j]);
            if (dist > ONE_HOP) continue; /* 1step でいけないとき */
            double nextPathLen = here.pathLen+dist;
            if(cost[j] <= nextPathLen) continue; 
            cost[j] = nextPathLen;   
            double est = distance(points[j], goal);    
            searchNode_t next = { nextPathLen, nextPathLen+ est, j};
            enqueue(&Q, next);
            count++;
        }
    }
    printf("A*: count %d\n", count);
    if(cost[n-1]==DBL_MAX) return 0.0;
    return cost[n-1];
}


int main(void) {
    char* inFile = "shortestPath/sample.in";
    char* ansFile = "shortestPath/sample.ans";
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
    int totalCount = 0, failCount = 0;
    while(1) {
        int n;
        double result, ans;
        fscanf(in, "%d", &n);
        if(n==0) break;
        setupBoard(in, n);
        result = solve(n);
        fscanf(ansIn, "%lf", &ans);
        if(fabs(ans-result) > 0.001) {
            failCount++;
            printf("You failed data No. %d (result: %f, ans: %f)\n", totalCount, result, ans);
        } else {
            printf("You passed data No. %d (result: %f, ans: %f)\n", totalCount, result, ans);
        }
        totalCount++;
    }
    if(failCount==0)
        printf("Congratulation! You passed all dataset!\n");

    return 0;
}


