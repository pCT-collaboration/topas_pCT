function testTrap(dx1, dx2, dy1, dy2, dz)
  t = trapiziod(dx1, dx2, dy1, dy2, dz);
  p1 = Point((dx1+dx2)/2, 0, 0);
  isIn(p1, t)
  p2 = Point(-(dx1+dx2)/2, 0, 0);
  isIn(p2, t)
  p3 = Point(0, (dy1+dy2)/2, 0);
  isIn(p3, t)
  p4 = Point(0, -(dy1+dy2)/2, 0);
  isIn(p4, t)
  p5 = Point(0, 0, dz);
  isIn(p5, t)
  p6 = Point(0, 0, -dz);
  isIn(p6, t)
  p7 = Point(dx1, dy1, -dz);
  isIn(p7, t)
  p8 = Point(-dx1, dy1, -dz);
  isIn(p8, t)
  p9 = Point(dx1, -dy1, -dz);
  isIn(p9, t)
  p10 = Point(-dx1, -dy1, -dz);
  isIn(p10, t)
  p11 = Point(dx2, dy2, dz);
  isIn(p11, t)
  p12 = Point(-dx2, dy2, dz);
  isIn(p12, t)
  p13 = Point(dx2, -dy2, dz);
  isIn(p13, t)
  p14 = Point(-dx2, -dy2, dz);
  isIn(p14, t)
  X = Ray(Point(-100, 0, 0), Point(1, 0, 0));
  x = rayIntersectShape(X, t);
  x(1)
  x(2)
  Y = Ray(Point(0, -100, 0), Point(0, 1, 0));
  y = rayIntersectShape(Y, t);
  size(y)
  y(1)
  y(2)
  Z = Ray(Point(0, 0, -100), Point(0, 0, 1));
  z = rayIntersectShape(Z, t);
  z(1)
  z(2)
endfunction