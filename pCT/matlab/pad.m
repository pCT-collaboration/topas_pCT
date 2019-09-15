function padVal = pad(value)
  if (value < 10)
    padVal = strcat("00", num2str(value));
  elseif (value < 100)
    padVal = strcat("0", num2str(value));
  else
    padVal = num2str(value);
  end
end