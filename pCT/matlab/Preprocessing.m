function Preprocessing(directory, phantom_name, angle, preparer)
  load P.mat
  load S.mat
  load MU.mat
  iFileName = strcat(directory, "/", phantom_name, "/", pad(angle));
  histories = getSize(strcat(iFileName, "/Tracker1t_Out.header"));
  p1tOut = GetDataPhSp(strcat(iFileName, "/Tracker1t_Out.phsp"), histories);
  p1vOut = GetDataPhSp(strcat(iFileName, "/Tracker1v_Out.phsp"), histories);
  p2tOut = GetDataPhSp(strcat(iFileName, "/Tracker2t_Out.phsp"), histories);
  p2vOut = GetDataPhSp(strcat(iFileName, "/Tracker2v_Out.phsp"), histories);
  p3tOut = GetDataPhSp(strcat(iFileName, "/Tracker3t_Out.phsp"), histories);
  p3vOut = GetDataPhSp(strcat(iFileName, "/Tracker3v_Out.phsp"), histories);
  p4tOut = GetDataPhSp(strcat(iFileName, "/Tracker4t_Out.phsp"), histories);
  p4vOut = GetDataPhSp(strcat(iFileName, "/Tracker4v_Out.phsp"), histories);
  sizes = [size(p1tOut, 1), size(p1vOut, 1), size(p2tOut, 1), size(p2vOut, 1), size(p3tOut, 1), size(p3vOut, 1), size(p4tOut, 1), size(p4vOut, 1)];
  imax = max(sizes);
  oFileName = strcat(directory, "/", phantom_name, "/bin/projection_", pad(angle), ".bin")
  Z_C = [-214.3, -164.3, 164.3, 214.3];
  i = 1;
  index = 1;
  flag = p1tOut(:,1) & p1vOut(:,1) & p2tOut(:,1) & p2vOut(:,1) & p3tOut(:,1) & p3vOut(:,1) & p4tOut(:,1) & p4vOut(:,1);
  Vw0 = p1vOut(:,4);
  Vw1 = p2vOut(:,4);
  Vw2 = p3vOut(:,4);
  Vw3 = p4vOut(:,4);

  Tw0 = p1tOut(:,3);
  Tw1 = p2tOut(:,3);
  Tw2 = p3tOut(:,3);
  Tw3 = p4tOut(:,3);

  WEPL = polyval(P, p2tOut(:,6)- p3tOut(:,6), S, MU);
  list = find(flag);
  s = size(list,1);
  Vw0 = Vw0(list);
  Vw1 = Vw1(list);
  Vw2 = Vw2(list);
  Vw3 = Vw3(list);
  Tw0 = Tw0(list);
  Tw1 = Tw1(list);
  Tw2 = Tw2(list);
  Tw3 = Tw3(list);
  Uw0(1:s) = Z_C(1);
  Uw1(1:s) = Z_C(2);
  Uw2(1:s) = Z_C(3);
  Uw3(1:s) = Z_C(4);
  WEPL = WEPL(list);
  index = size(list,1);
  ofile = fopen(oFileName,'w');
  magic_number = "PCTD";
  version_id = 0;
  beam_energy = 200.;
  current_time = now();
  projection_angle = single(angle);
  phantom_name_size = strlength(phantom_name);
  iFileName_size = strlength(iFileName);
  prepared_by_size = strlength(preparer);
  fwrite(ofile, magic_number, "4*char");
  fwrite(ofile, version_id, "int32");
  fwrite(ofile, index, "int32");
  fwrite(ofile, projection_angle, "single");
  fwrite(ofile, beam_energy, "single");
  fwrite(ofile, current_time, "int32");
  fwrite(ofile, current_time, "int32");
  fwrite(ofile, phantom_name_size, "int32");
  fwrite(ofile, phantom_name, strcat(num2str(phantom_name_size), "*char"));
  fwrite(ofile, iFileName_size, "int32");
  fwrite(ofile, iFileName, strcat(num2str(iFileName_size), "*char"));
  fwrite(ofile, prepared_by_size, "int32");
  fwrite(ofile, preparer, strcat(num2str(prepared_by_size), "*char"));
  fwrite(ofile, Tw0, "single");
  fwrite(ofile, Tw1, "single");
  fwrite(ofile, Tw2, "single");
  fwrite(ofile, Tw3, "single");
  fwrite(ofile, Vw0, "single");
  fwrite(ofile, Vw1, "single");
  fwrite(ofile, Vw2, "single");
  fwrite(ofile, Vw3, "single");
  fwrite(ofile, Uw0, "single");
  fwrite(ofile, Uw1, "single");
  fwrite(ofile, Uw2, "single");
  fwrite(ofile, Uw3, "single");
  fwrite(ofile, WEPL, "single");
  fclose(ofile);
end
