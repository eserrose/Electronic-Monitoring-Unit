`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Engineer: Emirhan Eser Gul
// 
// Create Date: 06/16/2021 07:50:04 PM
// Module Name: EMU_Top
// Project Name: EMU
// Description: Top module for EMU that includes all units
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module EMU_Top(
    input   clk,          //Clock
    input   rst,          //Active high synchronous reset
    input   main_avionic, //RX from main avionic system
    input   en_0,         //Enable pin from mechanical switch (open-circuit)
    output  ign           //Motor ignition signal
    );
    
 wire   rxd;    //RX of microblaze, connected to tx of comm unit
 wire   txd;    //TX of microblaze, connected to rx of calculation block 
 wire   en_1;   //Enable pin from comm unit to motor unit
 wire   en_2;   //Enable pin from calculation block to motor unit
 wire   addr;   //Address line of memory block
 wire   data;   //Data line of memory block
 wire   memw;   //Memory write
 
 Comm_Unit CommBlock(
    .clk       (clk),    //Clock signal
    .rxd       (main_avionic),
    //TODO: I2C Connection with sensors (bmp, mpu, laser)
    .txd       (rxd),    //UART tx, will be connected to RX of MicroBlaze
    .en_1      (en_1)    //Main avionic system enable line    
    );
    
  Memory_Unit MemBlock(
    .clk        (clk),       
    .addr       (addr), //connect this to mb        
    .write_data (data), //connect this to mb        
    .memwrite   (memw)  //connect this to mb 
    );
    
 EMU_design_wrapper MicroBlaze(
    .reset          (rst),
    .sys_clock      (clk),
    .usb_uart_rxd   (rxd),
    .usb_uart_txd   (txd)); //connect this to calculation ip
    
 Motor_Unit MotorBlock(
    .clk       (clk),    //Clock signal
    .rst       (rst),    //Reset signal
    .en_0      (en_0),   //Enable from open-circuit (Active-Low)
    .en_1      (en_1),   //Enable from main avionic system
    .en_2      (en_2),   //Enable from sensor calculation block
    .ign       (ign)     //Motor ignition signal    
    );
       
endmodule
