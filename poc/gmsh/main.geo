element_size
= 0.05;
l = -0.5; h = 0.5;

Point(1) = {l, l, 0, element_size };
Point(2) = {h, l, 0, element_size };
Point(3) = {h, h, 0, element_size };
Point(4) = {l, h, 0, element_size };

Line(7) = {1,2};
Line(8) = {2,3};
Line(9) = {3,4};
Line(10) = {4,1};

Line Loop(11) = {7,8,9,10};

Plane Surface(12) = {11};

