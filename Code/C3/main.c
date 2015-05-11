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
  int index;
  int adj;

  int c;
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
int K;

double thres;
double fact;
point *v;

link **heads;
link **tails;
link *pool;
int c;

int *w;

double max_x;
double max_y;
double min_x;
double min_y;

clock_t T;

double euclidean(point p,point q){
  return sqrt(((p.x-q.x)*(p.x-q.x))+((p.y-q.y)*(p.y-q.y)));
}

int compare(const void *p1, const void *p2){
    const point *elem1 = p1;    
    const point *elem2 = p2;

    return (elem1->x > elem2->x);
}

int readVector(){
  int i,j;
  scanf("%d %lf",&N,&fact);
  
  K=0;
  
  v=(point*)malloc(sizeof(point)*N); 
  max_x=max_y=0;
  min_x=min_y=INT_MAX;
  
  for(i=0;i<N;i++){
    scanf("%d %lf %lf",&v[i].index,&v[i].x,&v[i].y);
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

  pool=(link*)malloc(N*5000*sizeof(link));
  heads=(link**)malloc(N*sizeof(link*));
  tails=(link**)malloc(N*sizeof(link*));
  
  T=clock();
  
  qsort(v,N,sizeof(point),compare);
  
  for(i=0;i<N;i++){
    heads[i]=&pool[c++];
    heads[i]->mirr=heads[i];
    heads[i]->next=NULL;
    heads[i]->id=i;
  }
  
  for(i=0;i<N;i++){
    for(j=i+1;j<N;j++){
      if(v[j].x-v[i].x<=thres){
	if(euclidean(v[i],v[j])<=thres){
	  pool[c].next=heads[i];
	  heads[i]->prev=&pool[c];
	  heads[i]=heads[i]->prev;
	  heads[i]->id=i;
	  c++;

	  pool[c].next=heads[j];
	  heads[j]->prev=&pool[c];
	  heads[j]=heads[j]->prev;
	  heads[j]->id=j;
	  c++;

	  heads[i]->mirr=heads[j];
	  heads[j]->mirr=heads[i];
	  
	  v[i].adj++;
	  v[j].adj++;
	}
      }
      else
	break;
    }
  }
  return 0;
}

void coverage(){
  int i,j,m;
  link* it;
  i=m=0;
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
      
      for(it=heads[i];it && it->next;it=it->next){
	v[it->mirr->id].adj=-1;
	if(!it->mirr->prev){
	  heads[it->mirr->id]=it->next;
	  heads[it->mirr->id]->prev=NULL;
	}
	else{
	  it->mirr->prev->next=it->mirr->next;
	}
	it->mirr->next->prev=it->mirr->prev;	
	v[it->mirr->id].adj--;
	v[it->id].adj=-1;
      }      
      heads[i]=NULL;
      v[i].adj=-1;
      
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
      if(v[i].c==j)
	printf("%g %g\n",v[i].x,v[i].y);
  printf("%g\n",thres);

  printf("%d\t%g\t%d\t%g\n",N,fact,K,((float)T)/CLOCKS_PER_SEC);
  return 0;
}
