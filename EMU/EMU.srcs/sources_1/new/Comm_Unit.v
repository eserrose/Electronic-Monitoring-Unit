`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/30/2021 11:43:21 AM
// Module Name: Comm_Unit
// Description: Provide communication with main avionics, and sending sensor data to MB
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////

module Comm_Unit(
    input       clk,    //Clock signal
    input[15:0] bmp,    //Barometric pressure data
    input[15:0] acc,    //Acceleration data
    input[15:0] dst,    //Distance data (laser)
    //TODO: RS232 Connection with main avionics
    output reg tx,    //UART tx, will be connected to RX of MicroBlaze
    output reg en     //Main avionic system enable line    
    );
    
 endmodule;