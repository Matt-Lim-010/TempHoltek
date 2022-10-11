IAR0	EQU	[00H]
R0	EQU	[00H]	;old style declaration, not recommended for use
MP0	EQU	[01H]
IAR1	EQU	[02H]
R1	EQU	[02H]	;old style declaration, not recommended for use
MP1L	EQU	[03H]
MP1H	EQU	[04H]
ACC	EQU	[05H]
PCL	EQU	[06H]
TBLP	EQU	[07H]
TBLH	EQU	[08H]
TBHP	EQU	[09H]
STATUS	EQU	[0AH]
IAR2	EQU	[0CH]
MP2L	EQU	[0DH]
MP2H	EQU	[0EH]
RSTFC	EQU	[0FH]
TB0C	EQU	[010H]
TB1C	EQU	[011H]
SCC	EQU	[012H]
HIRCC	EQU	[013H]
PA	EQU	[014H]
PAC	EQU	[015H]
PAPU	EQU	[016H]
PAWU	EQU	[017H]
PB	EQU	[018H]
PBC	EQU	[019H]
PBPU	EQU	[01AH]
PC	EQU	[01BH]
PCC	EQU	[01CH]
PCPU	EQU	[01DH]
SLEDC0	EQU	[01EH]
SLEDC1	EQU	[01FH]
PSCR	EQU	[020H]
LVDC	EQU	[021H]
ORMC	EQU	[023H]
CTM0C0	EQU	[027H]
CTM0C1	EQU	[028H]
CTM0DL	EQU	[029H]
CTM0DH	EQU	[02AH]
CTM0AL	EQU	[02BH]
CTM0AH	EQU	[02CH]
SADOH	EQU	[02DH]
SADOL	EQU	[02EH]
SADC0	EQU	[02FH]
SADC1	EQU	[030H]
PLTSW	EQU	[031H]
PLTDACC	EQU	[032H]
PLTDA0L	EQU	[033H]
PLTDA1L	EQU	[034H]
PLTDA2L	EQU	[035H]
PLTC0C	EQU	[036H]
PLTC0VOS	EQU	[037H]
PLTC1C	EQU	[038H]
PLTC1VOS	EQU	[039H]
PLTCHYC	EQU	[03AH]
PLTAC	EQU	[03BH]
PLTAVOS	EQU	[03CH]
WDTC	EQU	[03DH]
EEA	EQU	[03EH]
EED	EQU	[03FH]
USR	EQU	[041H]
UCR1	EQU	[042H]
UCR2	EQU	[043H]
UCR3	EQU	[044H]
TXR_RXR	EQU	[045H]
BRG	EQU	[046H]
INTEG	EQU	[047H]
INTC0	EQU	[048H]
INTC1	EQU	[049H]
INTC2	EQU	[04AH]
INTC3	EQU	[04BH]
MFI0	EQU	[04CH]
MFI1	EQU	[04DH]
MFI2	EQU	[04EH]
MFI3	EQU	[04FH]
MFI4	EQU	[050H]
PTM0C0	EQU	[052H]
PTM0C1	EQU	[053H]
PTM0C2	EQU	[054H]
PTM0DL	EQU	[055H]
PTM0DH	EQU	[056H]
PTM0AL	EQU	[057H]
PTM0AH	EQU	[058H]
PTM0BL	EQU	[059H]
PTM0BH	EQU	[05AH]
PTM0RPL	EQU	[05BH]
PTM0RPH	EQU	[05CH]
PTM1C0	EQU	[05DH]
PTM1C1	EQU	[05EH]
PTM1DL	EQU	[05FH]
PTM1DH	EQU	[060H]
PTM1AL	EQU	[061H]
PTM1AH	EQU	[062H]
PTM1RPL	EQU	[063H]
PTM1RPH	EQU	[064H]
CTM1C0	EQU	[065H]
CTM1C1	EQU	[066H]
CTM1DL	EQU	[067H]
CTM1DH	EQU	[068H]
CTM1AL	EQU	[069H]
CTM1AH	EQU	[06AH]
PAS0	EQU	[06BH]
PAS1	EQU	[06CH]
PBS0	EQU	[06DH]
PCS0	EQU	[06EH]
IFS	EQU	[070H]
EEC	EQU	[0140H]

C	EQU	[0AH].0
AC	EQU	[0AH].1
Z	EQU	[0AH].2
OV	EQU	[0AH].3
PDF	EQU	[0AH].4
TO	EQU	[0AH].5
CZ	EQU	[0AH].6
SC	EQU	[0AH].7
WRF	EQU	[0FH].0
LVRF	EQU	[0FH].2
TB00	EQU	[010H].0
TB01	EQU	[010H].1
TB02	EQU	[010H].2
TB0ON	EQU	[010H].7
TB10	EQU	[011H].0
TB11	EQU	[011H].1
TB12	EQU	[011H].2
TB1ON	EQU	[011H].7
FSIDEN	EQU	[012H].0
FHIDEN	EQU	[012H].1
CKS0	EQU	[012H].5
CKS1	EQU	[012H].6
CKS2	EQU	[012H].7
HIRCEN	EQU	[013H].0
HIRCF	EQU	[013H].1
HIRC0	EQU	[013H].2
HIRC1	EQU	[013H].3
PA0	EQU	[014H].0
PA1	EQU	[014H].1
PA2	EQU	[014H].2
PA3	EQU	[014H].3
PA4	EQU	[014H].4
PA5	EQU	[014H].5
PA6	EQU	[014H].6
PA7	EQU	[014H].7
PAC0	EQU	[015H].0
PAC1	EQU	[015H].1
PAC2	EQU	[015H].2
PAC3	EQU	[015H].3
PAC4	EQU	[015H].4
PAC5	EQU	[015H].5
PAC6	EQU	[015H].6
PAC7	EQU	[015H].7
PAPU0	EQU	[016H].0
PAPU1	EQU	[016H].1
PAPU2	EQU	[016H].2
PAPU3	EQU	[016H].3
PAPU4	EQU	[016H].4
PAPU5	EQU	[016H].5
PAPU6	EQU	[016H].6
PAPU7	EQU	[016H].7
PAWU0	EQU	[017H].0
PAWU1	EQU	[017H].1
PAWU2	EQU	[017H].2
PAWU3	EQU	[017H].3
PAWU4	EQU	[017H].4
PAWU5	EQU	[017H].5
PAWU6	EQU	[017H].6
PAWU7	EQU	[017H].7
PB0	EQU	[018H].0
PB1	EQU	[018H].1
PB2	EQU	[018H].2
PB3	EQU	[018H].3
PB4	EQU	[018H].4
PBC0	EQU	[019H].0
PBC1	EQU	[019H].1
PBC2	EQU	[019H].2
PBC3	EQU	[019H].3
PBC4	EQU	[019H].4
PBPU0	EQU	[01AH].0
PBPU1	EQU	[01AH].1
PBPU2	EQU	[01AH].2
PBPU3	EQU	[01AH].3
PBPU4	EQU	[01AH].4
PC0	EQU	[01BH].0
PC1	EQU	[01BH].1
PC2	EQU	[01BH].2
PC3	EQU	[01BH].3
PC4	EQU	[01BH].4
PCC0	EQU	[01CH].0
PCC1	EQU	[01CH].1
PCC2	EQU	[01CH].2
PCC3	EQU	[01CH].3
PCC4	EQU	[01CH].4
PCPU0	EQU	[01DH].0
PCPU1	EQU	[01DH].1
PCPU2	EQU	[01DH].2
PCPU3	EQU	[01DH].3
PCPU4	EQU	[01DH].4
SLEDC00	EQU	[01EH].0
SLEDC01	EQU	[01EH].1
SLEDC02	EQU	[01EH].2
SLEDC03	EQU	[01EH].3
SLEDC04	EQU	[01EH].4
SLEDC05	EQU	[01EH].5
SLEDC06	EQU	[01EH].6
SLEDC07	EQU	[01EH].7
SLEDC10	EQU	[01FH].0
SLEDC11	EQU	[01FH].1
SLEDC12	EQU	[01FH].2
SLEDC13	EQU	[01FH].3
CLKSEL0	EQU	[020H].0
CLKSEL1	EQU	[020H].1
VLVD0	EQU	[021H].0
VLVD1	EQU	[021H].1
VLVD2	EQU	[021H].2
VBGEN	EQU	[021H].3
LVDEN	EQU	[021H].4
LVDO	EQU	[021H].5
ORMC0	EQU	[023H].0
ORMC1	EQU	[023H].1
ORMC2	EQU	[023H].2
ORMC3	EQU	[023H].3
ORMC4	EQU	[023H].4
ORMC5	EQU	[023H].5
ORMC6	EQU	[023H].6
ORMC7	EQU	[023H].7
CT0RP0	EQU	[027H].0
CT0RP1	EQU	[027H].1
CT0RP2	EQU	[027H].2
CT0ON	EQU	[027H].3
CT0CK0	EQU	[027H].4
CT0CK1	EQU	[027H].5
CT0CK2	EQU	[027H].6
CT0PAU	EQU	[027H].7
CT0CCLR	EQU	[028H].0
CT0DPX	EQU	[028H].1
CT0POL	EQU	[028H].2
CT0OC	EQU	[028H].3
CT0IO0	EQU	[028H].4
CT0IO1	EQU	[028H].5
CT0M0	EQU	[028H].6
CT0M1	EQU	[028H].7
SACS0	EQU	[02FH].0
SACS1	EQU	[02FH].1
SACS2	EQU	[02FH].2
SACS3	EQU	[02FH].3
ADRFS	EQU	[02FH].4
ADCEN	EQU	[02FH].5
ADBZ	EQU	[02FH].6
START	EQU	[02FH].7
SACKS0	EQU	[030H].0
SACKS1	EQU	[030H].1
SACKS2	EQU	[030H].2
SAVRS0	EQU	[030H].3
SAVRS1	EQU	[030H].4
SAINS0	EQU	[030H].5
SAINS1	EQU	[030H].6
SAINS2	EQU	[030H].7
PLTS0	EQU	[031H].0
PLTS1	EQU	[031H].1
PLTS2	EQU	[031H].2
PLTDAC0EN	EQU	[032H].0
PLTDAC1EN	EQU	[032H].1
PLTDAC2EN	EQU	[032H].2
PLTC0IS0	EQU	[036H].0
PLTC0IS1	EQU	[036H].1
PLTC0DEB0	EQU	[036H].2
PLTC0DEB1	EQU	[036H].3
PLTC0O	EQU	[036H].5
PLTC0EN	EQU	[036H].6
PLTC0OUT	EQU	[036H].7
PLTC0OF0	EQU	[037H].0
PLTC0OF1	EQU	[037H].1
PLTC0OF2	EQU	[037H].2
PLTC0OF3	EQU	[037H].3
PLTC0OF4	EQU	[037H].4
PLTC0RSP	EQU	[037H].5
PLTC0OFM	EQU	[037H].6
PLTC1IS0	EQU	[038H].0
PLTC1IS1	EQU	[038H].1
PLTC1DEB0	EQU	[038H].2
PLTC1DEB1	EQU	[038H].3
PLTC1O	EQU	[038H].5
PLTC1EN	EQU	[038H].6
PLTC1OUT	EQU	[038H].7
PLTC1OF0	EQU	[039H].0
PLTC1OF1	EQU	[039H].1
PLTC1OF2	EQU	[039H].2
PLTC1OF3	EQU	[039H].3
PLTC1OF4	EQU	[039H].4
PLTC1RSP	EQU	[039H].5
PLTC1OFM	EQU	[039H].6
PLTC0HYS0	EQU	[03AH].0
PLTC0HYS1	EQU	[03AH].1
PLTC1HYS0	EQU	[03AH].2
PLTC1HYS1	EQU	[03AH].3
PLTC0POL	EQU	[03AH].4
PLTC1POL	EQU	[03AH].5
PLTCXOSW	EQU	[03AH].6
PLTABW	EQU	[03BH].0
PLTAO	EQU	[03BH].5
PLTAEN	EQU	[03BH].6
PLTAOF0	EQU	[03CH].0
PLTAOF1	EQU	[03CH].1
PLTAOF2	EQU	[03CH].2
PLTAOF3	EQU	[03CH].3
PLTAOF4	EQU	[03CH].4
PLTAOF5	EQU	[03CH].5
PLTARSP	EQU	[03CH].6
PLTAOFM	EQU	[03CH].7
WS0	EQU	[03DH].0
WS1	EQU	[03DH].1
WS2	EQU	[03DH].2
WE0	EQU	[03DH].3
WE1	EQU	[03DH].4
WE2	EQU	[03DH].5
WE3	EQU	[03DH].6
WE4	EQU	[03DH].7
EEA0	EQU	[03EH].0
EEA1	EQU	[03EH].1
EEA2	EQU	[03EH].2
EEA3	EQU	[03EH].3
EEA4	EQU	[03EH].4
EEA5	EQU	[03EH].5
EED0	EQU	[03FH].0
EED1	EQU	[03FH].1
EED2	EQU	[03FH].2
EED3	EQU	[03FH].3
EED4	EQU	[03FH].4
EED5	EQU	[03FH].5
EED6	EQU	[03FH].6
EED7	EQU	[03FH].7
TXIF	EQU	[041H].0
TIDLE	EQU	[041H].1
RXIF	EQU	[041H].2
RIDLE	EQU	[041H].3
OERR	EQU	[041H].4
FERR	EQU	[041H].5
NF	EQU	[041H].6
PERR	EQU	[041H].7
TX8	EQU	[042H].0
RX8	EQU	[042H].1
TXBRK	EQU	[042H].2
STOPS	EQU	[042H].3
PRT	EQU	[042H].4
PREN	EQU	[042H].5
BNO	EQU	[042H].6
UARTEN	EQU	[042H].7
TEIE	EQU	[043H].0
TIIE	EQU	[043H].1
RIE	EQU	[043H].2
WAKE	EQU	[043H].3
ADDEN	EQU	[043H].4
BRGH	EQU	[043H].5
RXEN	EQU	[043H].6
TXEN	EQU	[043H].7
SWM	EQU	[044H].0
TXRX0	EQU	[045H].0
TXRX1	EQU	[045H].1
TXRX2	EQU	[045H].2
TXRX3	EQU	[045H].3
TXRX4	EQU	[045H].4
TXRX5	EQU	[045H].5
TXRX6	EQU	[045H].6
TXRX7	EQU	[045H].7
BRG0	EQU	[046H].0
BRG1	EQU	[046H].1
BRG2	EQU	[046H].2
BRG3	EQU	[046H].3
BRG4	EQU	[046H].4
BRG5	EQU	[046H].5
BRG6	EQU	[046H].6
BRG7	EQU	[046H].7
INT0S0	EQU	[047H].0
INT0S1	EQU	[047H].1
INT1S0	EQU	[047H].2
INT1S1	EQU	[047H].3
EMI	EQU	[048H].0
PLTC0E	EQU	[048H].1
INT0E	EQU	[048H].2
INT1E	EQU	[048H].3
PLTC0F	EQU	[048H].4
INT0F	EQU	[048H].5
INT1F	EQU	[048H].6
URE	EQU	[049H].0
LVE	EQU	[049H].1
MF0E	EQU	[049H].2
MF1E	EQU	[049H].3
URF	EQU	[049H].4
LVF	EQU	[049H].5
MF0F	EQU	[049H].6
MF1F	EQU	[049H].7
MF2E	EQU	[04AH].0
MF3E	EQU	[04AH].1
MF4E	EQU	[04AH].2
TB0E	EQU	[04AH].3
MF2F	EQU	[04AH].4
MF3F	EQU	[04AH].5
MF4F	EQU	[04AH].6
TB0F	EQU	[04AH].7
TB1E	EQU	[04BH].0
PLTC1E	EQU	[04BH].1
TB1F	EQU	[04BH].4
PLTC1F	EQU	[04BH].5
ADE	EQU	[04CH].0
DEE	EQU	[04CH].1
ADF	EQU	[04CH].4
DEF	EQU	[04CH].5
PTM0PE	EQU	[04DH].0
PTM0AE	EQU	[04DH].1
PTM0PF	EQU	[04DH].4
PTM0AF	EQU	[04DH].5
PTM1PE	EQU	[04EH].0
PTM1AE	EQU	[04EH].1
PTM1PF	EQU	[04EH].4
PTM1AF	EQU	[04EH].5
CTM0PE	EQU	[04FH].0
CTM0AE	EQU	[04FH].1
CTM0PF	EQU	[04FH].4
CTM0AF	EQU	[04FH].5
CTM1PE	EQU	[050H].0
CTM1AE	EQU	[050H].1
CTM1PF	EQU	[050H].4
CTM1AF	EQU	[050H].5
PT0ON	EQU	[052H].3
PT0CK0	EQU	[052H].4
PT0CK1	EQU	[052H].5
PT0CK2	EQU	[052H].6
PT0PAU	EQU	[052H].7
PT0CCLR	EQU	[053H].0
PT0CAPTS	EQU	[053H].1
PT0POL	EQU	[053H].2
PT0OC	EQU	[053H].3
PT0IO0	EQU	[053H].4
PT0IO1	EQU	[053H].5
PT0M0	EQU	[053H].6
PT0M1	EQU	[053H].7
PT0VLF	EQU	[054H].0
PT0TCLR0	EQU	[054H].1
PT0TCLR1	EQU	[054H].2
PT1ON	EQU	[05DH].3
PT1CK0	EQU	[05DH].4
PT1CK1	EQU	[05DH].5
PT1CK2	EQU	[05DH].6
PT1PAU	EQU	[05DH].7
PT1CCLR	EQU	[05EH].0
PT1CAPTS	EQU	[05EH].1
PT1POL	EQU	[05EH].2
PT1OC	EQU	[05EH].3
PT1IO0	EQU	[05EH].4
PT1IO1	EQU	[05EH].5
PT1M0	EQU	[05EH].6
PT1M1	EQU	[05EH].7
CT1RP0	EQU	[065H].0
CT1RP1	EQU	[065H].1
CT1RP2	EQU	[065H].2
CT1ON	EQU	[065H].3
CT1CK0	EQU	[065H].4
CT1CK1	EQU	[065H].5
CT1CK2	EQU	[065H].6
CT1PAU	EQU	[065H].7
CT1CCLR	EQU	[066H].0
CT1DPX	EQU	[066H].1
CT1POL	EQU	[066H].2
CT1OC	EQU	[066H].3
CT1IO0	EQU	[066H].4
CT1IO1	EQU	[066H].5
CT1M0	EQU	[066H].6
CT1M1	EQU	[066H].7
PAS00	EQU	[06BH].0
PAS01	EQU	[06BH].1
PAS02	EQU	[06BH].2
PAS03	EQU	[06BH].3
PAS04	EQU	[06BH].4
PAS05	EQU	[06BH].5
PAS06	EQU	[06BH].6
PAS07	EQU	[06BH].7
PAS10	EQU	[06CH].0
PAS11	EQU	[06CH].1
PAS12	EQU	[06CH].2
PAS13	EQU	[06CH].3
PAS14	EQU	[06CH].4
PAS15	EQU	[06CH].5
PAS16	EQU	[06CH].6
PAS17	EQU	[06CH].7
PBS00	EQU	[06DH].0
PBS01	EQU	[06DH].1
PBS02	EQU	[06DH].2
PBS03	EQU	[06DH].3
PBS04	EQU	[06DH].4
PBS05	EQU	[06DH].5
PBS06	EQU	[06DH].6
PBS07	EQU	[06DH].7
PCS00	EQU	[06EH].0
PCS01	EQU	[06EH].1
PCS02	EQU	[06EH].2
PCS03	EQU	[06EH].3
PCS04	EQU	[06EH].4
PCS05	EQU	[06EH].5
PCS06	EQU	[06EH].6
PCS07	EQU	[06EH].7
IFS0	EQU	[070H].0
IFS1	EQU	[070H].1
IFS2	EQU	[070H].2
IFS3	EQU	[070H].3
RD	EQU	[0140H].0
RDEN	EQU	[0140H].1
WR	EQU	[0140H].2
WREN	EQU	[0140H].3

LASTPAGE	EQU	0F00H