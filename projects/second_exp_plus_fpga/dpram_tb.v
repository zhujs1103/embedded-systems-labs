`timescale 1ns/1ns

`define clk_period 20

module dpram_tb;

	reg clock;
	reg [3:0]data;
	reg [2:0]rdaddress;
	reg [2:0]wraddress;
	reg wren;
	
	wire [3:0]q;
	
	integer i;
	
	double_ram double_ram(
		.clock(clock),
		.data(data),
		.rdaddress(rdaddress),
		.wraddress(wraddress),
		.wren(wren),
		.q(q)
	);
	
	initial clock = 1;
	always#(`clk_period/2)clock = ~clock;
	
	initial begin
		data = 0;
		rdaddress = 0;
		wraddress = 0;
		wren = 0;
		#(`clk_period*20 +1 );
		for (i=0;i<=7;i=i+1)begin
			wren = 1;
			data = i;
			rdaddress = i;
			wraddress = 0;      // M1
			#`clk_period;
		end
		for (i=0;i<=7;i=i+1)begin
			wren = 1;
			data = 7 - i;
			rdaddress = i;
			wraddress = 1;      // M2
			#`clk_period;
		end
		wren = 0;
		#(`clk_period*20);
		for (i=0;i<=7;i=i+1)begin
			rdaddress = i;
			wraddress = 0;      // read M1
			#`clk_period;
		end
		for (i=0;i<=7;i=i+1)begin
			rdaddress = i;
			wraddress = 1;      // read M2
			#`clk_period;
		end
		#(`clk_period*20);
		$stop;	
	end

endmodule
