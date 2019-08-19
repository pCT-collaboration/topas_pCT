function r = rotXPoint(point, angle)
  deg = pi / 180;
  theta = angle * deg;
  rotX = [1, 0, 0; 0, cos(theta), sin(theta); 0, -sin(theta), cos(theta)];
  r = rotX * point;
end