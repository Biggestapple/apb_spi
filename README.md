## 1. SpiMaster-ipcore Introduction
- 这是一个轻量级的、由Verilog 构建的可读源码 IP 核，实现了 APB 协议到 SPI协议（目前仅支持主机模式）的转换
- 该驱动器支持多种配置包括：时钟极性、时钟相位、传输字长等
- 具有三种可屏蔽中断：接受/发送完成中断、溢出中断
## 2. Document Structures
```markdown
─ hardware----------------------硬件代码
  ├─ sim  
  │  └─ apb_spi_tb.v------------仿真激励      
  └─ rtl   
     └─ cmsdk_apb_spi.v---------IP核主程序
```

## IP Core and Register Configuration

