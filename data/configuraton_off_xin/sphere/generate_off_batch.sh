#Please see README for the name convention of files
#
#Given a icosa.off file at the 0 lelve of refinement,
#that is, an icosahedron with 12 vertices defined on a unit sphere,
#we refine the icosahedron faces
#1a) divide each triangle into four new triangles or
#1b) divide each triangle into four new triangles
#    and project the vertices on the unit sphere
# ...
#na) ...
#nb) ...
#[n+1]a) ...
#[n+1]b) ...
#For each level of refinement we have two opitions,
#either simply refine the frace or
#refine the face and then project the vertices on the unit sphere


#0th level #faces=20 #vertices12
#the given icosa.off file

#1st level refinement #faces=80 #vertices=42 #choice=2
off.refine icosa.off    | tee icosa_r.off
off.refine -s icosa.off | tee icosa_s.off

#2nd level refinement #faces=320 #vertices=162 #choice=4
off.refine icosa_r.off    | tee icosa_rr.off
off.refine -s icosa_r.off | tee icosa_rs.off

off.refine icosa_s.off    | tee icosa_sr.off
off.refine -s icosa_s.off | tee icosa_ss.off

#3rd level refinement #faces=1280 #vertices=642 #choice=8
off.refine icosa_rr.off    | tee icosa_rrr.off
off.refine -s icosa_rr.off | tee icosa_rrs.off

off.refine icosa_rs.off    | tee icosa_rsr.off
off.refine -s icosa_rs.off | tee icosa_rss.off

off.refine icosa_sr.off    | tee icosa_srr.off
off.refine -s icosa_sr.off | tee icosa_srs.off

off.refine icosa_ss.off    | tee icosa_ssr.off
off.refine -s icosa_ss.off | tee icosa_sss.off

#4th level refinement #faces=5120 #vertices= 2562 #choice=16
off.refine icosa_rrr.off    | tee icosa_rrrr.off
off.refine -s icosa_rrr.off | tee icosa_rrrs.off

off.refine icosa_rrs.off    | tee icosa_rrsr.off
off.refine -s icosa_rrs.off | tee icosa_rrss.off

off.refine icosa_rsr.off    | tee icosa_rsrr.off
off.refine -s icosa_rsr.off | tee icosa_rsrs.off

off.refine icosa_rss.off    | tee icosa_rssr.off
off.refine -s icosa_rss.off | tee icosa_rsss.off

off.refine icosa_srr.off    | tee icosa_srrr.off
off.refine -s icosa_srr.off | tee icosa_srrs.off

off.refine icosa_srs.off    | tee icosa_srsr.off
off.refine -s icosa_srs.off | tee icosa_srss.off

off.refine icosa_ssr.off    | tee icosa_ssrr.off
off.refine -s icosa_ssr.off | tee icosa_ssrs.off

off.refine icosa_sss.off    | tee icosa_sssr.off
off.refine -s icosa_sss.off | tee icosa_ssss.off

#5th level refinement #faces=20480 #vertices=10242 # only those on spheres at all level 
off.refine -s icosa_ssss.off | tee icosa_sssss.off

#6th level refinement #faces=81920 #vertices=40962 # only those on spheres at all level
off.refine -s icosa_sssss.off | tee icosa_ssssss.off
