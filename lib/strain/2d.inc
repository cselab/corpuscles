int strain_2d(void *param, real (*F1)(void*, real, real), real (*F2)(void*, real, real),
              real bx, real cx, real cy, real ux, real wx, real wy,
              real *dvx, real *dvy, real *dux, real *duy, real *dwx,
              real *dwy, real *pI1, real *pI2, real *pA)
{
    real px, py, qy, I1, I2, Fa, Fb;

    NOT_ZERO(bx);
    NOT_ZERO(cy);
    NOT_ZERO(ux);
    NOT_ZERO(wy);
    px = ux/bx;
    py = (wx-(cx*ux)/bx)/cy;
    qy = wy/cy;
    I1 = px*qy-1;
    I2 = ((qy*(qy-2*px)+sq(py)+sq(px))/(px*qy))/2.0E+0;
    Fa = F1(param,I1,I2);
    Fb = F2(param,I1,I2);
    
    SET(dvx, 
    (((Fb*(cy*(sq(qy)+sq(py))+px*(2*cx*py-2*bx*py-cy*px)))/(cy*sq(px)*qy))/2.0E+0
     -Fa*qy)
     /bx);
    SET(dvy, 
    (Fa*(cy*py+(cx-bx)*px)-((Fb*((cy*py+(bx-cx)*px)*sq(qy)
                                +px*((cx-bx)*sq(py)+px*(cy*py+(cx-bx)*px))
                                +cy*pow(py,3)))
                           /(sq(px)*sq(qy)))
                           /2.0E+0)
     /(bx*cy));
    SET(dux, 
    (Fa*qy-((Fb*(cy*(sq(qy)+sq(py))+px*(2*cx*py-cy*px)))/(cy*sq(px)*qy))/2.0E+0)
     /bx);
    SET(duy, 
    (((Fb*((cy*py-cx*px)*sq(qy)+px*(cx*sq(py)+px*(cy*py+cx*px))+cy*pow(py,3)))
     /(sq(px)*sq(qy)))
     /2.0E+0
     -Fa*(cy*py+cx*px))
     /(bx*cy));
    SET(dwx, (Fb*py)/(cy*px*qy));
    SET(dwy, (((Fb*(sq(qy)-sq(py)-sq(px)))/(px*sq(qy)))/2.0E+0+Fa*px)/cy);
    SET(pI1, I1);
    SET(pI2, I2);
    SET(pA, -(bx*cy)/2.0E+0);

    return CO_OK;
}
