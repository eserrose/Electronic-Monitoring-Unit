//////////////////////////////////////////////////////////////////////////////////
// Author: E. Eser Gul
// No: 514191997
//
// Create Date: 04/23/2021 05:58:01 AM
// Design Name: Datapath
// Module Name: uart_datapath
// Project Name: HW1
//////////////////////////////////////////////////////////////////////////////////
`timescale 1ns / 1ps
module uart_datapath(
    input   clk,            //Clock
    input   rst,            //Reset
    
    input   rx,             //Serial RX pin
    output  reg tx,         //Serial TX pin
    
    input   [7:0] s_data,   //8-bit data to send via TX
    output  reg [7:0] msg,  //8-bit data received from rx
    
    input   load,           //Load enable for TX
    input   tx_inc,         //Increment bit_ctr_tx
    input   rx_inc,         //Increment bit_ctr_rx
    output  reg i_ge_8,     //Indicate if i >= 8
    output  reg i_ge_7      //Indicate if i >= 7
    );

parameter BAUD_RATE    = 115_200;            //Baud rate is 115200
parameter CLOCK_RATE   = 12_000_000;         //Clock signal is 12 MHz
    
//internal variables
reg[3:0]    bit_ctr_tx; //Counter for counting bits - tx
reg[3:0]    bit_ctr_rx; //Counter for counting bits - rx
reg[9:0]    bd_ctr_tx;  //Counter for baudrate (12 Mhz / 115200 = 104) tx
reg[9:0]    bd_ctr_rx;  //Counter for baudrate (12 Mhz / 115200 = 104) rx  
reg[9:0]    sr_reg;     //10 bits to be serially transmitted
reg         clr_tx;     //Clear current state
reg         clr_rx;     //Clear current state

//RX Block
always @(posedge clk)
begin
    if(rst | clr_rx) begin
        clr_rx     <= 0;
        bit_ctr_rx <= 0;
        bd_ctr_rx  <= 3; //3 clock cycles will have passed during the detection of receive start
        i_ge_7     <= 0;
    end
    else begin
        if(rx_inc) begin
            bd_ctr_rx <= bd_ctr_rx + 1;                    //Increment baud counter
            if(bd_ctr_rx ==  CLOCK_RATE/ BAUD_RATE) begin //start sampling after the initial LOW
                msg[bit_ctr_rx] <= rx;
                if(bit_ctr_rx > 6) begin 
                    i_ge_7 <= 1;
                    clr_rx <= 1; //return to initial state
                end
                bd_ctr_rx  <= 0;
                bit_ctr_rx <= bit_ctr_rx + 1;
            end
        end
    end
end //always

always @(posedge clk)
begin
    if(rst)
        msg <= 0;
end //always

//TX Block
always @(posedge clk)
begin
    if(rst | clr_tx) begin
        tx          <= 1;
        clr_tx      <= 0;
        bit_ctr_tx  <= 0;
        bd_ctr_tx   <= 0;
        i_ge_8      <= 0;
    end
    else begin
        if(load)
            sr_reg <= {1'b1, s_data, 1'b0};     //Loaded data to shift-right register
        if(tx_inc) begin
            bd_ctr_tx <= bd_ctr_tx + 1;             //Increment baud counter
            tx <= sr_reg[bit_ctr_tx];               //Write data to TX line
            if(bd_ctr_tx == CLOCK_RATE/BAUD_RATE) begin
                if(bit_ctr_tx > 7) begin 
                    i_ge_8 <= 1;
                    clr_tx <= 1;                //return to initial state
                end
                bd_ctr_tx <= 0;
                bit_ctr_tx <= bit_ctr_tx + 1;
            end
        end
    end
end //always
                        
endmodule
