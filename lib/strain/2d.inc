int strain_2d(void *param, real (*F1)(void*, real, real), real (*F2)(void*, real, real),
              real bx, real by, real cx, real cy, real ux, real uy,
              real wx, real wy, real *dvx, real *dvy, real *dux,
              real *duy, real *dwx, real *dwy, real *pI1, real *pI2,
              real *pA)
{
    real Q, px, py, qx, qy, I1, I2, Fa, Fb;

    Q = by*cx-bx*cy;
    NOT_ZERO(Q);
    px = (by*wx-cy*(ux+bx)+by*cx)/Q;
    py = (cx*ux-bx*wx)/Q;
    qx = (by*wy-cy*uy)/Q;
    qy = -(bx*wy-cx*uy+bx*cy-by*cx)/Q;
    I1 = sq(qy)+sq(qx)+sq(py)+sq(px);
    I2 = pow(px*qy-py*qx,2);
    Fa = F1(param,I1,I2);
    Fb = F2(param,I1,I2);
    
    SET(dvx, (2*(Fb*((cy-by)*qy+(cx-bx)*qx)*(px*qy-py*qx)-Fa*((cx-bx)*py+(by-cy)*px)))/Q);
    SET(dvy, -(2*(Fb*((cy-by)*py+(cx-bx)*px)*(px*qy-py*qx)+Fa*((cx-bx)*qy+(by-cy)*qx)))/Q);
    SET(dux, (2*(Fa*(cx*py-cy*px)-Fb*(cy*qy+cx*qx)*(px*qy-py*qx)))/Q);
    SET(duy, (2*(Fb*(cy*py+cx*px)*(px*qy-py*qx)+Fa*(cx*qy-cy*qx)))/Q);
    SET(dwx, (2*(Fb*(by*qy+bx*qx)*(px*qy-py*qx)-Fa*(bx*py-by*px)))/Q);
    SET(dwy, -(2*(Fb*(by*py+bx*px)*(px*qy-py*qx)+Fa*(bx*qy-by*qx)))/Q);
    SET(pI1, I1);
    SET(pI2, I2);
    SET(pA, Q/2.0E+0);

    return HE_OK;
}
