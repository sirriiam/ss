#ifndef _HEAD_H
#define _HEAD_H

//#define assembly

#ifndef assembly
typedef volatile unsigned char xdata vxu8;
#endif

/****************************
*sys
*****************************/
sfr PCON   = 0x87;
sfr RSTFLG = 0xEF;
sfr SP     = 0x81;
sfr DPL    = 0x82;
sfr DPH    = 0x83;
sfr DPSEL  = 0x84;
sfr RIADRH = 0x85;
sfr PSW    = 0xD0;
sfr ACC    = 0xE0;
sfr B      = 0xF0;
#ifdef assembly
  SYSCFG    EQU    0FF00H
  PDCFG     EQU    0FF02H
#else
  #define SYSCFG  (*(vxu8 *)0xFF00)
  #define PDCFG   (*(vxu8 *)0xFF02)
#endif

/****************************
*INTC
*****************************/
sfr IE       = 0xA8;
sfr IP       = 0xB8;
sfr EIE1     = 0xE6;
sfr EIE2     = 0xF6;
sfr EIP1     = 0xE7;
sfr EIP2     = 0xF7;
sfr EINTCON0 = 0xE4;
sfr EINTCON1 = 0xE5;

sbit EA  = IE^7;
sbit ET2 = IE^5;
sbit ES  = IE^4;
sbit ET1 = IE^3;
sbit EX1 = IE^2;
sbit ET0 = IE^1;
sbit EX0 = IE^0;

sbit PT2 = IP^5;
sbit PS  = IP^4;
sbit PT1 = IP^3;
sbit PX1 = IP^2;
sbit PT0 = IP^1;
sbit PX0 = IP^0;

/****************************
*CKMU
*****************************/
sfr CKEN   = 0xB1;
sfr CKSS   = 0xB2;
sfr CKDS   = 0xB4;
sfr PLLCON = 0xB5;
sfr SPKEY  = 0xB7;

#ifdef assembly
  PCGCFG    EQU    0FF20H
  CKOCFG    EQU    0FF3FH
#else
  #define PCGCFG  (*(vxu8 *)0xFF20)
  #define CKOCFG  (*(vxu8 *)0xFF3F)
#endif


/****************************
*PMU
*****************************/
sfr  LVDCON = 0xED;
sfr  BORCON = 0xEE;
sfr  VBGTD  = 0xD1;



/****************************
*IO PORT
*****************************/
sfr P0  =  0x80;
sfr P1  =  0x90;
sfr P2  =  0xA0;
sfr P3  =  0xB0;

sfr P0DPL  =  0xA4;
sfr P1DPL  =  0xA5;
sfr P2DPL  =  0xA6;
sfr P3DPL  =  0xA7;
sfr P0DPH  =  0xF1;
sfr P1DPH  =  0xF2;
sfr P2DPH  =  0xF3;
sfr P3DPH  =  0xF4;
#ifdef assembly
  P4      EQU    0FF04H
  P5      EQU    0FF09H
  P6      EQU    0FF0AH
  P4DPL   EQU    0FF14H
  P5DPL   EQU    0FF19H
  P6DPL   EQU    0FF1AH
  P4DPH   EQU    0FF24H
  P5DPH   EQU    0FF29H
  
  P3AMEN   EQU    0FF33H
  P5AMEN   EQU    0FF39H
  IOMUXC0 EQU    0FF48H
  IOMUXC1 EQU    0FF49H
  IOMUXC2 EQU    0FF4AH
#else
  #define P4     (*(vxu8 *)0xFF04)
  #define P5     (*(vxu8 *)0xFF09)
  #define P6     (*(vxu8 *)0xFF0A)
  #define P4DPL  (*(vxu8 *)0xFF14)
  #define P5DPL  (*(vxu8 *)0xFF19)
  #define P6DPL  (*(vxu8 *)0xFF1A)
  #define P4DPH  (*(vxu8 *)0xFF24)
  #define P5DPH  (*(vxu8 *)0xFF29)
  
  #define P3AMEN  (*(vxu8 *)0xFF33)
  #define P5AMEN  (*(vxu8 *)0xFF39)
  #define IOMUXC0 (*(vxu8 *)0xFF48)
  #define IOMUXC1 (*(vxu8 *)0xFF49)
  #define IOMUXC2 (*(vxu8 *)0xFF4A)
#endif

sbit P00 = P0^0;
sbit P01 = P0^1;
sbit P02 = P0^2;
sbit P03 = P0^3;
sbit P04 = P0^4;
sbit P05 = P0^5;
sbit P06 = P0^6;
sbit P07 = P0^7;

sbit P10 = P1^0;
sbit P11 = P1^1;
sbit P12 = P1^2;
sbit P13 = P1^3;
sbit P14 = P1^4;
sbit P15 = P1^5;
sbit P16 = P1^6;
sbit P17 = P1^7;

sbit P20 = P2^0;
sbit P21 = P2^1;
sbit P22 = P2^2;
sbit P23 = P2^3;
sbit P24 = P2^4;
sbit P25 = P2^5;
sbit P26 = P2^6;
sbit P27 = P2^7;

sbit P30 = P3^0;
sbit P31 = P3^1;
sbit P32 = P3^2;
sbit P33 = P3^3;
sbit P34 = P3^4;
sbit P35 = P3^5;
sbit P36 = P3^6;
sbit P37 = P3^7;

/****************************
*CWT
*****************************/
sfr  CWTCON  =  0xDB;
sfr  CWTPRS  =  0xDC;
sfr  CWTCNT  =  0xDD;
sfr  CWTD0  =  0xDE;
sfr  CWTD1  =  0xDF;

/****************************
*WDT
*****************************/
sfr  WDTEN0  =  0xFD;
sfr  WDTEN1  =  0xFE;
sfr  WDTCON  =  0xFF;

/****************************
*T0/T1
*****************************/
sfr TCON = 0x88;
sfr TMOD = 0x89;
sfr TL0  = 0x8A;
sfr TL1  = 0x8B;
sfr TH0  = 0x8C;
sfr TH1  = 0x8D;
#ifdef assembly
  TIMPRS    EQU    0FF1BH
#else
  #define TIMPRS  (*(vxu8 *)0xFF1B)
#endif

sbit TF1 = TCON^7;
sbit TR1 = TCON^6;
sbit TF0 = TCON^5;
sbit TR0 = TCON^4;
sbit IE1 = TCON^3;
sbit IT1 = TCON^2;
sbit IE0 = TCON^1;
sbit IT0 = TCON^0;

/****************************
*T2
*****************************/
sfr T2CON1 = 0xC8;
sfr RCAP2L = 0xCA;
sfr RCAP2H = 0xCB;
sfr TL2    = 0xCC;
sfr TH2    = 0xCD;
sfr T2DH   = 0xCE;
sfr T2DL   = 0xCF;
#ifdef assembly
  T2CON2    EQU    0FF1CH
#else
  #define T2CON2  (*(vxu8 *)0xFF1C)
#endif
sbit TF2   = T2CON1^7;
sbit EXF2  = T2CON1^6;

sbit FCPEN = T2CON1^3;
sbit TR2   = T2CON1^2;
sbit CT2   = T2CON1^1;
sbit CPRL2 = T2CON1^0;

/****************************
*UART
*****************************/
sfr SCON = 0x98;
sfr SBUF = 0x99;
sfr BRCON = 0x8E;
sfr BRTIM = 0x8F;

sbit SM0 = SCON^7;
sbit SM1 = SCON^6;
sbit SM2 = SCON^5;
sbit REN = SCON^4;
sbit TB8 = SCON^3;
sbit RB8 = SCON^2;
sbit TI  = SCON^1;
sbit RI  = SCON^0;

/****************************
*keyin
*****************************/
sfr KINFLG = 0xAF;

#ifdef assembly
  KINEN    EQU    0FF2BH
  KINPS    EQU    0FF2CH
#else
  #define KINEN  (*(vxu8 *)0xFF2B)
  #define KINPS  (*(vxu8 *)0xFF2C)
#endif



/****************************
*PWM8
*****************************/
  #ifdef assembly
  PWM8C    EQU    0FF0CH
  PWM8D    EQU    0FF0DH
  #else
  #define PWM8C  (*(vxu8 *)0xFF0C)
  #define PWM8D  (*(vxu8 *)0xFF0D)
  #endif

/****************************
*SPI
*****************************/
sfr SPICON0 = 0xF8;
sfr SPICON1 = 0xF9;
sfr SPIFLG  = 0xFA;
sfr SPIDAT  = 0xFB;


/****************************
*RTC
*****************************/
sfr RTCCON = 0xC9;

  #ifdef assembly
  RTCCNT    EQU    0FF0BH
  #else
  #define RTCCNT  (*(vxu8 *)0xFF0B)
  #endif



/****************************
*lcd
*****************************/
#ifdef assembly
  LCDPC0  EQU    0FF40H
  LCDPC1  EQU    0FF41H
  LCDPC2  EQU    0FF42H
  LCDPC3  EQU    0FF43H
  LCDPC4  EQU    0FF44H
  
  LCDCON    EQU    0FF50H
  LCDPCON   EQU    0FF51H
  TKCON     EQU    0FF52H
  TKADDR    EQU    0FF54H
  LCDM0     EQU    0FF60H
  LCDM1     EQU    0FF61H
  LCDM2     EQU    0FF62H
  LCDM3     EQU    0FF63H
  LCDM4     EQU    0FF64H
  LCDM5     EQU    0FF65H
  LCDM6     EQU    0FF66H
  LCDM7     EQU    0FF67H
  LCDM8     EQU    0FF68H
  LCDM9     EQU    0FF69H
  LCDM10    EQU    0FF6AH
  LCDM11    EQU    0FF6BH
  LCDM12    EQU    0FF6CH
  LCDM13    EQU    0FF6DH
  LCDM14    EQU    0FF6EH
  LCDM15    EQU    0FF6FH
  LCDM16    EQU    0FF70H
  LCDM17    EQU    0FF71H
  LCDM18    EQU    0FF72H
  LCDM19    EQU    0FF73H
  LCDM20    EQU    0FF74H
  LCDM21    EQU    0FF75H
  LCDM22    EQU    0FF76H
  LCDM23    EQU    0FF77H
  LCDM24    EQU    0FF78H
  LCDM25    EQU    0FF79H
  LCDM26    EQU    0FF7AH
  LCDM27    EQU    0FF7BH
  LCDM28    EQU    0FF7CH
  LCDM29    EQU    0FF7DH
  LCDM30    EQU    0FF7EH
  LCDM31    EQU    0FF7FH
#else
  #define LCDPC0  (*(vxu8 *)0xFF40)
  #define LCDPC1  (*(vxu8 *)0xFF41)
  #define LCDPC2  (*(vxu8 *)0xFF42)
  #define LCDPC3  (*(vxu8 *)0xFF43)
  #define LCDPC4  (*(vxu8 *)0xFF44)
  
  #define LCDCON  (*(vxu8 *)0xFF50)
  #define LCDPCON (*(vxu8 *)0xFF51)
  #define TKCON   (*(vxu8 *)0xFF52)
  #define TKADDR  (*(vxu8 *)0xFF54)
  #define LCDM0   (*(vxu8 *)0xFF60)
  #define LCDM1   (*(vxu8 *)0xFF61)
  #define LCDM2   (*(vxu8 *)0xFF62)
  #define LCDM3   (*(vxu8 *)0xFF63)
  #define LCDM4   (*(vxu8 *)0xFF64)
  #define LCDM5   (*(vxu8 *)0xFF65)
  #define LCDM6   (*(vxu8 *)0xFF66)
  #define LCDM7   (*(vxu8 *)0xFF67)
  #define LCDM8   (*(vxu8 *)0xFF68)
  #define LCDM9   (*(vxu8 *)0xFF69)
  #define LCDM10  (*(vxu8 *)0xFF6A)
  #define LCDM11  (*(vxu8 *)0xFF6B)
  #define LCDM12  (*(vxu8 *)0xFF6C)
  #define LCDM13  (*(vxu8 *)0xFF6D)
  #define LCDM14  (*(vxu8 *)0xFF6E)
  #define LCDM15  (*(vxu8 *)0xFF6F)
  #define LCDM16  (*(vxu8 *)0xFF70)
  #define LCDM17  (*(vxu8 *)0xFF71)
  #define LCDM18  (*(vxu8 *)0xFF72)
  #define LCDM19  (*(vxu8 *)0xFF73)
  #define LCDM20  (*(vxu8 *)0xFF74)
  #define LCDM21  (*(vxu8 *)0xFF75)
  #define LCDM22  (*(vxu8 *)0xFF76)
  #define LCDM23  (*(vxu8 *)0xFF77)
  #define LCDM24  (*(vxu8 *)0xFF78)
  #define LCDM25  (*(vxu8 *)0xFF79)
  #define LCDM26  (*(vxu8 *)0xFF7A)
  #define LCDM27  (*(vxu8 *)0xFF7B)
  #define LCDM28  (*(vxu8 *)0xFF7C)
  #define LCDM29  (*(vxu8 *)0xFF7D)
  #define LCDM30  (*(vxu8 *)0xFF7E)
  #define LCDM31  (*(vxu8 *)0xFF7F)
#endif


/****************************
*adc
*****************************/
sfr ADCON = 0xE8;
sfr ADRL  = 0xE9;
sfr ADRH  = 0xEA;

sbit CHS3 = ADCON^7;
sbit CHS2 = ADCON^6;
sbit CHS1 = ADCON^5;
sbit CHS0 = ADCON^4;
sbit ADEN = ADCON^3;
sbit ADGO = ADCON^2;

#ifdef assembly
  ADCFG    EQU    0FF3EH
#else
  #define ADCFG  (*(vxu8 *)0xFF3E)
#endif


/****************************
*CMP
*****************************/
sfr CPCON = 0x9E;
sfr VRCON = 0x9F;











#endif