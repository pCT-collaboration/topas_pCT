function newPoint = newPoint(ray, distance)
  newPoint = ray.point + distance * ray.direction;
end