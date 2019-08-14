function newShape = rotXShape(shape, angle)
  newShape = cell(size(shape));
  i = 1;
  for plane = shape.'
    planeMat = cell2mat(plane);
    newShape{i}.point = rotXPoint(planeMat.point, angle);
    newShape{i}.normal = rotXPoint(planeMat.normal, angle);
    newShape{i}.distance = planeMat.distance;
    ++i;
  endfor
endfunction