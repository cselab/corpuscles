int strain_2d(void *param, real (*F1)(void*, real, real), real (*F2)(void*, real, real),
              real ax, real ay, real bx, real by, real cx, real cy,
              real vx, real vy, real ux, real uy, real wx, real wy,
              real *dvx, real *dvy, real *dux, real *duy, real *dwx,
              real *dwy, real *pI1, real *pI2, real *pA)
{
    real Q, abx, aby, cax, cay, cbx, cby, px, py, qx, qy, I1, I2;
    real Fa, Fb, d;

    Q = ax*(cy-by)-bx*cy+by*cx+ay*(bx-cx);
    NOT_ZERO(Q);
    abx = ax-bx;
    aby = ay-by;
    cax = cx-ax;
    cay = cy-ay;
    cbx = cx-bx;
    cby = cy-by;
    px = -(aby*wx-cby*vx+cay*ux)/Q;
    py = (abx*wx-cbx*vx+cax*ux)/Q;
    qx = -(aby*wy-cby*vy+cay*uy)/Q;
    qy = (abx*wy-cbx*vy+cax*uy)/Q;
    I1 = (sq(qy)+2*qy+sq(qx)+sq(py)+sq(px)+2*px)/2.0E+0;
    I2 = 
    (pow(px,2)*pow(qy,2)+2*px*pow(qy,2)-2*px*py*qx*qy-2*py*qx*qy+2*pow(px,2)*qy
                        +4*px*qy+pow(py,2)*pow(qx,2)-pow(qx,2)-2*px*py*qx-2*py*qx
                        -pow(py,2))
     /4.0E+0;
    Fa = F1(param,I1,I2);
    Fb = F2(param,I1,I2);
    d = (px+1)*qy-py*qx+px+1;
    
    SET(dvx, 
    ((Fb*(qy*((cby*px+cy-by)*qy-(cby*py-(cbx*px+cx)+bx)*qx+2*(cby*px+cy-by))
         +qx*(py*((bx-cx)*qx-cy+by)+cbx*px+cx-bx)+cbx*py))
     /2.0E+0
     -Fa*(cbx*py-(cby*px+cy)+by))
     /Q);
    SET(dvy, 
    ((-(Fb*((px*(cy*py+cbx*(px+2))-(by*px-cy+by)*py)*qy
           -(py*(cby*py-(bx-cx)*px+cx-bx)-cy+by)*qx+(cby*px+cy-by)*py
           +cbx*px*(px+2)))
     /2.0E+0)
     -Fa*(cbx*qy-cby*qx+cx-bx))
     /Q);
    SET(dux, 
    (Fa*(cax*py-(cay*px+cy)+ay)-(Fb*(qy*((cay*px+cy-ay)*qy
                                        -(cay*py-(cax*px+cx)+ax)*qx
                                        +2*(cay*px+cy-ay))
                                    -qx*(py*(cax*qx+cy-ay)-(cax*px+cx)+ax)
                                    +cax*py))
                                /2.0E+0)
     /Q);
    SET(duy, 
    ((Fb*((px*(cy*py+cax*(px+2))-(ay*px-cy+ay)*py)*qy
         -(py*(cay*py+cax*px+cx-ax)-cy+ay)*qx+(cay*px+cy-ay)*py+cax*px*(px+2)))
     /2.0E+0
     +Fa*(cax*qy-cay*qx+cx-ax))
     /Q);
    SET(dwx, 
    (Fa*(abx*py-aby*px+by-ay)-(Fb*(qy*((aby*px-by+ay)*qy-(aby*py-abx*px+bx-ax)*qx
                                                        +2*(aby*px-by+ay))
                                  -qx*(py*((ax-bx)*qx-by+ay)-abx*px+bx-ax)
                                  +abx*py))
                              /2.0E+0)
     /Q);
    SET(dwy, 
    (Fa*(abx*qy-(aby*qx+bx)+ax)-(Fb*((px*(by*py-abx*(px+2))-(ay*px-by+ay)*py)*qy
                                    +(py*(aby*py+(ax-bx)*px-bx+ax)+by-ay)*qx
                                    -((aby*px-by+ay)*py+abx*px*(px+2))))
                                /2.0E+0)
     /Q);
    SET(pI1, I1);
    SET(pI2, I2);
    SET(pA, Q/2.0E+0);

    return HE_OK;
}
