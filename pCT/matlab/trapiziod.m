function trapizoid = trapiziod(dx1, dx2, dy1, dy2, dz)
  Point(0, -(dy1+dy2)/2.0, 0);
  Point((dy1-dy2)/2, -dz, 0);
  trapizoid = {
    Plane(Point(0,0,-dz), Point(0,0,1)),
    Plane(Point(0,0,dz), Point(0,0,-1)),
    Plane(Point(-(dx1+dx2)/2.0, 0, 0), Point(dz, 0, (dx2-dx1)/2)),
    Plane(Point((dx1+dx2)/2.0, 0, 0), Point(-dz, 0, (dx2-dx1)/2)),
    Plane(Point(0, -(dy1+dy2)/2.0, 0), Point(0, dz, (dy2-dy1)/2)),
    Plane(Point(0, (dy1+dy2)/2.0, 0), Point(0, -dz, (dy2-dy1)/2))
  };
end