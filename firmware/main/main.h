




//*****************************************************************************
// Defines
//*****************************************************************************
//--- Application info ---
#define AppCurrentVersion              "0.03"   // Alpha
//--- Command id's ---
#define CommandID_SetChannelName       0x01
#define CommandID_ReadChannelName      0x02
#define CommandID_GetChannelStatus     0x03
#define CommandID_SetChannelStatus     0x04

#define CommandID_StandBy              0xF9
#define CommandID_SendLcdContent       0xFA
#define CommandID_GetVersion           0xFB
#define CommandID_KeyStroke            0xFC
#define CommandID_Ping                 0xFD
#define CommandID_ResetCPU             0xFE
#define CommandID_UpdateFirmware       0xFF
//--- Error codes ---
#define ErrorID_BufferFull             0xF0
#define ErrorID_InvalidCommand         0xF1
#define ErrorID_InvalidParameter       0xF2
#define ErrorID_MissingParameter       0xF3
#define ErrorID_KeypadBufferFull       0xF4
#define ErrorID_Bootloader_BadCRC      0xFA
#define ErrorID_Bootloader_BadWRITE    0xFB
//--- Command reply codes ---
#define CodeID_Acknowledge             0xA1
#define CodeID_Executed                0xA2
#define CodeID_Bootloader_Retry        0xAF
//--- Pin definition ---
#define Pin_ClearToSend                PIN_C5
#define Pin_RequestToSend              PIN_D3
#define Pin_SegDisplay_Latch           PIN_E0
#define Pin_SegDisplay_Clock           PIN_E1
#define Pin_SegDisplay_Data            PIN_E2
#define Pin_KeypadRow4                 PIN_D7
#define Pin_KeypadRow3                 PIN_D6
#define Pin_KeypadRow2                 PIN_D5
#define Pin_KeypadRow1                 PIN_D4
#define Pin_KeypadCol1                 PIN_B0
#define Pin_KeypadCol2                 PIN_B1
#define Pin_KeypadCol3                 PIN_B2
#define Pin_KeypadCol4                 PIN_B3
//-- Pin definition ---
#define Keypad_Col1                    PIN_B0
#define Keypad_Col2                    PIN_B1
#define Keypad_Col3                    PIN_B2
#define Keypad_Col4                    PIN_B3
#define Keypad_Row1                    PIN_D4
#define Keypad_Row2                    PIN_D5
#define Keypad_Row3                    PIN_D6
#define Keypad_Row4                    PIN_D7
#define Diag_Led                       PIN_B4
//--- Keypad button id's ---
#define KeyCode_AV                     0x01
#define KeyCode_Num7                   0x02
#define KeyCode_Num8                   0x03
#define KeyCode_Num9                   0x04
#define KeyCode_Clear                  0x05
#define KeyCode_Num4                   0x06
#define KeyCode_Num5                   0x07
#define KeyCode_Num6                   0x08
#define KeyCode_Num1                   0x0A
#define KeyCode_Num2                   0x0B
#define KeyCode_Num3                   0x0C
#define KeyCode_Preset                 0x0D
#define KeyCode_Dest                   0x0E
#define KeyCode_Set                    0x0F
#define KeyCode_Source                 0x10
//--- Keypad characters ---
#define Char_Num1                      0x01
#define Char_Num2                      0x02
#define Char_Num3                      0x03
#define Char_Num4                      0x04
#define Char_Num5                      0x05
#define Char_Num6                      0x06
#define Char_Num7                      0x07
#define Char_Num8                      0x08
#define Char_Num9                      0x09
#define Char_StandBy                   0xF0
#define Char_Clear                     0xF1
#define Char_Source                    0xF2
#define Char_AV                        0xF3
#define Char_Preset                    0xF4
#define Char_Dest                      0xF5
#define Char_Set                       0xF6
//--- Led display characters ---
#define LedDisplay_Num0                0
#define LedDisplay_Num1                1
#define LedDisplay_Num2                2
#define LedDisplay_Num3                3
#define LedDisplay_Num4                4
#define LedDisplay_Num5                5
#define LedDisplay_Num6                6
#define LedDisplay_Num7                7
#define LedDisplay_Num8                8
#define LedDisplay_Num9                9
#define LedDisplay_Error               10
#define LedDisplay_Minus               11
#define LedDisplay_SegmentAB           12
#define LedDisplay_SegmentBC           13
#define LedDisplay_SegmentCD           14
#define LedDisplay_SegmentDE           15
#define LedDisplay_SegmentEF           16
#define LedDisplay_SegmentFA           17
#define LedDisplay_OFF                 18
//--- Error message id ---
#define MessageID_InvalidDestination   1
#define MessageID_InvalidSource        2
#define MessageID_InvalidSelection     3
//--- EEPROM memory location ---
#define MEMLOC_CHANNELNAMES            0
#define MEMLOC_PRESET                  75

#define I2CADDRESS_MAX6956_WRITE       0b10001000
#define I2CADDRESS_MAX6956_READ        0b10001001
#define MAX6956_COMMAND_PORT19         0x33
#define MAX6956_COMMAND_PORT20         0x34
#define MAX6956_COMMAND_PORT12_19      0x4C
#define MAX6956_COMMAND_PORT22_29      0x56


//--- Channel selection mode ---
#define ChannelSelect_Off              0
#define ChannelSelect_Destination      1
#define ChannelSelect_Both             2
#define ChannelSelect_Video            3
#define ChannelSelect_Audio            4




//*****************************************************************************
// Macros
//*****************************************************************************
#define ClearSegmentDisplay()            WriteToSegmentDisplay(LedDisplay_OFF);
#define ClearLcdDisplay()                Lcd_putc_ex("\f");
#define TurnOffLcdBackLight()            WriteToLedDriver(0x34, 0x00);
#define TurnOnLcdBackLight()            WriteToLedDriver(0x34, 0x01);



//*****************************************************************************
// Function prototypes
//*****************************************************************************
void ProcessCommandBuffer(void);
void PrepareFirmwareUpdate(void);
int WaitForKeypadChar(void);
void WriteToSegmentDisplay(int *iChar);
void SendReplyBlock(void);
void putc_ex(char iChar);
void ShowChannelStatus(void);
void LoadConfiguration(void);
void LoadPreset(int PresetID);
void ChangeChannelMenu(void);
void LoadChannelName(int ChannelID);
void SaveChannelNames();
void ShowErrorMessage(int MessageID);
void WriteToLedDriver(int iCommand, int iParameter);
void ActivateStandbyMode(void);
CHAR keypadread(); 
CHAR scankeypad();

