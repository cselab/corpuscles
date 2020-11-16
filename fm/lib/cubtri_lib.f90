module mod
  use iso_c_binding
  implicit none
  type(c_ptr)                       :: p
  interface
     function fun(p, x, y) bind(c)
       use iso_c_binding
       implicit none
       real(c_double), intent(in), value :: x, y
       type(c_ptr), intent(in), value :: p
       real(c_double) :: fun
     end function fun
  end interface
contains
  function F(x, y, idata, rdata)
    use iso_c_binding
    implicit none
    real :: x, y
    integer, intent(in) :: idata(:)
    real, intent(in) :: rdata(:)
    real :: F
    real(c_double) :: x0, y0
    x0 = x
    y0 = y
    F = fun(p, x0, y0)
  end function F
end module mod

subroutine cubtri2(PTR, T, EPS, MCALLS, ANS, ERR, NCALLS, W, NW, IER) bind(c)
  use iso_c_binding
  use mod
  implicit none
  real(c_double), intent(in) :: T(2, 3)
  real(c_double), intent(in), value :: EPS
  integer(c_int), intent(in), value ::  MCALLS
  type(c_ptr) , intent(in), value  :: PTR
  real(c_double), intent(out) :: ANS
  real(c_double), intent(out) :: ERR
  integer(c_int), intent(out) :: NCALLS
  real(c_double), intent(in) :: W(NW)
  integer(c_int), intent(in), value :: NW
  integer(c_int), intent(out) :: IER

  integer :: idata(1)
  real :: rdata(1)

  real :: T0(2, 3)
  real :: EPS0
  integer ::  MCALLS0
  real :: ANS0
  real :: ERR0
  integer :: NCALLS0
  real :: W0(NW)
  integer  :: NW0
  integer :: IER0

  T0 = T
  EPS0 = EPS
  MCALLS0 = MCALLS
  W0 = W
  NW0 = NW

  p = ptr

  call cubtri(F, T0, EPS0, MCALLS0, ANS0, ERR0, NCALLS0, W0, NW0, IER0, idata, rdata)
  ANS = ANS0
  ERR = ERR0
  NCALLS = NCALLS0
end subroutine cubtri2
