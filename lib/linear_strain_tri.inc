int linear_strain_tri(void *param, real (*F1)(void*, real, real),
                      real (*F2)(void*, real, real), real ax, real ay,
                      real bx, real by, real cx, real cy, real ux,
                      real uy, real vx, real vy, real wx, real wy,
                      real *dux, real *duy, real *dvx, real *dvy,
                      real *dwx, real *dwy, real *pI1, real *pI2)
{
    real Q, abx, aby, cax, cay, cbx, cby, px, py, qx, qy, I1, I2;
    real Fa, Fb, d;

    Q = ax*(cy-by)-bx*cy+by*cx+ay*(bx-cx);
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
    I1 = sq(qy)+2*py*qx+sq(px)-2;
    I2 = (px*qy-py*qx-1)*(px*qy-py*qx+1);
    Fa = F1(param,I1,I2);
    Fb = F2(param,I1,I2);
    d = px*qy-py*qx;
    
    SET(dvx, (2*(Fb*d*(cby*qy+cbx*qx)-Fa*(cbx*qx-cby*px)))/Q);
    SET(dvy, -(2*(Fa*(cbx*qy-cby*py)+Fb*d*(cby*py+cbx*px)))/Q);
    SET(dux, (2*(Fa*(cax*qx-cay*px)-Fb*d*(cay*qy+cax*qx)))/Q);
    SET(duy, (2*(Fa*(cax*qy-cay*py)+Fb*d*(cay*py+cax*px)))/Q);
    SET(dwx, -(2*(Fb*d*(aby*qy+abx*qx)+Fa*(aby*px-abx*qx)))/Q);
    SET(dwy, (2*(Fb*d*(aby*py+abx*px)-Fa*(aby*py-abx*qy)))/Q);
    SET(pI1, I1);
    SET(pI2, I2);

    return HE_OK;
}
