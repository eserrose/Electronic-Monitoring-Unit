#include "emu_board.h"
#include "platform_config.h"
#include <math.h>
#include <string.h>

ekf_t ekf;
float *x = ekf.x;
float P0 = 902; //hpa, initial air pressure

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

void model(float fx[Nsta], float F[Nsta][Nsta], float hx[Mobs], float H[Mobs][Nsta])
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

uint8_t step(float *z)
{
	model(ekf.fx, ekf.F, ekf.hx, ekf.H);
	return ekf_step(&ekf, z);
}

uint8_t check_crc(uint8_t *data){
	//implement crc-16
	return 1;
}

void process_rx_data(uint8_t *data)
{
	sensor_data_t sensor_data;

	if(check_crc(data) > 0){
		sensor_data.BMP    = (uint16_t) data[0] | (data[1] << 8);
		sensor_data.MPU[0] = (uint16_t) data[2] | (data[3] << 8);
		sensor_data.MPU[1] = (uint16_t) data[4] | (data[5] << 8);
		sensor_data.MPU[2] = (uint16_t) data[6] | (data[7] << 8);
		sensor_data.DIST   = (uint16_t) data[8] | (data[9] << 8);

		//process_data(sensor_data);
		process_save(sensor_data);
	}
}

void process_data(sensor_data_t data)
{

	float baro_alt = 44330*(1 - powf(data.BMP/P0, 1/5.255));
	float gnd_angle = sqrt(pow(data.MPU[0],2) + pow(data.MPU[1],2) + pow(data.MPU[2],2) );
	float z[3] = {gnd_angle, baro_alt, (float) data.DIST};
	step(z);
	check_conds(&data);
}

void check_conds(sensor_data_t* org_data)
{
	int16_t new_data[3] = {(int16_t) ekf.x[0], (int16_t) ekf.x[1], (int16_t) ekf.x[2]};

	if(ekf.x[0] > MIN_ANGLE && ekf.x[0] < MAX_ANGLE && ekf.x[1] > MIN_ALTITUDE){
		SET_ENABLE();
	}

	store_data(org_data, new_data);
}

void store_data(sensor_data_t* org_data, int16_t* filter_data)
{
	u8 data[DATA_SAVE_SIZE] = {0};
	memcpy(data,(uint8_t*) org_data, sizeof(sensor_data_t));
	memcpy(data+sizeof(sensor_data_t),(uint8_t*) filter_data, 3*2);

	XSpi_Transfer(&SpiInstance, data, NULL, DATA_SAVE_SIZE);
}

void process_save(sensor_data_t data)
{
	SET_ENABLE();
	u8 dt[10] = {0};
	memcpy(dt,(uint8_t*) &data, sizeof(sensor_data_t));
	XSpi_Transfer(&SpiInstance, dt, NULL, 10);
}
