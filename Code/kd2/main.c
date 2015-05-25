#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<limits.h>
#include<time.h>

#define max(a,b) ((a)>(b))?(a):(b)
#define min(a,b) ((a)<(b))?(a):(b)

typedef struct point point;
typedef struct link link;

struct point{
  int id;
  double x;
  double y;

  point* lft;
  point* rgt;  
};

struct link{
  link* next;
  link* prev;
  link* mirr;
  int id;
};

int N;	 
int K;

double thres;
double fact;

point *v;
link **heads;
link *pool;
int p;
int C;
int *adj;
int *ord;

double max_x;
double max_y;
double min_x;
double min_y;

clock_t T;

int compx(const void *p1, const void *p2){
  const point *elem1 = p1;    
  const point *elem2 = p2;

  return (elem1->x < elem2->x);
}

int compy(const void *p1, const void *p2){
  const point *elem1 = p1;    
  const point *elem2 = p2;

  return (elem1->y < elem2->y);
}

void rangeSearchY(point *root, point *tgt, double x1,double x2,double y1,double y2);
void rangeSearchX(point *root, point *tgt, double x1,double x2,double y1,double y2);

double euclidean(point p,point q){
  return sqrt(((p.x-q.x)*(p.x-q.x))+((p.y-q.y)*(p.y-q.y)));
}

void swap(point *x, point *y) {
  double tt;

  tt=x->x;
  x->x=y->x;
  y->x=tt;

  tt=x->y;
  x->y=y->y;
  y->y=tt;

  return;
}

point *findMedian(point *start, point *end, int idx){
  point *p, *store, *md = start + (end - start) / 2;
  double pivot;
  if (end <= start) return NULL;
  if (end == start + 1)
    return start;
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
  if(tgt->id<root->id){
    if(root->x>=x1 && root->x<=x2 && root->y>=y1 && root->y<=y2){
      if(euclidean(*tgt,*root)<=thres){
	if(p>C-2){
	  pool=(link*)malloc(C*sizeof(link));
	  p=0;
	}
	pool[p].next=heads[root->id];
	heads[root->id]->prev=&pool[p];
	heads[root->id]=heads[root->id]->prev;
	heads[root->id]->id=root->id;
	p++;

	pool[p].next=heads[tgt->id];
	heads[tgt->id]->prev=&pool[p];
	heads[tgt->id]=heads[tgt->id]->prev;
	heads[tgt->id]->id=tgt->id;
	p++;

	heads[root->id]->mirr=heads[tgt->id];
	heads[tgt->id]->mirr=heads[root->id];

	adj[root->id]++;
	adj[tgt->id]++;
      }
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
  if(tgt->id<root->id){
    if(root->x>=x1 && root->x<=x2 && root->y>=y1 && root->y<=y2){
      if(euclidean(*tgt,*root)<thres){
	if(p>C-2){
	  pool=(link*)malloc(C*sizeof(link));
	  p=0;
	}
	pool[p].next=heads[root->id];
	heads[root->id]->prev=&pool[p];
	heads[root->id]=heads[root->id]->prev;
	heads[root->id]->id=root->id;
	p++;

	pool[p].next=heads[tgt->id];
	heads[tgt->id]->prev=&pool[p];
	heads[tgt->id]=heads[tgt->id]->prev;
	heads[tgt->id]->id=tgt->id;
	p++;
	
	heads[root->id]->mirr=heads[tgt->id];
	heads[tgt->id]->mirr=heads[root->id];

	adj[root->id]++;
	adj[tgt->id]++;
      }
    }
  }
  return;
}

void printTree(point *rt,int l){
  int i;
  if(rt){
    printTree(rt->lft,l+1);
    for(i=0;i<l;i++)
      printf("\t");
    printf("%d:%g,%g",rt->id,rt->x,rt->y);
    printTree(rt->rgt,l+1);
  }
}

int readVector(){
  int i;
  point *root;

  scanf("%d%lf",&N,&fact);

  v=(point*)malloc(sizeof(point)*N);
  adj=(int*)calloc(N,sizeof(int));
  ord=(int*)calloc(N,sizeof(int));
  C=N*N/5;
  T=clock();
  pool=(link*)malloc(C*sizeof(link));
  heads=(link**)malloc(N*sizeof(link*));
  max_x=max_y=0;
  min_x=min_y=INT_MAX;


  for(i=0;i<N;i++){
    scanf("%*d %lf %lf",&v[i].x,&v[i].y);
    adj[i]=0;
    max_x=max(v[i].x,max_x);
    max_y=max(v[i].y,max_y);
    min_x=min(v[i].x,min_x);
    min_y=min(v[i].y,min_y);
  }

  fact=0.1;
  
  if((max_x-min_x)>max_y-min_y)
    thres=(max_x-min_x)*fact;
  else
    thres=(max_y-min_y)*fact;

  root=makeTree(v,N,0);
  /*printTree(root,0);*/
  for(i=0;i<N;i++)
    v[i].id=i;

  for(i=0;i<N;i++)
    rangeSearchX(root,&v[i],v[i].x-(thres),v[i].x+(thres),v[i].y-(thres),v[i].y+(thres));
  return 0;
}


void coverage(){
  int i,j,m;
  link* it;
  i=m=0;
  while(1){
    m=-1;
    for(j=0;j<N;j++){
      if(m<adj[j]){
	m=adj[j];
	i=j;
      }
    }
    if(m!=-1){
      K++;
      ord[i]=K;
      
      for(it=heads[i];it && it->next;it=it->next){
	adj[it->mirr->id]=-1;
	if(!it->mirr->prev){
	  heads[it->mirr->id]=it->next;
	  heads[it->mirr->id]->prev=NULL;
	}
	else{
	  it->mirr->prev->next=it->mirr->next;
	}
	it->mirr->next->prev=it->mirr->prev;	
	adj[it->mirr->id]--;
	adj[it->id]=-1;
      }      
      heads[i]=NULL;
      adj[i]=-1;
      
    }
    else
      break;
  }
}


int main(){
  int i,j;
  readVector();
  
  printf("%d\n",N);
  for(i=0;i<N;i++)
    printf("%g %g\n",v[i].x,v[i].y);
  coverage();
  
  T = clock() - T;
  printf("%d\n",K);
  for(j=1;j<=K;j++)
    for(i=N;i>=0;i--)
      if(ord[i]==j)
	printf("%g %g\n",v[i].x,v[i].y);
  
  printf("%g\n",thres);

  printf("%d\t%g\t%d\t%g\n",N,fact,K,((float)T)/CLOCKS_PER_SEC);
  return 0;
}
