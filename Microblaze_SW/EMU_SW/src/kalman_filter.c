#include "kalman_filter.h"
#include "linalg.h"

typedef struct {

	float * x;    /* state vector */

	float * P;  /* prediction error covariance */
	float * Q;  /* process noise covariance */
	float * R;  /* measurement error covariance */

	float * G;  /* Kalman gain; a.k.a. K */

	float * F;  /* Jacobian of process model */
	float * H;  /* Jacobian of measurement model */

	float * Ht; /* transpose of measurement Jacobian */
	float * Ft; /* transpose of process Jacobian */
	float * Pp; /* P, post-prediction, pre-update */

	float * fx;  /* output of user defined f() state-transition function */
	float * hx;  /* output of user defined h() measurement function */

    /* temporary storage */
	float * tmp0;
	float * tmp1;
	float * tmp2;
	float * tmp3;
	float * tmp4;
	float * tmp5;

} ekf_t;

static void unpack(void * v, ekf_t * ekf, int n, int m)
{
    /* skip over n, m in data structure */
    char * cptr = (char *)v;
    cptr += 2*sizeof(int);

    float * dptr = (float *)cptr;
    ekf->x = dptr;
    dptr += n;
    ekf->P = dptr;
    dptr += n*n;
    ekf->Q = dptr;
    dptr += n*n;
    ekf->R = dptr;
    dptr += m*m;
    ekf->G = dptr;
    dptr += n*m;
    ekf->F = dptr;
    dptr += n*n;
    ekf->H = dptr;
    dptr += m*n;
    ekf->Ht = dptr;
    dptr += n*m;
    ekf->Ft = dptr;
    dptr += n*n;
    ekf->Pp = dptr;
    dptr += n*n;
    ekf->fx = dptr;
    dptr += n;
    ekf->hx = dptr;
    dptr += m;
    ekf->tmp0 = dptr;
    dptr += n*n;
    ekf->tmp1 = dptr;
    dptr += n*m;
    ekf->tmp2 = dptr;
    dptr += m*n;
    ekf->tmp3 = dptr;
    dptr += m*m;
    ekf->tmp4 = dptr;
    dptr += m*m;
    ekf->tmp5 = dptr;
  }

void ekf_init(void * v, int n, int m)
{
    /* retrieve n, m and set them in incoming data structure */
    int * ptr = (int *)v;
    *ptr = n;
    ptr++;
    *ptr = m;

    /* unpack rest of incoming structure for initialization */
    ekf_t ekf;
    unpack(v, &ekf, n, m);

    /* zero-out matrices */
    zeros(ekf.P, n, n);
    zeros(ekf.Q, n, n);
    zeros(ekf.R, m, m);
    zeros(ekf.G, n, m);
    zeros(ekf.F, n, n);
    zeros(ekf.H, m, n);
}

uint8_t ekf_step(void * v, float * z)
{
    /* unpack incoming structure */
    int * ptr = (int *)v;
    int n = *ptr;
    ptr++;
    int m = *ptr;

    ekf_t ekf;
    unpack(v, &ekf, n, m);

    /* P_k = F_{k-1} P_{k-1} F^T_{k-1} + Q_{k-1} */
    mulmat(ekf.F, ekf.P, ekf.tmp0, n, n, n);
    transpose(ekf.F, ekf.Ft, n, n);
    mulmat(ekf.tmp0, ekf.Ft, ekf.Pp, n, n, n);
    accum(ekf.Pp, ekf.Q, n, n);

    /* G_k = P_k H^T_k (H_k P_k H^T_k + R)^{-1} */
    transpose(ekf.H, ekf.Ht, m, n);
    mulmat(ekf.Pp, ekf.Ht, ekf.tmp1, n, n, m);
    mulmat(ekf.H, ekf.Pp, ekf.tmp2, m, n, n);
    mulmat(ekf.tmp2, ekf.Ht, ekf.tmp3, m, n, m);
    accum(ekf.tmp3, ekf.R, m, m);
    if (cholsl(ekf.tmp3, ekf.tmp4, ekf.tmp5, m)) return 1;
    mulmat(ekf.tmp1, ekf.tmp4, ekf.G, n, m, m);

    /* \hat{x}_k = \hat{x_k} + G_k(z_k - h(\hat{x}_k)) */
    sub(z, ekf.hx, ekf.tmp5, m);
    mulvec(ekf.G, ekf.tmp5, ekf.tmp2, n, m);
    add(ekf.fx, ekf.tmp2, ekf.x, n);

    /* P_k = (I - G_k H_k) P_k */
    mulmat(ekf.G, ekf.H, ekf.tmp0, n, m, n);
    negate(ekf.tmp0, n, n);
    mat_addeye(ekf.tmp0, n);
    mulmat(ekf.tmp0, ekf.Pp, ekf.P, n, n, n);

    return 0;
}
