function newShape = rotZShape(shape, angle)
  newShape = cell(size(shape));
  i = 1;
  for plane = shape.'
    planeMat = cell2mat(plane);
    newShape{i}.point = rotZPoint(planeMat.point, angle);
    newShape{i}.normal = rotZPoint(planeMat.normal, angle);
    newShape{i}.distance = planeMat.distance;
    ++i;
  endfor
endfunction