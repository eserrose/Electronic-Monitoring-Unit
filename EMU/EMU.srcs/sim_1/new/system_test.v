`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 06/23/2021 04:18:20 AM
// Design Name: 
// Module Name: system_test
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


module system_test();

EMU_Top DUT(
    .clk            (clk),  
    .rst            (rst),
    .main_avionic   (ma), 
    .i2c            (i2c),
    .en_0           (en0),
    .ign            (ign)
    );

    reg rst;
    reg clk;
    reg ma;
    reg en0;
    reg i2c;
    wire ign;
    
    reg[9:0]sl_reg = 'hEE;    //shift-left register (to load with simulated RX data)
    
    always begin
        clk <= 1'b0;
        forever begin
            #41.666 clk <= ~clk;
        end
    end
    
    initial begin
        ma   <= 1'b1;
        en0  <= 1'b1;
        rst  <= 1'b1;
        i2c  <= 1'b0;
        #1000 rst <= 1'b0;
    end
    
    initial begin
        #5e5            //after 0.5 ms
        en0 <= 0'b0;    //First seperation occured
        #5e4            //after 0.05 ms
        for(integer i = 0; i < 9; i = i +1) begin   //Main avionic sent ignition signal
            ma <= sl_reg[9];
            sl_reg <= sl_reg << 1;
            #(1e9/uart_datapath.BAUD_RATE);
        end 
    end
    
    always begin
        #1e5      //every 0.1ms
        i2c <= 1; //indicate data has been read from sensors
        #1e4     //reading took 0.01ms
        i2c <= 0;
    end
    
endmodule