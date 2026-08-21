module double_ram(
       clock,
		 data,
		 rdaddress,
		 wraddress,
		 wren,
		 q
		 );
   input clock;
	input [3:0]data;
	input [2:0]rdaddress;
	input [2:0]wraddress;
	input wren;
	output [3:0]q;
	
	wire mem_select = wraddress[0];       // 0: M1, 1: M2
	wire [3:0]ram_addr = {1'b0,rdaddress};
	wire [3:0]q_m1;
	wire [3:0]q_m2;
	
ram2 m1_ram
(
	.clock(clock),
	.data(data),
	.rdaddress(ram_addr),
	.wraddress(ram_addr),
	.wren(wren & ~mem_select),
	.q(q_m1)
);

ram2 m2_ram
(
	.clock(clock),
	.data(data),
	.rdaddress(ram_addr),
	.wraddress(ram_addr),
	.wren(wren & mem_select),
	.q(q_m2)
);

assign q = mem_select ? q_m2 : q_m1;

endmodule
