#include<cstdlib>
#include<iostream>
#include<iomanip>
#include<cmath>
#include<climits>
#include<algorithm>
#include<vector>


using namespace std;

typedef struct point point;
typedef struct kd_node kd_node;

struct point{			
  int index;
  int adj;
  
  int c;
  double x;
  double y;

  point* lft;
  point* rgt;  
};

int N;	 
int K;

double thres;
double fact;

point *v;
char ** link;

int *w;
int c;

double max_x;
double max_y;
double min_x;
double min_y;

clock_t T;

bool compx(point i,point j) { return (i.x<j.x); }
bool compy(point i,point j) { return (i.y<j.y); }

void rangeSearchY(point *root, point *tgt, double x1,double x2,double y1,double y2);
void rangeSearchX(point *root, point *tgt, double x1,double x2,double y1,double y2);

double euclidean(point &p,point &q){
  return sqrt(((p.x-q.x)*(p.x-q.x))+((p.y-q.y)*(p.y-q.y)));
}

void swap(point *x, point *y) {
  int t;
  double tt;
  
  tt=x->x;
  x->x=y->x;
  y->x=tt;
  
  tt=x->y;
  x->y=y->y;
  y->y=tt;

  t=x->index;
  x->index=y->index;
  y->index=t;
  
  return;
}

point *findMedian(point *start, point *end, int idx){
  if (end <= start) return NULL;
  if (end == start + 1)
    return start;
  point *p, *store, *md = start + (end - start) / 2;
  double pivot;
  if(idx==0){
    while (1) {
      pivot = md->x;
 
      swap(md, end - 1);
      for (store = p = start; p < end; p++) {
	if (p->x < pivot) {
	  if (p != store)
	    swap(p, store);
	  store++;
	}
      }
      swap(store, end - 1);
 
      if (store->x == md->x)
	return md;
 
      if (store > md)	end = store;
      else		start = store;
    }
  }
  else{
    while (1) {
      pivot = md->y;
 
      swap(md, end - 1);
      for (store = p = start; p < end; p++) {
	if (p->y < pivot) {
	  if (p != store)
	    swap(p, store);
	  store++;
	}
      }
      swap(store, end - 1);
 
      /* median has duplicate values */
      if (store->y == md->y)
	return md;
 
      if (store > md)	end = store;
      else		start = store;
    }
  }
}

point *makeTree(point *t, int len, int i){
  point *n;
 
  if (!len) return 0;
 
  if ((n = findMedian(t, t + len, i))) {
    i^=1;
    n->lft = makeTree(t, n - t, i);
    n->rgt = makeTree(n + 1, t + len - (n + 1), i);
  }
  return n;
}

void rangeSearchX(point *root, point *tgt, double x1,double x2,double y1,double y2){
  if(!root)
    return;
  if(root->x>=x1)
    rangeSearchY(root->lft, tgt, x1, x2, y1, y2);
  if(root->x<=x2)
    rangeSearchY(root->rgt, tgt, x1, x2, y1, y2);
  if(root->x>=x1 && root->x<=x2 && root->y>=y1 && root->y<=y2){
    if(euclidean(*tgt,*root)<=thres){
      c++;
      link[root->index][tgt->index]=1;
      link[tgt->index][root->index]=1;
      root->adj++;
      //tgt->adj++;
    }
  }
  return;
}

void rangeSearchY(point *root, point *tgt, double x1,double x2,double y1,double y2){
  if(!root)
    return ;
  if(root->y>=y1)
    rangeSearchX(root->lft, tgt, x1, x2, y1, y2);
  if(root->y<=y2)
    rangeSearchX(root->rgt, tgt, x1, x2, y1, y2);
  if(root->x>=x1 && root->x<=x2 && root->y>=y1 && root->y<=y2){
    if(euclidean(*tgt,*root)<thres){
      c++;
      link[root->index][tgt->index]=1;
      link[tgt->index][root->index]=1;
      root->adj++;
      //tgt->adj++;
    }
  }
  return;
}

void printTree(point *rt,int l){
  if(rt){
    printTree(rt->lft,l+1);
    for(int i=0;i<l;i++)
      cout << "\t";
    cout << rt->index<<":"<<rt->x<<","<<rt->y << endl;
    printTree(rt->rgt,l+1);
  }
}

int readVector(){
  int i,j,it;
  double mx,mn;
  point *root;
  cin >> N;
  cin >> fact;
  
  K=0;
  v=new point[N]; 
  max_x=max_y=0;
  min_x=min_y=INT_MAX;
  
  for(i=0;i<N;i++){
    cin >> v[i].index;
    cin >> v[i].x;
    cin >> v[i].y;
    v[i].c=0;
    v[i].adj=0;
    max_x=max(v[i].x,max_x);
    max_y=max(v[i].y,max_y);
    min_x=min(v[i].x,min_x);
    min_y=min(v[i].y,min_y);
  }

  if((max_x-min_x)>max_y-min_y)
    thres=(max_x-min_x)*fact;
  else
    thres=(max_y-min_y)*fact;
  
  link = new char*[N+1];
  for(i=0;i<N;i++){
    link[i]=new char[N+1];
  }
  
  T=clock();  
  root=makeTree(v,N,0);
  //printTree(root,0);
  for(i=0;i<N;i++)
    v[i].index=i;
  
  for(i=0;i<N;i++)
    rangeSearchX(root,&v[i],v[i].x-(thres),v[i].x+(thres),v[i].y-(thres),v[i].y+(thres));
}

void coverage(){
  int i,j,k,c;
  double m;
  m=0;
  while(1){
    m=-1;
    for(j=0;j<N;j++){
      if(m<v[j].adj){
	m=v[j].adj;
	i=j;
      }
    }
        
    if(m!=-1){
      K++;
      v[i].c=K;
      for(j=0;j<N;j++){
	if(link[i][j]==1){
	  for(k=0;k<N;k++){
	    if(link[j][k]==1){
	      link[k][j]=0;
	      v[k].adj--;
	    }
	  }
	  v[j].adj=-1;
	}
      }
    }
    else
      break;
  }
}

int main(){
  int i,j;
  readVector();
  cout << N << endl;
  
  /*
  for(i=0;i<N;i++){
    for(j=0;j<N;j++){
      cout << link[i][j];
    }
    cout << endl;
  }
  */
  for(i=0;i<N;i++)
    cout << v[i].x << " " << v[i].y << endl; //" - " << v[i].adj <<endl;
    //cout << v[i].index << ":" << v[i].x << " " << v[i].y << "  --  " << v[i].adj << endl;
  
  coverage();
  
  T = clock() - T;
  cout << K << endl;
  
  for(j=1;j<=K;j++)
    for(i=N;i>=0;i--)
      if(v[i].c==j)
	cout << v[i].x << " " << v[i].y << endl; //" - " << v[i].adj <<endl;
  cout << thres << endl;
  printf("%d\t%g\t%d\t%f\n",N,fact,K,((float)T)/CLOCKS_PER_SEC);
  return 0;
}
