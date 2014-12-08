#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<iostream>
#include<map>
#include<set>
#include<stack>

#define SUPER 10000

using namespace std;

typedef struct edge edge;
typedef struct triangle tringle;
typedef struct point point;

struct edge{ 			//half-edge structure
  int index;
  
  point *s;   			//start point
  point *f;   			//end point

  edge *next;			//next ccw edge of triangle
  edge *prev;			//previous ccw edge of triangle

  edge *opp;			//opposite edge
}; 

struct point{			//point structure
  bool real;			//1 if part of point set, 0 if part of auxiliary triangle/structures
  
  double x;   			//x coordinate
  double y;   			//y chromossome
  double w;			//x^2+y^2. precalculated to speed circumference test
  
  set<int> edges;	//stack of edges STARTING AT the point
};

double ori(point p1, point p2, point p3){
  //returns cross product between p1p2 and p1p3
  return (p1.x - p3.x)*(p2.y - p3.y) - (p1.y - p3.y)*(p2.x - p3.x);
}

bool inTriangle(point p, edge e){
  //returns if point p is in the triangle that e is a part of
  return (ori(*e.s, *e.f, p) >= 0 && ori(*e.f, *e.next->f, p) >= 0 && ori(*e.prev->s, *e.s, p) >= 0);
}

bool inCircle(point p,edge e){
  point p1,p2,p3;
  p1=*e.s;
  p2=*e.f;
  p3=*e.next->f;
    
  return
    (p1.x - p.x) * (p2.y - p.y) * (p3.w - p.w) +
    (p1.y - p.y) * (p2.w - p.w) * (p3.x - p.x) +
    (p1.w - p.w) * (p2.x - p.x) * (p3.y - p.y) -
    (p1.w - p.w) * (p2.y - p.y) * (p3.x - p.x) -
    (p1.y - p.y) * (p2.x - p.x) * (p3.w - p.w) -
    (p1.x - p.x) * (p2.w - p.w) * (p3.y - p.y) >= 0;
}

int n_edges;
map<int,edge> E;

edge *createEdge(point *p1,point *p2){
  edge *e;
  e=new edge;
  e->index=++n_edges;
  e->s=p1;
  e->f=p2;
  E.insert(pair<int,edge>(n_edges,*e));
  p1->edges.insert(n_edges);
  return e;   
}

int makeEdge(edge *e,edge *f){
  //matches two half edges. points must be the opposite
  e->opp=f;
  f->opp=e;
  return 0;
}

int makeTriangle(edge *e,edge *f,edge *g){
  //matches three half edges into a ccw-oriented triangle  
  e->next=f;
  f->next=g;
  g->next=e;

  e->prev=g;
  g->prev=f;
  f->prev=e;
  return 0;
}

//TRIANGULATION FUNCTIONS

int findTriangle(point p,int i_start){
  edge e;
  double cx,cy;

  e=E[i_start];

  cout << e.next <<endl;
  
}

//MAIN FOR TESTING PURPOSES

int N; 			//total number of points
int K; 			//number of centroids
point *v;		//array of points

int readVector(){
  int i;
  cin >> N;
  cin >> K;
  v=new point[N];
  for(i=0;i<N;i++){
    cin >> v[i].x;
    cin >> v[i].y;
    v[i].w = v[i].x*v[i].x + v[i].y*v[i].y;
    v[i].real=true;
  }
}

point *st;		//super triangle, for accessing the triangulation

int initMesh(){
  st = new point[3];
  edge *e,*f,*g;
  
  st[0].x=-SUPER;
  st[0].y=-SUPER;
  st[0].w=SUPER*SUPER*2;
  
  st[1].x=SUPER;
  st[1].y=0;
  st[1].w=SUPER*SUPER;
  
  st[2].x=0;
  st[2].y=SUPER;
  st[2].w=SUPER*SUPER;
  
  e=createEdge(&st[0],&st[1]);
  f=createEdge(&st[1],&st[2]);
  g=createEdge(&st[2],&st[0]);

  makeTriangle(e,f,g);
  
  return 0;  
}

int main(){
  
  readVector();
  initMesh();

  cout << E[*st[0].edges.begin()].next <<endl;
  
  findTriangle(v[0],*st[0].edges.begin());
  
  return 0;
}






