make
tri.plain 10 12 > ref.off
./main strain ref.off lim 0.1 0.5 0 0 0 0 edge 1 < ref.off  > o.off
