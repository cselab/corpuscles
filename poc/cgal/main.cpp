#include <CGAL/Complex_2_in_triangulation_3.h>
#include <CGAL/Implicit_surface_3.h>
#include <CGAL/IO/Complex_2_in_triangulation_3_file_writer.h>
#include <CGAL/make_surface_mesh.h>
#include <CGAL/Surface_mesh_default_criteria_3.h>
#include <CGAL/Surface_mesh_default_triangulation_3.h>
#include <fstream>

typedef CGAL::Surface_mesh_default_triangulation_3 Tr;
typedef CGAL::Complex_2_in_triangulation_3<Tr> C2t3;
typedef Tr::Geom_traits GT;
typedef GT::Sphere_3 Sphere_3;
typedef GT::Point_3 Point_3;
typedef GT::FT FT;
typedef FT (*Function)(Point_3);
typedef CGAL::Implicit_surface_3<GT, Function> Surface_3;

FT sphere_function (Point_3 p) {
  const FT x2=p.x()*p.x(), y2=p.y()*p.y(), z2=p.z()*p.z();
  return x2 + y2 + z2/2 -1;
}

int main() {
  Tr tr;
  C2t3 c2t3 (tr);
  Surface_3 surface(sphere_function,
                    Sphere_3(CGAL::ORIGIN, 2.)); // bounding sphere
  CGAL::Surface_mesh_default_criteria_3<Tr> criteria(30.,  // angular bound
                                                     0.1,  // radius bound
                                                     0.1); // distance bound
  CGAL::make_surface_mesh(c2t3, surface, criteria, CGAL::Manifold_tag());
  std::ofstream out("out.off");
  CGAL::output_surface_facets_to_off(out, c2t3);
}
