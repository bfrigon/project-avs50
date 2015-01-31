#nolist

//----- Register Files -----------------------------------------------------
#BYTE TOSU           = 0x0FFF
#BYTE TOSH           = 0x0FFE
#BYTE TOSL           = 0x0FFD
#BYTE STKPTR         = 0x0FFC
#BYTE PCLATU         = 0x0FFB
#BYTE PCLATH         = 0x0FFA
#BYTE PCL            = 0x0FF9
#BYTE TBLPTRU        = 0x0FF8
#BYTE TBLPTRH        = 0x0FF7
#BYTE TBLPTRL        = 0x0FF6
#BYTE TABLAT         = 0x0FF5
#BYTE PRODH          = 0x0FF4
#BYTE PRODL          = 0x0FF3

#BYTE INTCON         = 0x0FF2
#BYTE INTCON1        = 0x0FF2
#BYTE INTCON2        = 0x0FF1
#BYTE INTCON3        = 0x0FF0

#BYTE INDF0          = 0x0FEF
#BYTE POSTINC0       = 0x0FEE
#BYTE POSTDEC0       = 0x0FED
#BYTE PREINC0        = 0x0FEC
#BYTE PLUSW0         = 0x0FEB
#BYTE FSR0H          = 0x0FEA
#BYTE FSR0L          = 0x0FE9
#BYTE WREG           = 0x0FE8

#BYTE INDF1          = 0x0FE7
#BYTE POSTINC1       = 0x0FE6
#BYTE POSTDEC1       = 0x0FE5
#BYTE PREINC1        = 0x0FE4
#BYTE PLUSW1         = 0x0FE3
#BYTE FSR1H          = 0x0FE2
#BYTE FSR1L          = 0x0FE1
#BYTE BSR            = 0x0FE0

#BYTE INDF2          = 0x0FDF
#BYTE POSTINC2       = 0x0FDE
#BYTE POSTDEC2       = 0x0FDD
#BYTE PREINC2        = 0x0FDC
#BYTE PLUSW2         = 0x0FDB
#BYTE FSR2H          = 0x0FDA
#BYTE FSR2L          = 0x0FD9
#BYTE STATUS         = 0x0FD8

#BYTE TMR0H          = 0x0FD7
#BYTE TMR0L          = 0x0FD6
#BYTE T0CON          = 0x0FD5

#BYTE OSCCON         = 0x0FD3
#BYTE LVDCON         = 0x0FD2
#BYTE WDTCON         = 0x0FD1
#BYTE RCON           = 0x0FD0

#BYTE TMR1H          = 0x0FCF
#BYTE TMR1L          = 0x0FCE
#BYTE T1CON          = 0x0FCD
#BYTE TMR2           = 0x0FCC
#BYTE PR2            = 0x0FCB
#BYTE T2CON          = 0x0FCA

#BYTE SSPBUF         = 0x0FC9
#BYTE SSPADD         = 0x0FC8
#BYTE SSPSTAT        = 0x0FC7
#BYTE SSPCON1        = 0x0FC6
#BYTE SSPCON2        = 0x0FC5

#BYTE ADRESH         = 0x0FC4
#BYTE ADRESL         = 0x0FC3
#BYTE ADCON0         = 0x0FC2
#BYTE ADCON1         = 0x0FC1

#BYTE CCPR1H         = 0x0FBF
#BYTE CCPR1L         = 0x0FBE
#BYTE CCP1CON        = 0x0FBD
#BYTE CCPR2H         = 0x0FBC
#BYTE CCPR2L         = 0x0FBB
#BYTE CCP2CON        = 0x0FBA
#BYTE TMR3H          = 0x0FB3
#BYTE TMR3L          = 0x0FB2
#BYTE T3CON          = 0x0FB1
#BYTE SPBRG          = 0x0FAF
#BYTE RCREG          = 0x0FAE
#BYTE TXREG          = 0x0FAD
#BYTE TXSTA          = 0x0FAC
#BYTE RCSTA          = 0x0FAB
#BYTE EEADR          = 0x0FA9
#BYTE EEDATA         = 0x0FA8
#BYTE EECON2         = 0x0FA7
#BYTE EECON1         = 0x0FA6
#BYTE IPR2           = 0x0FA2
#BYTE PIR2           = 0x0FA1
#BYTE PIE2           = 0x0FA0
#BYTE IPR1           = 0x0F9F
#BYTE PIR1           = 0x0F9E
#BYTE PIE1           = 0x0F9D
#BYTE TRISE          = 0x0F96
#BYTE TRISD          = 0x0F95
#BYTE TRISC          = 0x0F94
#BYTE TRISB          = 0x0F93
#BYTE TRISA          = 0x0F92
#BYTE LATE           = 0x0F8D
#BYTE LATD           = 0x0F8C
#BYTE LATC           = 0x0F8B
#BYTE LATB           = 0x0F8A
#BYTE LATA           = 0x0F89
#BYTE PORTE          = 0x0F84
#BYTE PORTD          = 0x0F83
#BYTE PORTC          = 0x0F82
#BYTE PORTB          = 0x0F81
#BYTE PORTA          = 0x0F80

//----- STKPTR Bits --------------------------------------------------------
#BIT STKFUL          = STKPTR.7
#BIT STKUNF          = STKPTR.6

//----- INTCON Bits --------------------------------------------------------
#BIT GIE             = INTCON.7
#BIT GIEH            = INTCON.7
#BIT PEIE            = INTCON.6
#BIT GIEL            = INTCON.6
#BIT TMR0IE          = INTCON.5
#BIT T0IE            = INTCON.5      // For backward compatibility
#BIT INT0IE          = INTCON.4
#BIT INT0E           = INTCON.4      // For backward compatibility
#BIT RBIE            = INTCON.3
#BIT TMR0IF          = INTCON.2
#BIT T0IF            = INTCON.2      // For backward compatibility
#BIT INT0IF          = INTCON.1
#BIT INT0F           = INTCON.1      // For backward compatibility
#BIT RBIF            = INTCON.0

//----- INTCON2 Bits --------------------------------------------------------
#BIT NOT_RBPU        = INTCON2.7
#BIT RBPU            = INTCON2.7
#BIT INTEDG0         = INTCON2.6
#BIT INTEDG1         = INTCON2.5
#BIT INTEDG2         = INTCON2.4
#BIT TMR0IP          = INTCON2.2
#BIT T0IP            = INTCON2.2      // For compatibility with T0IE and T0IF
#BIT RBIP            = INTCON2.0

//----- INTCON3 Bits --------------------------------------------------------
#BIT INT2IP          = INTCON3.7
#BIT INT1IP          = INTCON3.6
#BIT INT2IE          = INTCON3.4
#BIT INT1IE          = INTCON3.3
#BIT INT2IF          = INTCON3.1
#BIT INT1IF          = INTCON3.0

//----- STATUS Bits --------------------------------------------------------
#BIT N               = STATUS.4
#BIT OV              = STATUS.3
#BIT Z               = STATUS.2
#BIT DC              = STATUS.1
#BIT C               = STATUS.0

//----- T0CON Bits ---------------------------------------------------------
#BIT TMR0ON          = T0CON.7
#BIT T08BIT          = T0CON.6
#BIT T0CS            = T0CON.5
#BIT T0SE            = T0CON.4
#BIT PSA             = T0CON.3
#BIT T0PS2           = T0CON.2
#BIT T0PS1           = T0CON.1
#BIT T0PS0           = T0CON.0

//----- OSCCON Bits ---------------------------------------------------------
#BIT SCS             = OSCCON.0

//----- LVDCON Bits ---------------------------------------------------------
#BIT IRVST           = LVDCON.5
#BIT LVDEN           = LVDCON.4
#BIT LVDL3           = LVDCON.3
#BIT LVDL2           = LVDCON.2
#BIT LVDL1           = LVDCON.1
#BIT LVDL0           = LVDCON.0

//----- WDTCON Bits ---------------------------------------------------------
#BIT SWDTE           = WDTCON.0
#BIT SWDTEN          = WDTCON.0

//----- RCON Bits -----------------------------------------------------------
#BIT IPEN            = RCON.7
#BIT NOT_RI          = RCON.4
#BIT RI              = RCON.4
#BIT NOT_TO          = RCON.3
#BIT TO              = RCON.3
#BIT NOT_PD          = RCON.2
#BIT PD              = RCON.2
#BIT NOT_POR         = RCON.1
#BIT POR             = RCON.1
#BIT NOT_BOR         = RCON.0
#BIT BOR             = RCON.0

//----- T1CON Bits ---------------------------------------------------------
#BIT RD16            = T1CON.7
#BIT T1CKPS1         = T1CON.5
#BIT T1CKPS0         = T1CON.4
#BIT T1OSCEN         = T1CON.3
#BIT NOT_T1SYNC      = T1CON.2
#BIT T1SYNC          = T1CON.2
#BIT T1INSYNC        = T1CON.2      // For backward compatibility
#BIT TMR1CS          = T1CON.1
#BIT TMR1ON          = T1CON.0

//----- T2CON Bits ---------------------------------------------------------
#BIT TOUTPS3         = T2CON.6
#BIT TOUTPS2         = T2CON.5
#BIT TOUTPS1         = T2CON.4
#BIT TOUTPS0         = T2CON.3
#BIT TMR2ON          = T2CON.2
#BIT T2CKPS1         = T2CON.1
#BIT T2CKPS0         = T2CON.0

//----- SSPSTAT Bits -------------------------------------------------------
#BIT SMP             = SSPSTAT.7
#BIT CKE             = SSPSTAT.6
#BIT D               = SSPSTAT.5
#BIT I2C_DAT         = SSPSTAT.5
#BIT NOT_A           = SSPSTAT.5
#BIT NOT_ADDRESS     = SSPSTAT.5
#BIT D_A             = SSPSTAT.5
#BIT DATA_ADDRESS    = SSPSTAT.5
#BIT P               = SSPSTAT.4
#BIT I2C_STOP        = SSPSTAT.4
#BIT S               = SSPSTAT.3
#BIT I2C_START       = SSPSTAT.3
#BIT R               = SSPSTAT.2
#BIT I2C_READ        = SSPSTAT.2
#BIT NOT_W           = SSPSTAT.2
#BIT NOT_WRITE       = SSPSTAT.2
#BIT R_W             = SSPSTAT.2
#BIT READ_WRITE      = SSPSTAT.2
#BIT UA              = SSPSTAT.1
#BIT BF              = SSPSTAT.0

//----- SSPCON1 Bits --------------------------------------------------------
#BIT WCOL            = SSPCON1.7
#BIT SSPOV           = SSPCON1.6
#BIT SSPEN           = SSPCON1.5
#BIT CKP             = SSPCON1.4
#BIT SSPM3           = SSPCON1.3
#BIT SSPM2           = SSPCON1.2
#BIT SSPM1           = SSPCON1.1
#BIT SSPM0           = SSPCON1.0

//----- SSPCON2 Bits --------------------------------------------------------
#BIT GCEN            = SSPCON2.7
#BIT ACKSTAT         = SSPCON2.6
#BIT ACKDT           = SSPCON2.5
#BIT ACKEN           = SSPCON2.4
#BIT RCEN            = SSPCON2.3
#BIT PEN             = SSPCON2.2
#BIT RSEN            = SSPCON2.1
#BIT SEN             = SSPCON2.0

//----- ADCON0 Bits --------------------------------------------------------
#BIT ADCS1           = ADCON0.7
#BIT ADCS0           = ADCON0.6
#BIT CHS2            = ADCON0.5
#BIT CHS1            = ADCON0.4
#BIT CHS0            = ADCON0.3
#BIT GO              = ADCON0.2
#BIT NOT_DONE        = ADCON0.2
#BIT DONE            = ADCON0.2
#BIT GO_DONE         = ADCON0.2
#BIT ADON            = ADCON0.0

//----- ADCON1 Bits --------------------------------------------------------
#BIT ADFM            = ADCON1.7
#BIT ADCS2           = ADCON1.6
#BIT PCFG3           = ADCON1.3
#BIT PCFG2           = ADCON1.2
#BIT PCFG1           = ADCON1.1
#BIT PCFG0           = ADCON1.0

//----- CCP1CON Bits -------------------------------------------------------
#BIT DC1B1           = CCP1CON.5
#BIT CCP1X           = CCP1CON.5      // For backward compatibility
#BIT DC1B0           = CCP1CON.4
#BIT CCP1Y           = CCP1CON.4      // For backward compatibility
#BIT CCP1M3          = CCP1CON.3
#BIT CCP1M2          = CCP1CON.2
#BIT CCP1M1          = CCP1CON.1
#BIT CCP1M0          = CCP1CON.0

//----- CCP2CON Bits -------------------------------------------------------
#BIT DC2B1           = CCP2CON.5
#BIT CCP2X           = CCP2CON.5      // For backward compatibility
#BIT DC2B0           = CCP2CON.4
#BIT CCP2Y           = CCP2CON.4      // For backward compatibility
#BIT CCP2M3          = CCP2CON.3
#BIT CCP2M2          = CCP2CON.2
#BIT CCP2M1          = CCP2CON.1
#BIT CCP2M0          = CCP2CON.0

//----- T3CON Bits ---------------------------------------------------------
#BIT T3CCP2          = T3CON.6
#BIT T3CKPS1         = T3CON.5
#BIT T3CKPS0         = T3CON.4
#BIT T3CCP1          = T3CON.3
#BIT NOT_T3SYNC      = T3CON.2
#BIT T3SYNC          = T3CON.2
#BIT T3INSYNC        = T3CON.2      // For backward compatibility
#BIT TMR3CS          = T3CON.1
#BIT TMR3ON          = T3CON.0

//----- TXSTA Bits ---------------------------------------------------------
#BIT CSRC            = TXSTA.7
#BIT TX9             = TXSTA.6
#BIT NOT_TX8         = TXSTA.6      // For backward compatibility
#BIT TX8_9           = TXSTA.6      // For backward compatibility
#BIT TXEN            = TXSTA.5
#BIT SYNC            = TXSTA.4
#BIT BRGH            = TXSTA.2
#BIT TRMT            = TXSTA.1
#BIT TX9D            = TXSTA.0
#BIT TXD8            = TXSTA.0      // For backward compatibility

//----- RCSTA Bits ---------------------------------------------------------
#BIT SPEN            = RCSTA.7
#BIT RX9             = RCSTA.6
#BIT RC9             = RCSTA.6      // For backward compatibility
#BIT NOT_RC8         = RCSTA.6      // For backward compatibility
#BIT RC8_9           = RCSTA.6      // For backward compatibility
#BIT SREN            = RCSTA.5
#BIT CREN            = RCSTA.4
#BIT ADDEN           = RCSTA.3
#BIT FERR            = RCSTA.2
#BIT OERR            = RCSTA.1
#BIT RX9D            = RCSTA.0
#BIT RCD8            = RCSTA.0      // For backward compatibility

//----- IPR2 Bits ----------------------------------------------------------
#BIT EEIP            = IPR2.4
#BIT BCLIP           = IPR2.3
#BIT LVDIP           = IPR2.2
#BIT TMR3IP          = IPR2.1
#BIT CCP2IP          = IPR2.0

//----- PIR2 Bits ----------------------------------------------------------
#BIT EEIF            = PIR2.4
#BIT BCLIF           = PIR2.3
#BIT LVDIF           = PIR2.2
#BIT TMR3IF          = PIR2.1
#BIT CCP2IF          = PIR2.0

//----- PIE2 Bits ----------------------------------------------------------
#BIT EEIE            = PIE2.4
#BIT BCLIE           = PIE2.3
#BIT LVDIE           = PIE2.2
#BIT TMR3IE          = PIE2.1
#BIT CCP2IE          = PIE2.0

//----- IPR1 Bits ----------------------------------------------------------
#BIT PSPIP           = IPR1.7
#BIT ADIP            = IPR1.6
#BIT RCIP            = IPR1.5
#BIT TXIP            = IPR1.4
#BIT SSPIP           = IPR1.3
#BIT CCP1IP          = IPR1.2
#BIT TMR2IP          = IPR1.1
#BIT TMR1IP          = IPR1.0

//----- PIR1 Bits ----------------------------------------------------------
#BIT PSPIF           = PIR1.7
#BIT ADIF            = PIR1.6
#BIT RCIF            = PIR1.5
#BIT TXIF            = PIR1.4
#BIT SSPIF           = PIR1.3
#BIT CCP1IF          = PIR1.2
#BIT TMR2IF          = PIR1.1
#BIT TMR1IF          = PIR1.0

//----- PIE1 Bits ----------------------------------------------------------
#BIT PSPIE           = PIE1.7
#BIT ADIE            = PIE1.6
#BIT RCIE            = PIE1.5
#BIT TXIE            = PIE1.4
#BIT SSPIE           = PIE1.3
#BIT CCP1IE          = PIE1.2
#BIT TMR2IE          = PIE1.1
#BIT TMR1IE          = PIE1.0

//----- TRISE Bits ---------------------------------------------------------
#BIT IBF             = TRISE.7
#BIT OBF             = TRISE.6
#BIT IBOV            = TRISE.5
#BIT PSPMODE         = TRISE.4
#BIT TRISE2          = TRISE.2
#BIT TRISE1          = TRISE.1
#BIT TRISE0          = TRISE.0

//----- EECON1 Bits ---------------------------------------------------------
#BIT EEPGD		      = EECON1.7
#BIT CFGS		      = EECON1.6
#BIT FREE		      = EECON1.4
#BIT WRERR		      = EECON1.3
#BIT WREN		      = EECON1.2
#BIT WR		         = EECON1.1
#BIT RD		         = EECON1.0

#list
