./main < ../../../data/configuration_off_xin/sphere/icosa.off        | tee cm_eng_sph_dp.dat        | grep "##" | cut -c3- > nt_area_eng_sph.dat
./main < ../../../data/configuration_off_xin/sphere/icosa_s.off      | tee cm_eng_sph_s_dp.dat      | grep "##[1-9]" | cut -c3- >> nt_area_eng_sph.dat
./main < ../../../data/configuration_off_xin/sphere/icosa_ss.off     | tee cm_eng_sph_ss_dp.dat     | grep "##[1-9]" | cut -c3- >> nt_area_eng_sph.dat
./main < ../../../data/configuration_off_xin/sphere/icosa_sss.off    | tee cm_eng_sph_sss_dp.dat    | grep "##[1-9]" | cut -c3- >> nt_area_eng_sph.dat
./main < ../../../data/configuration_off_xin/sphere/icosa_ssss.off   | tee cm_eng_sph_ssss_dp.dat   | grep "##[1-9]" | cut -c3- >> nt_area_eng_sph.dat
./main < ../../../data/configuration_off_xin/sphere/icosa_sssss.off  | tee cm_eng_sph_sssss_dp.dat  | grep "##[1-9]" | cut -c3- >> nt_area_eng_sph.dat
./main < ../../../data/configuration_off_xin/sphere/icosa_ssssss.off | tee cm_eng_sph_ssssss_dp.dat | grep "##[1-9]" | cut -c3- >> nt_area_eng_sph.dat
