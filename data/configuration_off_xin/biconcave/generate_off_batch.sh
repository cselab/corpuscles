#Project triangular mesh of sphere onto biconcave shape


#0th level #faces=20 #vertices12
awk -f "../cmd/off.awk" -f "../cmd/sphere2biconcave.awk" icosa.off | tee biconcave.off

#1st level refinement #faces=80 #vertices=42
awk -f "../cmd/off.awk" -f "../cmd/sphere2biconcave.awk" icosa_s.off | tee biconcave_s.off

#2nd level refinement #faces=320 #vertices=162
awk -f "../cmd/off.awk" -f "../cmd/sphere2biconcave.awk" icosa_ss.off | tee biconcave_ss.off

#3rd level refinement #faces=1280 #vertices=642
awk -f "../cmd/off.awk" -f "../cmd/sphere2biconcave.awk" icosa_sss.off | tee biconcave_sss.off

#4th level refinement #faces=5120 #vertices= 2562
awk -f "../cmd/off.awk" -f "../cmd/sphere2biconcave.awk" icosa_ssss.off | tee biconcave_ssss.off

#5th level refinement #faces=20480 #vertices= 10242
awk -f "../cmd/off.awk" -f "../cmd/sphere2biconcave.awk" icosa_sssss.off | tee biconcave_sssss.off

#6th level refinement #faces=81920 #vertices= 40962
awk -f "../cmd/off.awk" -f "../cmd/sphere2biconcave.awk" icosa_ssssss.off | tee biconcave_ssssss.off
