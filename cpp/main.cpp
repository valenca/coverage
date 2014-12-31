#include<cstdlib>
#include<iostream>
#include<cmath>
#include<map>
#include<set>
#include<stack>
#include<ctime>
#include<climits>
#include<algorithm>

#include"del.h"

#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))

#define DEBUGY

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
int *centroid;
int *cent_of;
double best;
int *best_v;

int readVector(){
  int i,j;
  double mx,mn;
  cin >> N;
  cin >> K;
  v=new point[N+5]; //V[N+1]..V[N+4] are super triangles, V[N] is a debug point/buffer

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
  centroid = new int[N+5];
  cent_of  = new int[N];

  centroid[N+1]=centroid[N+2]=centroid[N+3]=centroid[N+4]=1;
  
  for(i=0;i<N;i++){
    v[i].index=i;
    best_v[i]=0;
    centroid[i]=0;
    cent_of[i]=-1;
  }
  
  dist = new double*[N+4];
  for(i=0;i<N+4;i++){
    dist[i]=new double[N+4];
    dist[i][i]=0;
  }
    
  for(i=0;i<N+3;i++){
    for(j=i+1;j<N+4;j++){
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
  return 0;
  int i;
  for (i = pos; i < N; i++){
    // an improvement can be found
    if (dist[idx][i] < score){
      return 1;
    }
  }
  return 0;
}

void coverage(int pos, int ncentral, double score, int idx,int last) {
  int i, j,clos;
  double nscore,val;
  int tmp_central_of[N];		
  
  if (ncentral == K) {
    for (i = pos; i < N; i++){
      val = INT_MAX;
      for (j = 0; j < pos; j++){
	if (centroid[j] == 1 && dist[i][j] < val) 
	  val = dist[i][j];
      }
      score = max(score,val);
    }
    // update the best
    if (score < best) {
      best = score;
      cout <<best<<endl;
      for(i=0;i<N;i++)
	best_v[i]=centroid[i];
    }
    return;
  }

  if (pos == N){
    //cout << 1 <<endl;
    return;
  }
  if(pos + K - ncentral > N){
    //cout << 2 <<endl;
    return;
  }
  if(N - pos + ncentral < K){
    cout << 3 <<endl;
    return;
  }

  
  if (pos > ncentral && score >= best && bound(pos, score, idx) == 0)
    return;

  // make temporary copy of central_of
  for (i = 0; i <= pos; i++)
    tmp_central_of[i] = cent_of[i];
	
  // Decision 1: the point at pos is a centroid
  // Then, assign non-centroids to the new centroid and change accordingly		
  // Note: overall score can be equal or decrease.

  insertPoint(v[pos]);  
  centroid[pos] = 1;
  nscore = 0;
  for (i = 0; i < pos; i++){
    if (centroid[i] == 0) {
      if (dist[pos][i] < dist[cent_of[i]][i]) 
	cent_of[i] = pos;
      if (nscore < dist[cent_of[i]][i]) {
	nscore = dist[cent_of[i]][i];
	idx = i;  // keep the largest link for the bound
      }
    }
  }	
  //Recursion
  coverage(pos+1, ncentral+1, nscore, idx,last);
  loadState();
  // recover the contents of the old central_of
  for (i = 0; i <= pos; i++)
    cent_of[i] = tmp_central_of[i];

  // Decision 2: the point at pos is a non- centroid
  // Then, find the closest centroid		
  // Note: overall score can be equal or increase.
  centroid[pos] = 0;
  for (i = 0; i < pos; i++){
    if (centroid[i] == 1){
      clos=route(pos,i);
      if (dist[pos][clos] < dist[cent_of[pos]][pos]) {
	cent_of[pos] = clos;	
      }
      break;
    }
  }
  if (score < dist[cent_of[pos]][pos]) {
    score = dist[cent_of[pos]][pos];
    idx = pos;	// keep the largest link for the bound 
  }

  // recursion
  coverage(pos+1, ncentral, score, idx,last);

  // undo
  centroid[pos] = 0;
  cent_of[pos] = pos;
}

void cov(int pos, int ncent, int nncent, double score,int far,int last,int c){
  int i,j,tfar,tnc[pos+1],tog=0;
  double tscore;

#ifdef DEBUG
  tscore=0;
  if(nncent>0){
    for(i=0;i<pos;i++){
      for(j=0;j<pos;j++){
	if(centroid[i]==0 && centroid[j]==1){
	  if(dist[i][j]>tscore){
	    tscore=dist[i][j];
	    tfar=i;
	  }
	}
      }
    }
    if(tscore != score)
      cout << score << " "<< tscore << endl;
  }
#endif
  
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
      cout << best << endl;
    }
    return;
  }
	
  if(pos==N || nncent+K>N)
    return;
	
	
  //if (pos > ncent && score >= best && bound(pos, score, far) == 0) return;
	
  //point is a centroid
  for(i=0;i<=pos;i++) tnc[i]=cent_of[i];
  tscore=score;
  tfar=far;
	
  insertPoint(v[pos]);
  centroid[pos]=1;
  cent_of[pos]=pos;
  //for(set_it it = v[pos].nbors.begin();it!=v[pos].nbors.end();it++){
    for(i=0;i<pos;i++){
      if(centroid[i]==0 && v[pos].nbors.count(cent_of[i])>0){
	if(dist[pos][i]<dist[i][cent_of[i]]){
	  cent_of[i]=pos;
	  //score=dist[i][*it];
	  if(i==far)
	    tog=0;
	}
      }
    }
    //}
	
  if (tog==0){
    score=0;
    for(i=0;i<pos;i++){
      if(centroid[i] == 0 && dist[i][cent_of[i]]>score){
	score=dist[i][cent_of[i]];
	far=pos;
      }
    }
  }
	
  //recursion
  cov(pos+1,ncent+1,nncent,score,far,last,1);
  //backtracking
  score	= tscore;
  far	= tfar;
  loadState();
	
  for(i=0;i<=pos;i++)
    cent_of[i]=tnc[i];
	
  //point is not a centroid
	
  centroid[pos]=0;
  if(ncent==0){
    cent_of[pos]=N+1;
    v[N+1].nbors.insert(pos);
    far=pos;
    score=INT_MAX;
  }
  else{
    if(centroid[last]!=1)
      last=N+1;
    last=route(pos,last);
    cent_of[pos]=last;
  }
	
  if(dist[pos][cent_of[pos]]>score){
    score=dist[pos][cent_of[pos]];
    far=pos;
  }
  cov(pos+1,ncent,nncent+1,score,far,pos,0);
	
  score=tscore;
  far=tfar;
  for(i=0;i<=pos;i++)
    cent_of[i]=tnc[i];
}	
  
int main(){
  int i,j,c;
  triangle *t;
  point p;

#ifdef DEBUG
  cout << "pre-processing..." << endl;
#endif
  
  readVector();
  initMesh(min_x,max_x,min_y,max_y,v[N+1],v[N+2],v[N+3],v[N+4]);

  /*
    for (i=0;i<(N+5);i++)
    cout << i << ": " << v[i].x<<" " << v[i].y << endl;
  */
#ifdef DEBUG
  cout << "processing..." << endl;
#endif
  
  cov(0,0,0,0,-1,N+1,-1);
  
  for(j=0;j<N;j++)
    for(i=0;i<N;i++)
      if(v[i].t_index==j)
	cout << best_v[i] << " ";
  
  cout << endl << best <<endl;
  return 0;
}

