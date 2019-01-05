int strain_2d(void *param, real (*F1)(void*, real, real), real (*F2)(void*, real, real),
              real bx, real cx, real cy, real ux, real wx, real wy,
              real *dvx, real *dvy, real *dux, real *duy, real *dwx,
              real *dwy, real *pI1, real *pI2, real *pA)
{
    real Q, px, py, qy, I1, I2, Fa, Fb;

    Q = -bx*cy;
    NOT_ZERO(Q);
    px = ux/bx;
    py = (wx-(cx*ux)/bx)/cy;
    qy = wy/cy;
    I1 = sq(qy)+sq(py)+sq(px);
    I2 = sq(px)*sq(qy);
    Fa = F1(param,I1,I2);
    Fb = F2(param,I1,I2);
    
    SET(dvx, (2*((Fa*(cx*py-(bx*py+cy*px)))/cy-Fb*px*pow(qy,2)))/bx);
    SET(dvy, (2*(cx-bx)*(Fb*pow(px,2)+Fa)*qy)/(bx*cy));
    SET(dux, (2*(Fb*px*pow(qy,2)-(Fa*(cx*py-cy*px))/cy))/bx);
    SET(duy, -(2*cx*(Fb*pow(px,2)+Fa)*qy)/(bx*cy));
    SET(dwx, (2*Fa*py)/cy);
    SET(dwy, (2*(Fb*pow(px,2)+Fa)*qy)/cy);
    SET(pI1, I1);
    SET(pI2, I2);
    SET(pA, Q/2.0E+0);

    return HE_OK;
}
