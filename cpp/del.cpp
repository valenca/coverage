#include<cstdlib>
#include<iostream>
#include<cmath>
#include<set>
#include<stack>
#include"del.h"


using namespace std;

int n_triangles;
triangle * last_t;			//last triangle created

map< pair<int,int> , triangle * > E;	//hashtable of half-edges

stack< triangle* > s_l_tri;		//stack of last triangle created
stack<int> c_edg_in;			//stack of number of h.edges created last
stack<int> c_edg_out;			//stack of number of h.edges removed last
stack<map_entry> s_edg_in;		//stack of half edges created from mesh
stack<map_entry> s_edg_out;		//stack of half edges removed from mesh

point s1,s2,s3,s4;

stack<triangle*> tri_pool;

//HILBERT FUNCTIONS
//convert (x,y) to d

int xy2d(int n,int x,int y){
  int rx,ry,s,d=0;
  for(s=n/2;s>0;s/=2){
    rx=(x&s)>0;
    ry=(y&s)>0;
    d+=s*s*((3*rx)^ry);
    rot(s,&x,&y,rx,ry);
  }
  return d;
}
 
//convert d to (x,y)
void d2xy(int n,int d,int *x,int *y){
  int rx,ry,s,t=d;
  *x=*y=0;
  for(s=1;s<n;s*=2){
    rx=1&(t/2);
    ry=1&(t^rx);
    rot(s,x,y,rx,ry);
    *x+=s*rx;
    *y+=s*ry;
    t/=4;
  }
}
 
//rotate/flipaquadrantappropriately
void rot(int n,int *x,int *y,int rx,int ry){
  if(ry==0){
    if(rx==1){
      *x=n-1-*x;
      *y=n-1-*y;
    }

    int t=*x;
    *x=*y;
    *y=t;
  }
}


//GEOMETRIC FUNCTIONS

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

double euclidean(point &p,point &q){
  return sqrt(((p.x-q.x)*(p.x-q.x))+((p.y-q.y)*(p.y-q.y)));
}


//TRIANGLE MANIPULATION
triangle * makeTriangle(point &p1,point &p2, point &p3){
  triangle *t;
  edge e1,e2,e3;

  if(tri_pool.empty()){
    triangle * pool_space;
    int i;
    pool_space=new triangle[N];
    cout << "more triangles" <<endl;
    for(i=0;i<N;i++){
      tri_pool.push(&pool_space[i]);
    }
  }  
  
  t=tri_pool.top();
  tri_pool.pop();  

  t->fake=false;
  t->v1=&p1;
  t->v2=&p2;
  t->v3=&p3;
  t->index=n_triangles++;
  
  e1=edge(p1.index,p2.index);
  e2=edge(p2.index,p3.index);
  e3=edge(p3.index,p1.index);

  E.insert(map_entry(e1,t));
  E.insert(map_entry(e2,t));
  E.insert(map_entry(e3,t));

  //update individual points
  p1.nbors.insert(p2.index);
  p2.nbors.insert(p3.index);
  p3.nbors.insert(p1.index);
  
  //save state
  s_edg_in.push(map_entry(e1,t));
  s_edg_in.push(map_entry(e2,t));
  s_edg_in.push(map_entry(e3,t));
  c_edg_in.top()++;
  
  last_t=t;
  s_l_tri.top()=t;
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

  //update individual points
  p1->nbors.erase(p1->nbors.find(p2->index));
  p2->nbors.erase(p2->nbors.find(p3->index));
  p3->nbors.erase(p3->nbors.find(p1->index));
    
  //save state
  s_edg_out.push(map_entry(e1,&t));
  s_edg_out.push(map_entry(e2,&t));
  s_edg_out.push(map_entry(e3,&t));
  c_edg_out.top()++;
  
  return false;
}

//MESH MANIPULATION
bool loadState(){
  triangle *t;
  bool a,b,c;
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

    tri_pool.push(t);
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

  s_l_tri.pop();
  last_t=s_l_tri.top();
  
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
  if(q.index > N && r.index > N){
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

int insertPoint(point &p){
  triangle *f;
  point *p1,*p2,*p3;

  c_edg_in.push(0);
  c_edg_out.push(0);
  s_l_tri.push(NULL);

  f = findTriangle(p,*last_t);
  p1=f->v1;
  p2=f->v2;
  p3=f->v3;
  deleteTriangle(*f);
  
  digCavity(p,*p2,*p1);
  digCavity(p,*p3,*p2);
  digCavity(p,*p1,*p3);
  return 0;
}

void initMesh(double min_x,double max_x,double min_y,double max_y){
  //initiate mesh with super rectangle containing all points
  triangle *t,*f;
  s1.x=-2*max_x-1;
  s1.y=-2*max_y-1;  
  s1.w=(s1.x*s1.x)+(s1.y*s1.y);
  s1.index=N+1;
  
  s2.x=2*max_x+1;
  s2.y=-2*max_y-1;
  s2.w=(s2.x*s2.x)+(s2.y*s2.y);
  s2.index=N+2;
  
  s3.x=-2*max_x-1;
  s3.y=2*max_x+1;
  s3.w=(s3.x*s3.x)+(s3.y*s3.y);
  s3.index=N+3;
  
  s4.x=2*max_x+1;
  s4.y=2*max_y+1;
  s4.w=(s4.x*s4.x)+(s4.y*s4.y);
  s4.index=N+4;
  
  //add fake triangles to outer edges of the rectangle
  f=new triangle;
  f->index=-1;
  f->fake=true;
  
  E.insert(map_entry(edge(s2.index,s1.index),f));
  E.insert(map_entry(edge(s1.index,s3.index),f));
  E.insert(map_entry(edge(s4.index,s2.index),f));
  E.insert(map_entry(edge(s3.index,s4.index),f));

  c_edg_in.push(0);
  s_l_tri.push(NULL);
  
  t=makeTriangle(s2,s4,s3);  
  t=makeTriangle(s1,s2,s3);
  return;  
}
