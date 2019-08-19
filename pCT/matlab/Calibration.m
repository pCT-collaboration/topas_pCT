function [P, S, MU] = Calibration(directory)
  dx1 = 4.75;
  dx2 = 104.5;
  dy1 = 60;
  dy2 = 60;
  dz = 25.4;
  transz = -101.6;
  wedge = trapiziod(dx1, dx2, dy1, dy2, dz);
  wedge = transShape(wedge, Point(0, 0, transz));
  Z_C = [-214.3, -164.3, 164.3, 214.3];
  w = [];
  e = [];
  index = 0;
  for bricks = 0:4
    brick_hx = dx2;
    brick_hy = dy2;
    brick_hz = 25.4 * bricks;
    brick = box(brick_hx, brick_hy, brick_hz);
    brick = transShape(brick, Point(0, 0, transz + dz + brick_hz));
    p1tOut = GetDataPhSp(strcat(directory, "/", num2str(bricks), "/Tracker1t_Out.phsp"));
    p1vOut = GetDataPhSp(strcat(directory, "/", num2str(bricks), "/Tracker1v_Out.phsp"));
    p2tOut = GetDataPhSp(strcat(directory, "/", num2str(bricks), "/Tracker2t_Out.phsp"));
    p2vOut = GetDataPhSp(strcat(directory, "/", num2str(bricks), "/Tracker2v_Out.phsp"));
    p3tOut = GetDataPhSp(strcat(directory, "/", num2str(bricks), "/Tracker3t_Out.phsp"));
    p3vOut = GetDataPhSp(strcat(directory, "/", num2str(bricks), "/Tracker3v_Out.phsp"));
    p4tOut = GetDataPhSp(strcat(directory, "/", num2str(bricks), "/Tracker4t_Out.phsp"));
    p4vOut = GetDataPhSp(strcat(directory, "/", num2str(bricks), "/Tracker4v_Out.phsp"));
    i = 1;
    sizes = [size(p1tOut,1), size(p1vOut,1), size(p2tOut,1), size(p2vOut,1), size(p3tOut,1), size(p3vOut,1), size(p4tOut,1), size(p4vOut,1)];
    imax = min(sizes);
    count = 0;
    while (count <= 500)
      if (p1tOut(i,1) == 1 && p1vOut(i,1) == 1 && ...
          p2tOut(i,1) == 1 && p2vOut(i,1) == 1 && ...
          p3tOut(i,1) == 1 && p3vOut(i,1) == 1 && ...
          p4tOut(i,1) == 1 && p4vOut(i,1) == 1)
        p1 = Point(p1vOut(i, 3), p1tOut(i,4), Z_C(1));
        p2 = Point(p2vOut(i, 3), p2tOut(i,4), Z_C(2));
        r12 = Ray(p1, p2-p1);
        intersect1 = rayIntersectShape(r12, wedge);
        p3 = Point(p3vOut(i, 3), p3tOut(i,4), Z_C(3));
        p4 = Point(p4vOut(i, 3), p4tOut(i,4), Z_C(4));
        r34 = Ray(p4, p3-p4);
        intersect2 = rayIntersectShape(r34, wedge);
        d = 0;
        if (size(intersect1,2) == 2 && size(intersect2,2) == 2)
          if (intersect1(1).distance < intersect1(2).distance)
            entry = intersect1(1).point;
          else
            entry = intersect1(2).point;
          end
          if (intersect2(1).distance < intersect2(2).distance)
            exit = intersect2(1).point;
          else
            exit = intersect2(2).point;
          end
          d = norm(entry - exit);
          if (bricks > 0)
            intersect3 = rayIntersectShape(r12, brick);
            intersect4 = rayIntersectShape(r34, brick);
            if (size(intersect3,2) == 2 && size(intersect4,2) == 2)
              if (intersect4(1).distance < intersect4(2).distance)
                exit = intersect4(1).point;
              else
                exit = intersect4(2).point;
              end
              d = norm(entry - exit);
            end
          end
        end
        if (d > 0)
          count = count + 1;
          index = index + 1;
          w(index) = d;
          e(index) = p2tOut(i,6)- p3tOut(i,6);
        end
      end  
      i = i + 1;
    end
  end
  [P, S, MU] = polyfit(e, w, 7);
%  figure(1);
%  hold on
%  title("Figure 1. Scatter plot of energy loss vs. WEPL")
%  xlabel("Energy Loss (MeV)");
%  ylabel("WEPL (mm)");
%  scatter(e, w, [], [1, 0, 0]);
%  x = 0:160;
%  plot(x, polyval(P, x, S, MU), "color", "black");
%  hold off
  w_exp = polyval(P, e, S, MU);
  mask = abs(w - w_exp) < 20.0;
  w2 = [];
  e2 = [];
  out = 0;
  for in = 1:size(mask,2)
    if (mask(in))
      out = out + 1;
      w2(out) = w(in);
      e2(out) = e(in);
    end
  end
  [P, S, MU] = polyfit(e2, w2, 7);
%  figure(2);
%  hold on
%  title("Figure 2. Scatter plot of same with outliers removed")
%  xlabel("Energy Loss (MeV)");
%  ylabel("WEPL (mm)");
%  scatter(e2, w2, [], [1, 0, 0]);
%  x = 0:160;
%  plot(x, polyval(P, x, S, MU), "color", "black");
%  hold off
  save P.mat P
  save S.mat S
  save MU.mat MU
end
