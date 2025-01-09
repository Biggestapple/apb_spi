//----------------------------------------------------------------------------------------------------------
//	FILE: 		cmsdk_apb_spi.v
// 	AUTHOR:		Biggest_apple
// 	
//	ABSTRACT:	This's a apb2spi ip core
// 	KEYWORDS:	fpga, basic module,signal process
// 
// 	MODIFICATION HISTORY:
//	$Log$
//			Biggest_apple 		2024.2.20		Create
//                              2024.2.21       Fixed Bit-Rate-Generator Bugs
//								2024.3.5		Fixed Interrupt logic Bugs
//-----------------------------------------------------------------------------------------------------------
module cmsdk_apb_spiMaster(
	input	wire		PCLK,			//Clock
	input	wire        PCLKG,			//Gated Clock
	input	wire        PRESETn,		//Reset
	
	input	wire        PSEL,			//Device select
	input	wire [11:2] PADDR,			//Address	(4Kbtye Memory Space)
	input	wire        PENABLE,		//Transfer control and this signal can be used 
										//to refer to the first transfer of the host
	input	wire        PWRITE,			//Write control
	input	wire [31:0] PWDATA,			//Write data

	output	wire [31:0] PRDATA,			//Read data
	output	wire        PREADY,			//Device ready
	output	wire        PSLVERR,		//Device error response
	
	output	wire		TXEINT,
	output	wire		RXNEINT,
	output	wire		OVRINT,
										//SPI Interface
	output	wire		MOSI,
	input	wire		MISO,
	output	wire		SCK
);
wire		apb_write	;
wire		apb_read	;
wire		is_tx_active				/* synthesis keep */;
wire		is_rd_rxbuffer				/* synthesis keep */;
reg			[7:0]	spi_bps_cnt_q;
wire		[7:0]	spi_bps_cnt_d;
reg			[3:0]	cur_txorx_bitth;
assign		apb_write	=PSEL &PWRITE &(~PENABLE);
assign		apb_read	=PSEL &(~PWRITE);

reg			CPOL_q;						//时钟极性
reg			CPHA_q;						//时钟相位
reg			SPIEN_q;
reg			LSBFIRST_q;
reg			DFF_q;						//16/8位选择字
reg			[3:0]	BR_q;				//SPI波特率发生器幅频系数
/*
	SPI_CLK_FRE	=APB_CLK/({BR_q,4'b1111});
*/
reg			RXNEINT_EN_q;
reg			TXEINT_EN_q;
wire		RXNEINT_EN_d;
wire		TXEINT_EN_d;
reg			OVRINT_EN_q;
wire		OVRINT_EN_d;

wire		CPOL_d;
wire		CPHA_d;
wire		SPIEN_d;	
wire		LSBFIRST_d;
wire		DFF_d;
wire		[3:0]	BR_d;

reg			[15:0]	spi_tx_shiftreg;	
reg			[15:0]	spi_rx_shiftreg;

reg			TXE_q						/* synthesis keep */;						
										//发送寄存器空标志
wire		TXE_d;
reg			RXNE_q						/* synthesis keep */;						
										//接收寄存器非空标志
wire		RXNE_d;
//reg			OVR_q;
//reg			OVR_d;


always @(posedge PCLKG or negedge PRESETn)
	if(!PRESETn) begin
		CPOL_q	<=1'b0;
		CPHA_q	<=1'b0;
		SPIEN_q	<=1'b0;
		DFF_q	<=1'b0;					//Default 8-bit mode
		BR_q	<=4'h0;
		LSBFIRST_q		<=1'b0;			//Default MSB first
		RXNEINT_EN_q	<=1'b0;
		TXEINT_EN_q		<=1'b0;
		OVRINT_EN_q		<=1'b0;
	end
	else begin
		CPOL_q	<=CPOL_d;
		CPHA_q	<=CPHA_d;
		SPIEN_q	<=SPIEN_d;
		DFF_q	<=DFF_d;
		BR_q	<=BR_d;
		LSBFIRST_q		<=LSBFIRST_d;
		RXNEINT_EN_q	<=RXNEINT_EN_d;
		TXEINT_EN_q		<=TXEINT_EN_d;
		OVRINT_EN_q		<=OVRINT_EN_d;
	end
always @(posedge PCLK or negedge PRESETn)
	if(!PRESETn) begin
		TXE_q	<=1'b1;
		RXNE_q	<=1'b0;
		//OVR_q	<=1'b0;		
		spi_bps_cnt_q	<=8'b0;
										//接收溢出错误标志位
	end
	else begin
		TXE_q	<=TXE_d;
		RXNE_q	<=RXNE_d;
		//OVR_q	<=OVR_d;
		spi_bps_cnt_q	<=spi_bps_cnt_d;
	end
reg		[1:0]	spi_state;
reg		mosi_reg;
wire	txArx_done;
										//SPI TX/RX FSM
always @(posedge PCLK or negedge PRESETn)
	if(!PRESETn) begin
		spi_state	<=2'd0;
		mosi_reg	<=1'b1;				//MOSI 空闲默认高电平
		cur_txorx_bitth	<=4'b0;
		spi_rx_shiftreg	<=16'b0;
		spi_tx_shiftreg	<=16'b0;
	end
	else
		case(spi_state)
			2'd0:						//IDLE 空闲态
				begin
					mosi_reg        <=1'b1;
                    cur_txorx_bitth <='d0;
					if(is_tx_active && SPIEN_q) begin
						spi_state		<=1'b1;
						spi_tx_shiftreg	<=PWDATA[15:0];
					end
				end
			2'd1:
				begin
					mosi_reg	<=	(LSBFIRST_q)	?spi_tx_shiftreg[0]:
									(DFF_q)			?spi_tx_shiftreg[15]:spi_tx_shiftreg[7];
					if(spi_bps_cnt_q =={BR_q,4'b1111}) begin
						spi_tx_shiftreg	<=	(LSBFIRST_q) ?{1'b1,spi_tx_shiftreg[15:1]}:{spi_tx_shiftreg[14:0],1'b1};
						cur_txorx_bitth	<=	cur_txorx_bitth +1'b1;
						spi_state		<=	2'd2;
					end
					else if(spi_bps_cnt_q =={1'b0,BR_q,3'b111})
										//Sampling the MISO signal
						spi_rx_shiftreg	<=	(LSBFIRST_q) ?{MISO,spi_rx_shiftreg[15:1]}:{spi_rx_shiftreg[14:0],MISO};
				end
			2'd2:
				begin
					if(DFF_q	)
						spi_state	<=(cur_txorx_bitth ==4'h0) ?2'd0:2'd1;
					else 
						spi_state	<=(cur_txorx_bitth ==4'h8) ?2'd0:2'd1;
				end
			default:
				spi_state	<=3'd0;
		endcase
assign	{OVRINT_EN_d,TXEINT_EN_d,RXNEINT_EN_d,LSBFIRST_d,DFF_d,
		CPHA_d,CPOL_d,SPIEN_d}
				=(apb_write && PADDR ==10'h000) ? PWDATA[7:0]:
		{OVRINT_EN_q,TXEINT_EN_q,RXNEINT_EN_q,LSBFIRST_q,DFF_q,
		CPHA_q,CPOL_q,SPIEN_q};
assign	BR_d	=(apb_write && PADDR ==10'h002) ? PWDATA[3:0]:BR_q;
assign	is_tx_active	=(apb_write &&PADDR ==10'h003);
assign	is_rd_rxbuffer	=(apb_read	&&PADDR ==10'h004);
assign	spi_bps_cnt_d	=(spi_state ==2'd1) ?spi_bps_cnt_q +1'b1:'d0;
assign	txArx_done		=(spi_state ==2'd2 && ((DFF_q  && cur_txorx_bitth ==4'h0) |(~DFF_q && cur_txorx_bitth ==4'h8)));

wire	RXNE_d0			=(~is_rd_rxbuffer) ?((RXNE_q ==1'b0) ?txArx_done:1'b1):1'b0;
wire	TXE_d0			=(~is_tx_active) ?((TXE_q ==1'b0) ?txArx_done:1'b1):1'b0;
wire	RXNE_d1			=(apb_write && PADDR ==10'h001) ?	PWDATA[1]:1'b1;
								//软件清零 OR置一
wire	TXE_d1			=(apb_write && PADDR ==10'h001)	?	PWDATA[0]:1'b0;

assign	TXE_d			=TXE_d0		|	TXE_d1;
assign	RXNE_d			=RXNE_d0	&	RXNE_d1;


wire	SCK_temp_ss		=(CPHA_q)	?(spi_bps_cnt_q <{1'b0,BR_q,3'b111}):(spi_bps_cnt_q >{1'b0,BR_q,3'b111});
assign	SCK				=(CPOL_q)	?~SCK_temp_ss:SCK_temp_ss;
assign	MOSI			=mosi_reg;

assign	OVRINT			=1'b0		&OVRINT_EN_q		&SPIEN_q;
assign	TXEINT			=TXE_q		&TXEINT_EN_q		&SPIEN_q;
assign	RXNEINT			=RXNE_q		&RXNEINT_EN_q		&SPIEN_q;
assign	PREADY			=1'b1;
assign	PSLVERR			=1'b0;

									//APB 读取电路
									//为保证数据的稳定性，在读取变化数据时应
									//先采样放入寄存器
reg		[31:0]			apb_rd_buffer;
wire	[31:0]			apb_rd_mux_o0;
always @(posedge PCLKG or negedge PRESETn)
	if(!PRESETn)
		apb_rd_buffer	<=32'b0;
	else if(apb_read)
		apb_rd_buffer	<=apb_rd_mux_o0;
		
									//First Level of read mux
assign	apb_rd_mux_o0	=	(PADDR[11:2]	==10'h000)	?		{24'b0,OVRINT_EN_q,TXEINT_EN_q,RXNEINT_EN_q,LSBFIRST_q,DFF_q,
							CPHA_q,CPOL_q,SPIEN_q}:
							(PADDR[11:2]	==10'h001)	?		{30'b0,RXNE_q,TXE_q}:
							(PADDR[11:2]	==10'h002)	?		{28'b0,BR_q}:
							(PADDR[11:2]	==10'h003)	?		{16'b0,spi_tx_shiftreg}:
							(PADDR[11:2]	==10'h004)	?		{16'b0,spi_rx_shiftreg}:
							32'h0;
assign	PRDATA			=	apb_rd_buffer;
endmodule