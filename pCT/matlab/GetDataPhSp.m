function data = GetDataPhSp(fileName, size)
  [fid,msg] = fopen(fileName, "r");
  i = 0;
  data = zeros(size,8);
  while (1)
    [x,c] = fread(fid, 1, "single");
    if (c == 0) 
      break
    end
    [y,c] = fread(fid, 1, "single");
    [z,c] = fread(fid, 1, "single");
    [cosx,c] = fread(fid, 1, "single");
    [cosy,c] = fread(fid, 1, "single");
    [e,c] = fread(fid, 1, "single");
    [w,c] = fread(fid, 1, "single");
    [p,c] = fread(fid, 1, "int32");
    [nc,c] = fread(fid, 1, "int8");
    [f,c] = fread(fid, 1, "int8");
    [rid,c] = fread(fid, 1, "int32");
    [eid,c] = fread(fid, 1, "int32");
    [pid,c] = fread(fid, 1, "int32");
    if (pid ~= 0 || eid == 0)
      continue
    end
    data(eid,1) = 1;
    data(eid,2) = eid;
    data(eid,3) = x*10.0;
    data(eid,4) = y*10.0;
    data(eid,5) = z*10.0;
    data(eid,6) = e;
    data(eid,7) = cosx;
    data(eid,8) = cosy;    
    i = i + 1;
  end
  fclose(fid);
end