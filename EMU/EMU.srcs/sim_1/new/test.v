`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: ITU 
// Engineer: E. Eser Gul
// 
// Create Date: 05/02/2021 07:43:42 AM
// Module Name: test
// Project Name: Electronic Monitoring Unit (EMU)
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments: Main test simulation
// 
//////////////////////////////////////////////////////////////////////////////////


module test();

EMU_design_wrapper DUT(
    .reset          (rst),
    .sys_clock      (clk),
    .usb_uart_rxd   (rxd),
    .usb_uart_txd   (txd));
    
    reg rst;
    reg clk;
    reg rxd;
    wire txd;
    
    always begin
        clk <= 1'b0;
        forever begin
            #41.666 clk <= ~clk;
        end
    end
    
    initial begin
        rst <= 1'b1;
        #1000 rst <= 1'b0;
    end

    initial rxd <= 1'b1;
    
endmodule
