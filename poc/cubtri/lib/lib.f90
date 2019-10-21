subroutine cubtri2(T, EPS, MCALLS, ANS) bind(c)
  use iso_c_binding
  implicit none
  real(c_double), intent(in) :: T(2, 3)
  real(c_double), intent(in), value :: EPS
  integer(c_int), intent(in), value ::  MCALLS
  real(c_double), intent(out) :: ANS
end subroutine cubtri2
