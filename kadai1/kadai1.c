#include <stdio.h>

#define BUFSIZE 100

typedef struct node {
    struct node * s; 
    struct node * t;
    int id;
    int visited;
} node_t, * node_tp;

void printNode(node_tp node) {
    printf("(%d, %d)\n", node->id, node->visited);
    fflush(stdout);
}

node_t nodes[BUFSIZE]; /* 大域変数（配列） */

void initNodes(int n) {
    int i;
    for(i = 0; i<n; i++) {
        nodes[i].id = i; nodes[i].visited = 0;
        nodes[i].s = nodes[i].t = NULL;
    }
}

void link(node_tp node, node_tp s, node_tp t) {
    node->s = s; node->t = t;
}

void dfs(node_tp node) {
    node->visited++;
    printNode(node);
    if(node->visited == 1) {
        node_tp s = node->s;
        node_tp t = node->t;
        if(s != NULL) dfs(s);
        if(t != NULL) dfs(t);
    }
}

void test1(void) { /* 2021 version */
    initNodes(4);
    link(&nodes[0], &nodes[2], &nodes[1]);
    link(&nodes[1], &nodes[2], &nodes[3]);
    link(&nodes[2], NULL, &nodes[3]);
    link(&nodes[3], &nodes[0], NULL);
    dfs(&nodes[0]); /* ノード 0 から探索 */
}

void test2(void) {
    int i;
    initNodes(12);
    for(i=0; i<10; i++) 
        link(&nodes[i], &nodes[i+1], &nodes[i+2]);
    dfs(&nodes[0]); /* ノード 0 から探索 */
}

int main(void)  {
    test1();
    /* test2(); */
    return 0;
}
