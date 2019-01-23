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
    I1 = qy+px-1;
    I2 = ((sq(qy)-2*px*qy+sq(qx)+2*py*qx+sq(py)+sq(px))/(px*qy-py*qx))/2.0E+0;
    Fa = F1(param,I1,I2);
    Fb = F2(param,I1,I2);
    
    SET(dvx, 
    (((Fb*(qy*(cy*(1-px)*pow(qy,2)+px*(qx*(cy*qx-2*cx*qy)+px*(cy*(px-1)-2*cx*py)
                                                         +2*(bx*px+cx-bx)*py)
                                  +(cy*py+2*bx*px+cx-bx)*qx*qy
                                  +cy*(pow(qx,2)+(1-px)*pow(py,2))
                                  +2*(cx-bx)*py*pow(qx,2))
          -qx*((cy*py-cx+bx)*pow(qx,2)-(px*(py*(2*cx*py+cy*(2-px))+(cx-bx)*px)
                                       +pow(py,2)*(cy*py-(2*bx*px+cx-bx))))))
     /pow(px*qy-py*qx,2))
     /2.0E+0
     -Fa*((cx-bx)*qx+cy*(1-px)))
     /(bx*cy));
    SET(dvy, 
    (((Fb*(qy*(px*(bx*pow(qy,2)-(bx*qy+qx*(bx*qx+2*cy))+cx*qy
                               +bx*(pow(py,2)-pow(px,2)))
              +py*(2*(bx-cx)*qx-(bx*qx+cy)*qy))
          +pow(qx,2)*(py*(bx*qx+cy)+(bx-cx)*px)
          +py*(bx*(pow(px,2)-pow(py,2))*qx-cy*pow(py,2))
          +px*((bx-cx)*pow(py,2)+px*((bx-cx)*px-cy*py))))
     /pow(px*qy-py*qx,2))
     /2.0E+0
     +Fa*(bx*qy+cx-bx))
     /(bx*cy));
    SET(dux, 
    (((Fb*(qx*(cx*(pow(py,2)-pow(qx,2))-px*(2*cy*py+cx*px))
          -qy*(qy*(cy*qy+cx*qx)+cy*(pow(qx,2)+pow(py,2))-px*(cy*px-2*cx*py))))
     /(cy*pow(px*qy-py*qx,2)))
     /2.0E+0
     +Fa)
     /bx);
    SET(duy, 
    ((-((Fb*((cx*px-cy*py)*pow(qy,2)+qx*(cy*(py*qx-2*px*qy)-2*cx*py*qy)
                                    -(cx*px*pow(qx,2)+cy*pow(py,3)
                                                     +px
                                                      *(cx*pow(py,2)
                                                       +px*(cy*py+cx*px)))))
     /pow(px*qy-py*qx,2))
     /2.0E+0)
     -cx*Fa)
     /(bx*cy));
    SET(dwx, 
    ((Fb*(qy*(qx*qy+2*px*py)+qx*(pow(qx,2)-pow(py,2)+pow(px,2))))
     /(cy*pow(px*qy-py*qx,2)))
     /2.0E+0);
    SET(dwy, 
    (((Fb*(px*pow(qy,2)-(2*py*qx*qy+px*(pow(qx,2)+pow(py,2)+pow(px,2)))))
     /pow(px*qy-py*qx,2))
     /2.0E+0
     +Fa)
     /cy);
    SET(pI1, I1);
    SET(pI2, I2);
    SET(pA, Q/2.0E+0);

    return HE_OK;
}
