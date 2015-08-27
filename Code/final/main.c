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
  double x;
  double y;  
};

struct link{
  link* next;
  link* prev;
  link* mirr;
  int id;
};

int M;
int N;	 
int N0;
int K;

double thres;
double dist;
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

int inSquare(point p){
  return (p.x>=min_x &&
	  p.x<=max_x &&
	  p.y>=min_y &&
	  p.y<=max_y);
}

int readVector(){
  int i;
  T=clock();

		/*All inputs by order from a text file or console input*/
  
  scanf("%lf %lf %lf %lf %lf %d %d",
	&min_x,	/*float: minimum x coordinate - west border  */
	&min_y,	/*float: minimum y coordinate - south border */
	&max_x,	/*float: maximum x coordinate - east border  */
	&max_y,	/*float: maximum y coordinate - north border */
	&dist,	/*float: minimum distance between points as a percentage of width*/
	&N,	/*int:	 number of points in current window*/
	&M);	/*int:	 number of points in previous window (priority points)*/
  
  v=(point*)malloc(sizeof(point)*(N+M)); /*vector of geographic points (includes priority points)*/
  adj=(int*)calloc(N+M,sizeof(int));     /*number adjacencies for each point*/
  ord=(int*)calloc(N+M,sizeof(int));     /*ordered centroid number
					  * 0 if non-centroid
					  * 1 through K for the ith point to be picked 
					  */
  
  C=(M+N)*(M+N)/5;
  pool=(link*)malloc(C*sizeof(link)); 
  heads=(link**)malloc(sizeof(link*)*(N+M));
  
  for(i=0;i<N;i++){
    scanf("%lf %lf",&v[i].x,&v[i].y); /* Scan geographic points within window*/
    adj[i]=0;
  }
  
  for(i=0;i<M;i++){
    scanf("%lf %lf",&v[i+N].x,&v[i+N].y);
    adj[i+N]=N;
    if(!inSquare(v[i+N])){
      i--;
      M--;
    }
  }
  thres=(max_x-min_x)*dist;
  N+=M;
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
      else{
	break;
      }
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
  readVector();
  
#ifdef DEBUG
  printf("%d\n",N);
  for(i=0;i<N;i++)
    printf("%g %g\n",v[i].x,v[i].y);
#endif

  N0=N;
  thres/=1;
  rangeSearch();
  coverage();
  /*
  thres*=10;
  compress();
  rangeSearch();
  coverage();
  */
  T = clock() - T;
  
#ifdef DEBUG
  printf("%d\n",K);
  for(j=1;j<=K;j++)
    for(i=N;i>=0;i--)
      if(ord[i]==j)
	printf("%g %g\n",v[i].x,v[i].y);
  printf("%g\n",thres);
#endif

  printf("%d\t%g\t%d\t%f\t%f\n",N0,dist,K,((float)T)/CLOCKS_PER_SEC,(m/1024.0/1024.0*(double)sizeof(struct link)));
  return 0;
}
