`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/30/2021 11:43:21 AM
// Module Name: Comm_Unit
// Description: Provides communication with main avionics and sensors, and sends sensor data to MB
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////

module Comm_Unit(
    input      clk,    //Clock signal
    input      rxd,    //Rx from main avionic (RS232)
    //TODO: I2C Connection with sensors (bmp, mpu, laser)
    output reg txd,    //UART tx, will be connected to RX of MicroBlaze
    output reg en_1    //Main avionic system enable line    
    );
    
    //UART (RS232) comm with main avionic, set EN_1 if received message equals the keyword
    
    //I2C comm with sensors, packet the data and send to microblaze
    
 endmodule