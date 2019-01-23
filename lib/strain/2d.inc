int strain_2d(void *param, real (*F1)(void*, real, real), real (*F2)(void*, real, real),
              real bx, real cx, real cy, real ux, real wx, real wy,
              real *dvx, real *dvy, real *dux, real *duy, real *dwx,
              real *dwy, real *pI1, real *pI2, real *pA)
{
    real Q, px, py, qx, qy, I1, I2, Fa, Fb;

    Q = -bx*cy;
    NOT_ZERO(Q);
    px = ux/bx;
    py = (wx-(cx*ux)/bx)/cy;
    qx = 0;
    qy = wy/cy;
    I1 = px*qy-py*qx-1;
    I2 = ((sq(qy)-2*px*qy+sq(qx)+2*py*qx+sq(py)+sq(px))/(px*qy-py*qx))/2.0E+0;
    Fa = F1(param,I1,I2);
    Fb = F2(param,I1,I2);
    
    SET(dvx, 0);
    SET(dvy, 0);
    SET(dux, 
    (Fa*(cy*qy+cx*qx)-((Fb*(qy*(qy*(cy*qy+cx*qx)+cy*(pow(qx,2)+pow(py,2))
                                                +px*(2*cx*py-cy*px))
                           +qx*(cx*(pow(qx,2)-pow(py,2))+px*(2*cy*py+cx*px))))
                      /pow(px*qy-py*qx,2))
                      /2.0E+0)
     /(bx*cy));
    SET(duy, 0);
    SET(dwx, 
    (((Fb*(qy*(qx*qy+2*px*py)+qx*(pow(qx,2)-pow(py,2)+pow(px,2))))
     /pow(px*qy-py*qx,2))
     /2.0E+0
     -Fa*qx)
     /cy);
    SET(dwy, 
    (((Fb*(px*pow(qy,2)-(2*py*qx*qy+px*(pow(qx,2)+pow(py,2)+pow(px,2)))))
     /pow(px*qy-py*qx,2))
     /2.0E+0
     +Fa*px)
     /cy);
    SET(pI1, I1);
    SET(pI2, I2);
    SET(pA, Q/2.0E+0);

    return HE_OK;
}
