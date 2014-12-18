#include<cstdlib>
#include<iostream>
#include<cmath>
#include<map>
#include<set>
#include<stack>
#include<ctime>
#include<climits>
#include"del.h"

#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))

using namespace std;

//triangulation variables - external

int N;	 				//total number of points
int K; 					//number of centroids
point *v;				//array of points

double max_x;
double max_y;
double min_x;
double min_y;

//branch and bound variables
double ** dist;
bool *centroid;
int *cent_of;

int readVector(){
  int i,j;
  cin >> N;
  cin >> K;
  v=new point[N];
  for(i=0;i<N;i++){
    cin >> v[i].x;
    cin >> v[i].y;
    
    v[i].w = v[i].x*v[i].x + v[i].y*v[i].y;
    v[i].index=i;
    
    max_x=max(v[i].x,max_x);
    max_y=max(v[i].y,max_y);
    min_x=min(v[i].x,min_x);
    min_y=min(v[i].y,min_y);
  }
  dist = new double*[N];
  for(i=0;i<N;i++){
    dist[i]=new double[N];
    dist[i][i]=INT_MAX;
  }

  centroid = new bool[N];
  cent_of = new int[N];
  
  for(i=0;i<N-1;i++)
    for(j=i+1;j<N;j++)
      dist[i][j]=dist[j][i]=euclidean(v[i],v[j]);
  
  return 0;
}

int route(int p,int q){
  point *t;
  double mn,tmp;
  int mz,mz2;
  mz2=q;
  tmp=dist[q][p];
  do{
    mz=mz2;
    mn=tmp;
    for(set_it it=v[mz].nbors.begin();it!=v[mz].nbors.end();it++){
      if(*it < N && dist[*it][p]<mn){
	tmp=dist[*it][p];
	mz2=*it;
      }
    }
  }while(mz!=mz2);
  cout << mz << " - " << mn <<endl;
  return mz;
}

void coverage(){
  return;
}

int main(){
  int i,c;
  triangle *t;
  point p,s1,s2,s3,s4;
  
  readVector();
  initMesh(min_x,max_x,min_y,max_y);
  
  for(i=0;i<N/2;i++){
    insertPoint(v[i]);
    centroid[i]=1;
  }


  for(int pt=N/2;pt<N;pt++){
    int mz;
    double mn=10000;
  
    for(i=0;i<N/2;i++){
      if(centroid[i]){
	if(mn>dist[pt][i]){
	  mn=dist[pt][i];
	  mz=i;
	}
      }
    }
  
    route(pt,2);
    cout << mz << " - " << mn <<endl;  
    cout << "--------" <<endl;
  }
  
  cout << E.size() <<endl;
  return 0;
}






