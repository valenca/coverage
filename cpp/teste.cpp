#include<stdio.h>
#include<stdlib.h>

typedef struct edge{
  struct triangle *t_rgt;
  struct triangle *t_lft;

  struct point *a;
  struct point *b;

  struct edge *e_ccw;
  struct edge *e_cw;
}edge;

typedef struct triangle{
  struct edge e[3];
  
}triangle;

typedef struct point{
  double x;
  double y;
  struct edge edges[100];
  int q;
  
}point;

edge * createEdge(){
  edge e;
  e.a=(point*)malloc(1*sizeof(point));
  return *e;
}

int main(){
  edge e;
  e.a=(point*)malloc(1*sizeof(point));
  e.a->x=1;
  e.a->y=2;
  printf("%f %f\n",e.a->x,e.a->y);
  return 0;
}
