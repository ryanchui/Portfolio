function [limit] = imaginaryUnits(x0,y0)
% x0: Real part of starting value z0
% y0: Imaginary part of starting value z0
% limit: The limit of the sequence starting at z0
        z = [x0,y0];
        for i = 1:1000
            z = z - formF(z(1),z(2))\formJacobian(z(1),z(2));
            % use vector norm to check for convergence
            if norm(2) < 1e-16
                limit = z(1) + 1j * z(2);
                return % jump out of loop
            end 
                limit = NaN;
        end
end
       
function [J]= formJacobian(x,y)
    J = [2*x -2*y ; 2*y  2*x];
end

function[z] = formF(x,y)
    z = [x^2-y^2+1; 2*x*y];
end




% function [ limit ] = imaginaryUnits( x0, y0 )
% % x0 = real part
% % y0 = imaginary part
% %l limit = limit of sequence starting at z0
% clear i;
% N = 100; % The maximum number of iterations is 100. We think this is sufficient 
%         % as newton's method usually converges quadratically.
% TOL = 1e-16; % This is to know clearly when the inital approximates result
%              % in divergence. We could potenitally use 1e-5, or 1e-6?
% g = zeros(1,N);
% g(1) = 0;
% g(2) = (x0 + i*y0);
% j = 2;
% while j < 100
%     g(j + 1) = (1/2)*(g(j) - (1/g(j)));
%     if abs((g(j+1) - g(j))/g(j+1)) < TOL
%         dist_plus_i = abs(g(2) - i);
%         dist_minus_i = abs(g(2) - (-i));
%         if dist_plus_i <= dist_minus_i
%             limit = i;
%             return;
%         else 
%             limit = -i;
%             return;
%         end
%     else
%         j = j+1;
%     end
% end
% 
% if j == N || isinf(abs(g(j)))
%     limit = NaN;
%     return;
% end
% 


