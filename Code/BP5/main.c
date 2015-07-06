#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<limits.h>
#include<time.h>

#define max(a,b) ((a)>(b))?(a):(b)
#define min(a,b) ((a)<(b))?(a):(b)
#define DEBUGo 1
#define SKIP 20

typedef struct point point;
typedef struct link link;

struct point{
  double x;
  double y;  
};

struct link{
  link* next;
  link* prev;
  link* mirr;
  int id;
};

int N;	 
int N0;
int K;

double thres;
double fact;
point *v;
int *adj;
int *ord;
link **heads;
link *pool;
int p;
int C;

int *w;

double max_x;
double max_y;
double min_x;
double min_y;

clock_t T;

double c;
double m;

double euclidean(point p,point q){
  return sqrt(((p.x-q.x)*(p.x-q.x))+((p.y-q.y)*(p.y-q.y)));
}

int compare(const void *p1, const void *p2){
    const point *elem1 = p1;    
    const point *elem2 = p2;

    return (elem1->x > elem2->x);
}

int readVector(){
  int i;
  scanf("%d %lf",&N,&fact);
    
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
  
  if((max_x-min_x)>max_y-min_y)
    thres=(max_x-min_x)*fact;
  else
    thres=(max_y-min_y)*fact;
  return 0;
}

int rangeSearch(){
  int i,j;
  qsort(v,N,sizeof(point),compare);
  
  for(i=0;i<N;i++){
    heads[i]=&pool[p++];
    heads[i]->mirr=heads[i];
    heads[i]->next=NULL;
    heads[i]->id=i;
  }
  
  for(i=0;i<N;i++){
    for(j=i+1;j<N;j++){
      if(v[j].x-v[i].x<=thres){
	if(euclidean(v[i],v[j])<=thres){
	  if(p>C-2){
	    pool=(link*)malloc(C*sizeof(link));
	    c+=p;
	    p=0;
	  }
	  pool[p].next=heads[i];
	  heads[i]->prev=&pool[p];
	  heads[i]=heads[i]->prev;
	  heads[i]->id=i;
	  p++;

	  pool[p].next=heads[j];
	  heads[j]->prev=&pool[p];
	  heads[j]=heads[j]->prev;
	  heads[j]->id=j;
	  p++;

	  heads[i]->mirr=heads[j];
	  heads[j]->mirr=heads[i];
	  
	  adj[i]++;
	  adj[j]++;
	}
      }
      else
	break;
    }
  }
  c+=p;
  if (c>m) m=c;
  return 0;
}

void coverage(){
  int i,j,m;
  link* it;
  i=m=0;
  K=0;
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

int compress(){
  int i,j;
  j=0;
  for(i=0;i<N0;i++){
    ord[j]=0;
    adj[j]=0;
    if(ord[i]!=0){
      v[j]=v[i];
      j++;
    }
  } 
  N=K;
  return 0;
}

int main(){
  int i,j;
  readVector();
  
#ifndef DEBUG
  printf("%d\n",N);
  for(i=0;i<N;i++)
    printf("%g %g\n",v[i].x,v[i].y);
#endif
  
  N0=N;
  thres/=SKIP;
  rangeSearch();
  coverage();
  
  thres*=SKIP;
  compress();
  rangeSearch();
  coverage();

  T = clock() - T;
  
#ifndef DEBUG
  printf("%d\n",K);
  for(j=1;j<=K;j++)
    for(i=N;i>=0;i--)
      if(ord[i]==j)
	printf("%g %g\n",v[i].x,v[i].y);
  printf("%g\n",thres);
#endif

  printf("%d\t%g\t%d\t%f\t%f\n",N0,fact,K,((float)T)/CLOCKS_PER_SEC,(m/1024.0/1024.0*(double)sizeof(struct link)));
  return 0;
}
