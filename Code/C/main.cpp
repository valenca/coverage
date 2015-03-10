#include<cstdlib>
#include<iostream>
#include<iomanip>
#include<cmath>
#include<climits>
#include<algorithm>

using namespace std;

typedef struct point point;

struct point{			//point structure
  int index;			//index of the point

  int adj;

  int c;
  
  double x; 			//x coordinate
  double y;			//y chromossome

  bool operator< (const point& str)const{
    return (adj > str.adj);
  }
  
};

int N;	 
int K; 					//number of centroids

double thres;
point *v;				//array of points
int *w;
double ** dist;
double ** link;

double max_x;
double max_y;
double min_x;
double min_y;

double euclidean(point &p,point &q){
  return sqrt(((p.x-q.x)*(p.x-q.x))+((p.y-q.y)*(p.y-q.y)));
}

int readVector(){
  int i,j;
  double mx,mn;
  cin >> N;
  cin >> K;
  
  K=0;
  
  v=new point[N]; //V[N+1]..V[N+4] are super triangles, V[N] is a debug point/buffer
  w=new int[N];
  
  max_x=max_y=0;
  min_x=min_y=INT_MAX;
  
  for(i=0;i<N;i++){
    cin >> v[i].x;
    cin >> v[i].y;
    v[i].index=i;
    v[i].c=0;
    max_x=max(v[i].x,max_x);
    max_y=max(v[i].y,max_y);
    min_x=min(v[i].x,min_x);
    min_y=min(v[i].y,min_y);
  }

  thres=(max_x-min_x)*0.20;
  
  dist = new double*[N];
  for(i=0;i<N;i++){
    dist[i]=new double[N];
    dist[i][i]=0;
  }

  link = new double*[N];
  for(i=0;i<N;i++){
    link[i]=new double[N];
    link[i][i]=1;
  }
  
  for(i=0;i<N;i++){
    for(j=i+1;j<N;j++){
      dist[i][j]=dist[j][i]=euclidean(v[i],v[j]);
      if(dist[i][j]<=thres){
	link[i][j]=link[j][i]=1;
	v[i].adj++;
	v[j].adj++;
      }
    }
  }
  
  sort(v,v+N);
    
  for(i=0;i<N;i++)
    w[v[i].index]=i;  
  
  for(i=0;i<N;i++){
    for(j=i+1;j<N;j++){
      dist[i][j]=dist[j][i]=euclidean(v[i],v[j]);
      if(dist[i][j]<=thres){
	link[i][j]=link[j][i]=1;
      }
      else{
	link[i][j]=link[j][i]=0;
      }
    }
  }
  
}

void coverage(){
  int i,j,k;
  for(i=N-1;K<N && i>=0;i--){
    if(v[i].adj!=0){
      v[i].c=1;
      K++;
      for(j=0;j<N;j++){
	if(link[i][j]==1){
	  for(k=0;k<N;k++){
	    link[j][k]=link[k][j]=0;
	  }
	  v[j].adj=0;
	  link[i][j]=link[j][i]=0;
	}
      }
    }
    sort(v+i+1,v+N);
    //for(i=0;i<N;i++)
      //  w[v[i].index]=i;
    
  }
}

int main(){
  int i,j;
  clock_t T;
  readVector();
  cout << N << endl;
  for(i=0;i<N;i++)
    cout << v[i].x << " " << v[i].y << endl; //" - " << v[i].adj <<endl;
  T = clock();
  coverage();
  T = clock() - T;
  cout << K << endl;
  for(i=0;i<N;i++)
    if(v[i].c==1)
      cout << v[i].x << " " << v[i].y << endl; //" - " << v[i].adj <<endl;
  cout << thres << endl;
  printf("N = %d,\tK = %d,\tB = %f,\tT = %f\n",N,K,thres,((float)T)/CLOCKS_PER_SEC);
  return 0;
}
