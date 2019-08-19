function Preprocessing(directory, phantom_name, angle, preparer)
  load P.mat
  load S.mat
  load MU.mat
  iFileName = [directory "/" phantom_name "/" num2str(angle)];
  p1tOut = GetDataPhSp([iFileName "/Tracker1t_Out.phsp"]);
  p1vOut = GetDataPhSp([iFileName "/Tracker1v_Out.phsp"]);
  p2tOut = GetDataPhSp([iFileName "/Tracker2t_Out.phsp"]);
  p2vOut = GetDataPhSp([iFileName "/Tracker2v_Out.phsp"]);
  p3tOut = GetDataPhSp([iFileName "/Tracker3t_Out.phsp"]);
  p3vOut = GetDataPhSp([iFileName "/Tracker3v_Out.phsp"]);
  p4tOut = GetDataPhSp([iFileName "/Tracker4t_Out.phsp"]);
  p4vOut = GetDataPhSp([iFileName "/Tracker4v_Out.phsp"]);
  sizes = [size(p1tOut)(1), size(p1vOut)(1), size(p2tOut)(1), size(p2vOut)(1), size(p3tOut)(1), size(p3vOut)(1), size(p4tOut)(1), size(p4vOut)(1)];
  imax = min(sizes);
  oFileName = [directory "/" phantom_name "/projection_" num2str(angle) ".bin"];
  Z_C = [-214.3, -164.3, 164.3, 214.3];
  Vw0 = [];
  Vw1 = [];
  Vw2 = [];
  Vw3 = [];
  
  Tw0 = [];
  Tw1 = [];
  Tw2 = [];
  Tw3 = [];
  
  Uw0 = [];
  Uw1 = [];
  Uw2 = [];
  Uw3 = [];
  
  WEPL = [];
  i = 1;
  index = 1;
  while (i <= imax)
    if (p1tOut(i,1) == 1 && p1vOut(i,1) == 1 && 
        p2tOut(i,1) == 1 && p2vOut(i,1) == 1 && 
        p3tOut(i,1) == 1 && p3vOut(i,1) == 1 && 
        p4tOut(i,1) == 1 && p4vOut(i,1) == 1)
      Vw0(index) = p1vOut(i,4);
      Vw1(index) = p2vOut(i,4);
      Vw2(index) = p3vOut(i,4);
      Vw3(index) = p4vOut(i,4);

      Tw0(index) = p1tOut(i,3);
      Tw1(index) = p2tOut(i,3);
      Tw2(index) = p3tOut(i,3);
      Tw3(index) = p4tOut(i,3);

      Uw0(index) = Z_C(1);
      Uw1(index) = Z_C(2);
      Uw2(index) = Z_C(3);
      Uw3(index) = Z_C(4);

      WEPL(index) = polyval(P, p2tOut(i,6)- p3tOut(i,6), S, MU);
      index = index + 1;
    end
    i = i + 1;
  end
  index = index - 1;
  ofile = fopen(oFileName,'w');
  magic_number = "PCTD";
  version_id = 0;
  beam_energy = 200.;
  current_time = now();
  projection_angle = single(angle);
  phantom_name_size = length(phantom_name);
  iFileName_size = length(iFileName);
  prepared_by_size = length(preparer);
  fwrite(ofile, magic_number, "4*char");
  fwrite(ofile, version_id, "int32");
  fwrite(ofile, index, "int32");
  fwrite(ofile, projection_angle, "single");
  fwrite(ofile, beam_energy, "single");
  fwrite(ofile, current_time, "int32");
  fwrite(ofile, current_time, "int32");
  fwrite(ofile, phantom_name_size, "int32");
  fwrite(ofile, phantom_name, [num2str(phantom_name_size) "*char"]);
  fwrite(ofile, iFileName_size, "int32");
  fwrite(ofile, iFileName, [num2str(iFileName_size) "*char"]);
  fwrite(ofile, prepared_by_size, "int32");
  fwrite(ofile, preparer, [num2str(prepared_by_size) "*char"]);
  fwrite(ofile, Tw0, [num2str(index) "*single"]);
  fwrite(ofile, Tw1, [num2str(index) "*single"]);
  fwrite(ofile, Tw2, [num2str(index) "*single"]);
  fwrite(ofile, Tw3, [num2str(index) "*single"]);
  fwrite(ofile, Vw0, [num2str(index) "*single"]);
  fwrite(ofile, Vw1, [num2str(index) "*single"]);
  fwrite(ofile, Vw2, [num2str(index) "*single"]);
  fwrite(ofile, Vw3, [num2str(index) "*single"]);
  fwrite(ofile, Uw0, [num2str(index) "*single"]);
  fwrite(ofile, Uw1, [num2str(index) "*single"]);
  fwrite(ofile, Uw2, [num2str(index) "*single"]);
  fwrite(ofile, Uw3, [num2str(index) "*single"]);
  fwrite(ofile, WEPL, [num2str(index) "*single"]);
  fclose(ofile);
end