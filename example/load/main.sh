make
tri.plain 10 10 > ref.off
./main strain ref.off lim 1 1 0 0 0 0 edge 1 < ref.off  > o.off
