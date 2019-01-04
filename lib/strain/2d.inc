int strain_2d(void *param, real (*F1)(void*, real, real), real (*F2)(void*, real, real),
              real bx, real by, real cx, real cy, real ux, real uy,
              real wx, real wy, real *dvx, real *dvy, real *dux,
              real *duy, real *dwx, real *dwy, real *pI1, real *pI2,
              real *pA)
{
    real Q, abx, aby, cax, cay, cbx, cby, px, py, qx, qy, I1, I2;
    real Fa, Fb, d;

    Q = by*cx-bx*cy;
    NOT_ZERO(Q);
    abx = -bx;
    aby = -by;
    cax = cx;
    cay = cy;
    cbx = cx-bx;
    cby = cy-by;
    px = -(aby*wx+cay*ux-(abx*cay-aby*cax))/Q;
    py = (abx*wx+cax*ux)/Q;
    qx = -(aby*wy+cay*uy)/Q;
    qy = (abx*wy+cax*uy-(aby*cax-abx*cay))/Q;
    I1 = sq(qy)+sq(qx)+sq(py)+sq(px);
    I2 = pow(px*qy-py*qx,2);
    Fa = F1(param,I1,I2);
    Fb = F2(param,I1,I2);
    d = (px+1)*qy-py*qx+px+1;
    
    SET(dvx, (2*(Fb*(cby*qy+cbx*qx)*(px*qy-py*qx)-Fa*(cbx*py-cby*px)))/Q);
    SET(dvy, -(2*(Fb*(cby*py+cbx*px)*(px*qy-py*qx)+Fa*(cbx*qy-cby*qx)))/Q);
    SET(dux, (2*(Fa*(cax*py-cay*px)-Fb*(cay*qy+cax*qx)*(px*qy-py*qx)))/Q);
    SET(duy, (2*(Fb*(cay*py+cax*px)*(px*qy-py*qx)+Fa*(cax*qy-cay*qx)))/Q);
    SET(dwx, -(2*(Fb*(aby*qy+abx*qx)*(px*qy-py*qx)+Fa*(aby*px-abx*py)))/Q);
    SET(dwy, (2*(Fb*(aby*py+abx*px)*(px*qy-py*qx)-Fa*(aby*qx-abx*qy)))/Q);
    SET(pI1, I1);
    SET(pI2, I2);
    SET(pA, Q/2.0E+0);

    return HE_OK;
}
