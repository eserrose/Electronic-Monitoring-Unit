`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 06/23/2021 04:02:49 AM
// Design Name: 
// Module Name: comm_controller
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module comm_controller(
    input clk,          //clock
    input[7:0] msg,     //received message from main avionic
    output[7:0] s_data, //sensor data to send via uart
    input  rr,          //find if new data read from uart
    output reg tr,      //allow transmit when data is read from sensors
    output reg en       //enable pin for motor ignition
    );
    
    always @(posedge clk)
    begin
    
    if(rr) begin
        if(msg == 'hEE) begin
           en <= 1; //set enable pin when the received message is 0xEE
        end 
    end
    
    //if sensor data available
    /*
    if(1) begin
        tr <= 1;
    end*/
    
    end
    
endmodule
