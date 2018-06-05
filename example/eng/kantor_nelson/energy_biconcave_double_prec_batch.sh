./main < ../../../data/configuration_off_xin/biconcave/biconcave.off        | tee cm_eng_bic_dp.dat        | grep "##" | cut -c3- > nt_area_eng_bic.dat
./main < ../../../data/configuration_off_xin/biconcave/biconcave_s.off      | tee cm_eng_bic_s_dp.dat      | grep "##[1-9]" | cut -c3- >> nt_area_eng_bic.dat
./main < ../../../data/configuration_off_xin/biconcave/biconcave_ss.off     | tee cm_eng_bic_ss_dp.dat     | grep "##[1-9]" | cut -c3- >> nt_area_eng_bic.dat
./main < ../../../data/configuration_off_xin/biconcave/biconcave_sss.off    | tee cm_eng_bic_sss_dp.dat    | grep "##[1-9]" | cut -c3- >> nt_area_eng_bic.dat
./main < ../../../data/configuration_off_xin/biconcave/biconcave_ssss.off   | tee cm_eng_bic_ssss_dp.dat   | grep "##[1-9]" | cut -c3- >> nt_area_eng_bic.dat
./main < ../../../data/configuration_off_xin/biconcave/biconcave_sssss.off  | tee cm_eng_bic_sssss_dp.dat  | grep "##[1-9]" | cut -c3- >> nt_area_eng_bic.dat
./main < ../../../data/configuration_off_xin/biconcave/biconcave_ssssss.off | tee cm_eng_bic_ssssss_dp.dat | grep "##[1-9]" | cut -c3- >> nt_area_eng_bic.dat
