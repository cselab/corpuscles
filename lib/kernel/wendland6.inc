#define wendland6_1d (55/32.0)
#define wendland6_2d (78/(7*pi))
#define wendland6_3d (1365/(64*pi))

/* 1D */
static double wendland6_w1(double c, double x) {
    double q, a;
    q = fabs(x)/c;
    if (q < 1) {
        a = pow(1-q,7)*(21*pow(q,3)+19*pow(q,2)+7*q+1);
        return a*wendland6_1d/c;
    }
    else return 0;
}
static double wendland6_dw1(double c, double x) {
    double q, a;
    q = fabs(x)/c;    
    if (q < 1) {
        a = -6*pow(q-1,6)*q*(35*pow(q,2)+18*q+3);
        return a*wendland6_1d/(c*c);
    }
    else return 0;
}

/* 2D and 3D */
static double wendland6_w0(double q) { /* 2D and 3D */
    if (q < 1) return pow(1-q,8)*(32*pow(q,3)+25*pow(q,2)+8*q+1);
    else return 0;
}
static double wendland6_dw0(double q) {
    if (q < 1) return 22*pow(q-1,7)*q*(16*pow(q,2)+7*q+1);
    else return 0;
}
static double wendland6_w2(double c, double x) {
    double q;
    q = fabs(x)/c;
    return wendland6_2d * wendland6_w0(q) / (c*c);
}
static double wendland6_w3(double c, double x) {
    double q;
    q = fabs(x)/c;
    return wendland6_3d * wendland6_w0(q) / (c*c*c);
}
static double wendland6_dw2(double c, double x) {
    double q;
    q = fabs(x)/c;
    return wendland6_2d * wendland6_dw0(q) / (c*c*c);
}
static double wendland6_dw3(double c, double x) {
    double q;
    q = fabs(x)/c;
    return wendland6_3d * wendland6_dw0(q) / (c*c*c*c);
}
