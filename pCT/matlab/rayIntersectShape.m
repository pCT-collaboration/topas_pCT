function intersections = rayIntersectShape(ray, shape)
  intersections = [];
  i = 0;
  for plane = shape.'
    distance = intersection(ray, cell2mat(plane));
    if (distance < Inf)
      point2 = newPoint(ray, distance);
      if (isIn(point2, shape))
        i = i + 1;
        intersections(i).distance = distance;
        intersections(i).point = point2;
      end
    end
  end
end