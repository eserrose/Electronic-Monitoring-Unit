`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 06/02/2021 09:12:46 AM
// Design Name: 
// Module Name: Memory_Unit
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

module Memory_Unit(
    input wire clk,                  // Clock signal
    input wire [31:0] addr,          // Memory Address
    input wire [31:0] write_data,    // Memory Address Contents
    input wire memwrite, memread,
    output reg [31:0] read_data      // Output of Memory Address Contents
    );
    
reg [31:0] MEMO[0:255];  // 256 words of 32-bit memory

integer i;

initial begin
  read_data <= 0;
  for (i = 0; i < 256; i = i + 1) begin
    MEMO[i] = i;
  end
end

always @(posedge clk) begin
  if (memwrite == 1'b1) 
    MEMO[addr] <= write_data;
  
  if (memread == 1'b1) 
    read_data <= MEMO[addr]; 
end

endmodule
