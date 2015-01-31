//*****************************************************************************
// Defines
//*****************************************************************************
#define LOADER_SIZE                 1450
#define LOADER_END                  getenv("PROGRAM_MEMORY")-1
#define LOADER_START                LOADER_END-LOADER_SIZE
#define BUFFER_MAX_SIZE             64




//=========================================================
// UpdateFirmware
// --------------------
// Description : Begin firmware update process
//
// Parameters  : *none*
//
// Return      : *none*
//=========================================================
#ORG LOADER_START+18, LOADER_END auto=0 Default


//*****************************************************************************
// Function prototype
//*****************************************************************************
#SEPARATE
unsigned int HexToInt(char *HexString);



//*****************************************************************************
// Variables
//*****************************************************************************
int  Buffer_Cursor;
char Buffer[BUFFER_MAX_SIZE];


//*****************************************************************************
// Built-in functions
//*****************************************************************************
// IMPORTANT! : A variable or a function is required beetween #ORG statement
//              and these statements below in order for those built-in functions
//              to be included with the bootloader, otherwise, they will remain
//              in lower memory and cause a terrible bug when the bootloader
//              tries to overwrite them.
//*****************************************************************************
#use delay(clock=McuClock)
#use rs232(baud=DefaultUartSpeed,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8,errors)

void UpdateFirmware_real(int UartSpeed)
{
   BOOLEAN bDone;
   BOOLEAN bSuccess;
   unsigned int8 iLine_Type;
   unsigned int8 iBlockSize;
   unsigned int16 iAddress_Low, iAddress_High;
   unsigned int32 iAddress;
   unsigned int8 iBuffer_Checksum;

   int8 i;
   int8 iDataIdx;
   unsigned int16 aData[32];
   unsigned int16 idata_check;
   
   //**** Set UART speed ****
   switch (UartSpeed)
   {
      Case 0:  Set_Uart_Speed(2400); break;
      Case 1:  Set_Uart_Speed(4800); break;
      Case 2:  Set_Uart_Speed(9600); break;  
      Case 4:  Set_Uart_Speed(38400); break;
      Case 5:  Set_Uart_Speed(57600); break;        
      Case 6:  Set_Uart_Speed(115200); break;
      Default: Set_Uart_Speed(19200);      
   }

   iAddress_High = 0;
   bDone = false;
   bSuccess = true;

   while ((bDone == false) && (bSuccess == true))
   {

      //**** Resume sender ****
      OUTPUT_LOW(pin_Diag_Led);
      OUTPUT_LOW(Pin_ClearToSend);

      //**** Read into the buffer until 0x0D ('\r') is received or the buffer is full
      Buffer_Cursor = 0;
      do
      {
         Buffer[Buffer_Cursor++] = getc();
      }
      while(Buffer[Buffer_Cursor-1] != 0x0D);

      //**** Suspend sender ****
      OUTPUT_HIGH(Pin_ClearToSend);
      OUTPUT_HIGH(Pin_Diag_Led);

      //**** Only process the data blocks starting with a ":" ****
      if (Buffer[0] == ':')
      {

         //**** Get the size of the block (only data) ****
         iBlockSize = HexToInt(&Buffer[1]);

         //**** Get the checksum value of the data block ****
         iBuffer_Checksum = 0;
         for (i=1; i<(Buffer_Cursor-3); i+=2) iBuffer_Checksum += HexToInt(&buffer[i]);
         iBuffer_Checksum = 0xFF - iBuffer_Checksum + 1;

         iLine_Type = HexToInt(&Buffer[7]);

         //**** Compare this checksum with the one in data block ****
         if (iBuffer_Checksum != HexToInt(&buffer[Buffer_Cursor-3]))
         {
            //**** Checksum failed, terminate firmware update ****
            bSuccess = false;
            putchar(ErrorID_Bootloader_BadCRC);
         }
         else
         {
            if (iLine_Type == 1)
            {
               //**** This is and EOF (End of file) ****
               bDone = true;
            }
            else if (iLine_Type == 4)
            {
               //**** Establish address extention ****
               iAddress_high = make16(HexToInt(&Buffer[9]), HexToInt(&Buffer[11]));
            }
            else
            {
               iAddress_low = make16(HexToInt(&Buffer[3]), HexToInt(&Buffer[5]));
					iAddress = make32(iAddress_high, iAddress_low);

               // Address is in the valid firmware area
               if (iAddress < LOADER_START)
               {
                  for (i = 9, iDataIdx = 0; i < (Buffer_Cursor - 3); i += 4)
                  {
                     //**** Build the 16 bit data from two 8 bit fields****
                     aData[iDataIdx++] = make16(HexToInt(&Buffer[i + 2]), HexToInt(&Buffer[i]));
                  }

                  //**** Write block to program memory ****
                  write_program_memory(iAddress, aData, iBlockSize);

                  //**** Verify the block ****
                  for (i = 0; i < iDataIdx; i++)
                  {
                     iData_check = read_Program_EEPROM(iAddress);
                     iAddress = iAddress + 2;

                     if (iData_check != aData[i])
                     {
                        //**** Write failed, terminate firmware update ****
                        bsuccess = false;
                        putchar(ErrorID_Bootloader_BadWRITE);

                        break;
                     }
                  }
               }
            }
         }
      }

      //**** Send the acknowledge code ****
      if (bSuccess) Putchar(CodeID_Acknowledge);
   }

   //**** Give time to the microcontroller to finish sending acknowledge code ****
   delay_ms(25);


   if(!bsuccess)
   {
      OUTPUT_HIGH(PIN_B4);
      while(true);
   }

   //**** Once the new firmware is uploaded, reset the microcontroller ****
   reset_cpu();
}

//=========================================================
// HexToInt
// --------------------
// Description : Convert two hex characters to a int8
//
// Parameters  : HexString : Hex string to be converted
//
// Return      : The resulting integer
//=========================================================
#Separate
unsigned int HexToInt(char *HexString)
{
   unsigned int result = 0;
   int i;

   for (i=0; i<2; i++,HexString++)
   {
      if (*HexString >= 'A')
         result = 16*result + (*HexString) - 'A' + 10;
      else
         result = 16*result + (*HexString) - '0';
   }

   return(result);
}


#ORG LOADER_START, LOADER_START+16
void UpdateFirmware(int UartSpeed)
{
   UpdateFirmware_real(UartSpeed);
}


#ORG DEFAULT
