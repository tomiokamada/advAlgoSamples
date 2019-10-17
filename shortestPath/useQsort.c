#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct point { /* 座標 */
    int x, y;
} point_t, * point_tp;


/* 比較用関数。a, b アドレスにある座標データを比較する。
   x 座標が小さいものが前で、x座標が同じ場合は、y座標で比較 */
int compare0(const void * a, const void * b) {
    /* ポインタ型を、本来の point_tp に変換しよう */
    point_tp ap = (point_tp)a;
    point_tp bp = (point_tp)b;
    int result = ap->x - bp->x;
    if(result==0) return ap->y-bp->y;
    return result;
}
int main(void) {
    int i;
    /* ポイントをソートしよう */
    point_t ps[] = {{1,3}, {12,4}, {5,2}, {7, 8}};
    /* qsort 呼出し */
    qsort(&ps, 4, sizeof(point_t), compare0);
    /* 表示してみる */
    for(i=0; i<4; i++) {
        printf("(%d %d), ", ps[i].x, ps[i].y);
    }
    printf("\n");
}
