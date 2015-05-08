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

int N2;	 
int K2;

double thres;
double fact;
point *v;
point *w;

int ** link;
int c;

double max_x;
double max_y;
double min_x;
double min_y;

double max_x2;
double max_y2;
double min_x2;
double min_y2;

clock_t T;

bool inRectangle(double x, double y,double x1,double x2,double y1,double y2){
  return (x >=x1 && x <=x2 && y >=y1 && y <=y2);
}

double euclidean(point &p,point &q){
  return sqrt(((p.x-q.x)*(p.x-q.x))+((p.y-q.y)*(p.y-q.y)));
}

int readVector(){
  int i,j=0,it;
  double mx,mn,tx,ty;
  cin >> N;
  cin >> fact;
  
  K=N;
  
  w=new point[N];
  
  cin >> min_x;
  cin >> max_x;
  cin >> min_y;
  cin >> max_y;
  
  for(i=0;i<N;i++){
    //cin >> w[i].index;
    cin >> w[i].x;
    cin >> w[i].y;
    w[i].c=0;
    w[i].adj=0;
  }


  cin >> N2;
  cin >> fact;

  
  if((max_x-min_x)>max_y-min_y)
    thres=(max_x-min_x)*fact;
  else
    thres=(max_y-min_y)*fact;
  
  cin >> min_x2;
  cin >> max_x2;
  cin >> min_y2;
  cin >> max_y2;

  v=new point[N2];
  
  int k=0,l;
  for(i=0;i<N2;i++){
    cin >> it;
    cin >> tx;
    cin >> ty;
    
    if(!inRectangle(tx,ty,min_x-thres,max_x+thres,min_y-thres,max_y+thres)){
      v[j].x=tx;
      v[j].y=ty;
      v[j].c=0;
      v[j].adj=0;
      j++;
    }
    else if (!inRectangle(tx,ty,min_x,max_x,min_y,max_y)){
      l=1;
      for(k=0;k<N;k++){
	if(distance(&v[i],&w[k])<=thres){
	  l=0;
	  break;
	}
      }
      if(l!=1){
	v[j].x=tx;
	v[j].y=ty;
	v[j].c=0;
	v[j].adj=0;
	j++;
      }
    }
  }

  N2=j;
  
  link = new int*[N2];
  
  for(i=0;i<N2;i++){
    link[i]=new int[N2];
  }
  
  T=clock();
  
  sort(v,v+N2);
  
  for(i=0;i<N2;i++){
    for(j=i;j<N2;j++){
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
    for(j=0;j<N2;j++){
      if(m<v[j].adj){
	m=v[j].adj;
	i=j;
      }
    }
        
    if(m!=-1){
      K2++;
      v[i].c=K2;
      for(j=0;j<N2;j++){
	if(link[i][j]==1){
	  for(k=0;k<N2;k++){
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
  cout << N2 << endl;
  for(i=0;i<N2;i++)
    cout << v[i].x << " " << v[i].y /*<< " - " << i*/ << endl;
  coverage();
  T = clock() - T;
  
  cout << K2+K << endl;
  for(j=1;j<=K2;j++)
    for(i=N2;i>=0;i--)
      if(v[i].c==j)
	cout << v[i].x << " " << v[i].y << endl; //" - " << v[i].adj <<endl;
  for(i=0;i<N;i++)
    cout << w[i].x << " " << w[i].y << endl; //" - " << v[i].adj <<endl;
    
  
  
  cout << thres << endl;
  printf("%d\t%g\t%d\t%f\n",N2,fact,K+K2,((float)T)/CLOCKS_PER_SEC);
  return 0;
}
