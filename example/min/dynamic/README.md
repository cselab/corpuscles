# Data

    $ ls Nt_*/dt_*/mu_*/all | sed 4q
    Nt_1280/dt_0.001/mu_0/all
    Nt_1280/dt_0.001/mu_10/all
    Nt_1280/dt_0.001/mu_1/all
    Nt_1280/dt_0.001/mu_2.5/all
	...
	
	$ sed 1q ./Nt_1280/dt_0.005/mu_7/all
	A/A0 V/V0 Vr eb eb_bend eb_ad ea ega ev ek ee  h0 h1 h2

    gnuplot> plot "./Nt_1280/dt_0.005/mu_7/all" u 'h1'
	
