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

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define MAXN 300

using namespace std;


/*
  Important:
  When adding centroids, the value is the same or decreases
  When adding non-centroids, the value is the same or increases
*/

typedef struct point point;

struct point{
  double x;
  double y;
};


double **P;

int n, k;
double best;
int *central;	// Mark as centroid (1) or non-centroid (0) at postion i
int *central_of;   // Assign the closest centroid to the non-centroid at position i 

point *v;

// Bound as look-ahead strategy
// check if largest link vanishes by considering the remaining points as centroids.
// If no, it is not possible to improve the current solution. 
// it assumes distinct distances


double euclidean(point &p,point &q){
  return sqrt(((p.x-q.x)*(p.x-q.x))+((p.y-q.y)*(p.y-q.y)));
}

int bound (int pos, int score, int idx){
  int i;	
		
  for (i = pos; i < n; i++)
    // an improvement can be found
    if (P[idx][i] < score)
      return 1; 
  return 0;
}


void coverage(int pos, int ncentral, double score, int idx) {

  int i, j;
  int tmp_central_of[n];
  double nscore;
  
  //base case: number of centroids is k
  //then, compute the coverage value with remaining as non-centroids.  
  if (ncentral == k) {	
    for (i = pos; i < n; i++){
      double val = INT_MAX;
      for (j = 0; j < pos; j++){
	if (central[j] == 1 && P[i][j] < val) 
	  val = P[i][j];
      }
      score = MAX(score,val);
    }
    // update the best
    if (score < best) {
      best = score;

    }
    return;
  }

  // base case:
  // 1) there are no more points
  // 2) the number of remaining points as centroids is less than k
  if (pos == n || pos + k - ncentral - 1 >= n)
    return;

  // Bound -> terminates if the largest link (idx) is the same with the remaining as centroids
  // This is the best possible way as the value can only decrease or keep the same
  // Other words: if there is no possiblity of finding an improvement 
  // Further conditions must be true:	
  // 1) There is at least one fixed non-centroid
  // 2) The current score is larger or equal than the best 
  if (pos > ncentral && score >= best && bound(pos, score, idx) == 0)
    return;

  // make temporary copy of central_of
  for (i = 0; i <= pos; i++)
    tmp_central_of[i] = central_of[i];
	
  // Decision 1: the point at pos is a centroid
  // Then, assign non-centroids to the new centroid and change accordingly		
  // Note: overall score can be equal or decrease.
  central[pos] = 1;
  nscore = 0;
  for (i = 0; i < pos; i++){
    if (central[i] == 0) {
      if (P[pos][i] < P[central_of[i]][i]) 
	central_of[i] = pos;
      if (nscore < P[central_of[i]][i]) {
	nscore = P[central_of[i]][i];
	idx = i;  // keep the largest link for the bound
      }
    }
  }
	
  //Recursion
  coverage(pos+1, ncentral+1, nscore, idx);
	
  // recover the contents of the old central_of
  for (i = 0; i <= pos; i++)
    central_of[i] = tmp_central_of[i];

  // Decision 2: the point at pos is a non- centroid
  // Then, find the closest centroid		
  // Note: overall score can be equal or increase.
  central[pos] = 0;
  for (i = 0; i < pos; i++){
    if (central[i] == 1){
      if (P[pos][i] < P[central_of[pos]][pos]) {
	central_of[pos] = i;	
      }
    }
  }
  if (score < P[central_of[pos]][pos]) {
    score = P[central_of[pos]][pos];
    idx = pos;	// keep the largest link for the bound 
  }
	

  // recursion
  coverage(pos+1, ncentral, score, idx);

  // undo
  central[pos] = -1;
  central_of[pos] = pos;

}

int main(){
  int i, j, x, y, z;

  cin >> n;
  cin >> k;

  clock_t T=clock();
  
  v=new point[n]; //V[N+1]..V[N+4] are super triangles, V[N] is a debug point/buffer
  
  for(i=0;i<n;i++){
    cin >> v[i].x;
    cin >> v[i].y; 
  }
  
  P = new double*[n];
  for(i=0;i<n;i++){
    P[i]=new double[n];
    P[i][i]=INT_MAX;
  }

  central_of = new int[n];
  central   = new int[n];
  
  for(i=0;i<n;i++){
    for(j=i+1;j<n;j++){
      P[i][j]=P[j][i]=euclidean(v[i],v[j]);
    }
  }
    //every element is its own center and distance is infinite
  for (i=0; i<n; i++) { 
    central_of[i] = i;
    central[i] = -1;	
  }
  best = INT_MAX;

  coverage(0, 0, INT_MAX, 0);
  
  T = clock() - T;
  printf("\nN = %d,\tK = %d,\tB = %f,\tT = %f\n",n,k,best,((float)T)/CLOCKS_PER_SEC);
    
  return 0;
}
