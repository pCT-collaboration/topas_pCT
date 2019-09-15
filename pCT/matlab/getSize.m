function histories = getSize(headerFileName)
  headerFileName
  [fid,msg] = fopen(headerFileName, "r");
  count = 1;
  histories = 1000000;
  while (count == 1)
    [value, count] = fscanf(fid, "%s", 1);
    if (strcmp(value, "Histories:"))
      [value, count] = fscanf(fid, "%s", 1);
      histories = str2num(value);
      break;
    end
  end
end
