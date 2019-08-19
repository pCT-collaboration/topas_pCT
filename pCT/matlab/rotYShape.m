function newShape = rotYShape(shape, angle)
  newShape = cell(size(shape));
  i = 1;
  for plane = shape.'
    planeMat = cell2mat(plane);
    newShape{i}.point = rotYPoint(planeMat.point, angle);
    newShape{i}.normal = rotYPoint(planeMat.normal, angle);
    newShape{i}.distance = planeMat.distance;
    i = i + 1;
  end
end