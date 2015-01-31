//**********************************************************************
// Lcd initialization sequence
//**********************************************************************
BYTE const LCD_INIT[4] = {
0b00111000,    // 0 0 1 DL N F * *
               // DL=1 : 8 bits interface
               // N=1  : 1/16 duty (2 lines)
               // F=0  : 5x7 dots characters

0b00001100,    // 0 0 0 0 1 D C B
               // D=1  : Display on
               // C=0  : Cursor off
               // B=0  : Cursor blink off

0b00000001,    // 0 0 0 0 0 0 0 1
               // Clear display

0b00000110};   // 0 0 0 0 0 1 I/D S
               // I/D=1 : Increment cursor position
               // S=0   : No display shift


BYTE Lcd_Buffer[32];
int Lcd_BufferIDX = 0;
BOOLEAN Lcd_NoEcho = false;



//**********************************************************************
// Lcd functions
//**********************************************************************
//=========================================================
// Lcd_Write
// --------------------
// Description : Send data to the lcd
//
// Parameters  : -Mode  : Register select (0=Instruction, 1=Data)
//               -iChar : The byte to send
//
// Return      : *none*
//=========================================================
void Lcd_write(int8 Mode, BYTE iChar)
{
   int i;

   for (i=1;i<=8;i++)
   {
      //**** Shift out the next bit ****
      Output_Bit(Pin_LcdData, Shift_left(&iChar, 1, 0));

      //**** Send a clock pulse ****
      Output_High(Pin_LcdClock);
      Delay_cycles(1);
      Output_Low(Pin_LcdClock);
   }

   //**** Select transmision mode (0=Instruction, 1=Data) ****
   Output_BIT(Pin_LcdData, Mode);
   Delay_us(1);

   //**** Send a clock pulse ****
   Output_High(Pin_LcdEnable);
   Delay_cycles(1);
   Output_Low(Pin_LcdEnable);

   delay_us(50);
}

//=========================================================
// InitializeLCD
// --------------------
// Description : LCD initialization sequence
//
// Parameters  : *none*
//
// Return      : *none*
//=========================================================
void InitializeLCD(void)
{
   int i;

   //**** Wait at least 15ms after power up of LCD ****
   delay_ms(15);

   //**** Function set (8 bits interface) ****
   for (i=0;i<=2;i++)
   {
      lcd_write(0, 0b0011);

      delay_ms(5);
   }

   //**** Function set (4 bits interface) ****
   lcd_write(0, 0b0010);
   delay_ms(50);

   //**** Function set (8 bit interface, number of lines, font) ****
   for(i=0;i<=3;++i)
   {
       lcd_write(0, LCD_INIT[i]);
   }
}


//=========================================================
// lcd_putc
// --------------------
// Description : Send a character to the lcd's DDRAM
//
// Parameters  : -iChar : The character to send
//
// Return      : *none*
//=========================================================
void lcd_putc(char iChar)
{
   int i;

   switch (iChar) {

      //**** Clear screen ****
      case '\f':
         Lcd_Write(0,1);
         delay_ms(2);

         //**** Clear the local buffer ****
         for (i=0; i<32; i++) Lcd_Buffer[i] = 32;
         Lcd_bufferIDX = 0;

         break;

      //**** Change line ****
      case '\n':
      case '\r':
         //**** Set lcd DDRAM address to the begining of the second line (0x40) ****
         Lcd_Write(0, 0x80 | 0x40);

         //**** Set the local buffer position accordingly ****
         Lcd_BufferIDX = 16;
         break;

      //**** Normal character output ****
      default:
         Lcd_Write(1, iChar);

         lcd_Buffer[Lcd_BufferIDX] = iChar;
         lcd_BufferIDX++;
         break;
   }

   //**** Echo the character back to the host ****
   if (!Lcd_NoEcho) putc(ichar);
}
