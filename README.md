# SpiMaster-ipcore
* The implemention of spi master protocol, standard APB, for future reference

## IP Core Configuration 

|  表头   | 表头  |
|  ----  | ----  |
| 单元格  | 单元格 |
| 单元格  | 单元格 |

## Timing Diagram
基本按照AXI4协议,tvalid拉高表示数据有效,给了N个数据之后,给一个tlast表示是最后一个数据

## Usage
* fft_wrapper.v (FFT_WRAPPER.v) 完成了对FFT核的配置
* fft_wrapper_tb.v 从本地文件中读取出数据进行FFT之后的结果存在本地文件中
* 其他一些模块是与其他东西的接口

## Tips
* FFT模块的后级ready信号(m_axis_tready)一定要拉高,不然会出错
* FFT模块的所有控制信号在一开始rst的时候一定要给初值(如果为Z或者是X)会导致仿真错误无法执行
