#ifndef LINALG_H_
#define LINALG_H_

/**
 *@brief Create a matrix of size mxn filled with zeros
 *@param a array pointer
 *@param m row size
 *@param n col size
 */
void zeros(double * a, int m, int n);

/**
 * @brief Matrix multiplication C <- A * B
 * @param a left-hand side matrix
 * @param b right-hand side matrix
 * @param c multiplication result
 * @param arows number of rows of a
 * @param acols number of cols of a
 * @param bcols number of cols of b
 */
void mulmat(double * a, double * b, double * c, int arows, int acols, int bcols);

/**
 * @brief Vector multiplication y <- a * x
 * @param a left-hand side vector
 * @param x righ-hand side vector
 * @param y resultant vector
 * @param m size of a
 * @param n size of x
 */
void mulvec(double * a, double * x, double * y, int m, int n);

/**
 * @brief Transposes a matrix At <- A^T
 * @param a matrix to be transposed
 * @param at matrix to store the transpose of a
 * @param m number of rows of a
 * @param n number of cols of a
 */
void transpose(double * a, double * at, int m, int n);

/**
 * @brief Accumulates two vectors of into the first one A <- A + B
 * @param a left-hand side vector
 * @param b right-hand side vector
 * @param m number of elements of a
 * @param n number of elements of b
 */
void accum(double * a, double * b, int m, int n);

/**
 * @brief Adds two vectors of same size into a new one C <- A + B
 * @param a left-hand side vector
 * @param b right-hand side vector
 * @param c resultant vector
 * @param n number of elements
 */
void add(double * a, double * b, double * c, int n);

/**
 * @brief Subtracts two vectors of same size into a new one C <- A - B
 * @param a left-hand side vector
 * @param b right-hand side vector
 * @param c resultant vector
 * @param n number of elements
 */
void sub(double * a, double * b, double * c, int n);

/**
 * @brief negates a matrix A <- -A
 * @param a matrix to be negated
 * @param m number of rows
 * @param n number of cols
 */
void negate(double * a, int m, int n);

/**
 * @brief adds the identity matrix (eye) to another one
 * @param a square matrix
 * @param n size of matrix
 */
void mat_addeye(double * a, int n);

/*
 * @brief Cholesky-decomposition matrix-inversion code, adapated from
 * http://jean-pierre.moreau.pagesperso-orange.fr/Cplus/choles_cpp.txt
 *
 * @return 0 if success, 1 otherwise
 */
int choldc1(double * a, double * p, int n);

int choldcsl(double * A, double * a, double * p, int n);

int cholsl(double * A, double * a, double * p, int n);

#endif /* LINALG_H_ */
