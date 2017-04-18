function [ylast] = Euler(t0,y0,tlast,N,a)
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
    s = F(t,y);
    y = y + h*s;
    [ylast] = [ylast; y];
end
