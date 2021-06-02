`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 06/02/2021 09:49:24 AM
// Module Name: Sensor_Controller
// Description: Receives data from sensors and transmits them to COMM block
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////

module Sensor_Controller(
    input clk,  //Clock signal
    //TODO: I2C / SPI connections between Pressure, Accel, and Laser sensors
    output reg[15:0] bmp,    //Barometric pressure data
    output reg[15:0] acc,    //Acceleration data
    output reg[15:0] dst     //Distance data (laser)
    );
endmodule
