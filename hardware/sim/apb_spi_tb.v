//----------------------------------------------------------------------------------------------------------
//	FILE: 		apb_spi_tb.v
// 	AUTHOR:		Biggest_apple
// 	
//	ABSTRACT:	This's a apb2spi ip core's testbench
// 	KEYWORDS:	fpga, basic module,signal process
// 
// 	MODIFICATION HISTORY:
//	$Log$
//			Biggest_apple 		2024.2.29		Create
//-----------------------------------------------------------------------------------------------------------
module apb_spi_tb();
reg PCLK    =1'b0;
wire PCLKG  =PCLK;
reg PRESETn  =1'b1;
reg PSEL    =1'b0;
reg [11:2]  PADDR;
reg PENABLE;
reg PWRITE;
reg [31:0]  PWDATA;
wire [31:0] PRDATA;
wire PREADY;
wire PSLVERR;
wire TXEINT;
wire RXNEINT;
wire OVRINT;
wire MOSI;
wire MISO;
wire SCK;
cmsdk_apb_spiMaster u_cmsdk_apb_spiMaster(
    .PCLK      (PCLK      ),
    .PCLKG     (PCLKG     ),
    .PRESETn   (PRESETn   ),

    .PSEL      (PSEL      ),
    .PADDR     (PADDR     ),
    .PENABLE   (PENABLE   ),
    .PWRITE    (PWRITE    ),
    .PWDATA    (PWDATA    ),

    .PRDATA    (PRDATA    ),
    .PREADY    (PREADY    ),
    .PSLVERR   (PSLVERR   ),
    .TXEINT    (TXEINT    ),
    .RXNEINT   (RXNEINT   ),
    .OVRINT    (OVRINT    ),
    .MOSI      (MOSI      ),
    .MISO      (MISO      ),
    .SCK       (SCK       )
);
                            //Clock Generater
always #1   PCLK    <=~PCLK;
initial begin
    #0; PSEL    <=1'b0;
        PADDR   <=10'h000;
        PENABLE <=1'b0;
        PWRITE  <=1'b0;
        PWDATA  <=32'b0;
        PRESETn  <=1'b1;
    #2; PRESETn  <=1'b0;
    #3; PRESETn  <=1'b1;
        PSEL    <=1'b1;
        PADDR   <=10'h000;
        PENABLE <=1'b0;
        PWRITE  <=1'b1;
        PWDATA  <=32'h0000_0061;
                            //测试写入控制字并使能SPI，使能中断标志位
    #2; PENABLE <=1'b1;
    #2; PADDR   <=10'h000;
        PENABLE <=1'b0;
        PWRITE  <=1'b0;
        PWDATA  <=32'b0;
        PSEL    <=1'b0;

    #2  PSEL    <=1'b1;
        PADDR   <=10'h002;
        PENABLE <=1'b0;
        PWRITE  <=1'b1;
        PWDATA  <=32'h0000_0001;
                            //写入波特率发生器参数
    #2; PENABLE <=1'b1;
    #2; PADDR   <=10'h000;
        PENABLE <=1'b0;
        PWRITE  <=1'b0;
        PWDATA  <=32'b0;
        PSEL    <=1'b0;

    #2  PSEL    <=1'b1;
        PADDR   <=10'h003;
        PENABLE <=1'b0;
        PWRITE  <=1'b1;
        PWDATA  <=32'h0000_0012;
                            //写入发送寄存器
    #2; PENABLE <=1'b1;
    #2; PADDR   <=10'h000;
        PENABLE <=1'b0;
        PWRITE  <=1'b0;
        PWDATA  <=32'b0;
        PSEL    <=1'b0;
                            //等待只到发生发送完成与接收中断
                            //测试下一次传输是否正常
    wait(RXNEINT)
    #2  PSEL    <=1'b1;
        PADDR   <=10'h004;
        PENABLE <=1'b0;
        PWRITE  <=1'b0;
        PWDATA  <=32'h0000_0000;
                            //读接收寄存器
    #2; PENABLE <=1'b1;
    #2; PADDR   <=10'h000;
        PENABLE <=1'b0;
        PWRITE  <=1'b0;
        PWDATA  <=32'b0;
        PSEL    <=1'b0;

    #2  PSEL    <=1'b1;
        PADDR   <=10'h003;
        PENABLE <=1'b0;
        PWRITE  <=1'b1;
        PWDATA  <=32'h0000_0012;
                            //写入发送寄存器
    #2; PENABLE <=1'b1;
    #2; PADDR   <=10'h000;
        PENABLE <=1'b0;
        PWRITE  <=1'b0;
        PWDATA  <=32'b0;
        PSEL    <=1'b0;

    #200; $finish;

end
reg [7:0]   test_slave_txdata   =8'h34;
assign      MISO    =test_slave_txdata[7];
always @(negedge SCK)
    if(~PRESETn)
        test_slave_txdata   <=8'h34;
    else
        test_slave_txdata   <={test_slave_txdata[6:0],1'b0};
endmodule