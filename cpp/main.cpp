#include<cstdlib>
#include<iostream>
#include<iomanip>
#include<cmath>
#include<map>
#include<set>
#include<stack>
#include<ctime>
#include<climits>
#include<algorithm>

#include"del.h"

using namespace std;

int N;	 				//total number of points
int N5;
int K; 					//number of centroids
point *v;				//array of points

double max_x;
double max_y;
double min_x;
double min_y;

//branch and bound variables
double ** dist;
int *centroid;
int *cent_of;
double best;
int *best_v;

int readVector(){
  int i,j;
  double mx,mn;
  cin >> N;
  cin >> K;
  
  N5=N+5;
  
  v=new point[N5]; //V[N+1]..V[N+4] are super triangles, V[N] is a debug point/buffer
  
  max_x=max_y=0;
  min_x=min_y=INT_MAX;
  
  for(i=0;i<N;i++){
    cin >> v[i].x;
    cin >> v[i].y;
    
    v[i].w = v[i].x*v[i].x + v[i].y*v[i].y;

    v[i].t_index=i;
    
    max_x=max(v[i].x,max_x);
    max_y=max(v[i].y,max_y);
    min_x=min(v[i].x,min_x);
    min_y=min(v[i].y,min_y);
  }
  
  mx=max(max_x-min_x,max_y-min_y);
  
  for(i=0;i<N;i++){
    v[i].d = xy2d(HIL,(int)((v[i].x-min_x)/(mx/HIL)),(int)((v[i].y-min_y)/(mx/HIL)));
  }

  sort(v,v+N); 

  best_v   = new int[N];
  centroid = new int[N5];
  cent_of  = new int[N];

  centroid[N+1]=centroid[N+2]=centroid[N+3]=centroid[N+4]=1;
  
  for(i=0;i<N;i++){
    v[i].index=i;
    best_v[i]=0;
    centroid[i]=0;
    cent_of[i]=-1;
  }
  
  dist = new double*[N5];
  for(i=0;i<N5-1;i++){
    dist[i]=new double[N5];
    dist[i][i]=0;
  }
    
  for(i=0;i<N5-2;i++){
    for(j=i+1;j<N5-1;j++){
      if(i<N && j<N){
	dist[i][j]=dist[j][i]=euclidean(v[i],v[j]);
      }
      else{
	dist[i][j]=dist[j][i]=INT_MAX;
      }
    }
  }
  
  best=INT_MAX;  
  
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

#ifdef DEBUG  
  double tmn=INT_MAX;
  int tmz=mz;
  for(int i=0;i<N;i++){
    if(centroid[i]==1 && dist[i][p]<tmn){
      tmn=dist[i][p];
      tmz=i;
    }
  }
  if(tmn!=mn){
    cout << p <<" "<< q << " ROUTE ERROR: "<< mz << " should be "<< tmz << " " << centroid[q] <<endl;
  }
#endif

  return mz;
}

int bound (int pos, double score, int idx){
  int i,tog;
  for (i = N-1; i >= pos; i--){
    // an improvement can be found
    if (dist[idx][i]<best)
      return 0;
  }
  return 1;
}

stack<int> s_change;
stack<int> c_change;
stack<int> n_change;

void cov(int pos, int ncent, int nncent, double score,int far,int last,int c){
  int i,tfar,tog=0;
  uset_it jt;
  set_it it;
  double tscore;
    
  if(ncent==K){
    for(i=pos;i<N;i++){
      if(last[centroid]!=1)
	last=N+1;
      last=route(i,last);
      if(dist[i][last]>score){
	score=dist[i][last];
      }
    }
    if(score<best){
      best=score;
      for(i=0;i<N;i++)
	best_v[i]=centroid[i];
      printf("%f\n",best);
    }
    return;
  }
	
  if(pos==N || nncent+K>N)
    return;
		
  if (pos > ncent && score >= best && bound(pos, score, far) == 1){
    return;
  }
	
  //point is a centroid
  tscore=score;
  tfar=far;
	
  insertPoint(v[pos]);
  centroid[pos]=1;
  c_change.push(0);
  for(it=v[pos].nbors.begin();it!=v[pos].nbors.end();it++){
    for(jt=v[*it].covered.begin();jt!=v[*it].covered.end();jt++){
      if(dist[pos][*jt]<dist[*jt][*it]){
	c_change.top()++;
	s_change.push(*jt);
	n_change.push(*it);
	cent_of[*jt]=pos;
	v[pos].covered.insert(*jt);
	v[*it].covered.erase(jt);
	if(*jt==far){
	  tog=1;
	}
      }
    }
  }
	
  if (tog==1){
    score=0;
    for(i=0;i<pos;i++){
      if(centroid[i] == 0 && dist[i][cent_of[i]]>score){
	score=dist[i][cent_of[i]];
	far=i;
      }
    }
  }
  
  //recursion
  cov(pos+1,ncent+1,nncent,score,far,last,1);
  //backtracking
  loadState();
  
  score = tscore;
  far	= tfar;
  
  cent_of[pos]=-1;
  for(i=0;i<c_change.top();i++){
    v[n_change.top()].covered.insert(s_change.top());
    v[pos].covered.erase(s_change.top());
    cent_of[s_change.top()]=n_change.top();
    n_change.pop();
    s_change.pop();
  }
  c_change.pop();
  
  //point is not a centroid
  centroid[pos]=0;
  
  if(centroid[last]!=1)
    last=N+1;
  last=route(pos,last);
  v[last].covered.insert(pos);
  cent_of[pos]=last;
	
  if(dist[pos][last]>score){
    score=dist[pos][cent_of[pos]];
    far=pos;
  }
  
  cov(pos+1,ncent,nncent+1,score,far,pos,0);

  score=tscore;
  far=tfar;
  v[last].covered.erase(pos);
  cent_of[pos]=-1;  
}	
  
int main(){
  int i,j,c;
  triangle *t;
  point p;
  clock_t T=clock();
    
  readVector();
  initMesh(min_x,max_x,min_y,max_y,v[N+1],v[N+2],v[N+3],v[N+4]);
  cov(0,0,0,0,-1,N+1,-1);
  T = clock() - T;
  
  for(i=0;i<N;i++)
    cout << best_v[i] << " ";
  
  printf("\nN = %d,\tK = %d,\tB = %f,\tT = %f\n",N,K,best,((float)T)/CLOCKS_PER_SEC);
  return 0;
}

