#include<cstdlib>
#include<iostream>
#include<iomanip>
#include<cmath>
#include<climits>
#include<algorithm>

using namespace std;

typedef struct point point;

struct point{			
  int index;
  int adj;

  int c;
  double x;
  double y;

  bool operator< (const point& str)const{
    return (x < str.x);
  }
  
};

int N;	 
int K;

double thres;
double fact;
point *v;
int *w;
int ** link;
int c;

double max_x;
double max_y;
double min_x;
double min_y;

clock_t T;

double euclidean(point &p,point &q){
  return sqrt(((p.x-q.x)*(p.x-q.x))+((p.y-q.y)*(p.y-q.y)));
}

int readVector(){
  int i,j,it;
  double mx,mn;
  cin >> N;
  cin >> fact;
  
  K=0;
  
  v=new point[N]; //V[N+1]..V[N+4] are super triangles, V[N] is a debug point/buffer
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
  
  link = new int*[N];
  for(i=0;i<N;i++){
    link[i]=new int[N];
  }
  T=clock();
  
  sort(v,v+N);
  
  for(i=0;i<N;i++){
    for(j=i;j<N;j++){
      if(v[j].x-v[i].x<=thres){
	if(euclidean(v[i],v[j])<=thres){
	  link[i][j]=link[j][i]=1;
	  v[i].adj++;
	  v[j].adj++;
	}
      }
      else
	break;
    }
  }
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
  /*
  for(i=0;i<N;i++){
    for(j=0;j<N;j++){
      cout << link[i][j];
    }
    cout << endl;
  }
  */
  cout << N << endl;
  for(i=0;i<N;i++)
    cout << v[i].x << " " << v[i].y /*<< " - " << i*/ << endl;
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
