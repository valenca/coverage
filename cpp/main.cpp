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

#define HIL 32

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
double best;
int *best_v;

int readVector(){
  int i,j;
  double mx,mn;
  cin >> N;
  cin >> K;
  v=new point[N];

  max_x=max_y=0;
  min_x=min_y=INT_MAX;
  
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
  
  mx=max(max_x-min_x,max_y-min_y);
  
  for(i=0;i<N;i++){
    v[i].d = xy2d(HIL,(int)((v[i].x-min_x)/(mx/HIL)),(int)((v[i].y-min_y)/(mx/HIL)));
  }

  sort(v,v+N);
  
  dist = new double*[N];
  for(i=0;i<N;i++){
    dist[i]=new double[N];
    dist[i][i]=INT_MAX;
  }

  best_v = new int[N];
  
  centroid = new bool[N];
  cent_of = new int[N];
  best = INT_MAX;
  for(i=0;i<N-1;i++)
    for(j=i+1;j<N;j++)
      dist[i][j]=dist[j][i]=euclidean(v[i],v[j]);

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
  centroid[pos] = -1;
  cent_of[pos] = pos;
}

void cov(int pos, int ncent, double score,int far,int last,int c){
  int i;
  double nscore;
  int tmp_central_of[pos];

  
  if(pos>=N)
    return;
  if(ncent==K){
    if(score<best){
      best=score;
      for(i=0;i<N;i++){
	best_v[i]=centroid[i];
      }
    }
    return;
  }
  else{
    
    //point is a centroid
    centroid[pos]=1;
    insertPoint(v[pos]);
    
    for (i = 0; i <= pos; i++)
      tmp_central_of[i] = cent_of[i];
    
    nscore=0;
    for (i = 0; i < pos; i++){
      if (centroid[i] == 0) {
	if (dist[pos][i] < dist[cent_of[i]][i]) 
	  cent_of[i] = pos;
	if (nscore < dist[cent_of[i]][i]) {
	  nscore = dist[cent_of[i]][i];
	  far = i;  // keep the largest link for the bound
	}
      }
    }

    cout << nscore << endl;
    
    cov(pos+1,ncent+1,nscore,far,last,1);
    loadState();

    for (i = 0; i <= pos; i++){
      cent_of[i] = tmp_central_of[i];
    }
    //point is a non-centroid

    centroid[pos]=0;

    if(centroid[last]!=1){
      for(i=0;i<pos;i++){
	if(centroid[i]==1){
	  last=i;
	  break;
	}
      }
    }

    
    last=route(pos,last);
    cent_of[pos]=last;
    
    if (score < dist[cent_of[pos]][pos]) {
      score = dist[cent_of[pos]][pos];
      far = pos;	// keep the largest link for the bound 
    }
    cov(pos+1,ncent,score,far,last,0);
    
    centroid[pos]=-1;
    cent_of[pos]=pos;
  }
}

int main(){
  int i,c;
  triangle *t;
  point p,s1,s2,s3,s4;
  
  readVector();
  initMesh(min_x,max_x,min_y,max_y);
  
  //coverage(0, 0, INT_MAX, 0,0);
  cov(0,0,INT_MAX,0,0,-1);
  for(i=0;i<N;i++)
    cout << best_v[i] << " ";
  cout <<endl << best <<endl;
  
  return 0;
}

