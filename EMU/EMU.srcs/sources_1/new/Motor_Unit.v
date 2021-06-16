`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 06/02/2021 08:53:32 AM
// Module Name: Motor_Unit
// Description: Sends ignition signal to motor if conditions are met
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
//          1.00 - Motor Block Ready
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////

module Motor_Unit(
    input       clk,    //Clock signal
    input       rst,    //Reset signal
    input       en_0,   //Enable from open-circuit (Active-Low)
    input       en_1,   //Enable from main avionic system
    input       en_2,   //Enable from sensor calculation block
    output reg  ign     //Motor ignition signal    
    );

localparam WAIT_TIME = 5*12*1e3;    //5 ms
    
// State encoding for main FSM
 localparam 
    IDLE     = 2'b00,
    WAIT_EN  = 2'b01,
    IGNITE   = 2'b10;  
    
 reg[1:0]   state = IDLE;
 integer   counter = 0;

 // Ignition state machine
 always @(posedge clk)
 begin
     if(rst)
        state  <= IDLE;
    else begin
    case (state)
      IDLE: begin
        ign <= 0;
        if(!en_0)
        begin
            state <= WAIT_EN;
        end
      end //IDLE state
      
      WAIT_EN: begin
        if(en_1 && en_2)
            counter <= counter + 1;
        if(counter >= integer(WAIT_TIME))
            state <= IGNITE;
      end //WAIT_EN state
      
      IGNITE: begin
        ign <= 1;
      end //IGNITE state
      
    endcase   
    end     //if rst
 end       //always   
 
endmodule
