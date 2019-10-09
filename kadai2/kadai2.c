#include <stdio.h>
#include <assert.h>
#include <math.h>

/* 部屋のサイズの上限 (W:幅, H:高さ) */
#define MAX_W 30
#define MAX_H 30

/*****************
 * point
 * 迷図の各場所を表現するのに使います。起点を {0, 0}, 
 * ゴールを {w-1, h-1}として扱います。
 */
typedef struct point {
    int x, y;
} point_t, * point_tp;

/******************
 * ついでに、方向は、
 * {{1, 0}, {-1, 0}, {0, 1}, {0, -1}}
 * という方向で表現しましょう。
 * directions[0], .., directions[3]でもOKです。
 */
point_t directions[] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

point_t move(point_t from, point_t direct) {
    point_t result = {from.x+direct.x, from.y+direct.y};
    return result;
}

/*****************
 * queue (汎用のつもり)
 */

#define BUFSIZE 900
#define ELEM point_t
/* 型は、利用用途に合わせて変えてください */

typedef struct queue {
    int head, tail, size;
    ELEM buf[BUFSIZE];
} queue_t, * queue_tp;

/****
 * queue の reset をおこなう
 * q: 対象 queue へのポインタ
 */
void reset(queue_tp q) {
    assert(q != NULL);
    q->head = q->tail = q->size = 0;
}
/****
 * queue のサイズを返す
 * q: 対象 queue へのポインタ
 * 返り値: queue のサイズ
 */
int qSize(queue_tp q) {
    assert(q != NULL);
    return q->size;
}
/****
 * queue への要素追加
 * q: 対象 queue へのポインタ
 * data: 追加をおこなう要素
 */
void enqueue(queue_tp q, ELEM data) {
    assert(q != NULL);
    assert(q->size < BUFSIZE);
    q->buf[q->tail++]=data;
    q->size++;
    if(q->tail==BUFSIZE) q->tail = 0;
}
/****
 * queue からの要素取り出し
 * q: 対象 queue へのポインタ
 * 返り値: 取得要素
 */
ELEM dequeue(queue_tp q) {
    assert(q != NULL);
    assert(q->size > 0);
    {
        ELEM result = q->buf[q->head++];
        q->size--;
        if(q->head==BUFSIZE) q->head = 0;
        return result;
    }
}

/**********************
 * 壁の処理（鎌田の方で作成済み）
 */

int canGoX[MAX_H+2][MAX_W+2];
int canGoY[MAX_H+2][MAX_W+2];

/*****
 * 迷図の各位置から指定された方向にすすめるかどうか。
 * from: 迷図上の位置, {0, 0} が起点、{w-1, h-1)がゴール
 * direct: 進む方向。{1, 0}, {-1, 0}, {0, 1}, {0, -1}のいずれか。
 * 返り値: 進める場合は 1, 進めない場合は 0
 */
int canGo(point_t from, point_t direct) {
    if(direct.y ==0) {
        if(direct.x>0) return canGoX[1+from.y][1+from.x];
        else return canGoX[1+from.y][1+from.x-1];
    } else { /* direct.x==0*/
        if(direct.y>0) return canGoY[1+from.y][1+from.x];
        else return canGoY[1+from.y-1][1+from.x];
    }
}
/* 迷図 setup用の関数、皆さんは見る必要はない */
void setupBoard(FILE * in, int w, int h) {
    int i=0;
    for(i=0; i< h+2; i++) {
        int j;
        for(j=0; j< w+2; j++) {
            canGoX[i][j]=0;
            canGoY[i][j]=0;
        }
    }
    i=0;
    while(1) {
        int j;
        for(j=0; j < w-1; j++) {
            int d;
            fscanf(in, "%d", &d);
            canGoX[1+i][1+j] = 1-d;
        }
        if(i+1 == h) break;
        for(j=0; j < w; j++) {
            int d;
            fscanf(in, "%d", &d);
            canGoY[1+i][1+j] = 1-d;
        }
        i++;
    }
}
/* 迷図 debug 用の関数、皆さんは見る必要はない */
void printBoard(int w, int h) {
    int i;
    for(i=0; i< h+2; i++) {
        int j;
        for(j=0; j< w+2; j++) {
            printf("%d ", canGoX[i][j]);
        }
        printf("\n");
    }
    printf("-----------\n");
    for(i=0; i< h+2; i++) {
        int j;
        for(j=0; j< w+2; j++) {
            printf("%d ", canGoY[i][j]);
        }
        printf("\n");
    }
}

/*********************
 * プログラム本体
 */
queue_t strQ;

/******
 * 各迷図に対する最短路を解く問題。``未完成``につき完成させること。
 * 迷図の情報は、canGo() 関数により取得すること。
 * w: 迷図の横幅 (問題文で定義)
 * h: 迷図の縦幅 (問題文で定義)
 * 返り値: 最短経路長 (問題文で定義)
 */
int solve(int w, int h) {
    point_t start = {0, 0};

    reset(&strQ);
    enqueue(&strQ, start);

    while(qSize(&strQ) > 0) {
        point_t here = dequeue(&strQ);
        /* debug print */
        printf("here: (%d, %d)\n", here.x, here.y); 
        /* TODO */
    }
    return 0;
}

/*******
 * こちらで用意したmain 関数。
 * 問題準備してから、solve() をよび、正解比較もおこなう。
 */
int main(int argc, char* argv[]) {
    
    struct {
        char * in, * ans;
    } filepairs[] = 
    /*{ {"sample.in", "sample.ans"}, { "B1.in", "B1.ans"}}; */
    { {"kadai2/sample.in", "kadai2/sample.ans"}, { "kadai2/B1.in", "kadai2/B1.ans"}};

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
            int w, h,result, ans;
            fscanf(in, "%d %d", &w, &h);
            if(w==0 && h==0) break;
            assert(w>0 && h>0);
            setupBoard(in, w, h);
            result = solve(w, h);
            fscanf(ansIn, "%d", &ans);
            if(ans!=result) {
                failCount++;
                printf("! You failed data No. %d (result: %d, ans: %d)\n", totalCount, result, ans);
            }
            totalCount++;
        }
        if(failCount==0) {
            printf("!! Congratulations! You passed all data (%d) in %s!\n", totalCount, inFile);
        } else {
            printf("! I'm sorry you missed %d/%d in %s!\n", failCount, totalCount, inFile);
            return 0;
        }
    }
    return 0;
}
    
