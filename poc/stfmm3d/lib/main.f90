subroutine stfmm3_dpartself(&
     ier,iprec,nparts,source,&
     ifsingle,sigma_sl,ifdouble,sigma_dl,sigma_dv,&
     ifpot,pot,pre,ifgrad,grad)bind(c)

  use iso_c_binding
  implicit real *8 (a-h,o-z)

  real *8 source(3,nparts)
  real *8 sigma_sl(3,nparts)
  real *8 sigma_dl(3,nparts),sigma_dv(3,nparts)
  real *8 pot(3,nparts),pre(nparts),train(3,3,nparts)
  integer nparts,ntargs

  call stfmm3dpartself(&
       ier,iprec,nparts,source,&
       ifsingle,sigma_sl,ifdouble,sigma_dl,sigma_dv,&
       ifpot,pot,pre,ifgrad,grad)

end subroutine stfmm3_dpartself
