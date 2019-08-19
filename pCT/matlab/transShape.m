function newShape = transShape(shape, vector)
  newShape = cell(size(shape));
  i = 1;
  for plane = shape.'
    planeMat = cell2mat(plane);
%    newShape{i}.point = planeMat.point - vector;
%    newShape{i}.normal = planeMat.normal;
%    newShape{i}.distance = norm(newShape{i}.point);
    newShape{i} = transPlane(planeMat, vector);
    i = i + 1;
  end
end