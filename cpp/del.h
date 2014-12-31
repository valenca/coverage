#include<cstdlib>
#include<iostream>
#include<cmath>
#include<map>
#include<set>
#include<stack>

#define HIL 32

using namespace std;

typedef struct triangle tringle;
typedef struct point point;
typedef pair<int,int> edge;
typedef pair<edge,triangle*> map_entry;

typedef set<int>::iterator set_it;
typedef map<edge,triangle*>::iterator map_it;

extern int N;			//number of points
extern map<edge,triangle*> E;	//hashtable of half-edges

struct triangle{		//half-edge str
  bool fake;			//auxiliary triangle flag. true if triangle is outside of the convex hull
  int index;			//index of the triangle
  
  point *v1;    		//points of the triangle
  point *v2;
  point *v3;
};

struct point{			//point structure
  int index;			//index of the point

  int t_index;
  
  double x; 			//x coordinate
  double y;			//y chromossome
  double w;			//x^2+y^2. precalculated to speed circumference test

  int d;			//position in hilbert curve
  
  set<int> nbors;
  
  bool operator< (const point& str)const{
    return (d < str.d);
  }
  
};

//hilbert functions;
int xy2d(int n,int x,int y);
void d2xy(int n,int d,int *x,int *y);
void rot(int n,int *x,int *y,int rx,int ry);

//geometric functions
double ori(point &p1, point &p2, point &p3);
bool intersect(point &p1, point &q1, point &p2,point &q2);
bool inTriangle(point &p, triangle &t);
double inCircle(point &p,point &q,point &r,point &s);
point * adjacent(point &p, point &q);
double euclidean(point &p,point &q);
  
//triangle manipulation
triangle * makeTriangle(point &p1,point &p2, point &p3);
bool deleteTriangle(triangle &t);

//mesh manipulation
bool loadState();
triangle *findTriangle(point &p,triangle &t);
int digCavity(point &p,point &q, point &r);
int insertPoint(point &p);
void initMesh(double min_x,double max_x,double min_y,double max_y,
	      point &s1,point &s2,point &s3,point &s4);
