module mod
contains
  function F(x, y, idata, rdata)
    use iso_c_binding    
    implicit none
    real :: x, y
    integer(c_int), intent(in) :: idata(:)
    real(c_double), intent(in) :: rdata(:)
    real :: F
    print *, x, y
    F = 1.0
  end function F
end module mod

subroutine cubtri2(T, EPS, MCALLS, ANS, ERR, NCALLS, W, NW, IER) bind(c)
  use iso_c_binding
  use mod
  implicit none
  real(c_double), intent(in) :: T(2, 3)
  real(c_double), intent(in), value :: EPS
  integer(c_int), intent(in), value ::  MCALLS
  real(c_double), intent(out) :: ANS
  real(c_double), intent(out) :: ERR
  integer(c_int), intent(out) :: NCALLS
  real(c_double), intent(in) :: W(NW)
  integer(c_int), intent(in), value :: NW
  integer(c_int), intent(out) :: IER

  integer :: idata(1)
  real :: rdata(1)
  real :: tt(2, 3)

  tt = TT
  call cubtri(tt, EPS, MCALLS, ANS, ERR, NCALLS, W, NW, IER, idata, rdata)
  !print *, F(1.3, 4.5, idata, rdata)
  !ans = 4.2
  !err = 1e-12
  !ncalls = 123
  !ier = 0
end subroutine cubtri2
