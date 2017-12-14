`timescale 1 ps / 1 ps

module dummy
  (
   input 	     clk,
   output reg	     interrupt
   );

   localparam DWELL = 249999999;
   localparam LEN   = 1;
   
   reg [31:0] 	     cnt;
   
   always @(posedge clk)
     if (cnt < LEN)
       interrupt <= 1;
     else
       interrupt <= 0;
   
   always @(posedge clk) 
     if (cnt < DWELL)
       cnt <= cnt + 'b1;
     else
       cnt <= 0;
   
endmodule

   
      
