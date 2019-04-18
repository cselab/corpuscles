#define gauss_1d 1.0/3
#define gauss_2d 10.0/(7*pi)
#define gauss_3d 2.0/pi

/* 2D and 3D */
static real gauss_w0(real q) { /* 2D and 3D */
	if (q < 1) return pow(2-q,3)-4*pow(1-q,3);
	else if (q < 2) return pow(2-q,3);
	else return 0;
}
static real gauss_dw0(real q) {
	if (q < 1) return 12*pow(1-q,2)-3*pow(2-q,2);
	else if (q < 2) return -3*pow(2-q,2);
	else return 0;
}
static real gauss_dwr0(real q) {
	if (q < 1) return 3*(3*q-4);
	else if (q < 2) return -3*pow(2-q,2)/q;
	else return 0;
}

static real gauss_w1(real c, real x) {
	real q;
	q = 2*fabs(x)/c;
	return gauss_1d*gauss_w0(q)/c;
}
static real gauss_w2(real c, real x) {
	real q;
	q = 2*fabs(x)/c;
	return gauss_2d*gauss_w0(q)/(c*c);
}
static real gauss_w3(real c, real x) {
	real q;
	q = 2*fabs(x)/c;
	return gauss_3d*gauss_w0(q)/(c*c*c);
}

static real gauss_dw1(real c, real x) {
	real q;
	q = 2*fabs(x)/c;
	return 2*gauss_2d*gauss_dw0(q)/(c*c);
}
static real gauss_dw2(real c, real x) {
	real q;
	q = 2*fabs(x)/c;
	return 2*gauss_2d*gauss_dw0(q)/(c*c*c);
}
static real gauss_dw3(real c, real x) {
	real q;
	q = 2*fabs(x)/c;
	return 2*gauss_3d*gauss_dw0(q)/(c*c*c*c);
}

static real gauss_dwr1(real c, real x) {
	real q;
	q = 2*fabs(x)/c;
	return 4*gauss_2d*gauss_dwr0(q)/(c*c*c);
}
static real gauss_dwr2(real c, real x) {
	real q;
	q = 2*fabs(x)/c;
	return 4*gauss_2d*gauss_dwr0(q)/(c*c*c*c);
}
static real gauss_dwr3(real c, real x) {
	real q;
	q = 2*fabs(x)/c;
	return 4*gauss_3d*gauss_dwr0(q)/(c*c*c*c*c);
}