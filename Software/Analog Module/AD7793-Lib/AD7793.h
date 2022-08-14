/************************************************************************************************
*												*
* This code is not supported by Analog Device Inc, has not been fully tested or verified,	*
* is for Demo purposes only and is used at ones own risk.		 			*
*		 								24-11-2008	*
************************************************************************************************/

#ifndef   AD7793_h
#define   AD7793_h

typedef struct
{
  BYTE len;                                             // Register size (bits)
  BYTE WR_RD;                                           // Register mode (read/write)
}REGISTER_TYPE;

#define REG_RD          0                               // Read-only Register
#define REG_WR          1                               // Write-only Register
#define REG_RD_WR       2                               // Read or Write Register

// Communications Register
#define   WEN     (1 <<7)
#define   RW      (1 <<6)
#define   RS(x)   ((x) <<3)
  // Register Address
  #define   COMM_REG    0
  #define   STAT_REG    0
  #define   MODE_REG    1
  #define   CONF_REG    2
  #define   DATA_REG    3
  #define   ID_REG      4
  #define   IO_REG      5
  #define   OFFS_REG    6
  #define   FUL_SC_REG  7
  #define   ALL_REGS    8
#define   CREAD   (1 << 2)

// Status Register
#define   RDY     (1 <<7)
#define   ERR     (1 <<6)
#define   CH(x)   ((x) <<0)

// Mode Register
#define   MD(x)   ((x)<<13)
  // Operating Modes
  #define   CONT_CONV     0
  #define   SINGLE_CONV   1
  #define   IDLE          2
  #define   POWER_DOWN    3
  #define   INT_ZERO_CAL  4
  #define   INT_FULL_CAL  5
  #define   SYS_ZERO_CAL  6
  #define   SYS_FULL_CAL  7
#define   CLK(x)  ((x) <<6)
  // ADC Clock Source
  #define   INT_64KHZ_UNAV  0
  #define   INT_64KHZ_AV    1
  #define   EXT_64KHZ       2
  #define   EXT_DIV2        3
#define   FS(x)   ((x) <<0)                             // See AD7793 Datasheet (p16/17)

// Configuration Register
#define   VBIAS(x)  ((x) <<15)
  // Bias Voltage
  #define   BIAS_GEN_DIS  0
  #define   BIAS_AIN1     1
  #define   BIAS_AIN2     2
#define   BO      (1 <<13)
#define   UB      (1 <<12)
#define   BOOST   (1 <<11)
#define   G(x) ((x) <<8)
  // Gain Select Bits
  #define   GAIN_1        0                             // (In-amp not used) 2.5V ADC Input Range (2.5V Ref)
  #define   GAIN_2        1                             // (In-amp not used) 1.25V ADC Input Range (2.5V Ref)
  #define   GAIN_4        2                             // 625mV ADC Input Range (2.5V Ref)
  #define   GAIN_8        3                             // 312.5mV ADC Input Range (2.5V Ref)
  #define   GAIN_16       4                             // 156.2mV ADC Input Range (2.5V Ref)
  #define   GAIN_32       5                             // 78.125mV ADC Input Range (2.5V Ref)
  #define   GAIN_64       6                             // 39.06mV ADC Input Range (2.5V Ref)
  #define   GAIN_128      7                             // 19.53mV ADC Input Range (2.5V Ref)
#define   REFSEL(x) ((x) <<7)
  // Reference Source
  #define   EXT_REF       0
  #define   INT_REF       1
#define   BUF       (1 <<4)
#define   CH_SEL(x) ((x) <<0)
  // Channel Select Bits
  #define   AIN1_POSNEG   0
  #define   AIN2          1
  #define   AIN3          2
  #define   AIN1_NEGNEG   3
  #define   TEMP_SEN      6
  #define   AVDD_MON      7

// IO Register
#define   IEXCDIR(x)  ((x) <<2)
  // Current Source Direction
  #define   DIR0          0                             // Source IEXC1 connected to Pin IOUT1, Source IEXC2 connected to Pin IOUT2
  #define   DIR1          1                             // Source IEXC1 connected to Pin IOUT2, Source IEXC2 connected to Pin IOUT1
  #define   DIR2          2                             // Both sources connected to Pin IOUT1
  #define   DIR3          3                             // Both sources connected to Pin IOUT2
#define   IEXCEN(x)   ((x) <<0)
  // Current Source Value
  #define   IEXC_DIS      0
  #define   uA10          1
  #define   uA210         2
  #define   mA1           3

#endif
