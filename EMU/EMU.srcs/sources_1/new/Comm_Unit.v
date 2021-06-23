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
    input      rst,    //Active high synchronous reset
    input      rxd,    //Rx from main avionic (RS232)
    output     txd,    //UART tx, will be connected to RX of MicroBlaze
    //TODO: I2C Connection with sensors (bmp, mpu, laser)
    output     en_1    //Main avionic system enable line    
    );
    
 //UART (RS232) comm with main avionic, set EN_1 if received message equals the keyword
    
  wire  load;       //TX load signal
  wire  tx_inc;     //Bit counter increment signal
  wire  rx_inc;     //Bit counter increment signal
  wire  i_ge_8;     //i >= 8 flag
  wire  i_ge_7;     //i >= 7 flag
  wire  tr;         //Signals ready to transmit
  wire  rr;         //Signals ready to receive
  wire[7:0] s_data; //sensor data line
  wire[7:0] msg;    //message from main computer
  
comm_controller controller(
    .clk    (clk),
    .msg    (msg),
    .s_data (s_data),
    .tr     (tr),
    .rr     (rr),
    .en     (en_1)
);
  
uart_controller uart_ctrl (
    .clk     (clk),
    .rst     (rst),
    .tr      (tr),
    .rx      (rxd),
    .rr      (rr),
    .load    (load),
    .tx_inc  (tx_inc),
    .rx_inc  (rx_inc),
    .i_ge_8  (i_ge_8),
    .i_ge_7  (i_ge_7)
);

uart_datapath uart_dp (
    .clk     (clk),
    .rst     (rst),
    .rx      (rxd),
    .tx      (txd),
    .s_data  (s_data),
    .msg     (msg),
    .load    (load),
    .tx_inc  (tx_inc),
    .rx_inc  (rx_inc),
    .i_ge_8  (i_ge_8),
    .i_ge_7  (i_ge_7)
);
    
    //I2C comm with sensors, packet the data and send to microblaze
    
 endmodule