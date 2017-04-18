function [ylast] = RungeKutta(t0,y0,tlast,N,a)
% t0: initial value of t
% y0: initial value of y
% tlast: final value of t
% ylast: final value of y
% N: number of steps in Euler scheme
% a: parameter of the problem
F = @(t, y) a*(y.^2)*(1-y);

y = y0;
ylast = y;
h = tlast / N;
for t = t0: h : tlast - h
    s1 = F(t,y);
    s2 = F(t+h/2,y+h*s1/2);
    s3 = F(t+h/2,y+h*s2/2);
    s4 = F(t+h,y+h*s3);
    y = y + h*(s1+ 2*s2 + 2*s3 + s4) / 6;
    [ylast] = [ylast; y];
end

