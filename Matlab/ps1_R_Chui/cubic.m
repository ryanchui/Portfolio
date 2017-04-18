function [largest]=cubic(a, b, c, d)
      if (a == 0.)
        [largest] = quadfcn(b, c, d);
        return
      end

      p  = c/a - b*b/a/a/3. ;
      q  = (2.*b*b*b/a/a/a - 9.*b*c/a/a + 27.*d/a) / 27. ;
      
      DD = p*p*p/27. + q*q/4. ;
        
          if (DD < 0.)
            phi = acos(-q/2./sqrt(abs(p*p*p)/27.));
            temp1=2.*sqrt(abs(p)/3.);
            y1 =  temp1*cos(phi/3.);
            y2 = -temp1*cos((phi+pi)/3.);
            y3 = -temp1*cos((phi-pi)/3.);
          else
            temp1 = -q/2. + sqrt(DD);
            temp2 = -q/2. - sqrt(DD);
            u = abs(temp1)^(1./3.);
            v = abs(temp2)^(1./3.);
            if (temp1 < 0.) u=-u; end
            if (temp2 < 0.) v=-v; end
            y1  = u + v;
            y2r = -(u+v)/2.;
            y2i =  (u-v)*sqrt(3.)/2.;
          end

          temp1 = b/a/3.;
          y1 = y1-temp1;
          if (DD < 0.)
            y2 = y2-temp1;
            y3 = y3-temp1;
          else
            y2r=y2r-temp1;
          end

          if (DD < 0.)
            x(1) = y1;
            x(2) = y2;
            x(3) = y3;
          elseif (DD == 0.)
            x(1) = y1;
            x(2) = y2r;
            x(3) = y2r;
          else
            x(1) = y1;
            x(2) = y2r + y2i*1i;
            x(3) = y2r - y2i*1i;
          end
          largest = x(:);
end

function [x,nroot]=quadfcn(a, b, c)
      if (a == 0)
        if (b == 0)
          nroot = 0;
          x = [];
        else
          nroot = 1;
          x = -c/b;
        end
      else
        nroot = 2;
        DD = b*b-4*a*c;
        x(1) = (-b+sqrt(DD))/2/a;
        x(2) = (-b-sqrt(DD))/2/a;
      end
end

function relativeError = rootRelativeError(currRoot, computedRoot)
    if currRoot == 0
        if computedRoot == 0
            relativeError = 1e-6;
        else
            relativeError = Inf;
        end
    else
        relativeError = abs(currRoot - computedRoot) / abs(currRoot);
    end
end