//////////////////////////////////////////////////////////////////////////////////
// Author: E. Eser Gul
// No: 514191997
//
// Create Date: 04/23/2021 05:58:59 AM
// Design Name: Controller
// Module Name: uart_controller
// Project Name: HW1
//////////////////////////////////////////////////////////////////////////////////
`timescale 1ns / 1ps

module uart_controller(
   input   clk,             //Clock
   input   rst,             //Active high synchronous reset
   input   tr,              //Active high transmit request input (indicates new data available to send
   input   rx,              //Serial RX pin
   input   i_ge_8,          //If bit counter is greater than 7
   input   i_ge_7,          //If bit counter is greater than 6
   output reg  rr,          //Active high receive-ready pin (indicates new data received)
   output reg  ta,          //Transmit end acknowledge
   output reg  load,        //Load data for TX
   output reg  tx_inc,      //Increment bit counter
   output reg  rx_inc       //Increment bit counter
    );
    
 
//***************************************************************************
// Parameter definitions
//***************************************************************************
parameter BAUD_RATE    = 115_200;            //Baud rate is 115200
parameter CLOCK_RATE   = 12_000_000;         //Clock signal is 12 MHz
// State encoding for main FSM
 localparam 
    IDLE     = 3'b000,
    TX_START = 3'b001,
    TX_DATA  = 3'b010,
    TX_STOP  = 3'b011,
    RX_START = 3'b100,
    RX_DATA  = 3'b101,
    RX_STOP  = 3'b110;

//internal variables
 reg[2:0]    TX_state;   //tx state machine
 reg[2:0]    RX_state;   //rx state machine
//***************************************************************************
// Code
//***************************************************************************

 // RX state machine
 always @(posedge clk)
 begin
     if(rst)
        RX_state  <= IDLE;
    else begin
    case (RX_state)
      IDLE: begin
        rx_inc <= 0;
        rr     <= 0;
        //On detection of rx being low, transition to RX_START
        if(!rx)
        begin
            RX_state <= RX_START;
        end
      end //IDLE state
      
      RX_START: begin
        rx_inc <= 1;
        RX_state <= RX_DATA;
      end //RX_START state
      
      RX_DATA: begin
        if(i_ge_7)begin
            rx_inc <= 0;
            RX_state  <= RX_STOP;
        end
      end //RX_DATA state
      
      RX_STOP: begin
        rr <= 1;
        RX_state <= IDLE;
      end //RX_STOP state
    endcase   
    end     //if rst
 end       //always   
 
 integer counter;
 // TX state machine
 always @(posedge clk)
 begin
    if(rst)
        TX_state  <= IDLE;
    else begin
    case (TX_state)
      IDLE: begin
        load    <= 0; //Set initial values
        tx_inc  <= 0;
        ta      <= 0;
        counter = 0;
        //On detection of tr being high, transition to TX_START
        if(tr)
            TX_state <= TX_START;
      end //IDLE state
     
      TX_START: begin
        load  <= 1;
        TX_state <= TX_DATA;
      end //TX_START state
      
      TX_DATA: begin //Clear load and set bit_inc
        load   <= 0;
        tx_inc <= 1;
        if(i_ge_8)begin
            tx_inc <= 0;
            TX_state <= TX_STOP;
        end
      end //TX_DATA state
      
      TX_STOP: begin
        //wait a bit so that the receiver can understand the next start bit
        counter = counter + 1;
        if(counter == CLOCK_RATE/BAUD_RATE) begin
            ta <= 1;    //transmit acknowledge
            TX_state <= IDLE;
        end
      end //TX_END state
    endcase   
    end    //if reset
 end       //always   
  
endmodule
