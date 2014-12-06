#include<stdio.h>
#include<stdlib.h>
/*
struct HE_edge{
  HE_vert* vert;   // vertex at the end of the half-edge
  HE_edge* pair;   // oppositely oriented adjacent half-edge 
  HE_face* face;   // face the half-edge borders
  HE_edge* next;   // next half-edge around the face
};

struct HE_vert{
  float x;
  float y;
  float z;

  HE_edge* edge;  // one of the half-edges emantating from the vertex
};

struct HE_face{
  HE_edge* edge;  // one of the half-edges bordering the face
};
 
*/
int N,K;
int **v;

int main(void){
  int i;
  scanf("%d %d",&N,&K);
  *v=(int*)malloc(N*sizeof(int)*2);
  for(i=0;i<N;i++)
    scanf("%d %d",&v[i][0],&v[i][1]);
    
  return 0;
}
