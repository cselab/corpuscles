% Solve y'(t)=-2y(t) with y0=3, 4th order Runge Kutta
y0 = 3;
h=0.2;
t = 0:h:2;
yexact = 3*exp(-2*t)
y = zeros(size(t));
y(1) = y0;
for i=1:(length(t)-1)
  k1 = -2*y(i)
  y1 = y(i)+k1*h/2;
  k2 = -2*y1;
  y2 = y(i)+k2*h/2;
  k3 = -2*y2;
  y3 = y(i)+k3*h;
  k4 = -2*y3;
  y(i+1) = y(i) + (k1+2*k2+2*k3+k4)*h/6;
end
plot(t,yexact,t,y);
