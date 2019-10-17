#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <float.h>

/************************************************
 * Test Data
 * まずは、priority queue の動作テスト用要素
 */

typedef struct myitem
{
  double priority; /* 優先度、低いほうが優先ってことにします */
  int id;          /* 要素識別用の番号 */
} myitem_t, *myitem_tp;

void printItem(myitem_tp elem)
{
  printf("[%f,%d]", elem->priority, elem->id);
  fflush(stdout);
}

/***********************************************
 * priority queue (汎用のつもり)
 */

#define BUFSIZE 900
/* ELEM は要素の型を表す。利用用途に合わせて型名を変えてください
 * ちなみに、今回は item_t (= struct myitem) を要素にする */
#define ELEM myitem_t

/***
 * 比較機(comparator)のプロトタイプ宣言
 * 比較演算はデータ構造に合わせて定義すること
 * a,b: 要素へのポインタ
 * 返り値： aとb が同じ大きさなら0,
 *          [a, b] の順で並べるべきなら負の値、
 *          [b, a] の順で並べるべきなら正の値を返す
 * 例：要素が整数値なら、 return *a - *b; で OK.
 */
int compare(ELEM *a, ELEM *b);

typedef struct priorityQ
{
  int size;
  ELEM buf[BUFSIZE];
} priorityQ_t, *priorityQ_tp;

void reset(priorityQ_tp q)
{
  assert(q != NULL);
  q->size = 0;
}

int qSize(priorityQ_tp q)
{
  assert(q != NULL);
  return q->size;
}

void enqueue(priorityQ_tp q, ELEM data)
{
  assert(q != NULL);
  assert(q->size < BUFSIZE);
  { /* upheap */
    int target = q->size++;
    while (target != 0)
    {
      int parent = (target - 1) / 2;
      ELEM *pref = &q->buf[parent];
      if (compare(pref, &data) > 0)
      {
        q->buf[target] = *pref;
        target = parent;
      }
      else
      {
        break;
      }
    }
    q->buf[target] = data;
  }
}

ELEM dequeue(priorityQ_tp q)
{
  assert(q != NULL);
  assert(q->size > 0);
  {
    ELEM result = q->buf[0];
    q->size--;
    ELEM *moved = &q->buf[q->size];
    { /* downheap */
      int target = 0;
      while (1)
      {
        int child = target * 2 + 1;
        if (child >= q->size)
          break;
        if (child + 1 != q->size &&
            compare(&q->buf[child],
                    &q->buf[child + 1]) > 0)
        {
          child = child + 1;
        }
        if (compare(moved, &q->buf[child]) > 0)
        {
          q->buf[target] = q->buf[child];
          target = child;
        }
        else
        {
          break;
        }
      }
      q->buf[target] = *moved;
      return result;
    }
  }
}
void printQueueInside(priorityQ_tp p)
{
  int i;
  printf("[priorityQ, size: %d, body:{", p->size);
  for (i = 0; i < p->size; i++)
  {
    if (i != 0)
      printf(", ");
    printItem(&p->buf[i]);
  }
  printf("}]\n");
}

/*
 * ここまで、優先度キューのライブラリ
 *************************************/

/*********************
 * ここから、テストコード。
 * 上記で宣言された 比較機を実装しましょう。
 * (以下、再掲)
 * 比較機(comparator)のプロトタイプ宣言
 * 比較演算はデータ構造に合わせて定義すること
 * a,b: 要素へのポインタ
 * 返り値： aとb が同じ大きさなら0,
 *          [a, b] の順で並べるべきなら負の値、
 *          [b, a] の順で並べるべきなら正の値を返す
 * 例：要素が整数値なら、 return *a - *b; で OK.
 */
int compare(myitem_tp a, myitem_tp b) {
  /* まずは、優先度の比較 */
  int ap = a->priority;
  int bp = b->priority;
  if(ap<bp) return -1;
  if(ap>bp) return 1;
  /* 勝負がつかなかったときは、idの小さい順にしましょう。 idが一致することはないって前提 */
  if(a->id<b->id) return -1;
  if(a->id>b->id) return 1;
  return 0;
}

/****************************************************
 * test 系
 */

priorityQ_t Q;

int main(void) {
  int i;
  myitem_t items[] = {
    { 100.0, 0 }, /* priority, id */
    {  10.0, 1 },
    {  20.0, 2 },
    {  10.0, 3 },
    { 100.0, 4 },
    {   5.5, 5 },
    {   3.0, 6 },
    {  30.0, 7 }
  };
  for(i=0; i<4; i++) {
    enqueue(&Q, items[i]);
  }
  printQueueInside(&Q);
  for(i=4; i<8; i++) {
    myitem_t r = dequeue(&Q);
    printf("dequeue (2nd stage):"); printItem(&r);printf("\n");
    enqueue(&Q, items[i]);
  }
  while(qSize(&Q)>0) {
    myitem_t r = dequeue(&Q);
    printf("dequeue (3rd stage):"); printItem(&r);printf("\n");
  }
  printf("finish!");
  return 0;
}
