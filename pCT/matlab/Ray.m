function Ray = Ray(point, direction)
  Ray.point = point;
  Ray.direction = direction / norm(direction);
end