function newPlane = transPlane(plane, vector)
  newPlane = Plane(plane.point + vector, plane.normal);
endfunction