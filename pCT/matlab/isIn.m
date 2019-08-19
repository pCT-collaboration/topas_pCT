function isIn = isIn(point, shape)
  isIn = true;
  for plane = shape.'
    if (~inHalfSpace(point, cell2mat(plane)))
      isIn = false;
      break;
    end
  end
end