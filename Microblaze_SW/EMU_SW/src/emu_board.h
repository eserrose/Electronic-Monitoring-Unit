#ifndef EMU_BOARD_H_
#define EMU_BOARD_H_

#include "stdint.h"
#include "kalman_filter.h"

#define PACKET_SIZE		 12 //Bytes, sensors + 2-bytes CRC
#define DATA_SAVE_SIZE	 16 //Bytes, filtered and original sensor data
#define MIN_ANGLE		 60	//Min allowable pitch angle
#define MAX_ANGLE		 90 //Max allowable pitch angle
#define MIN_ALTITUDE	300 //Min allowable altitude

#define Nsta	2 //state variables: altitude and angle
#define Mobs	3 //observations: mpu angles, barometric altitude, and laser altitude

typedef struct {

    int n;          /* number of state values */
    int m;          /* number of observables */

    float x[Nsta];    /* state vector */

    float P[Nsta][Nsta];  /* prediction error covariance */
    float Q[Nsta][Nsta];  /* process noise covariance */
    float R[Mobs][Mobs];  /* measurement error covariance */

    float G[Nsta][Mobs];  /* Kalman gain; a.k.a. K */

    float F[Nsta][Nsta];  /* Jacobian of process model */
    float H[Mobs][Nsta];  /* Jacobian of measurement model */

    float Ht[Nsta][Mobs]; /* transpose of measurement Jacobian */
    float Ft[Nsta][Nsta]; /* transpose of process Jacobian */
    float Pp[Nsta][Nsta]; /* P, post-prediction, pre-update */

    float fx[Nsta];   /* output of user defined f() state-transition function */
    float hx[Mobs];   /* output of user defined h() measurement function */

    /* temporary storage */
    float tmp0[Nsta][Nsta];
    float tmp1[Nsta][Mobs];
    float tmp2[Mobs][Nsta];
    float tmp3[Mobs][Mobs];
    float tmp4[Mobs][Mobs];
    float tmp5[Mobs];

} ekf_t;

typedef struct sensor_data_struct {
	uint16_t BMP;		//Barometric pressure
	uint16_t MPU[3];	//Gyroscope angles in 3-axes
	uint16_t DIST;		//Distance from the ground
}sensor_data_t;

/**
 * Initializes the kalman filter instance to be used
 */
void init_filter();

/**
	* The EKF model to be used.
	* @param fx gets output of state-transition function
	* @param F gets nxn Jacobian of f(x)
	* @param hx gets output of observation function h(x0 .. n-1)
	* @param H gets mxn Jacobian of h(x)
*/
void model(float fx[Nsta], float F[Nsta][Nsta], float hx[Mobs], float H[Mobs][Nsta]);

/**
	  Performs one step of the prediction and update.
	 * @param z observation vector, length m
	 * @return 0 on success, 1 on failure caused by non-positive-definite matrix.
 */
uint8_t step(float *z);

/**
 * Checks 16-bit crc to confirm received data
 */
uint8_t check_crc(uint8_t *data);

/**
 * Processes data received by UART and constructs a sensor_data_t
 */
void process_rx_data(uint8_t *data);

/**
 * @brief processes the oncoming sensor data
 *
 * @param data received data from sensors
 *
 * @return 0 if conditions are not met, 1 otherwise
 */
void process_data(sensor_data_t data);

/**
 * @brief checks if the sensor data satisfies the conditions and set enable pin if that's the case
 */
void check_conds(sensor_data_t* org_data);

/**
 * @brief stores the received sensor data inside the storage unit
 *
 * @param data received data from sensors
 */
void store_data(sensor_data_t* org_data, int16_t* filter_data);

/**
 * It takes a very long time for microblaze to do fpu operations so this is just to make it simpler
 */
void process_save(sensor_data_t data);
#endif /* EMU_BOARD_H */
