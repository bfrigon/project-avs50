#include <12F675.h>
#use delay(clock=16000000)
#fuses NOWDT,HS, NOCPD, NOPROTECT,  NOPUT, BROWNOUT

#use fast_io(A)

#byte PortA = 0x5
#bit BlueScreenEnabled = PortA.0
#bit BlueSignal = PortA.1
#bit SYNC = PortA.2


#define SYNC_LEVEL   BIT_CLEAR(PORTA,2);
#define BLANK_LEVEL  BIT_SET(PORTA,2);


BOOLEAN OddField = false;

void main()
{
   int i;

   setup_adc_ports(0);
   setup_comparator(NC_NC_NC_NC);

   Set_tris_a(0b00000001);
   PortA = 0;

   //***********************************
   // Vertical blanking interval
   //***********************************
   while(true)
   {
      //**** Pre-equalizing pulses (6) ****
      SYNC_LEVEL;
      delay_cycles(9);
      BLANK_LEVEL;
      delay_cycles(110);
      for(i=0;i<5;i++)
      {
         delay_cycles(1);
         SYNC_LEVEL;
         delay_cycles(9);
         BLANK_LEVEL;
         delay_cycles(108);
      }

      //**** Serration pulses (6) ****
      SYNC_LEVEL;
      delay_cycles(108);
      BLANK_LEVEL;
      delay_Cycles(11);
      for(i=0;i<5;i++)
      {
         delay_cycles(1);
         SYNC_LEVEL;
         delay_cycles(108);
         BLANK_LEVEL;
         delay_cycles(9);
      }

      //**** Post-equalizing pulses (6) ****
      SYNC_LEVEL;
      delay_cycles(9);
      BLANK_LEVEL;
      delay_cycles(107);
      for(i=0;i<5;i++)
      {
         delay_cycles(4);
         SYNC_LEVEL;
         delay_cycles(9);
         BLANK_LEVEL;
         delay_cycles(105);
      }


      //**** Blank half line (Even field) ****
      if (!OddField)
      {
         delay_cycles(128);
      }

      //**** Blank video lines (10) ****
      SYNC_LEVEL;
      delay_cycles(19);
      BLANK_LEVEL;
      delay_cycles(226);
      for(i=0;i<8;i++)
      {
         delay_cycles(1);
         SYNC_LEVEL;
         delay_cycles(19);
         BLANK_LEVEL;
         delay_cycles(224);
      }
      SYNC_LEVEL;
      delay_cycles(19);
      BLANK_LEVEL;
      delay_cycles(226);


      //***********************************
      // Visible video lines (243)
      //***********************************
      for (i=0;i<243;i++)
      {
         delay_cycles(1);
         PortA = 0;
         delay_cycles(19);
         BLANK_LEVEL;
         delay_cycles(15);

         if (BlueScreenEnabled) BlueSignal = 1;
         delay_cycles(206);
         
         BlueSignal = 0;
      }

      PortA = 0;
      delay_cycles(19);
      BLANK_LEVEL;
      delay_cycles(15);
      if (BlueScreenEnabled) BlueSignal = 1;
      delay_cycles(206);
      BlueSignal = 0;
      delay_cycles(1);

      //***********************************
      // Blank half line (Odd field)
      //***********************************
      if (OddField)
      {
         delay_cycles(5);
         SYNC_LEVEL;
         delay_cycles(19);
         BLANK_LEVEL;
         delay_cycles(101);
      }

      //**** Toogle odd/even field ***
      OddField = !oddField;
   }
}
