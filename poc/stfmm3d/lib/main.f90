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

subroutine stfmm3_dparttarg(&
     ier,iprec,nparts,source,&
     ifsingle,sigma_sl,ifdouble,sigma_dl,sigma_dv,&
     ifpot,pot,pre,ifgrad,grad,&
     ntargs,target,ifpottarg,pottarg,pretarg,&
     ifgradtarg,gradtarg) &
     bind(c)
  use iso_c_binding
  implicit none
  real(c_double), intent(out) :: &
       pot(3,nparts),pre(nparts),grad(3,3,nparts),&
       pottarg(3,nparts), pretarg(nparts), gradtarg(3,3,nparts)
  real(c_double), intent(in) :: &
       sigma_dl(3,nparts),sigma_dv(3,nparts),&
       sigma_sl(3,nparts), source(3,nparts),&
       target(3,nparts)
  integer(c_int), intent(out) ::  ier
  integer(c_int), intent(in), value :: &
       iprec, ifsingle, ifdouble, ifpot, ifgrad,&
       nparts, ntargs, ifpottarg, ifgradtarg
  call stfmm3dparttarg(&
       ier,iprec,nparts,source,&
       ifsingle,sigma_sl,ifdouble,sigma_dl,sigma_dv,&
       ifpot,pot,pre,ifgrad,grad,&
       ntargs,target,ifpottarg,pottarg,pretarg,&
       ifgradtarg,gradtarg)
end subroutine stfmm3_dparttarg

subroutine stfmm3_dpartdirect(&
     nparts,source,&
     ifsingle,sigma_sl,ifdouble,sigma_dl,sigma_dv,&
     ifpot,pot,pre,ifgrad,grad,&
     ntargs,target,ifpottarg,pottarg,pretarg,&
     ifgradtarg,gradtarg) &
     bind(c)
  use iso_c_binding
  implicit none
  real(c_double), intent(out) :: &
       pot(3,nparts),pre(nparts),grad(3,3,nparts),&
       pottarg(3,nparts), pretarg(nparts), gradtarg(3,3,nparts)
  real(c_double), intent(in) :: &
       sigma_dl(3,nparts),sigma_dv(3,nparts),&
       sigma_sl(3,nparts), source(3,nparts),&
       target(3,nparts)
  integer(c_int), intent(in), value :: &
       ifsingle, ifdouble, ifpot, ifgrad,&
       nparts, ntargs, ifpottarg, ifgradtarg
  call st3dpartdirect(&
       nparts,source,&
       ifsingle,sigma_sl,ifdouble,sigma_dl,sigma_dv,&
       ifpot,pot,pre,ifgrad,grad,&
       ntargs,target,ifpottarg,pottarg,pretarg,&
       ifgradtarg,gradtarg)
end subroutine stfmm3_dpartdirect
