int constant_strain_2d(void *param, real (*F1)(void*, real, real),
                       real (*F2)(void*, real, real), real ax,
                       real ay, real bx, real by, real cx, real cy,
                       real vx, real vy, real ux, real uy, real wx,
                       real wy, real *dvx, real *dvy, real *dux,
                       real *duy, real *dwx, real *dwy, real *pI1,
                       real *pI2, real *pA)
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
    I1 = sq(qy)+2*qy+2*py*qx+sq(px)+2*px;
    I2 = (px*qy+qy-py*qx+px)*(px*qy+qy-py*qx+px+2);
    Fa = F1(param,I1,I2);
    Fb = F2(param,I1,I2);
    d = px*qy-py*qx;
    
    SET(dvx, 
    (2*(Fb*(cby*qy+cbx*qx+cy-by)*((px+1)*qy-py*qx+px+1)
       -Fa*(cbx*qx-(cby*px+cy)+by)))
     /Q);
    SET(dvy, 
    -(2*(Fb*(cby*py+cbx*px+cx-bx)*((px+1)*qy-py*qx+px+1)
        +Fa*(cbx*qy-cby*py+cx-bx)))
     /Q);
    SET(dux, 
    (2*(Fa*(cax*qx-(cay*px+cy)+ay)-Fb*(cay*qy+cax*qx+cy-ay)
                                     *((px+1)*qy-py*qx+px+1)))
     /Q);
    SET(duy, 
    (2*(Fb*(cay*py+cax*px+cx-ax)*((px+1)*qy-py*qx+px+1)+Fa*(cax*qy-cay*py+cx-ax)))
     /Q);
    SET(dwx, 
    (2*(Fa*(abx*qx-aby*px+by-ay)-Fb*(aby*qy+abx*qx-by+ay)*((px+1)*qy-py*qx+px+1)))
     /Q);
    SET(dwy, 
    (2*(Fb*(aby*py+abx*px-bx+ax)*((px+1)*qy-py*qx+px+1)
       +Fa*(abx*qy-(aby*py+bx)+ax)))
     /Q);
    SET(pI1, I1);
    SET(pI2, I2);
    SET(pA, REAL_ABS(Q)/2.0E+0);

    return HE_OK;
}
