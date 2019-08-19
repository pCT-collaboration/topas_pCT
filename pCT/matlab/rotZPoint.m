function r = rotZPoint(point, angle)
  deg = pi / 180;
  theta = angle * deg;
  rotZ = [cos(theta), sin(theta), 0; -sin(theta), cos(theta), 0; 0, 0, 1];
  r = rotZ * point;
end