-- generated by newgenasym Fri Aug 30 19:39:47 2013

library ieee;
use     ieee.std_logic_1164.all;
use     work.all;
entity ad54x2 is
    port (    
	\+vsense\: IN     STD_LOGIC;    
	\-vsense\: IN     STD_LOGIC;    
	AVDD:      IN     STD_LOGIC;    
	AVSS1:     IN     STD_LOGIC;    
	AVSS2:     IN     STD_LOGIC;    
	BOOST:     IN     STD_LOGIC;    
	CAP1:      INOUT  STD_LOGIC;    
	CAP2:      INOUT  STD_LOGIC;    
	CCOMP:     INOUT  STD_LOGIC;    
	CLEAR:     IN     STD_LOGIC;    
	\clear select\: IN     STD_LOGIC;    
	DVCC:      IN     STD_LOGIC;    
	\dvcc select\: IN     STD_LOGIC;    
	FAULT_N:   IN     STD_LOGIC;    
	GND1:      IN     STD_LOGIC;    
	GND2:      IN     STD_LOGIC;    
	GND3:      IN     STD_LOGIC;    
	GND4:      IN     STD_LOGIC;    
	IOUT:      OUT    STD_LOGIC;    
	LATCH:     IN     STD_LOGIC;    
	NC1:       INOUT  STD_LOGIC;    
	NC10:      INOUT  STD_LOGIC;    
	NC11:      INOUT  STD_LOGIC;    
	NC12:      INOUT  STD_LOGIC;    
	NC13:      INOUT  STD_LOGIC;    
	NC2:       INOUT  STD_LOGIC;    
	NC3:       INOUT  STD_LOGIC;    
	NC4:       INOUT  STD_LOGIC;    
	NC5:       INOUT  STD_LOGIC;    
	NC6:       INOUT  STD_LOGIC;    
	NC7:       INOUT  STD_LOGIC;    
	NC8:       INOUT  STD_LOGIC;    
	NC9:       INOUT  STD_LOGIC;    
	PAD:       IN     STD_LOGIC;    
	REFIN:     IN     STD_LOGIC;    
	REFOUT:    OUT    STD_LOGIC;    
	RSET:      IN     STD_LOGIC;    
	SCLK:      IN     STD_LOGIC;    
	SDIN:      IN     STD_LOGIC;    
	SDO:       OUT    STD_LOGIC;    
	VOUT:      OUT    STD_LOGIC);
end ad54x2;