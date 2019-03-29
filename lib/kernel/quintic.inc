#define quintic_2d (63/(478*pi))
#define quintic_3d (9/(40*pi))

static double quintic_w0(double q) {
    if (q < 1)
        return pow(3 - q, 5) - 6*pow(2 - q, 5) + 15*pow(1 - q, 5);
    else if (q < 2)
        return pow(3 - q, 5) - 6*pow(2 - q, 5);
    else if (q < 3)
        return pow(3 - q, 5);
    else
        return 0;
}
static double quintic_dw0(double q) {
    if (q < 1)
        return -5*(pow(3 - q, 4) - 6*pow(2 - q, 4) + 15*pow(1 - q, 4));
    else if (q < 2)
        return -5*(pow(3 - q, 4) - 6*pow(2 - q, 4));
    else if (q < 3)
        return -5*pow(3 - q, 4);
    else
        return 0;
}
static double quintic_w2(double c, double x) {
    double q;
    q = 3*fabs(x)/c;
    return quintic_2d * quintic_w0(q) / (c*c);
}
static double quintic_w3(double c, double x) {
    double q;
    q = 3*fabs(x)/c;
    return quintic_3d * quintic_w0(q) / (c*c*c);
}

static double quintic_dw2(double c, double x) {
    double q;
    q = 3*fabs(x)/c;
    return 3 * quintic_2d * quintic_dw0(q) / (c*c*c);
}
static double quintic_dw3(double c, double x) {
    double q;
    q = 3*fabs(x)/c;
    return 3 * quintic_3d * quintic_dw0(q) / (c*c*c*c);
}
