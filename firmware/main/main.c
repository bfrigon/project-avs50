//***************************************************************
// Project	 : AVS-50 8 ch. A/V Routing Switcher
// App.			 : Main MCU firmware
// Ver.			 : 0.05 alpha
// Author		 : Benoit Frigon
// Last upd. : 01-sept-2004
//***************************************************************


//*****************************************************************************
// Includes
//*****************************************************************************
#include <18F452.h>
#include "18F452_REG.h"


//*****************************************************************************
// Fuses
//*****************************************************************************
#fuses NOWDT,HS, NOPROTECT, NOOSCSEN, BROWNOUT, BORV20, NOPUT, STVREN, NODEBUG, NOLVP, NOWRT, NOWRTD, NOWRTB, NOWRTC, NOCPD, NOCPB, NOEBTR, NOEBTRB

//*****************************************************************************
// Config
//*****************************************************************************
#define McuClock                          22118400
#define DefaultUartSpeed                  19200


//*****************************************************************************
// Defines
//*****************************************************************************
//--- Application info ---
#define AppCurrentVersion                 "0.05"   // Alpha
//--- Application configuration ---
#define KeyScanTime_BeforeShift           255
#define KeyScanTime_Debounce              30
#define KeypadBacklight_Normal            0x06     // Active key intensity
#define KeypadBacklight_Inactive          0x01     // Inactive key intensity
//--- Command id's ---
#define CommandID_SetChannelName          0x01
#define CommandID_ReadChannelName         0x02
#define CommandID_GetChannelStatus        0x03
#define CommandID_SetChannelStatus        0x04
#define CommandID_ReadLcdContent          0x05
#define CommandID_GetPhotoLevel           0x06
#define CommandID_WriteConfig             0x07
#define CommandID_ReadConfig              0x08
#define CommandID_SaveConfig              0x09
#define CommandID_LoadConfig              0x0A
#define CommandID_StandBy                 0xFA
#define CommandID_GetVersion              0xFB
#define CommandID_KeyStroke               0xFC
#define CommandID_Ping                    0xFD
#define CommandID_ResetCPU                0xFE
#define CommandID_UpdateFirmware          0xFF
//--- Config slot ID ---
#define ConfigID_StartupPreset            0x01
#define ConfigID_StartupMonitorChannel    0x02
#define ConfigID_UARTSpeed                0x03
#define ConfigID_AutoStandbyEnabled       0x04
#define ConfigID_AutoSleepEnabled         0x05
#define ConfigID_AutoKeypadLights         0x06
#define ConfigID_DimInactiveKeys          0x07
#define ConfigID_AutoMonitorSelect        0x08
#define ConfigID_Backlight_AutoThreshold  0x09
#define ConfigID_Backlight_AutoOffset     0x0A
#define ConfigID_AutoStandbyLevel         0x0B
#define ConfigID_TimeBeforeStandby        0x0C
#define ConfigID_TimeBeforeSleep          0x0D
#define ConfigID_InhibitedSource1         0x0E
#define ConfigID_InhibitedSource2         0x0F
#define ConfigID_InhibitedSource3         0x10
#define ConfigID_InhibitedSource4         0x11
#define ConfigID_InhibitedSource5         0x12
#define ConfigID_InhibitedSource6         0x13
#define ConfigID_InhibitedSource7         0x14
#define ConfigID_ChannelNameSource0       0x15
#define ConfigID_ChannelNameSource1       0x16
#define ConfigID_ChannelNameSource2       0x17
#define ConfigID_ChannelNameSource3       0x18
#define ConfigID_ChannelNameSource4       0x19
#define ConfigID_ChannelNameSource5       0x1A
#define ConfigID_ChannelNameSource6       0x1B
#define ConfigID_ChannelNameSource7       0x1C
#define ConfigID_ChannelNameDest1         0x1D
#define ConfigID_ChannelNameDest2         0x1E
#define ConfigID_ChannelNameDest3         0x1F
#define ConfigID_ChannelNameDest4         0x20
#define ConfigID_ChannelNameDest5         0x21
#define ConfigID_ChannelNameDest6         0x22
#define ConfigID_ChannelNameDest7         0x23
#define ConfigID_MAX                      0x23
//--- Event ID (20 to 7F) ---
#define EventID_Reboot                    0x20
#define EventID_ChannelChanged            0x21
#define EventID_OnStandby                 0x22
#define EventID_onExitStandby             0x23
//--- Command reply codes (80 to 9F) ---
#define CodeID_Acknowledge                0x80
#define CodeID_Executed                   0x81
//--- Error codes (A0 to FF) ---
#define ErrorID_BufferFull                0xA0
#define ErrorID_InvalidCommand            0xA1
#define ErrorID_InvalidParameter          0xA2
#define ErrorID_MissingParameter          0xA3
#define ErrorID_KeypadBufferFull          0xA4
#define ErrorID_Bootloader_BadCRC         0xAA
#define ErrorID_Bootloader_BadWRITE       0xAB
//--- Pin definition ---
#define Pin_PhotoResistor                 PIN_A0
#define Pin_VideoMatrix_Data              PIN_A1
#define Pin_VideoMatrix_Clock             PIN_A2
#define Pin_VideoMatrix_Latch             PIN_A3
#bit Pin_VideoGen_Enable                = PORTA.5
#define Pin_Keypad_Col1                   PIN_B0
#define Pin_Keypad_Col2                   PIN_B1
#define Pin_Keypad_Col3                   PIN_B2
#define Pin_Keypad_Col4                   PIN_B3
#define Pin_Keypad_Row1                   PIN_B4
#define Pin_Keypad_Row2                   PIN_B5
#define Pin_Keypad_Row3                   PIN_B6
#define Pin_Keypad_Row4                   PIN_B7
#define Pin_AudioMatrix_Reset             PIN_C0
#define Pin_AudioMatrix_Update            PIN_C1
#define Pin_Diag_Led                      PIN_C2
#define Pin_SCL                           PIN_C3
#define Pin_SDA                           PIN_C4
#define Pin_ClearToSend                   PIN_C5
#define Pin_Tx                            PIN_C6
#define Pin_Rx                            PIN_C7
#define Pin_LcdData                       PIN_D0
#define Pin_LcdClock                      PIN_D1
#define Pin_LcdEnable                     PIN_D2
#define Pin_RequestToSend                 PIN_D3
#define Pin_AudioMatrix_Data              PIN_D4
#define Pin_AudioMatrix_Clock             PIN_D5
#define Pin_UARTSpeedSelect1              PIN_D6
#define Pin_UARTSpeedSelect2              PIN_D7
#define Pin_SegDisplay_Latch              PIN_E0
#define Pin_SegDisplay_Clock              PIN_E1
#define Pin_SegDisplay_Data               PIN_E2
//--- Keypad button id's ---
#define KeyCode_AV                        0x01
#define KeyCode_Num7                      0x02
#define KeyCode_Num8                      0x03
#define KeyCode_Num9                      0x04
#define KeyCode_Clear                     0x05
#define KeyCode_Num4                      0x06
#define KeyCode_Num5                      0x07
#define KeyCode_Num6                      0x08
#define KeyCode_Standby                   0x09
#define KeyCode_Num1                      0x0A
#define KeyCode_Num2                      0x0B
#define KeyCode_Num3                      0x0C
#define KeyCode_Preset                    0x0D
#define KeyCode_Dest                      0x0E
#define KeyCode_Set                       0x0F
#define KeyCode_Source                    0x10
//--- Keypad characters ---
#define Char_Clear                        0x00
#define Char_Num1                         0x01
#define Char_Num2                         0x02
#define Char_Num3                         0x03
#define Char_Num4                         0x04
#define Char_Num5                         0x05
#define Char_Num6                         0x06
#define Char_Num7                         0x07
#define Char_Num8                         0x08
#define Char_Num9                         0x09
#define Char_StandBy                      0x0A
#define Char_Source                       0x0B
#define Char_AV                           0x0C
#define Char_Preset                       0x0D
#define Char_Dest                         0x0E
#define Char_Set                          0x0F
//--- Active key set ---
#define KeySet_Boot                       0b0000000000000000
#define KeySet_MainMenu                   0b1110110011111111
#define KeySet_ChangeChannelMenu          0b1101110011111111
#define KeySet_LoadPresetMenu             0b1000011111111111
#define KeySet_SavePresetMenuStep1        0b1000011111111111
#define KeySet_SavePresetMenuStep2        0b1000011011111111
#define KeySet_SavePresetMenuStep3        0b1000010000000001
#define KeySet_ConfigMenu                 0b1000010101010101
#define Keyset_ClearChannelsMenu          0b1000010000000001
//--- Led display characters ---
#define LedDisplay_Num0                   0
#define LedDisplay_Num1                   1
#define LedDisplay_Num2                   2
#define LedDisplay_Num3                   3
#define LedDisplay_Num4                   4
#define LedDisplay_Num5                   5
#define LedDisplay_Num6                   6
#define LedDisplay_Num7                   7
#define LedDisplay_Num8                   8
#define LedDisplay_Num9                   9
#define LedDisplay_Error                  10
#define LedDisplay_Minus                  11
#define LedDisplay_SegmentAB              12
#define LedDisplay_SegmentBC              13
#define LedDisplay_SegmentCD              14
#define LedDisplay_SegmentDE              15
#define LedDisplay_SegmentEF              16
#define LedDisplay_SegmentFA              17
#define LedDisplay_OFF                    18
//--- Error message id ---
#define MessageID_InvalidDestination      1
#define MessageID_InvalidSource           2
#define MessageID_InvalidSelection        3
//--- EEPROM memory location ---
#define MEMLOC_CHANNELNAMES               0
#define MEMLOC_PRESET                     75
#define MEMLOC_CONFIG_SLOT1               138
#define MEMLOC_CONFIG_SLOT2               139
#define MEMLOC_CONFIG_SLOT3               140
#define MEMLOC_CONFIG_SLOT4               141
#define MEMLOC_CONFIG_SLOT5               142
#define MEMLOC_CONFIG_SLOT6               143
#define MEMLOC_CONFIG_SLOT7               144
#define MEMLOC_CONFIG_SLOT8               145
#define MEMLOC_CONFIG_SLOT9               146
#define MEMLOC_CONFIG_SLOT10              147

#define I2CADDRESS_MAX6956_WRITE          0b10001000
#define I2CADDRESS_MAX6956_READ           0b10001001
#define I2CADDRESS_EEPROM_WRITE           0b10100000
#define I2CADDRESS_EEPROM_READ            0b10100001

#define MAX6956_COMMAND_PORT19            0x33
#define MAX6956_COMMAND_PORT20            0x34
#define MAX6956_COMMAND_PORT12_19         0x4C
#define MAX6956_COMMAND_PORT22_29         0x56
#define MAX6956_COMMAND_CURRENT           0x02
//--- Channel selection mode ---
#define ChannelSelect_Off                 0
#define ChannelSelect_Destination         1
#define ChannelSelect_Both                2
#define ChannelSelect_Video               3
#define ChannelSelect_Audio               4



//*****************************************************************************
// Macros
//*****************************************************************************
#define ClearSegmentDisplay()             WriteToSegmentDisplay(LedDisplay_OFF);
#define ClearLcdDisplay()                 Lcd_putc("\f");
#define TurnOffLcdBackLight()             WriteToLedDriver(0x34, 0x00);
#define TurnOnLcdBackLight()              WriteToLedDriver(0x34, 0x01);



//*****************************************************************************
// Function prototypes
//*****************************************************************************
void ProcessCommandBuffer(void);
void PrepareFirmwareUpdate(void);
short WaitForKeypadChar(void);
void WriteToSegmentDisplay(int *iChar);
void SendReplyBlock(void);
void ShowChannelStatus(void);
void ConfigurationMenu(void);
void ClearChannelsMenu(void);
void LoadConfiguration(void);
void SaveConfiguration(void);
int ReadConfig(int ConfigID);
void WriteConfig(int ConfigID, int Value);
void LoadPreset(int PresetID);
void ChangeChannelMenu(void);
void LoadPresetMenu(void);
void SavePresetMenu(void);
void LoadChannelName(int ChannelID);
void SaveChannelNames();
void ShowErrorMessage(int MessageID);
void ShowConfirmed(void);
void WriteToLedDriver(int iCommand, int iParameter);
void ActivateStandbyMode(short bSleep);
Void AutoKeypadBacklight(void);
void SetKeypadBacklight(void);
void UpdateVideoCrosspoint(int Mode);
void UpdateAudioCrosspoint(short Enable);
short ChannelsCleared(void);
short IsChannelNameEmpty(int ChannelID);

//*****************************************************************************
// Variables
//*****************************************************************************
//--- Remote control interface ---
int CommandBuffer_Length = 0;
int CommandBuffer_Buffer[255];
int CommandBuffer_Cursor = 0;
short CommandBuffer_Start = false;
short CommandBuffer_Full = false;
int ReplyBlockData[255];
int ReplyBlockLength;
int ReplyBlockCode;
//--- Keypad ---
signed int Keypad_CharID = -1;
short Keypad_KeyShift = false;
Int16 Keypad_ActiveKey = KeySet_Boot;
//--- Display ---
int CurrentLedDisplay_Character;
short Backlight_On = true;
//--- Channel ---
int CurrentMonitoringChannel = 0;
char ChannelNames[15][5];
signed int ChannelStatus[17];
int ChannelSelectionMode = 0;
#define SelectedDestination   ChannelStatus[14]
#define SelectedVideoSource   ChannelStatus[15]
#define SelectedAudioSource   ChannelStatus[16]
//--- System ---
short bOnStandby = false;
int32 SleepTimer = 0;
int32 StandbyTimer = 0;
int LastPhotoLevel = 0;

//--- Configuration ----
int DefaultStartupPreset;
int DefaultStartupMonitorChannel;
struct
{
   int   UartSpeed : 3;
   short AutoStandbyEnabled;
   short AutoSleepEnabled;
   short AutoKeypadLightsEnabled;
   short DimInactiveKeys;
   short EnableAutoMonitorSelect;
} Config_General;
int BackLight_Auto_Threshold;
int BackLight_Auto_Offset;
int BackLight_AutoStandbyLevel;
int TimeBefore_AutoStandby = 13;
int TimeBefore_Sleep = 13;
int inhibitedSource[7];



//*****************************************************************************
// Constants
//*****************************************************************************
Const byte KeyCharMap[] = {
  Char_AV, Char_Num7, Char_Num8, Char_Num9,
  Char_Clear, Char_Num4, Char_Num5, Char_Num6,
  Char_StandBy, Char_Num1, Char_Num2, Char_Num3,
  Char_Preset, Char_Dest, Char_Set, Char_Source};

const int8 Lcd_CustomChars[] = {
  0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000,
  0b00100, 0b01110, 0b01110, 0b01110, 0b00100, 0b00000, 0b00100, 0b00000,
  0b00000, 0b01000, 0b10101, 0b11100, 0b10101, 0b10100, 0b00000, 0b00000,
  0b00000, 0b10100, 0b10101, 0b10100, 0b10101, 0b01000, 0b00000, 0b00000,
  0b00000, 0b00000, 0b00000, 0b01110, 0b01110, 0b00000, 0b00000, 0b00000,
  0b00000, 0b01000, 0b01100, 0b01110, 0b01100, 0b01000, 0b00000, 0b00000,
  0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000,
  0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000};

const byte LedDisplayChars[] = {
  0b01111110, 0b00001100, 0b10110110,
  0b10011110, 0b11001100, 0b11011010,
  0b11111010, 0b00001110, 0b11111110,
  0b11011110, 0b11110010, 0b10000000,
  0b00000110, 0b00001100, 0b00011000,
  0b00110000, 0b01100000, 0b01000010,
  0b00000000};

const int Max6956_init[] = {
  KeypadBacklight_Normal,  // Global current register (0x02)
  0b00000000,  // Not used (0x03)
  0b01000001,  // Configuration register (0x04)
  0b00000000,  // Not used (0x05)
  0b00000000,  // Transistion detect mask (0x06)
  0b00000000,  // Display test register (0x07)
  0b00000000,  // Not used (0x08)
  0b00000000,  // Port 7,6,5,4 configuration (0x09)
  0b00000000,  // Port 11,10,9,8 configuration (0x0A)
  0b00000000,  // Port 15,14,13,12 configuration (0x0B)
  0b00000000,  // Port 19,18,17,16 configuration (0x0C)
  0b00000001,  // Port 23,22,21,20 configuration (0x0D)
  0b00000000,  // Port 27,26,25,24 configuration (0x0E)
  0b00000000}; // Port 31,30,29,28 configuration (0x0F)


const int32 TimeValues[] = {
   2541,    // 15 sec.
   5082,    // 30 sec.
   7623,    // 45 sec.
   10164,   // 1 min.
   15247,   // 1 min. 30 sec.
   20329,   // 2 min.
   30494,   // 3 min.
   40659,   // 4 min.
   50823,   // 5 min.
   60988,   // 6 min.
   71152,   // 7 min.
   81317,   // 8 min.
   91482,   // 9 min.
   101647,  // 10 min.
   152470,  // 15 min.
   203294,  // 20 min.
};



//******************************************************************************
// Default configuration
//******************************************************************************
#rom 0xF00000 = {
   //--- Default channel names ---
   'B', 'L', 'K', ' ', 0x00, 'S', 'R', 'C', '1', 0x00,
   'S', 'R', 'C', '2', 0x00, 'S', 'R', 'C', '3', 0x00,
   'S', 'R', 'C', '4', 0x00, 'S', 'R', 'C', '5', 0x00,
   'S', 'R', 'C', '6', 0x00, 'S', 'R', 'C', '7', 0x00,
   'D', 'S', 'T', '1', 0x00, 'D', 'S', 'T', '2', 0x00,
   'D', 'S', 'T', '3', 0x00, 'D', 'S', 'T', '4', 0x00,
   'D', 'S', 'T', '5', 0x00, 'D', 'S', 'T', '6', 0x00,
   'D', 'S', 'T', '7', 0x00,

   //--- User channel preset ---
   255, 255, 255, 255, 255, 255, 255,
   255, 255, 255, 255, 255, 255, 255,
   255, 255, 255, 255, 255, 255, 255,
   255, 255, 255, 255, 255, 255, 255,
   255, 255, 255, 255, 255, 255, 255,
   255, 255, 255, 255, 255, 255, 255,
   255, 255, 255, 255, 255, 255, 255,
   255, 255, 255, 255, 255, 255, 255,
   255, 255, 255, 255, 255, 255, 255,

   //--- Configuration ---
   0b00010000, //01.low nibble : Default channel preset at startup
               //02.high nibble: Default monitor channel at startup

   0b01111011, //bit 0: \
               //bit 1:  } 03.Uart spped (Default: 3=19200bps)
               //bit 2: /
               //bit 3: 04.Enable Auto-standby feature (Default: on)
               //bit 4: 05.Enable Auto-sleep feature (Default: on)
               //bit 5: 06.Enable automatic keypad intensity control (Default: on)
               //bit 6: 07.Dim inactive keypad button's (Default: on)
               //bit 7: 08.Auto monitor select after channel programming (Default: off)

   205,        //09.Auto-Keypad backlight ON threshold (Default: 205/255)

   15,         //10.auto-Keypad backlight ON offset (Default: 15/100)

   60,         //11.Minimum ambiant light intensity before auto-standby feature is activated (Default: 60/255)

   0b11100001, //12.low nibble : Time before auto-standby (Default: 30sec)
               //13.high nibble: Time before auto sleep (Default: 15min)

   0b00000000, //14.low nibble : Inhibited source for destination #1
               //15.high nibble: Inhibited source for destination #2

   0b00000000, //16.low nibble : Inhibited source for destination #3
               //17.high nibble: Inhibited source for destination #4

   0b00000000, //18.low nibble : Inhibited source for destination #5
               //19.high nibble: Inhibited source for destination #6

   0b00000000  //20.low nibble : Inhibited source for destination #7
               //   high nibble: *unused*
}



//*****************************************************************************
// Built-in functions
//*****************************************************************************
#use delay(clock=McuClock)
#use rs232(baud=DefaultUartSpeed,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8,errors)
#use i2c(Master,FAST,sda=Pin_SDA,scl=Pin_SCL,FORCE_HW)


//*****************************************************************************
// Drivers
//*****************************************************************************
#include "lcd.c"

//*****************************************************************************
// UART receive interrupt
//*****************************************************************************
#int_rda
void uart_interrupt()
{
   //**** Read the uart ****
   int iChar;
   iChar = getch();

   if (CommandBuffer_Start)
   {
      if (CommandBuffer_Length == 0)
      {
         //**** The second character received is the length of the command block
         //**** (excluding the stop, start and LOC character)
         CommandBuffer_Length = iChar;

      }
      else
      {
         //**** Check if the cursor have reach the end of the command block
         if (CommandBuffer_Cursor == CommandBuffer_Length)
         {
            if (iChar != 0x11) return;

            //**** Send an acknowledgement code to the host ****
            ReplyBlockCode = CodeID_Acknowledge;
            ReplyBlockLength = 0;
            SendReplyBlock();

            //**** Set buffer full flag ****
            CommandBuffer_Full = true;

            //**** Suspend receiving until the command is processed
            OUTPUT_HIGH(Pin_ClearToSend);

            OUTPUT_HIGH(Pin_Diag_Led);
         }
         else
         {
            //**** If not, put the next character in the buffer ****
            CommandBuffer_Buffer[CommandBuffer_Cursor] = iChar;

            //**** Increment the cursor ****
            CommandBuffer_Cursor++;
         }
      }
   }
   else
   {
      //**** The start character (0x10) is expected, if an other character was
      //**** sent discard it and return
      if (iChar == 0x10) CommandBuffer_Start = true;
   }
}



//******************************************************************************
//******************************************************************************
// Timer1 interrupt
//******************************************************************************
//******************************************************************************
#int_timer1
void Timer1_isr(void)
{
   static int iCurrentSegment = 12;

   WriteToSegmentDisplay(iCurrentSegment++);
   if (iCurrentSegment > 17) iCurrentSegment = 12;
}


//******************************************************************************
//******************************************************************************
// Timer2 interrupt
//******************************************************************************
//******************************************************************************
#int_timer2
void Timer2_tick(void)
{
   static int iColumn;
   static int iKeyID;
   static int iLastKeyID = 0;
   static signed int16 iScanTime = 0;

   SleepTimer++;
   StandbyTimer++;

   //**** Debounce delay ****
   if (iScanTime < 0)
   {
      iScanTime = iScanTime + 1;
   }
   else
   {
      iKeyID = 0;

      for (iColumn=0;iColumn<4;iColumn++)
      {
         //**** Set the current row high and wait for it to go high ****
         Bit_Set(PORTB, iColumn);
         delay_cycles(1);

         switch (PORTB >> 4)
         {
            case 0x1: iKeyID = 1 + iColumn;  break;
            case 0x2: iKeyID = 5 + iColumn;  break;
            case 0x4: iKeyID = 9 + iColumn;  break;
            case 0x8: iKeyID = 13 + iColumn; break;
         }

         //**** Set the current row low ****
         Bit_Clear(PORTB, iColumn);
      }

      if (iLastKeyID != iKeyID)
      {
         if (iKeyID != 0)
         {
            iScanTime = 1;
            iLastKeyID = iKeyID;
         }
         else
         {
            if (Keypad_CharID == -1)
            {
               if (iScanTime > 0)
               {
                  Keypad_CharID = KeyCharMap[iLastKeyID - 1];
                  Keypad_KeyShift = False;
               }

               iLastKeyID = 0;
               iScanTime = -KeyScanTime_Debounce;
            }
         }
      }
      else
      {
         if (iScanTime > 0) iScanTime++;
         if ((iScanTime > KeyScanTime_BeforeShift) && (Keypad_CharID == -1))
         {
            Keypad_CharID = KeyCharMap[iLastKeyID - 1];
            Keypad_KeyShift = True;
            iScanTime = 0;
         }
      }
   }
}


//*****************************************************************************
//*****************************************************************************
// Program entry
//*****************************************************************************
//*****************************************************************************
void main()
{
   int i;
   int16 KeypadAnim[12] = {0b0000000010000000,0b0000000110010000,0b0000001110110010
                           0b0001001111110110,0b0101001111111111,0b1101101111111101,
                           0b1111101111101001,0b1111101101000001,0b1111001000000001
                           0b1001000000000001,0b0000000000000001,0b0000000000000000};

   //**** Set pins direction ****
   LatA = 0b00000000;
   LatB = 0b00000000;
   LatC = 0b00000010;
   LatD = 0b00100000;
   LatE = 0b00000000;

   Set_Tris_A(0b00000001);
   Set_Tris_B(0b11110000);
   Set_Tris_C(0b10000000);
   Set_Tris_D(0b00000000);
   Set_Tris_E(0b00000000);

   OUTPUT_FLOAT(Pin_SDA);
   OUTPUT_FLOAT(Pin_SCL);
   Pin_VideoGen_Enable = false;

   //**** Begin initialization sequence ***
   OUTPUT_HIGH(Pin_Diag_Led);
   WriteToSegmentDisplay(8);

   //**** Disable command receiving during start-up ****
   OUTPUT_HIGH(Pin_ClearToSend);

   delay_ms(200);

   //**** Setup timers ****
   Setup_timer_1(T1_INTERNAL|T1_DIV_BY_8);
   Setup_timer_2(T2_DIV_BY_16, 255, 8);

   //**** Start animation on segment display ****
   enable_interrupts(int_timer1);

   //**** Setup ADC for photo resistor reading ****
   setup_adc_ports(RA0_ANALOG);
   setup_adc(ADC_CLOCK_INTERNAL);
   set_adc_channel(0);

   //**** Configure interrupts ****
   enable_interrupts(int_rda);
   enable_interrupts(int_timer2);
   enable_interrupts(global);

   //**** Initialize LCD display ****
   InitializeLCD();

   //**** Show start-up screen ****
   lcd_NoEcho = true;
   lcd_putc("\f* AVS-50 v" AppCurrentVersion " *\rInitializing...");
   lcd_NoEcho = false;

   Pin_VideoGen_Enable = true;

   //**** Reset crosspoint data ****
   for (i=0;i<14;i++) ChannelStatus[i] = 0;

   //**** Initialize video crosspoint (MAX456) ****
   UpdateVideoCrosspoint(0x99);

   //**** Initialize audio crosspoint (AD8113) ****
   UpdateAudioCrosspoint(True);


   //**** Initialize led driver (MAX6956) ****
   i2c_start();
   i2c_write(I2CADDRESS_MAX6956_WRITE);
   i2c_write(0x02);
   for (i=0; i<15;i++) i2c_write(Max6956_init[i]);
   i2c_stop();

   SetKeypadBacklight();

   //**** Turn on lcd backlight ****
   WriteToLedDriver(0x34, 0x01);
   WritetoLedDriver(MAX6956_COMMAND_PORT12_19, 0xFF);
   WritetoLedDriver(MAX6956_COMMAND_PORT22_29, 0xFF);

   //**** Animation on keypad backlight ***
   Config_General.AutoKeypadLightsEnabled = false;
   Config_General.DimInactiveKeys = true;
   for (i=0;i<12;i++)
   {
      Keypad_ActiveKey = KeypadAnim[i];
      SetKeypadBacklight();

      delay_ms(100);
   }

   //**** Load configuration from EEPROM ****
   LoadConfiguration();

   //**** Set UART speed ****
   switch (Config_General.UartSpeed)
   {
      Case 0:  Set_Uart_Speed(2400); break;
      Case 1:  Set_Uart_Speed(4800); break;
      Case 2:  Set_Uart_Speed(9600); break;
      Case 4:  Set_Uart_Speed(38400); break;
      Case 5:  Set_Uart_Speed(57600); break;
      Case 6:  Set_Uart_Speed(115200); break;
      Default: Set_Uart_Speed(19200);
   }

   //**** Put custom character in lcd's GCRAM ****
   lcd_write(0, 0x40);
   for (i=0;i<64;i++)
   {
      lcd_write(1, Lcd_CustomChars[i]);
   }


   delay_ms(250);


   //**** End initialization sequence ***
   disable_interrupts(int_timer1);
   setup_timer_1(T1_DISABLED);

   ClearSegmentDisplay();
   ClearLcdDisplay();
   OUTPUT_LOW(Pin_Diag_Led);

   //**** Enable command receiving ****
   OUTPUT_LOW(Pin_ClearToSend);

   CurrentMonitoringChannel = DefaultStartupMonitorChannel;
   LoadPreset(DefaultStartupPreset);
   UpdateVideoCrosspoint(0);

   UpdateAudioCrosspoint(True);


   while(true)
   {
      //**** Display channel status ****
      ShowChannelStatus();

      Keypad_ActiveKey = Keyset_MainMenu;
      If (WaitForKeypadChar())
      {
         switch (Keypad_CharID)
         {
            case Char_Source:
               ChannelSelectionMode = ChannelSelect_Both;
               ChangeChannelMenu();
               break;

            case Char_Dest:
               ChannelSelectionMode = ChannelSelect_Destination;
               ChangeChannelMenu();
               break;

            case Char_Preset:
               LoadPresetMenu();
               break;

            case Char_Set:
               if (Keypad_KeyShift)
               {
                  ConfigurationMenu();
               }
               else
               {
                  SavePresetMenu();
               }
               break;

            case Char_Clear:
               if (Keypad_KeyShift) ClearChannelsMenu();

            default:
               if (Keypad_CharID < 8)
               {
                  CurrentMonitoringChannel = Keypad_CharID;

                  UpdateVideoCrosspoint(0);
                  UpdateAudioCrosspoint(True);
               }
         }
      }

      ChannelSelectionMode = ChannelSelect_Off;
   }
}


//******************************************************************************
//******************************************************************************
// User interface
//******************************************************************************
//******************************************************************************
//=========================================================
// ChangeChannelMenu
// --------------------
// Description : Display Channel selection menu
//
// Parameters  : *none*
//
// Return      : *none*
//=========================================================
void ChangeChannelMenu(void)
{
   int InhibitedSrc = 0;

   SelectedVideoSource = -1;
   SelectedAudioSource = -1;
   SelectedDestination = -1;

   Keypad_ActiveKey = Keyset_ChangeChannelMenu;

   while(true)
   {
      //**** Turn on cursor blinking ****
      Lcd_write(0, 0b00001101);

      //**** Refresh channel status display ****
      ShowChannelStatus();

      if (WaitForKeypadChar())
      {
         //**** Turn off cursor blinking ****
         Lcd_write(0, 0b00001100);

         switch (Keypad_CharID)
         {
            case Char_Dest:
               ChannelSelectionMode = ChannelSelect_Destination;
               break;

            case Char_Source:
               ChannelSelectionMode = ChannelSelect_Both;
               break;

            case Char_AV:
               ChannelSelectionMode++;
               if (ChannelSelectionMode > ChannelSelect_Audio) ChannelSelectionMode = ChannelSelect_Both;
               break;

            case Char_Set:
               //**** Check if the selected video & audio source are valid ****
               if (SelectedAudioSource == -1 || SelectedVideoSource == -1)
               {
                  ShowErrorMessage(MessageID_InvalidSource);
                  if (ChannelSelectionMode == ChannelSelect_Destination) ChannelSelectionMode = ChannelSelect_Both;
                  break;
               }

               //**** Check if the selected destination is valid ****
               if (SelectedDestination == -1)
               {
                  ShowErrorMessage(MessageID_InvalidDestination);
                  ChannelSelectionMode = ChannelSelect_Destination;
                  break;
               }

               //**** Check if the source is inhibited for the selected destination ****
               InhibitedSrc = InhibitedSource[SelectedDestination-1];
               if (InhibitedSrc != 0)
               {
                  if (SelectedAudioSource == InhibitedSrc && SelectedVideoSource == InhibitedSrc)
                  {
                     ShowErrorMessage(MessageID_InvalidSelection);
                     SelectedAudioSource = -1;
                     SelectedVideoSource = -1;
                     ChannelSelectionMode = ChannelSelect_Both;
                     break;
                  }
                  if (SelectedVideoSource == InhibitedSrc)
                  {
                     ShowErrorMessage(MessageID_InvalidSelection);
                     SelectedVideoSource = -1;
                     ChannelSelectionMode = ChannelSelect_Video;
                     break;
                  }
                  if (SelectedAudioSource == InhibitedSrc)
                  {
                     ShowErrorMessage(MessageID_InvalidSelection);
                     SelectedAudioSource = -1;
                     ChannelSelectionMode = ChannelSelect_Audio;
                     break;
                  }
               }

               ChannelStatus[SelectedDestination - 1] = SelectedVideoSource;
               ChannelStatus[SelectedDestination + 6] = SelectedAudioSource;

               if (Config_General.EnableAutoMonitorSelect)
               {
                  CurrentMonitoringChannel = SelectedDestination;
                  WriteToSegmentDisplay(CurrentMonitoringChannel);
               }

               UpdateVideoCrosspoint(0);
               UpdateAudioCrosspoint(True);

               ShowConfirmed();
               return;

            case Char_Clear:

               if (ChannelSelectionMode == ChannelSelect_Destination)
               {
                  if (SelectedAudioSource == -1 && SelectedVideoSource == -1)
                  {
                     SelectedAudioSource = 0;
                     SelectedVideoSource = 0;
                  }
                  else
                  {
                     return;
                  }
                  break;
               }

               if (ChannelSelectionMode == ChannelSelect_Both && SelectedVideoSource == 0 && SelectedAudioSource == 0) return;
               if (ChannelSelectionMode == ChannelSelect_Audio && SelectedAudioSource == 0) return;
               if (ChannelSelectionMode == ChannelSelect_Video && SelectedVideoSource == 0) return;

            default:
               switch (ChannelSelectionMode)
                  {
                     case ChannelSelect_Both:
                        SelectedVideoSource = Keypad_CharID;
                        SelectedAudioSource = Keypad_CharID;
                        break;

                     case ChannelSelect_Audio:
                        SelectedAudioSource = Keypad_CharID;
                        break;

                     case ChannelSelect_Video:
                        SelectedVideoSource = Keypad_CharID;
                        break;

                     case ChannelSelect_Destination:
                        SelectedDestination = Keypad_CharID;
                        break;
                  }
         }
      }
   }
}

//=========================================================
// LoadPresetMenu
// --------------------
// Description : Display Load preset menu
//
// Parameters  : *none*
//
// Return      : *none*
//=========================================================
void LoadPresetMenu(void)
{
   int iPresetID = 1;

   Keypad_ActiveKey = Keyset_LoadPresetMenu;

   while(true)
   {
      printf(lcd_putc, "\fLoad preset?  \x05%d\r(1-9)", iPresetID);

      //**** Turn on cursor blinking & set position ****
      Lcd_write(0, 0b00001101);
      Lcd_write(0, 0b10001111);


      if (WaitForKeypadChar())
      {

         //**** Turn off cursor blinking ****
         Lcd_write(0, 0b00001100);

         Switch (Keypad_CharID)
         {
            case Char_Clear:
               return;

            case Char_Set:
               LoadPreset(iPresetID);

               UpdateVideoCrosspoint(0);
               UpdateAudioCrosspoint(True);
               ShowConfirmed();
               return;

            default:
               iPresetID = Keypad_CharID;
         }
      }
   }
}

//=========================================================
// SavePresetMenu
// --------------------
// Description : Display Save preset menu
//
// Parameters  : *none*
//
// Return      : *none*
//=========================================================
void SavePresetMenu(void)
{
   int iStep = 0;
   int i;
   int iNumSelect = 1;
   int iPreset;
   int iChannel;
   int iNewPreset[14] = {255,255,255,255,255,255,255,255,255,255,255,255,255,255};

   Keypad_ActiveKey = Keyset_SavePresetMenuStep1;

   while(true)
   {
      if (iStep == 0) Printf(lcd_putc, "\fSave preset?  \x05%d\r(1-9)", iNumSelect);
      if (iStep == 1) Printf(lcd_putc, "\fChannel?      \x05%d\r(1-7, 9=All)", iNumSelect);
      if (iStep == 2) Printf(lcd_putc, "\fSave?\r(SET=Yes CLR=No)");

      //**** Turn on cursor blinking & set position ****
      if (iStep != 2)
      {
         Lcd_write(0, 0b00001101);
         Lcd_write(0, 0b10001111);
      }

      if (WaitForKeypadChar())
      {

         //**** Turn off cursor blinking ****
         Lcd_write(0, 0b00001100);

         switch (Keypad_CharID)
         {
            case Char_Clear:
               return;

            case Char_Set:
               switch (iStep)
               {
                  case 0:
                     iPreset = iNumSelect;
                     iNumSelect = 9;

                     Keypad_ActiveKey = KeySet_SavePresetMenuStep2;
                     break;

                  case 1:
                     iChannel = iNumSelect - 1;

                     Keypad_ActiveKey = KeySet_SavePresetMenuStep3;
                     break;

                  case 2:
                     if (iChannel == 8)
                     {
                        memcpy(&iNewPreset[0], &ChannelStatus[0], 14);
                     }
                     else
                     {
                        iNewPreset[iChannel] = ChannelStatus[iChannel];
                        iNewPreset[iChannel+7] = ChannelStatus[iChannel+7];
                     }

                     for (i=0;i<7;i++)
                     {
                        WRITE_EEPROM(MEMLOC_PRESET + ((iPreset - 1) * 7) + i, (iNewPreset[i] & 0x0F) | (iNewPreset[i+7] << 4));
                     }

                     ShowConfirmed();
                     return;
               }
               iStep++;
               break;

            default:
               iNumSelect = Keypad_CharID;
               break;
         }
      }
   }
}

//=========================================================
// ClearChannelsMenu
// --------------------
// Description : Display the clear channel menu
//
// Parameters  : *none*
//
// Return      : *none*
//=========================================================
void ClearChannelsMenu(void)
{
   Keypad_ActiveKey = Keyset_ClearChannelsMenu;

   while(true)
   {
      printf(lcd_putc, "\fClear all?\rSET=Yes CLR=No");

      if (WaitForKeypadChar())
      {
         Switch (Keypad_CharID)
         {
            case Char_Clear:
               return;

            case Char_Set:
               LoadPreset(0);

               if (Config_General.EnableAutoMonitorSelect)
               {
                  WriteToSegmentDisplay(LedDisplay_Minus);
                  CurrentMonitoringChannel = 0;
               }

               UpdateVideoCrosspoint(0);
               UpdateAudioCrosspoint(True);

               ShowConfirmed();
               return;
         }
      }
   }

}



//=========================================================
// ConfigurationMenu
// --------------------
// Description : Display the configuration men
//
// Parameters  : *none*
//
// Return      : *none*
//=========================================================
void ConfigurationMenu(void)
{
   int iCurrentConfigID = 1;
   int iConfigValue;
   int iMin = 0;
   int iMax = 0;
   int iStep = 0;
   int PreviousUartSpeed;
   char EmptyChannelName[5] = "    ";
   SIGNED int iCursor = -1;


   PreviousUartSpeed = Config_General.UartSpeed;

   Keypad_ActiveKey = Keyset_ConfigMenu;

   while(true)
   {
      printf(lcd_putc, "\f%c%02d: ", 0x05, iCurrentConfigID);

      iConfigValue = ReadConfig(iCurrentConfigID);
      iStep = 1;
      switch (iCurrentConfigID)
      {
         case ConfigID_StartupPreset:
            if (iConfigValue !=0)
            {
               printf(lcd_putc, "#%d", iConfigValue);
            }
            else
            {
               printf(lcd_putc, "Disabled");
            }
            iMin = 0;
            iMax = 9;
            break;

         case ConfigID_StartupMonitorChannel:
            if (iConfigValue !=0)
            {
               printf(lcd_putc, "%d-%s", iConfigValue, ChannelNames[iConfigValue + 7]);
            }
            else
            {
               printf(lcd_putc, "Disabled");
            }
            iMin = 0;
            iMax = 7;
            break;

         case ConfigID_UartSpeed:
            switch(iConfigValue)
            {
               case 0: printf(lcd_putc, "2400 bps"); break;
               case 1: printf(lcd_putc, "4800 bps"); break;
               case 2: printf(lcd_putc, "9600 bps"); break;
               case 3: printf(lcd_putc, "19200 bps"); break;
               case 4: printf(lcd_putc, "38400 bps"); break;
               case 5: printf(lcd_putc, "57600 bps"); break;
               case 6: printf(lcd_putc, "115200 bps");
            }

            iMin = 0;
            iMax = 6;
            iStep = 1;
            break;

         case ConfigID_AutoStandbyEnabled:
         case ConfigID_AutoSleepEnabled:
         case ConfigID_AutoKeypadLights:
         case ConfigID_DimInactiveKeys:
         case ConfigID_AutoMonitorSelect:
            if (iConfigValue != 0)
            {
               printf(lcd_putc, "Enabled");
            }
            else
            {
               printf(lcd_putc, "Disabled");
            }
            iMin = 0;
            iMax = 1;
            break;

         case ConfigID_Backlight_AutoOffset:
            #define NumBars iMax
            #define iCount  iMin

            printf(lcd_putc, "%3u ", iConfigValue);

            NumBars = (iConfigValue / 16);
            for (iCount=0;iCount<NumBars+1;iCount++) lcd_putc(255);

            imin = 5;
            imax = 100;
            iStep = 5;
            break;

         case ConfigID_Backlight_AutoThreshold:
         case ConfigID_AutoStandbyLevel:

            printf(lcd_putc, "%3u ", iConfigValue);

            NumBars = (iConfigValue / 40);
            for (iCount=0;iCount<NumBars+1;iCount++) lcd_putc(255);

            imin = 5;
            iMax = 255;
            iStep = 5;
            break;

         case ConfigID_TimeBeforeStandby:
         case ConfigID_TimeBeforeSleep:
            switch(iConfigValue)
            {
               case 0: printf(lcd_putc, "00:15"); break;
               case 1: printf(lcd_putc, "00:30"); break;
               case 2: printf(lcd_putc, "00:45"); break;
               case 3: printf(lcd_putc, "01:00"); break;
               case 4: printf(lcd_putc, "01:30"); break;
               case 5: printf(lcd_putc, "02:00"); break;
               case 6: printf(lcd_putc, "03:00"); break;
               case 7: printf(lcd_putc, "04:00"); break;
               case 8: printf(lcd_putc, "05:00"); break;
               case 9: printf(lcd_putc, "06:00"); break;
               case 10:printf(lcd_putc, "07:00"); break;
               case 11:printf(lcd_putc, "08:00"); break;
               case 12:printf(lcd_putc, "09:00"); break;
               case 13:printf(lcd_putc, "10:00"); break;
               case 14:printf(lcd_putc, "15:00"); break;
               case 15:printf(lcd_putc, "20:00"); break;
            }

            imin = 0;
            iMax = 15;
            break;

         case ConfigID_InhibitedSource1:
         case ConfigID_InhibitedSource2:
         case ConfigID_InhibitedSource3:
         case ConfigID_InhibitedSource4:
         case ConfigID_InhibitedSource5:
         case ConfigID_InhibitedSource6:
         case ConfigID_InhibitedSource7:
            if (iConfigValue !=0)
            {
               printf(lcd_putc, "%d-%s", iConfigValue, ChannelNames[iConfigValue]);
            }
            else
            {
               printf(lcd_putc, "Disabled");
            }
            iMin = 0;
            iMax = 7;
            break;

         default:
            iConfigValue = (iCurrentConfigID - ConfigID_ChannelNameSource0);
            printf(lcd_putc, "[%s]", ChannelNames[iConfigValue]);

      }
      lcd_putc("\r");

      Switch (iCurrentConfigID)
      {
         case ConfigID_StartupPreset:
            printf(lcd_putc, "Startup preset");
            break;

         case ConfigID_StartupMonitorChannel:
            printf(lcd_putc, "Startup monitor");
            break;

         case ConfigID_UARTSpeed:
            printf(lcd_putc, "UART speed");
            break;

         case ConfigID_AutoStandbyEnabled:
            printf(lcd_putc, "Auto-standby");
            break;

         case ConfigID_AutoSleepEnabled:
            printf(lcd_putc, "Auto-sleep");
            break;

         case ConfigID_AutoKeypadLights:
            printf(lcd_putc, "Auto kpd. lights");
            break;

         case ConfigID_DimINactiveKeys:
            printf(lcd_putc, "Dim inactive key");
            break;

         case ConfigID_AutoMonitorSelect:
            printf(lcd_putc, "Auto monitor");
            break;

         case ConfigID_Backlight_AutoThreshold:
            printf(lcd_putc, "Auto-BL Thrld.");
            break;

         case ConfigID_Backlight_AutoOffset:
            printf(lcd_putc, "Auto-BL offset");
            break;

         case ConfigID_AutoStandbyLevel:
            printf(lcd_putc, "Auto-standby lvl");
            break;

         case ConfigID_TimeBeforeStandby:
            printf(lcd_putc, "Auto-Stby. delay");
            break;

         case ConfigID_TimeBeforeSleep:
            printf(lcd_putc, "Auto-Sleep delay");
            break;

         case ConfigID_InhibitedSource1:
         case ConfigID_InhibitedSource2:
         case ConfigID_InhibitedSource3:
         case ConfigID_InhibitedSource4:
         case ConfigID_InhibitedSource5:
         case ConfigID_InhibitedSource6:
         case ConfigID_InhibitedSource7:
            printf(lcd_putc, "Src inhibit:");
            printf(lcd_putc, "%s", ChannelNames[iCurrentConfigID - ConfigID_InhibitedSource1 + 8]);
            break;

         case ConfigID_ChannelNameSource0:
         case ConfigID_ChannelNameSource1:
         case ConfigID_ChannelNameSource2:
         case ConfigID_ChannelNameSource3:
         case ConfigID_ChannelNameSource4:
         case ConfigID_ChannelNameSource5:
         case ConfigID_ChannelNameSource6:
         case ConfigID_ChannelNameSource7:
            printf(lcd_putc, "Source #%d name", iCurrentConfigID - ConfigID_ChannelNameSource0);
            break;

         case ConfigID_ChannelNameDest1:
         case ConfigID_ChannelNameDest2:
         case ConfigID_ChannelNameDest3:
         case ConfigID_ChannelNameDest4:
         case ConfigID_ChannelNameDest5:
         case ConfigID_ChannelNameDest6:
         case ConfigID_ChannelNameDest7:
            printf(lcd_putc, "Dest. #%d name", iCurrentConfigID - ConfigID_ChannelNameSource7);
            break;
      }

      if (iCursor != -1)
      {
         //**** Turn on cursor blink and set cursor position ****
         Lcd_write(0, 0b00001101);
         Lcd_write(0, 0b10000110 + iCursor);
      }

      if (WaitForKeypadChar())
      {

         if (iCursor !=-1) Lcd_write(0, 0b00001100);

         Switch (Keypad_CharID)
         {
            case Char_Num2:
            case Char_Num8:
               if (iCursor != -1)
               {
                  //**** Read character value****
                  iConfigValue = ChannelNames[(iCurrentConfigID - ConfigID_ChannelNameSource0)][iCursor];

                  if (Keypad_CharID == Char_Num2)
                  {
                     iConfigValue++;
                  }
                  else
                  {
                     iConfigValue--;
                  }

                  if (iConfigValue == 31) iConfigValue = 57;
                  if (iConfigValue == 64) iConfigValue = 32;
                  if (iConfigValue == 47) iConfigValue = 90;

                  if (iConfigValue == 33) iConfigValue = 65;
                  if (iConfigValue == 58) iConfigValue = 32;
                  if (iConfigValue == 91) iConfigValue = 48;

                  ChannelNames[(iCurrentConfigID - ConfigID_ChannelNameSource0)][iCursor] = iConfigValue;
                  break;
               }


               if (Keypad_CharID == Char_Num2)
               {
                  iCurrentConfigID--;
                  if (iCurrentConfigID < 1) iCurrentConfigID = ConfigID_MAX;
               }
               else
               {
                  iCurrentConfigID++;
                  if (iCurrentConfigID > ConfigID_MAX) iCurrentConfigID = 1;
               }
               break;

            case Char_Num4:
            case Char_Num6:
               if (iCurrentConfigID >= ConfigID_ChannelNameSource0)
               {
                  if (Keypad_CharID == Char_Num4)
                  {
                     iCursor--;
                     if (iCursor < -1) iCursor = 3;
                  }
                  else
                  {
                     iCursor++;
                     if (iCursor > 3) iCursor = -1;
                  }
               }
               else
               {
                  if (Keypad_CharID == Char_Num4)
                  {
                     if (iConfigValue <= iMin)
                     {
                        iConfigValue = iMax;
                     }
                     else
                     {
                        iConfigValue -= iStep;
                     }
                  }
                  else
                  {
                     if (iConfigValue >= iMax)
                     {
                        iConfigValue = iMin;
                     }
                     else
                     {
                        iConfigValue += iStep;
                     }
                  }

                  WriteConfig(iCurrentConfigID, iConfigValue);
               }
               break;

            case Char_Clear:
               if (iCursor != -1 && !Keypad_KeyShift)
               {

                  if (IsChannelNameEmpty(iCurrentConfigID - ConfigID_ChannelNameSource0))
                  {
                     iCursor = -1;
                     break;
                  }
                  else
                  {
                     strcpy(ChannelNames[iCurrentConfigID - ConfigID_ChannelNameSource0], EmptyChannelName);
                     iCursor = 0;
                     break;
                  }
               }

               LoadConfiguration();
               return;

            case Char_Set:
               SaveConfiguration();

               ShowConfirmed();

               if (Config_General.UartSpeed != PreviousUartSpeed)
               {
                  lcd_putc("\fThe router must\rreboot...");
                  delay_ms(2000);
                  reset_cpu();
               }
               return;
         }
      }
   }
}



//******************************************************************************
//******************************************************************************
// Audio/Video crosspoint functions
//******************************************************************************
//******************************************************************************
//=========================================================
// UpdateVideoCrosspoint
// --------------------
// Description :
//
// Parameters  :
//
// Return      : *none*
//=========================================================
void UpdateVideoCrosspoint(int Mode)
{
   int Buffer[4];
   int i;
   #define iMonitorChannelID  i

   if (Mode != 0)
   {
      Buffer[0] = Mode;
      Buffer[1] = Mode;
      Buffer[2] = Mode;
      Buffer[3] = Mode;
   }
   else
   {
      iMonitorChannelID = (CurrentMonitoringChannel == 0) ?
      (0) : (ChannelStatus[CurrentMonitoringChannel - 1]);

      Buffer[0] = iMonitorChannelID + (ChannelStatus[6] << 4);
      Buffer[1] = ChannelStatus[5] + (ChannelStatus[4] << 4);
      Buffer[2] = ChannelStatus[3] + (ChannelStatus[2] << 4);
      Buffer[3] = ChannelStatus[1] + (ChannelStatus[0] << 4);
   }

   Output_low(Pin_VideoMatrix_Latch);
   Output_low(Pin_VideoMatrix_Clock);

   for (i=0;i<32;i++)
   {
      //**** Shift out the next bit ****
      Output_Bit(Pin_VideoMatrix_Data, Shift_left(Buffer, 4, 0));
      delay_cycles(1);

      Output_high(Pin_VideoMatrix_Clock);
      delay_ms(1);
      Output_low(Pin_VideoMatrix_Clock);
   }

   Output_high(Pin_VideoMatrix_Latch);
   delay_ms(1);
   Output_low(Pin_VideoMatrix_Latch);
}


//=========================================================
// UpdateAudioCrosspoint
// --------------------
// Description :
//
// Parameters  :
//
// Return      : *none*
//=========================================================
void UpdateAudioCrosspoint(short Enable)
{
   signed int MatrixData[16];
   signed int i;
   int iBit;
   int SourceID;
   int ChannelData;

   int RightSourceChannel[8] = {0,0,1,4,5,8,9,12};
   int LeftSourceChannel[8] = {0,2,3,6,7,10,11,14};
   int RightDestChannel[8] = {15,14,11,10,7,6,3,2};
   int LeftDestChannel[8] = {13,12,9,8,5,4,1,0};

   Output_high(Pin_audioMatrix_Update);
   OUtput_high(Pin_AudioMatrix_Clock);
   delay_us(5);

   output_bit(Pin_AudioMatrix_Reset, Enable);

   ChannelStatus[14] = (CurrentMonitoringChannel != 0) ?
   (ChannelStatus[CurrentMonitoringChannel+6]) : (0);

   for (i=0;i<8;i++)
   {
      SourceID = ChannelStatus[i+7];

      if (SourceID != 0)
      {
         MatrixData[LeftDestChannel[i]] = (LeftSourceChannel[SourceID] | 0x10);
         MatrixData[RightDestChannel[i]] = (RightSourceChannel[SourceID] | 0x10);
      }
      else
      {
         MatrixData[LeftDestChannel[i]] = 0;
         MatrixData[RightDestChannel[i]] = 0;
      }
   }


   //**** Send data to the audio crosspoint (AD8113), 80 bits, MSB first ****
   for (i=0;i<16;i++)
   {
      ChannelData = (MatrixData[i] << 3);

      if (!Enable) ChannelData = 0;

      for (ibit=0;ibit<5;ibit++)
      {
         Output_Bit(Pin_AudioMatrix_Data, shift_left(&ChannelData, 1, 0));
         delay_cycles(2);

         Output_Low(Pin_AudioMatrix_Clock);
         delay_us(1);
         Output_High(Pin_AudioMatrix_Clock);
         delay_us(1);
      }
   }

   Output_low(Pin_AudioMatrix_Update);
   delay_us(5);
   Output_high(Pin_AudioMatrix_Update);
}



//=========================================================
// ChannelsCleared
// --------------------
// Description : Check if the video and audio crosspoint are empty
//
// Parameters  : *none
//
// Return      : True if cleared
//=========================================================
short ChannelsCleared(void)
{
   int i;
   for(i=0;i<14;i++)
   {
      if (ChannelStatus[i] != 0) return false;
   }

   return true;
}





//******************************************************************************
//******************************************************************************
// Display functions
//******************************************************************************
//******************************************************************************

//=========================================================
// WriteToLedDriver
// --------------------
// Description : Sends a command to the MAX6956 led driver
//
// Parameters  : -iCommand : Command id
//               -iParameter : Command data
//
// Return      : *none*
//=========================================================
void WriteToLedDriver(int iCommand, int iParameter)
{
   //**** Issue a start condition on I2C line ****
   i2c_start();

   //**** Send device address ****
   i2c_write(I2CADDRESS_MAX6956_WRITE);

   //**** Send command id ****
   i2c_write(iCommand);

   //**** Send command data ****
   i2c_write(iParameter);

   //**** Issue a stop condition ****
   i2c_stop();
}

//=========================================================
// WriteToSegmentDisplay
// --------------------
// Description : Shift out a character to the shift register
//               of the segment display
//
// Parameters  : -iChar : The character to send
//
// Return      : *none*
//=========================================================
void WriteToSegmentDisplay(int *iChar)
{
   int i;
   int iByte;

   Output_Low(Pin_SegDisplay_Latch);
   Output_Low(Pin_SegDisplay_Clock);

   iByte = LedDisplayChars[iChar];

   for (i=1;i<=8;i++)
   {
      //**** Shift out the next bit ****
      Output_Bit(Pin_SegDisplay_data, Shift_left(&iByte, 1, 0));

      Output_High(Pin_SegDisplay_Clock);
      delay_cycles(1);
      Output_Low(Pin_SegDisplay_Clock);
   }

   //**** Once all bit have been shifted out, release them ****
   Output_High(Pin_SegDisplay_Latch);
   delay_cycles(1);
   Output_Low(Pin_SegDisplay_Latch);

   CurrentLedDisplay_Character = iChar;
}

//=========================================================
// ShowChannelStatus
// --------------------
// Description : Update channel status on LCD screen
//
// Parameters  : *none*
//
// Return      : *none*
//=========================================================
void ShowChannelStatus(void)
{
   int VideoSourceIndex;
   int AudioSourceIndex;
   int ChannelIndex;

   ClearLcdDisplay();

   if (CurrentMonitoringChannel == 0 && ChannelSelectionMode == 0)
   {
      WriteToSegmentDisplay(LedDisplay_Minus);
      lcd_putc("Monitor off");
   }
   else
   {
      if (ChannelSelectionMode == ChannelSelect_Off)
      {
         ChannelIndex = CurrentMonitoringChannel;
         VideoSourceIndex = ChannelStatus[ChannelIndex - 1];
         AudioSourceIndex = ChannelStatus[ChannelIndex + 6];

         WriteToSegmentDisplay(CurrentMonitoringChannel);
      }
      else
      {
         ChannelIndex = SelectedDestination;
         VideoSourceIndex = SelectedVideoSource;
         AudioSourceIndex = SelectedAudioSource;

         lcd_putc(0x05);
      }


      if (ChannelIndex != -1)
      {
         printf(lcd_putc, "%u-", ChannelIndex);
         printf(lcd_putc, "%s", ChannelNames[ChannelIndex + 7]);
      }
      else
      {
         lcd_putc("*-****");
      }

      switch (ChannelSelectionMode)
      {
         case ChannelSelect_Destination: lcd_putc("    Dest?\r");break;
         case ChannelSelect_Both       : lcd_putc("     Src?\r");break;
         case ChannelSelect_Video      : lcd_putc("  Src(V)?\r");break;
         case ChannelSelect_Audio      : lcd_putc("  Src(A)?\r");break;
         default:lcd_putc("\r");
      }


      lcd_putc(0x03);
      if (VideoSourceIndex != -1)
      {
         printf(lcd_putc, "%u-", VideoSourceIndex);
         printf(lcd_putc, "%s ", ChannelNames[VideoSourceIndex]);
      }
      else
      {
         lcd_putc("*-**** ");
      }

      lcd_putc(0x02);
      if (AudioSourceIndex != -1)
      {
         printf(lcd_putc, "%u-", AudioSourceIndex);
         printf(lcd_putc, "%s", ChannelNames[AudioSourceIndex]);
      }
      else
      {
         lcd_putc("*-****");
      }

      switch (ChannelSelectionMode)
      {
         case ChannelSelect_Destination: Lcd_write(0, 0b10000001);break;
         case ChannelSelect_Video      :
         case ChannelSelect_Both       : Lcd_write(0, 0b11000001);break;
         case ChannelSelect_Audio      : Lcd_write(0, 0b11001001);break;
      }

   }
}


//=========================================================
// ShowErrorMessage
// --------------------
// Description : Display an error message
//
// Parameters  : -MessageID : Error message code
//
// Return      : *none*
//=========================================================
void ShowErrorMessage(int MessageID)
{
   int i;

   //**** Save what's on the led display ****
   int PrevLedDisplay;
   PrevLedDisplay = CurrentLedDisplay_Character;


   //**** Display the error message ****
   printf(lcd_putc, "\f   - \x01Error -\r");

   switch(MessageID)
   {
      case MessageID_InvalidDestination: lcd_putc("No destination");break;
      case MessageID_InvalidSource: lcd_putc("No source");break;
      case MessageID_InvalidSelection: lcd_putc("Source inhibit"); break;
   }

   for (i=0;i<5;i++)
   {
      WriteToSegmentDisplay(LedDisplay_error);
      Delay_ms(250);
      WriteToSegmentDisplay(LedDisplay_off);
      Delay_ms(250);
   }

   //**** Restore what was on the led display ****
   WriteToSegmentDisplay(PrevLedDisplay);
}


//=========================================================
// ShowConfirmed
// --------------------
// Description : Display "Confirmed" on the LCD screen
//
// Parameters  : *none*
//
// Return      : *none*
//=========================================================
void ShowConfirmed(void)
{

   SetKeypadBacklight();

   lcd_putc("\f    - Done -");
   delay_ms(1000);
}


//=========================================================
// SetKeypadBacklight
// --------------------
// Description : Set backlight intensity for each key
//
// Parameters  : *none*
//
// Return      : *none*
//=========================================================
void SetKeypadBacklight(void)
{
   int CurrentReg[18];
   int i;
   int iState;
   const int iPortID[16] = {10,17,16,15,14,12,4,2,3,5,7,0,6,1,11,13};

   for (i=0;i<18;i++)
   {
      CurrentReg[i] = 0;
   }

   if (bOnStandby)
   {
      CurrentReg[7] = KeypadBacklight_Normal;
   }


   if (backlight_On)
   {
      for (i=0;i<16;i++)
      {
         if (Config_General.DimInactiveKeys)
         {
            iState = (Bit_Test(Keypad_ActiveKey, i)) ?
            (KeypadBacklight_Normal) : (KeypadBacklight_Inactive);
         }
         else
         {
            iState = KeypadBacklight_Normal;
         }


         CurrentReg[iPortID[i]] = iState;
      }
   }

   i2c_start();
   i2c_write(I2CADDRESS_MAX6956_WRITE);
   i2c_write(0x16);
   for (i=0; i<18;i+=2)
   {
      iState = CurrentReg[i] + (CurrentReg[i+1] << 4);
      i2c_write(iState);
   }

   i2c_stop();
}


//=========================================================
// AutoKeypadBacklight
// --------------------
// Description : Read the ambiant light intensity and set the
//               keypad backlight state in consequence.
//
// Parameters  : *none*
//
// Return      : *none*
//=========================================================
Void AutoKeypadBacklight(void)
{
   //**** Read the photoresistor level (0-255) ****
   LastPhotoLevel = read_adc();

   if (Config_General.AutoKeypadLightsEnabled)
   {
      //**** The level is above the auto off threshold ****
      if ((LastPhotoLevel >= (BackLight_Auto_Threshold+Backlight_Auto_Offset)) && (Backlight_On))
      {
         Backlight_On = false;
         SetKeypadBacklight();
      }

      //**** The level is below the auto on threshold ****
      if ((LastPhotoLevel <= (BackLight_Auto_Threshold-BackLight_Auto_Offset)) && (!Backlight_On))
      {
         Backlight_On = true;
         SetKeypadBacklight();
      }
   }
   else
   {
      if (!Backlight_On)
      {
         BackLight_On = true;
         SetKeypadBacklight();
      }
   }
}


//=========================================================
// ActivateStandbyMode
// --------------------
// Description : Put the device in standby mode
//
// Parameters  : bSleep : True if sleep mode false if standard standby
//
// Return      : *none*
//=========================================================
void ActivateStandbyMode(short bSleep)
{
   int PrevLedDisplay;


   bOnStandby = true;

   Lcd_write(0, 0b00001100);

   if (bSleep)
   {
      lcd_putc("\fShutting down...");
      delay_ms(1000);
   }

   //**** Turn LCD off ****
   ClearLcdDisplay();
   Lcd_write(0, 0b00001000);

   //**** Turn off lcd and keypad backlight ****
   TurnOffLcdBacklight();
   WritetoLedDriver(MAX6956_COMMAND_PORT12_19, 0x00);
   WritetoLedDriver(MAX6956_COMMAND_PORT22_29, 0x00);

   //**** Only keeps the standby button's led on ****
   if (!bSleep)
   {
      WritetoLedDriver(MAX6956_COMMAND_PORT19, 0x01);
      SetKeypadBacklight();
   }

   //**** Save what's on the led display ****
   PrevLedDisplay = CurrentLedDisplay_Character;

   //**** Turn off segment display ****
   WriteToSegmentDisplay(LedDisplay_off);

   //**** Turn off video & audio crosspoint ****
   if (bSleep)
   {
      UpdateVideoCrosspoint(0xAA);
      UpdateAudioCrosspoint(false);
   }

   //**** Loop until the a key is pressed again ****
   Keypad_CharID = -1;

   While (Keypad_CharID == -1 && bOnStandby)
   {
      //**** Execute the pending command in command buffer ****
      ProcessCommandBuffer();

   }
   bOnStandby = false;
   Keypad_CharID = -1;


   //**** Restore video & audio crosspoint ****
   if (bSleep)
   {
      UpdateVideoCrosspoint(0);
      UpdateAudioCrosspoint(True);
   }


   //**** Restore what was on the led display ****
   WriteToSegmentDisplay(PrevLedDisplay);

   //**** Turn LCD back on ****
   Lcd_write(0, 0b00001100);

   //**** Turn back on lcd backlight ****
   TurnOnLcdBacklight();
   WritetoLedDriver(MAX6956_COMMAND_PORT12_19, 0xFF);
   WritetoLedDriver(MAX6956_COMMAND_PORT22_29, 0xFF);
}




//******************************************************************************
//******************************************************************************
// Keypad functions
//******************************************************************************
//******************************************************************************

//=========================================================
// WaitForKeypadChar
// --------------------
// Description : Wait until a key code is received
//
// Parameters  : *none*
//
// Return      : True if a key was pressed
//               False if an event interrupted
//=========================================================
short WaitForKeypadChar(void)
{


   //**** Empty the keypad buffer ****
   Keypad_CharID = -1;

   //**** Reset auto standby & idle timer ****
   SleepTimer = 0;
   StandbyTimer = 0;

   AutoKeypadBacklight();
   SetKeypadBacklight();

   While (true)
   {
      //**** Execute the pending command in command buffer ****
      ProcessCommandBuffer();

      //**** Automatically turn on or off the keypad backlight ****
      AutoKeypadBacklight();

      if (bOnStandby == True) Keypad_CharID = Char_Standby;

      //**** Auto standby timer ****
      if (LastphotoLevel > Backlight_AutoStandbyLevel) StandbyTimer = 0;
      if ((StandbyTimer > TimeValues[TimeBefore_autoStandby]) && Config_General.AutoStandbyEnabled)
      {
         ActivateStandbyMode(False);
         return false;
      }

      //**** Auto sleep timer ****
      if ((SleepTimer > TimeValues[TimeBefore_Sleep]) && Config_General.AutoSleepEnabled)
      {
         //**** Activate sleep mode only if all channels are cleared ****
         if (ChannelsCleared()) ActivateStandbyMode(True);
         return false;
      }

      //**** Check if a key was pressed ***
      if ((Keypad_CharID > -1) && (Keypad_CharID <= 0xF))
      {
         //**** Request standby mode ****
         if (Keypad_CharID == Char_Standby)
         {
            ActivateStandbyMode(Keypad_KeyShift);
            Return false;
         }

         //**** Check if the key is currently active ****
         if (Bit_Test(Keypad_ActiveKey, Keypad_CharID))
         {
            return true;
         }
         else
         {
            //TODO : Invalid key feedback (BEEP)

            //**** Empty the buffer and continue to scan ****
            Keypad_CharID = -1;
         }
      }
   }
}





//******************************************************************************
//******************************************************************************
// Configuration functions
//******************************************************************************
//******************************************************************************
//=========================================================
// LoadConfiguration
// --------------------
// Description : Load configuration from EEPROM
//
// Parameters  : *none*
//
// Return      : *none*
//=========================================================
void LoadConfiguration(void)
{
   int i;
   int iCursor;
   int iData;

   for (i=0;i<15;i++) LoadChannelName(i);

   iData = READ_EEPROM(MEMLOC_CONFIG_SLOT1);
   DefaultStartupPreset = iData & 0xF;
   DefaultStartupMonitorChannel = (iData >> 4) & 0xF;

   Config_General = READ_EEPROM(MEMLOC_CONFIG_SLOT2);

   BackLight_Auto_Threshold = READ_EEPROM(MEMLOC_CONFIG_SLOT3);
   BackLight_Auto_offset = READ_EEPROM(MEMLOC_CONFIG_SLOT4);
   BackLight_AutoStandbyLevel = READ_EEPROM(MEMLOC_CONFIG_SLOT5);

   iData = Read_EEPROM(MEMLOC_CONFIG_SLOT6);
   TimeBefore_AutoStandby = iData & 0xF;
   TimeBefore_Sleep = (iData >> 4) & 0xF;

   for (i=0,iCursor=0;i<6;i+=2,iCursor++)
   {
      iData = Read_EEPROM(MEMLOC_CONFIG_SLOT7+iCursor);
      InhibitedSource[i] = iData & 0xF;
      InhibitedSource[i+1] = (iData >> 4) & 0xF;
   }
   inhibitedSource[6] = READ_EEPROM(MEMLOC_CONFIG_SLOT10);

   AutoKeypadBacklight();
   SetKeypadBacklight();
}


//=========================================================
// SaveConfiguration
// --------------------
// Description : Load configuration to EEPROM
//
// Parameters  : *none*
//
// Return      : *none*
//=========================================================
void SaveConfiguration(void)
{
   int i;
   int iCursor;

   SaveChannelNames();

   WRITE_EEPROM(MEMLOC_CONFIG_SLOT1, DefaultStartupPreset + (DefaultStartupMonitorChannel << 4));

   WRITE_EEPROM(MEMLOC_CONFIG_SLOT2, *&Config_General);

   WRITE_EEPROM(MEMLOC_CONFIG_SLOT3, BackLight_Auto_Threshold);
   WRITE_EEPROM(MEMLOC_CONFIG_SLOT4, BackLight_Auto_offset);
   WRITE_EEPROM(MEMLOC_CONFIG_SLOT5, BackLight_AutoStandbyLevel);

   WRITE_EEPROM(MEMLOC_CONFIG_SLOT6, TimeBefore_AutoStandby + (TimeBefore_Sleep << 4));

   for (i=0,iCursor=0;i<6;i+=2,iCursor++)
   {
      WRITE_EEPROM(MEMLOC_CONFIG_SLOT7+iCursor, InhibitedSource[i]  + (InhibitedSource[i+1] << 4));
   }
   WRITE_EEPROM(MEMLOC_CONFIG_SLOT10, InhibitedSource[6]);
}



//=========================================================
// ReadConfig
// --------------------
// Description : REad configuration item from RAM only.
//
// Parameters  : ConfigID : Configuration slot
//
// Return      : Value of the configuration item
//=========================================================
int ReadConfig(int ConfigID)
{
   switch(ConfigID)
   {
      case ConfigID_StartupPreset:
         Return DefaultStartupPreset;

      case ConfigID_StartupMonitorChannel:
         Return DefaultStartupMonitorChannel;

      case ConfigID_UartSpeed:
         Return Config_General.UartSpeed;

      case ConfigID_AutoStandbyEnabled:
         Return Config_General.AutoStandbyEnabled;

      case ConfigID_AutoSleepEnabled:
         Return Config_General.AutoSleepEnabled;

      case ConfigID_AutoKeypadLights:
         Return Config_General.AutoKeypadLightsEnabled;

      case ConfigID_DimInactiveKeys:
         Return Config_General.DimInactiveKeys;

      case ConfigID_AutoMonitorSelect:
         Return Config_General.EnableAutoMonitorSelect;

      case ConfigID_Backlight_AutoThreshold:
         Return BackLight_Auto_Threshold;

      case ConfigID_Backlight_AutoOffset:
         Return BackLight_Auto_Offset;

      case ConfigID_AutoStandbyLevel:
         Return BackLight_AutoStandbyLevel;

      case ConfigID_TimeBeforeStandby:
         return TimeBefore_AutoStandby;

      case ConfigID_TimeBeforeSleep:
         Return TimeBefore_Sleep;

      case ConfigID_InhibitedSource1:
      case ConfigID_InhibitedSource2:
      case ConfigID_InhibitedSource3:
      case ConfigID_InhibitedSource4:
      case ConfigID_InhibitedSource5:
      case ConfigID_InhibitedSource6:
      case ConfigID_InhibitedSource7:
         Return InhibitedSource[ConfigID - ConfigID_InhibitedSource1];

      default:
         Return 0;
   }
}




//=========================================================
// WriteConfig
// --------------------
// Description : Write configuration item to RAM only.
//
// Parameters  : ConfigID : Configuration slot
//               Value : Value to write
//
// Return      : *none*
//=========================================================
void WriteConfig(int ConfigID, int Value)
{
   switch(ConfigID)
   {
      case ConfigID_StartupPreset:
         if (Value > 9) Value = 9;
         DefaultStartupPreset = Value;
         break;

      case ConfigID_StartupMonitorChannel:
         if (Value > 7) Value = 7;
         DefaultStartupMonitorChannel = Value;
         break;

      case ConfigID_UartSpeed:
         if (Value > 6) Value = 6;
         Config_General.UartSpeed = Value;
         break;

      case ConfigID_AutoStandbyEnabled:
         Config_General.AutoStandbyEnabled = Value;
         break;

      case ConfigID_AutoSleepEnabled:
         Config_General.AutoSleepEnabled = Value;
         break;

      case ConfigID_AutoKeypadLights:
         Config_General.AutoKeypadLightsEnabled = Value;

         AutoKeypadBacklight();
         SetKeypadBacklight();
         break;

      case ConfigID_DimInactiveKeys:
         Config_General.DimInactiveKeys = Value;
         SetKeypadBacklight();
         break;

      case ConfigID_AutoMonitorSelect:
         Config_General.EnableAutoMonitorSelect = Value;
         break;

      case ConfigID_Backlight_AutoThreshold:
         if (Value < 5) Value = 5;
         BackLight_Auto_Threshold = Value;
         break;

      case ConfigID_Backlight_AutoOffset:
         if (Value > 100) Value = 100;
         if (Value < 5) Value = 5;

         BackLight_Auto_Offset = Value;
         break;

      case ConfigID_AutoStandbyLevel:
         if (Value < 5) Value = 5;
         BackLight_AutoStandbyLevel = Value;
         break;

      case ConfigID_TimeBeforeStandby:
         if (Value > 15) Value = 15;
         TimeBefore_AutoStandby = Value;
         break;

      case ConfigID_TimeBeforeSleep:
         if (Value > 15) Value = 15;
         TimeBefore_Sleep = Value;
         break;

      case ConfigID_InhibitedSource1:
      case ConfigID_InhibitedSource2:
      case ConfigID_InhibitedSource3:
      case ConfigID_InhibitedSource4:
      case ConfigID_InhibitedSource5:
      case ConfigID_InhibitedSource6:
      case ConfigID_InhibitedSource7:
         if (Value > 7) Value = 7;
         InhibitedSource[ConfigID - ConfigID_InhibitedSource1] = Value;
         break;
   }

   //**** Make sure auto-backlight threshold is legal ****
   if ((signed int16)Backlight_Auto_Threshold + Backlight_Auto_Offset > 255)
   {
      Backlight_Auto_Threshold = (255 - Backlight_Auto_Offset);
   }
   if ((signed int16)Backlight_Auto_Threshold - Backlight_Auto_Offset < 5)
   {
      Backlight_Auto_Threshold = (5 + Backlight_Auto_Offset);
   }
}



//=========================================================
// LoadPreset
// --------------------
// Description : Load channel routing preset from EEPROM
//
// Parameters  : -PresetID : The preset to load
//
// Return      : *none*
//=========================================================
void LoadPreset(int PresetID)
{
   int iPresetdata;
   int iVideoSource = 0;
   int iAudioSource = 0;
   int i;

   for (i=0; i<7; i++)
   {
      if (PresetID != 0)
      {
         iPresetData = READ_EEPROM(MEMLOC_PRESET + ((PresetID - 1) * 7) + i);
         iVideoSource = iPresetData & 0xF;
         iAudioSource = (iPresetData >> 4) & 0xF;

         if (iVideoSource < 7) ChannelStatus[i] = iVideoSource;
         if (iAudioSource < 7) ChannelStatus[i+7] = iAudioSource;
      }
      else
      {
         ChannelStatus[i] = 0;
         ChannelStatus[1+7] = 0;
      }
   }
}


//=========================================================
// LoadChannelName
// --------------------
// Description : Read the name of a channel from EEPROM
//
// Parameters  : -ChannelID : The channel id
//
// Return      : *none*
//=========================================================
void LoadChannelName(int ChannelID)
{
   int i;
   for (i=0;i<5;i++)
   {
      ChannelNames[ChannelID][i] = READ_EEPROM(MEMLOC_CHANNELNAMES + (ChannelID * 5) + i);
   }
}

//=========================================================
// SaveChannelName
// --------------------
// Description : Save the name of all channel to EEPROM
//
// Parameters  : *none*
//
// Return      : *none*
//=========================================================
void SaveChannelNames()
{
   int i;
   int iChannel;
   for (iChannel=0;iChannel<15;iChannel++)
   {
      for (i=0;i<5;i++)
      {
         Write_EEPROM(MEMLOC_CHANNELNAMES + (iChannel * 5) + i, ChannelNames[iChannel][i]);
      }
   }
}


//=========================================================
// IsChannelNameEmpty
// --------------------
// Description : Test if the
//
// Parameters  : -ChannelID : The channel name id
//
// Return      : True if empty
//=========================================================
short IsChannelNameEmpty(int ChannelID)
{
   int i;
   for (i=0;i<4;i++)
   {
      if (ChannelNames[ChannelID][i] != 32) Return false;
   }

   return true;
}






//******************************************************************************
//******************************************************************************
// Remote control interface functions
//******************************************************************************
//******************************************************************************
//=========================================================
// ProcessCommandBuffer
// --------------------
// Description : Execute the command in command buffer
//               and prepare the buffer for the next cycle
//
// Parameters  : *none*
//
// Return      : *none*
//=========================================================
void ProcessCommandBuffer(void)
{
   int iParamLength;
   int i;

   //**** Return if the buffer is empty ****
   if (!CommandBuffer_full) return;

   //**** The first character in buffer is the command id, the rest are
   //**** Parameters
   #define iCommandID CommandBuffer_Buffer[0]
   iParamLength = CommandBuffer_Length - 1 ;

   //**** Reset the command buffer ****
   CommandBuffer_Cursor = 0;
   CommandBuffer_Length = 0;
   CommandBuffer_Start = false;
   CommandBuffer_Full = false;

   //--- This is the default reply code ---
   ReplyBlockCode = CodeID_Executed;
   ReplyBlockLength = 0;

   switch (iCommandID)
   {
      //--- 0x01 : Write channel name ---
      case CommandID_SetChannelName:
         //**** Check if all parameters are present ****
         if (iParamLength != 75)
         {
            ReplyBlockCode = ErrorID_MissingParameter;
            break;
         }

         //**** Copy the name list from command buffer ****
         memcpy(&ChannelNames[0][0], &CommandBuffer_Buffer[1], 75);

         //**** Save the new channel names to EEPROM ****
         SaveChannelNames();

         break;

      //--- 0x02 : Read channel name ---
      case CommandID_ReadChannelName:
         ReplyBlockLength = 75;

         //**** Copy the name list to reply block ****
         memcpy(&ReplyblockData[0], &ChannelNames[0][0], 75);
         break;

      //--- 0x03 : Get channel status (All channels) ---
      case CommandID_GetChannelStatus:

         ReplyBlockLength = 14;
         for (i=0;i<14;i++)
         {
            ReplyBlockData[i] = ChannelStatus[i];
         }
         break;

      //--- 0x05 : Send LCD content to the host ---
      case CommandID_ReadLcdContent:
         ReplyBlockLength = 32;

         //**** Copy the local lcd buffer to reply block ****
         memcpy(&ReplyblockData[0], &Lcd_Buffer[0], 32);
         break;

      //--- 0x06 : Read the photoresistor level ---
      case CommandID_GetPhotoLevel:
         ReplyBlockLength = 1;
         ReplyblockData[0] = Read_adc();
         break;

      //--- 0x07 : Update configuration item ---
      case CommandID_WriteConfig:
         //**** Check if all parameters are present ****
         if (iParamLength < 2)
         {
            ReplyBlockCode = ErrorID_MissingParameter;
            break;
         }

         WriteConfig(CommandBuffer_Buffer[1], CommandBuffer_Buffer[2]);
         break;

      //--- 0x08 : Read configuration item ---
      case CommandID_ReadConfig:
          //**** Check if all parameters are present ****
         if (iParamLength < 1)
         {
            ReplyBlockCode = ErrorID_MissingParameter;
            break;
         }

         ReplyBlockLength = 1;
         ReplyBlockData[0] = ReadConfig(CommandBuffer_Buffer[1]);
         break;

      //--- 0x09 : Save configuration to eeprom ---
      case CommandID_SaveConfig:
         SaveConfiguration();
         break;

      //--- 0x0A : Reload configuration from eeprom ---
      case CommandID_LoadConfig:
         LoadConfiguration();
         break;

      //--- 0xFA : Activate standby mode ---
      case CommandID_StandBy:
         bOnStandby =! bOnStandby;
         break;

      //--- 0xFB : Get version ---
      case CommandID_GetVersion:
         ReplyBlockLength = 4;
         strcpy(ReplyBlockData, AppCurrentVersion);
         break;


      //--- 0xFC : Send keystroke ---
      case CommandID_KeyStroke:
         #define iKeyStroke   CommandBuffer_Buffer[1]

         //**** Quit standby mode ****
         bOnStandby = false;

         if (Keypad_CharID != -1)
         {
            ReplyBlockCode = ErrorID_KeypadBufferFull;
         }
         else
         {
            Keypad_CharID = iKeyStroke;
            Keypad_KeyShift = false;
         }
         break;


      //--- 0xFD : Ping test ---
      case CommandID_PING:
         break;

      //--- 0xFE : Reset CPU ---
      case CommandID_ResetCPU:
         SendReplyBlock();
         delay_ms(25);

         reset_cpu();

      //--- 0xFF : Update Firmware ---
      case CommandID_UpdateFirmware:
         //**** Quit standby mode ****
         bOnStandby = false;

         SendReplyBlock();

         PrepareFirmwareUpdate();
         break;


      //--- Invalid command ---
      default:

         //**** Notify the host that the command was invalid ****
         ReplyBlockCode = ErrorID_InvalidCommand;
   }

   //**** Send the reply block ****
   SendReplyBlock();

   //**** Enable receiving ****
   OUTPUT_LOW(Pin_ClearToSend);
}


//=========================================================
// SendReplyBlock
// --------------------
// Description : Send a reply code the the host
//
// Parameters  : -Code
//
// Return      : *none*
//=========================================================
void SendReplyBlock(void)
{
   int i;

   //**** Reply block start character ****
   putc(0x10);

   //**** Reply block length ****
   putc(ReplyBlockLength + 1);

   //**** Reply code ****
   putc(ReplyBlockCode);

   if (ReplyBlockLength > 0)
   {
      for(i=0;i<ReplyBlockLength;i++)
      {
         putc(ReplyBlockData[i]);
      }
   }

   //**** Reply block stop character ****
   putc(0x11);

   OUTPUT_LOW(Pin_Diag_Led);
}

//=========================================================
// SendEvent
// --------------------
// Description : Send an event the the host
//
// Parameters  : -Event code
//
// Return      : *none*
//=========================================================
void SendEvent(int EventID)
{
   //--- This is the default reply code ---
   ReplyBlockCode = EventID;
   ReplyBlockLength = 1;
   ReplyBlockData[0] = EventID;

   SendReplyBlock();
}




//******************************************************************************
//******************************************************************************
// Communication functions
//******************************************************************************
//******************************************************************************

//=========================================================
// putc_ex
// --------------------
// Description : Same as putc except that it wait until the host
//               ready to receive the character
//
// Parameters  : -iChar : Character to send
//
// Return      : *none*
//=========================================================
void putc_ex(char iChar)
{
   //**** Wait until the host is ready to receive ****
   while (input(Pin_RequestToSend));

   putc(iChar);
}






//******************************************************************************
//******************************************************************************
// Firmware update
//******************************************************************************
//******************************************************************************
#include "avs50_loader.h"

//=========================================================
// PrepareFirmwareUpdate
// --------------------
// Description : Prepare the microcontroller for a firmware
//               update
//
// Parameters  : *none*
//
// Return      : *none*
//=========================================================
void PrepareFirmwareUpdate(void)
{
   //**** Disable all interrupts ****
   disable_interrupts(int_rda);
   disable_interrupts(int_timer2);
   disable_interrupts(global);

   lcd_putc("\fUpdating\rfirmware...");

   //**** Turn on cursor blink ****
   Lcd_write(0, 0b00001101);

   Keypad_ActiveKey = KeySet_Boot;
   SetKeypadBacklight();

   delay_ms(250);

   //**** Begin the firmware update procedure ****
   UpdateFirmware(Config_General.UartSpeed);
}



