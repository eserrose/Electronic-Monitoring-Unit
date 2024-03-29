#include "linalg.h"
#include <math.h>

void zeros(float * a, int m, int n)
{
    for (int j=0; j<m*n; ++j)
        a[j] = 0;
}

void mulmat(float * a, float * b, float * c, int arows, int acols, int bcols)
{
    for(int i=0; i<arows; ++i)
        for(int j=0; j<bcols; ++j) {
            c[i*bcols+j] = 0;
            for(int l=0; l<acols; ++l)
                c[i*bcols+j] += a[i*acols+l] * b[l*bcols+j];
        }
}

void mulvec(float * a, float * x, float * y, int m, int n)
{
    for(int i=0; i<m; ++i) {
        y[i] = 0;
        for(int j=0; j<n; ++j)
            y[i] += x[j] * a[i*n+j];
    }
}

void transpose(float * a, float * at, int m, int n)
{
    for(int i=0; i<m; ++i)
        for(int j=0; j<n; ++j) {
            at[j*m+i] = a[i*n+j];
        }
}

void accum(float * a, float * b, int m, int n)
{
    for(int i=0; i<m; ++i)
        for(int j=0; j<n; ++j)
            a[i*n+j] += b[i*n+j];
}

void add(float * a, float * b, float * c, int n)
{
    for(int j=0; j<n; ++j)
        c[j] = a[j] + b[j];
}

void sub(float * a, float * b, float * c, int n)
{
    for(int j=0; j<n; ++j)
        c[j] = a[j] - b[j];
}

void negate(float * a, int m, int n)
{
    for(int i=0; i<m; ++i)
        for(int j=0; j<n; ++j)
            a[i*n+j] = -a[i*n+j];
}

void mat_addeye(float * a, int n)
{
    for (int i=0; i<n; ++i)
        a[i*n+i] += 1;
}

int choldc1(float * a, float * p, int n) {
    float sum;

    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            sum = a[i*n+j];
            for (int k = i - 1; k >= 0; k--) {
                sum -= a[i*n+k] * a[j*n+k];
            }
            if (i == j) {
                if (sum <= 0) {
                    return 1; /* error */
                }
                p[i] = sqrt(sum);
            }
            else {
                a[j*n+i] = sum / p[i];
            }
        }
    }

    return 0; /* success */
}

int choldcsl(float * A, float * a, float * p, int n)
{
    int i,j,k; float sum;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            a[i*n+j] = A[i*n+j];
    if (choldc1(a, p, n)) return 1;
    for (i = 0; i < n; i++) {
        a[i*n+i] = 1 / p[i];
        for (j = i + 1; j < n; j++) {
            sum = 0;
            for (k = i; k < j; k++) {
                sum -= a[j*n+k] * a[k*n+i];
            }
            a[j*n+i] = sum / p[j];
        }
    }

    return 0; /* success */
}

int cholsl(float * A, float * a, float * p, int n)
{
    int i,j,k;
    if (choldcsl(A,a,p,n)) return 1;
    for (i = 0; i < n; i++) {
        for (j = i + 1; j < n; j++) {
            a[i*n+j] = 0.0;
        }
    }
    for (i = 0; i < n; i++) {
        a[i*n+i] *= a[i*n+i];
        for (k = i + 1; k < n; k++) {
            a[i*n+i] += a[k*n+i] * a[k*n+i];
        }
        for (j = i + 1; j < n; j++) {
            for (k = j; k < n; k++) {
                a[i*n+j] += a[k*n+i] * a[k*n+j];
            }
        }
    }
    for (i = 0; i < n; i++) {
        for (j = 0; j < i; j++) {
            a[i*n+j] = a[j*n+i];
        }
    }

    return 0; /* success */
}

