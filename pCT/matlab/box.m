function box = box(hlx, hly, hlz)
  box = {
    Plane(Point(hlx, 0, 0), Point(-1, 0, 0)),
    Plane(Point(0, hly, 0), Point(0, -1, 0)),
    Plane(Point(0, 0, hlz), Point(0, 0, -1)),
    Plane(Point(-hlx, 0, 0), Point(1, 0, 0)),
    Plane(Point(0, -hly, 0), Point(0, 1, 0)),
    Plane(Point(0, 0, -hlz), Point(0, 0, 1))
  };
endfunction