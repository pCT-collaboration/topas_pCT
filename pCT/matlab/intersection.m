function distance = intersection (ray, plane)
  rd = ray.direction;
  rp = ray.point;
  pn = plane.normal;
  denom = dot(rd, pn);
  if (abs(denom) < 1.0e-6)
    distance = Inf;
  else
    distance = (plane.distance - dot(pn, rp)) / denom;
  endif
endfunction
