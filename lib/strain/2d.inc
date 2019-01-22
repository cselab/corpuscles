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
    I1 = qy+px-1;
    I2 = ((sq(qy)-2*px*qy+sq(py)+sq(px))/(px*qy))/2.0E+0;
    Fa = F1(param,I1,I2);
    Fb = F2(param,I1,I2);
    
    SET(dvx, 
    ((Fb*(cy*(pow(qy,2)+pow(py,2))+px*(2*cx*py-(2*bx*py+cy*px))))
     /(cy*pow(px,2)*qy)
     -2*Fa)
     /bx);
    SET(dvy, ((cx-bx)*((Fb*(pow(qy,2)-(pow(py,2)+pow(px,2))))/(px*pow(qy,2))+2*Fa))/(bx*cy));
    SET(dux, 
    (Fa-((Fb*(cy*(pow(qy,2)+pow(py,2))+px*(2*cx*py-cy*px)))/(cy*pow(px,2)*qy))
        /2.0E+0)
     /bx);
    SET(duy, 
    (cx*((-((Fb*(pow(qy,2)-(pow(py,2)+pow(px,2))))/(px*pow(qy,2)))/2.0E+0)-Fa))
     /(bx*cy));
    SET(dwx, (Fb*py)/(cy*px*qy));
    SET(dwy, (((Fb*(pow(qy,2)-(pow(py,2)+pow(px,2))))/(px*pow(qy,2)))/2.0E+0+Fa)/cy);
    SET(pI1, I1);
    SET(pI2, I2);
    SET(pA, Q/2.0E+0);

    return HE_OK;
}
