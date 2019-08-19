function inHalfSpace = inHalfSpace(point, halfSpace)
  inHalfSpace = dot(halfSpace.normal, point) - halfSpace.distance > -1.0e-6;
end