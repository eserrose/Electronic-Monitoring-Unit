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
    .gpio_rtl_tri_o     (en),
    .qspi_flash_io0_io  (spi_0),
    .qspi_flash_io1_io  (spi_1),
    .qspi_flash_io2_io  (spi_2),
    .qspi_flash_io3_io  (spi_3),
    .qspi_flash_ss_io   (spi_ss),
    .reset              (rst),
    .sys_clock          (clk),
    .usb_uart_rxd       (rxd),
    .usb_uart_txd       (txd));
    
    reg rst;
    reg clk;
    reg rxd;
    wire txd;
    wire en;
    wire spi_0, spi_1, spi_2, spi_3;
    wire spi_ss;
    
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
    
    reg[9:0]sl_reg = 10'b0100100011;  
    integer i = 0;  
    always begin
        if(i > 5000) begin
            #(1e9/9600)
            rxd <= sl_reg[9];
            sl_reg <= sl_reg << 1;
        end
        if(i == 5009) begin
            i = 0;
        end
        i = i + 1;
     end
    
endmodule
