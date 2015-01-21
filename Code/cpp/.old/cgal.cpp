#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>                   Delaunay;    
typedef K::Point_2                                          Point;

void load_points(std::vector< Point >& points){
  points.push_back(Point(1., 1.));
  points.push_back(Point(2., 1.));
  points.push_back(Point(2., 2.));
  points.push_back(Point(1., 2.));      
}

int main(){
  std::vector< Point > points;
  load_points(points);
  Delaunay dt;
  dt.insert(points.begin(), points.end());
  std::cout << dt.number_of_vertices() << std::endl;
  return 0;
}