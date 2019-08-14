function Plane = Plane(point, normal)
  Plane.point = point;
  Plane.normal = normal / norm(normal);
  Plane.distance = dot(Plane.point, Plane.normal);
endfunction