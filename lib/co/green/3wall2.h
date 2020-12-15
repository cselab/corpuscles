#define T Green3Wall2
typedef struct T T;
typedef struct He He;
typedef struct Ten Ten;
struct Tensor3x3;
int green3_wall2_ini(He *, real w, T **);
int green3_wall2_fin(T *);
int green3_wall2_apply(T *, He *, const real *, const real *, const real *,
                       struct Tensor3x3 *);
int green3_wall2_s(T *, const real[3], const real[3], Ten *);
int green3_wall2_s0(T *, const real[3], Ten *);
int green3_wall2_t(T *, const real[3], const real normal[3], const real[3],
                   Ten *);
int green3_wall2_stresslet(T *, He *, const real *, const real *,
                           const real *, struct Tensor3x3 *);
int green3_wall2_single_velocity(T * q, He * he, const real * x,
                                 const real * y, const real * z,
                                 const real * fx, const real * fy,
                                 const real * fz, const real r[3],
                                 /**/ real v[3]);
int green3_wall2_double_velocity(T * q, He * he, const real * x,
                                 const real * y, const real * z,
                                 const real * ux, const real * uy,
                                 const real * uz, const real r[3],
                                 /**/ real v[3]);
#undef T
