`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Engineer: E. Eser Gul
// 
// Create Date: 06/02/2021 09:12:46 AM
// Module Name: Memory_Unit
// Description: This is only a place-holder memory module that will be replaced by an external flash device 
// 
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////

module Memory_Unit(
    input wire clk,      // Clock signal
    input wire cs,       //SPI Chip select (active low)
    inout wire spi_io    //SPI Input/Output
    );

reg [31:0] addr;          // Memory Address
reg [31:0] MEMO[0:255];  // 256 words of 32-bit memory

integer i;

initial begin
  for (i = 0; i < 256; i = i + 1) begin
    MEMO[i] = i;
  end
end

always @(posedge clk) begin
  if (cs == 1'b0) 
    MEMO[addr] <= spi_io;
end

endmodule
