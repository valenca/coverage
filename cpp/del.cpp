#include<stdio.h>
#include<cstdlib>
#include<iostream>
#include<map>
#include<set>
#include<stack>
#include<ctime>

#define SUPER 10000

using namespace std;

typedef struct triangle tringle;
typedef struct point point;
typedef pair<int,int> edge;

typedef set< int >::iterator 	 set_it;
typedef map< pair<int,int>,triangle *>::iterator map_it;

struct triangle{ 			//half-edge str
  point *v1;
  point *v2;
  point *v3;

  int index;
  
  bool fake;
};

struct point{				//point structure
  bool real;				//1 if part of point set, 0 if part of auxiliary triangle/structures
  int index;
  
  double x;   				//x coordinate
  double y;   				//y chromossome
  double w;				//x^2+y^2. precalculated to speed circumference test
  
  set<int> nbors;	
};

int N;	 				//total number of points
int K; 					//number of centroids
point *v;				//array of points
point *st;				//super triangle, for accessing the triangulation
int n_triangles;
map< pair<int,int> , triangle * > E;	//hashtable of

stack< pair< pair<int,int>, triangle*> > s_edg_out;
stack<int> c_edg_out;
stack< pair< pair<int,int>, triangle*> > s_edg_in;
stack<int> c_edg_in;

double ori(point &p1, point &p2, point &p3){
  //returns cross product between p1p2 and p1p3
  return
    (p1.x - p3.x)*(p2.y - p3.y) -
    (p1.y - p3.y)*(p2.x - p3.x);
}

bool intersect(point &p1, point &q1, point &p2,point &q2){
  //checks if line segment p1q1 intersects p2q2 using cross products
  return (ori(p1,q1,p2)>0 != ori(p1,q1,q2)>0 &&
	  ori(p2,q2,p1)>0 != ori(p2,q2,q1)>0 );
}

bool inTriangle(point &p, triangle &t){
  //returns if point p is in the triangle that e is a part of
  return (ori(*t.v1, *t.v2, p) >= 0 && ori(*t.v2, *t.v3, p) >= 0 && ori(*t.v3, *t.v1, p) >= 0);
}

double inCircle(point &p,point &q,point &r,point &s){    
  return
    (q.x - p.x) * (r.y - p.y) * (s.w - p.w) +
    (q.y - p.y) * (r.w - p.w) * (s.x - p.x) +
    (q.w - p.w) * (r.x - p.x) * (s.y - p.y) -
    (q.w - p.w) * (r.y - p.y) * (s.x - p.x) -
    (q.y - p.y) * (r.x - p.x) * (s.w - p.w) -
    (q.x - p.x) * (r.w - p.w) * (s.y - p.y);
}

point * adjacent(point &p, point&q){
  triangle *t=E[make_pair(p.index,q.index)];
  if (t->v1->index != p.index && t->v1->index != q.index){
    return t->v1;
  }
  if (t->v2->index != p.index && t->v2->index != q.index){
    return t->v2;
  }
  return t->v3;
}

triangle * makeTriangle(point &p1,point &p2, point &p3){
  triangle *t;
  edge e1,e2,e3;
  t=new triangle;

  t->fake=false;
  t->v1=&p1;
  t->v2=&p2;
  t->v3=&p3;
  t->index=n_triangles++;
  
  e1=edge(p1.index,p2.index);
  e2=edge(p2.index,p3.index);
  e3=edge(p3.index,p1.index);

  E.insert(pair<pair<int,int>,triangle*>(e1,t));
  E.insert(pair<pair<int,int>,triangle*>(e2,t));
  E.insert(pair<pair<int,int>,triangle*>(e3,t));

  p1.nbors.insert(p2.index);
  p2.nbors.insert(p3.index);
  p3.nbors.insert(p1.index);

  
  //save state
  s_edg_in.push(pair<pair<int,int>,triangle*>(e1,t));
  s_edg_in.push(pair<pair<int,int>,triangle*>(e2,t));
  s_edg_in.push(pair<pair<int,int>,triangle*>(e3,t));
  c_edg_in.top()++;
  
  return t;
}


bool deleteTriangle(triangle &t){
  point *p1,*p2,*p3;
  edge e1,e2,e3;
  p1=t.v1;
  p2=t.v2;
  p3=t.v3;

  e1=edge(p1->index,p2->index);
  e2=edge(p2->index,p3->index);
  e3=edge(p3->index,p1->index);
  
  E.erase(E.find(e1));
  E.erase(E.find(e2));
  E.erase(E.find(e3));

  p1->nbors.erase(p1->nbors.find(p2->index));
  p2->nbors.erase(p2->nbors.find(p3->index));
  p3->nbors.erase(p3->nbors.find(p1->index));
    
  //save state
  s_edg_out.push(pair<pair<int,int>,triangle*>(e1,&t));
  s_edg_out.push(pair<pair<int,int>,triangle*>(e2,&t));
  s_edg_out.push(pair<pair<int,int>,triangle*>(e3,&t));
  c_edg_out.top()++;
  return false;
}

bool loadState(){
  triangle *t;
  for(int i=0;i<c_edg_in.top();i++){
    t=s_edg_in.top().second;
    t->v1->nbors.erase(t->v1->nbors.find(t->v2->index));
    t->v2->nbors.erase(t->v2->nbors.find(t->v3->index));
    t->v3->nbors.erase(t->v3->nbors.find(t->v1->index));
    E.erase(E.find(s_edg_in.top().first));
    s_edg_in.pop();
    E.erase(E.find(s_edg_in.top().first));
    s_edg_in.pop();
    E.erase(E.find(s_edg_in.top().first));
    s_edg_in.pop();
  }
  c_edg_in.pop();
  for(int i=0;i<c_edg_out.top();i++){
    t=s_edg_out.top().second;
    t->v1->nbors.insert(t->v2->index);
    t->v2->nbors.insert(t->v3->index);
    t->v3->nbors.insert(t->v1->index);
    E.insert(s_edg_out.top());
    s_edg_out.pop();
    E.insert(s_edg_out.top());
    s_edg_out.pop();
    E.insert(s_edg_out.top());
    s_edg_out.pop();
  }
  c_edg_out.pop();
  return false;
}

triangle *findTriangle(point &p,triangle &t){
  point m,*v1,*v2,*v3,*tmp;
  bool first=true;

  v1=t.v1;
  v2=t.v2;
  v3=t.v3;
  
  m.x=(v1->x+v2->x+v3->x)/3;
  m.y=(v1->y+v2->y+v3->y)/3;
  
  while(inCircle(p,*v1,*v2,*v3)<=0){    
    if (first && intersect(p,m,*v1,*v2)){
      tmp=v1;
      v1=v2;
      v2=tmp;
    }
    else if (intersect(p,m,*v2,*v3)){
      v1=v3;
    }
    else{
      v2=v3;
    }
    v3=adjacent(*v1,*v2);
    first=false;
  }
  return E[edge(v1->index,v2->index)];
}

int digCavity(point &p,point &q, point &r){
  point *s;
  if(q.index >=N && r.index>=N){
    makeTriangle(p,r,q);
    return 0;
  }
  if(E.count(edge(q.index,r.index))>=0){
    s=adjacent(q,r);
    if( inCircle(p,q,r,*s)>0){
      deleteTriangle(*E[edge(q.index,r.index)]);
      digCavity(p,q,*s);
      digCavity(p,*s,r);
    }
    else{
      makeTriangle(p,r,q);
    }
  }
  return 0;
}

int insertPoint(point &p,triangle &t){
  triangle *f;
  point *p1,*p2,*p3;

  c_edg_in.push(0);
  c_edg_out.push(0);
  
  f = findTriangle(p,t);
  p1=f->v1;
  p2=f->v2;
  p3=f->v3;
  deleteTriangle(*f);
  
  digCavity(p,*p2,*p1);
  digCavity(p,*p3,*p2);
  digCavity(p,*p1,*p3);
  return 0;
}
//MAIN FOR TESTING PURPOSES

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
    v[i].index=i;
  }
}

triangle * initMesh(point &s1,point &s2,point &s3){
  triangle *t,*f;
  s1.x=-1;
  s1.y=-1;  
  s1.w=1;
  s1.index=N+1;
  
  s2.x=SUPER;
  s2.y=0;
  s2.w=SUPER*SUPER;
  s2.index=N+2;
  
  s3.x=0;
  s3.y=SUPER;
  s3.w=SUPER*SUPER;
  s3.index=N+3;

  //outside edges with a fake triangle
  
  f=new triangle;
  f->index=-1;
  f->fake=true;
  
  E.insert(pair<pair<int,int>,triangle*>(edge(s2.index,s1.index),f));
  E.insert(pair<pair<int,int>,triangle*>(edge(s3.index,s2.index),f));
  E.insert(pair<pair<int,int>,triangle*>(edge(s1.index,s3.index),f));

  c_edg_in.push(0);
  
  t=makeTriangle(s1,s2,s3);

  return t;  
}

int main(){
  int i,c;
  triangle *t;
  point p,s1,s2,s3;
  
  readVector();
  t=initMesh(s1,s2,s3);
  
  for(i=0;i<N;i++)
    insertPoint(v[i],*E.begin()->second);
  for(i=0;i<N;i++)
    loadState();
  cout << E.size() <<endl;
  for(i=0;i<N;i++)
    insertPoint(v[i],*E.begin()->second);
  for(i=0;i<N;i++)
    loadState();
  cout << E.size() <<endl;
  
  return 0;
}






