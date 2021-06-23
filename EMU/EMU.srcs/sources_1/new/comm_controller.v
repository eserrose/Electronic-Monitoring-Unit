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
    input i2c,          //received data from sensors
    input[7:0]  msg,    //received message from main avionic
    output reg [7:0] s_data, //sensor data to send via uart
    input  rr,          //find if new data read from uart
    input  ta,          //last transmission completed
    output reg tr,      //allow transmit when data is read from sensors
    output reg en       //enable pin for motor ignition
    );
    
    parameter MAGIC_WORD = 'hEE;
    
    localparam 
        IDLE = 2'b00,
        TX_0 = 2'b01,
        TX_W = 2'b10;
        
    reg[2:0] state = IDLE;
    integer i;
    reg[7:0] sensor_data[0:9];
    
    initial begin
        sensor_data[0] <= 'h0x12;
        sensor_data[1] <= 'h0x15;
        sensor_data[2] <= 'h0xa5;
        sensor_data[3] <= 'h0xb5;
        sensor_data[4] <= 'h0xe5;
        sensor_data[5] <= 'h0xc5;
        sensor_data[6] <= 'h0x05;
        sensor_data[7] <= 'h0x06;
        sensor_data[8] <= 'h0x0a;
        sensor_data[9] <= 'h0x11;
        
        en <= 0;
    end
    
    always @(posedge clk)
    begin
    
    if(rr) begin
        if(msg == MAGIC_WORD)
           en <= 1; //set enable pin when the received message is 0xEE
    end
    
    case(state)
        IDLE: begin
            i = 0;
            if(i2c)
                state <= TX_0;
        end // IDLE state
        
        TX_0: begin
         s_data <= sensor_data[i];
         tr <= 1;               //set transmit ready
         state <= TX_W;
        end //TX_0 state
        
        TX_W: begin
            if(ta == 1) begin
                tr <= 0;    //clear transmit ready
                i = i +1;   //increment i
                state <= TX_0;
            end //if
            if(i == 10)
                state <= IDLE;
        end //TX_W state
    endcase
    
    end //always
    
endmodule
