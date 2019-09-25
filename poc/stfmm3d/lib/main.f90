subroutine stfmm3_dpartself(&
     ier,iprec,nparts,source,&
     ifsingle,sigma_sl,ifdouble,sigma_dl,sigma_dv,&
     ifpot,pot,pre,ifgrad,grad) &
     bind(c)
  use iso_c_binding
  implicit none
  real(c_double), intent(out) :: &
       pot(3,nparts),pre(nparts),grad(3,3,nparts)
  real(c_double), intent(in) :: &
       sigma_dl(3,nparts),sigma_dv(3,nparts),&
       sigma_sl(3,nparts), source(3,nparts)
  integer(c_int), intent(out) ::  ier
  integer(c_int), intent(in), value :: &
       iprec, ifsingle, ifdouble, ifpot, ifgrad,&
       nparts
  call stfmm3dpartself(&
       ier,iprec,nparts,source,&
       ifsingle,sigma_sl,ifdouble,sigma_dl,sigma_dv,&
       ifpot,pot,pre,ifgrad,grad)
end subroutine stfmm3_dpartself
