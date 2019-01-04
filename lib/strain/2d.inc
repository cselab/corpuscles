int strain_2d(void *param, real (*F1)(void*, real, real), real (*F2)(void*, real, real),
              real bx, real cx, real cy, real ux, real wx, real wy,
              real *dvx, real *dvy, real *dux, real *duy, real *dwx,
              real *dwy, real *pI1, real *pI2, real *pA)
{
    real Q, px, py, qx, qy, I1, I2, Fa, Fb;

    Q = -bx*cy;
    NOT_ZERO(Q);
    px = ux/bx+1;
    py = (wx-(cx*ux)/bx)/cy;
    qx = 0;
    qy = wy/cy+1;
    I1 = sq(qy)+sq(qx)+sq(py)+sq(px);
    I2 = pow(px*qy-py*qx,2);
    Fa = F1(param,I1,I2);
    Fb = F2(param,I1,I2);
    
    SET(dvx, (2*(Fa*(cx*py-(bx*py+cy*px))-Fb*(cy*qy+(cx-bx)*qx)*(px*qy-py*qx)))/(bx*cy));
    SET(dvy, (2*(Fb*(cy*py+(cx-bx)*px)*(px*qy-py*qx)+Fa*(cx*qy-(bx*qy+cy*qx))))/(bx*cy));
    SET(dux, (2*(Fb*(cy*qy+cx*qx)*(px*qy-py*qx)-Fa*(cx*py-cy*px)))/(bx*cy));
    SET(duy, -(2*(Fb*(cy*py+cx*px)*(px*qy-py*qx)+Fa*(cx*qy-cy*qx)))/(bx*cy));
    SET(dwx, (2*(Fa*py-Fb*qx*(px*qy-py*qx)))/cy);
    SET(dwy, (2*(Fb*px*(px*qy-py*qx)+Fa*qy))/cy);
    SET(pI1, I1);
    SET(pI2, I2);
    SET(pA, Q/2.0E+0);

    return HE_OK;
}
