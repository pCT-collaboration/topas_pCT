function r = rotYPoint(point, angle)
  deg = pi / 180;
  theta = angle * deg;
  rotY = [cos(theta), 0, sin(theta); 0, 1, 0; -sin(theta), 0, cos(theta)]
  r = rotY * point;
end