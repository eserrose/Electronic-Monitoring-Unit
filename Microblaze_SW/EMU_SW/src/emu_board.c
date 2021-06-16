#include "emu_board.h"
#include <math.h>

ekf_t ekf;
double *x = ekf.x;
double P0 = 902; //hpa, initial air pressure

void init_filter(){
	//Initialize the filter
	ekf_init(&ekf, Nsta, Mobs);

	//Set the process noise covariance to a small constant
	ekf.Q[0][0] = 0.0001;
	ekf.Q[1][1] = 0.0001;

	//Set the observation noise covariance to a small constant
	ekf.R[0][0] = 0.0001;
	ekf.R[1][1] = 0.0001;
	ekf.R[2][2] = 0.0001;
}

void model(double fx[Nsta], double F[Nsta][Nsta], double hx[Mobs], double H[Mobs][Nsta])
{
	// Process model is f(x) = x
	fx[0] = x[0];
	fx[1] = x[1];

	// So process model Jacobian is identity matrix
	F[0][0] = 1;
	F[1][1] = 1;

	// Measurement function simplifies the relationship between state and sensor readings for convenience.
	hx[0] = x[0]; // Gyro angles from previous state
	hx[1] = x[1]; // Barometric altitude from previous state
	hx[2] = x[2]; // Laser altitude from previous state

	// Jacobian of measurement function
	H[0][0] = 1;        // Gyro angles from previous state
	H[1][1] = 1 ;       // Barometric altitude from previous state
	H[2][1] = 1 ;       // Laser altitude from previous state
}

uint8_t step(double *z)
{
	model(ekf.fx, ekf.F, ekf.hx, ekf.H);
	return ekf_step(&ekf, z);
}

void process_data(sensor_data_t data)
{
	double baro_alt = 44330*(1 - pow(data.BMP/P0, 1/5.255));
	double gnd_angle = sqrt(pow(data.MPU[0],2) + pow(data.MPU[1],2) + pow(data.MPU[2],2) );
	double z[3] = {gnd_angle, baro_alt, (double) data.DIST};
	step(z);
	transfer_new_data(&data);
}

void transfer_new_data(sensor_data_t* org_data)
{
	double new_data[3] = {ekf.x[0], ekf.x[1], ekf.x[2]};

	store_data(org_data, new_data);
}

void store_data(sensor_data_t* org_data, double* filter_data)
{

}
