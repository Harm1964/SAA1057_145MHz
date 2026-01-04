// software voor SAA1057 PLL met Arduino Uno TX 145.675MHz
// 22 febr. 2025

/*
Dit is een test voor de SAA1057 om te kijken of die ook op 145.675MHz wil werken....
Update: Ja dat kan hij, tot 160MHz gaat goed (24 febr. 2025)
*/

#include <avr/interrupt.h>
#include <avr/sleep.h>

#define CLB 5
#define DLEN 6
#define DATA 7

bool DataBit = false;
unsigned int  DividerValue = 11648;  //Deler waarde voor 145,675MHz
//unsigned int  DividerValue = 21845;  //Deler waarde voor test 101010101010101
//unsigned int  DividerValue = 10880;  //Deler waarde voor 136,000MHz 
int bp;


void setup() {
  // put your setup code here, to run once:

pinMode(5,OUTPUT);
pinMode(6,OUTPUT);
pinMode(7,OUTPUT);

digitalWrite(CLB, LOW);   //Init CLB
digitalWrite(DLEN, LOW);  //Init DLEN
digitalWrite(DATA, LOW);  //Init DATA

delay(10);

for (int i=0; i < 16; i++)
  {
    sendbit(false);  // The stuff to repeat
  }
sendbit(false);  //init SAA1057, see page 11 datasheet
sendbit(false);  // 16 clock pulses tot init the device
sendbit(false);
sendbit(false);
sendbit(false);
sendbit(false);
sendbit(false);
sendbit(false);
sendbit(false);
sendbit(false);
sendbit(false);
sendbit(false);
sendbit(false);
sendbit(false);
sendbit(false);
sendbit(false);

delay(10);
//-----------------------------------------------
//start programming

digitalWrite(DATA, LOW);
digitalWrite(DLEN, HIGH);
delay(1);

sendbit(false);   //leading zero
delay(1);

// send control information word B
sendbit(true);    // first true control word B
sendbit(true);   // FM/AM   (true = FM input active)
sendbit(true);   // REFH  1kHz/1,25 kHz (1,25 kHz = 40kHz op de testpin (/32 = 1,25kHz))
sendbit(true);   // CP3
sendbit(true);   // CP2
// Both CP0 and CP1 zero means 0,023 gain(lowest)
sendbit(true);   // CP1  only CP1=true =0,23x 
sendbit(false);    // CP0  only CP0=true =0,07x
sendbit(true);    // SB2   true for last 8 bits active
sendbit(false);   // SLA
sendbit(false);   // PDM1  if true then only 32kHz/40kHz dig. phase detector
sendbit(false);   // PDM0
sendbit(false);   // BRM
sendbit(false);   // T3
sendbit(true);    // T2    T2 + T0 : Test bit = lock status
sendbit(false);   // T1
sendbit(true);    // T0     Only T0: Test bit = programmeble counter (1,25kHz signaal op pin 18)

digitalWrite(DLEN, LOW);
sendbit(false);   //Belangrijk!!!!!! LOAD-PULS Laten staan
//einde word B

delay(10);  // vertraging tussen beide datawoorden

// ------------------------------------------------------------
// Sends frequentie to Data Word A
//-------------------------------------------------------------

// 29,620 @ 1kHz : Binary : 111001110110100 (29620)
// 29,630 @ 1kHz : Binary : 111 0011 1011 1110 (29630)
// 29,985 @ 1kHz : Binary : 111 0101 0010 0001 (29985) LO voor 29,530MHz RX

// 145,675 @ 12,5Khz = 11654 = binary : 0010 1101 1000 0110
// 180.000 @ 12,5kHz = 14400 = binary : 0011 1000 0100 0000

// send dividingnummer  for 145,675 MHz = dataword A
SendDataWordA();

//digitalWrite(DATA, LOW);
//digitalWrite(DLEN, HIGH);
//delay(1);
//sendbit(false);   //leading zero
//delay(1);
//sendbit(false);   // first 0 (false) = dataword A
//sendbit(false);
//sendbit(true);
//sendbit(false);

//sendbit(true);
//sendbit(true);
//sendbit(false);
//sendbit(true);

//sendbit(true);
//sendbit(false);
//sendbit(false);
//sendbit(false);

//sendbit(false);
//sendbit(true);
//sendbit(true);
//sendbit(false);

//digitalWrite(DLEN, LOW);
//sendbit(false);   //Belangrijk!!!!!! LOAD-PULS !!! Laten staan
//einde word A
}

// ----------------------------------
// Sendbit subroutine
// ----------------------------------
void sendbit(bool x) {
delay(1);
digitalWrite(DATA, x);
delay(1);
digitalWrite(CLB, HIGH);
delay(1);
digitalWrite(CLB, LOW);
delay(1);
}

//-------------------------------------------------------------
//-------------------------------------------------------------

void stop()
    {
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    cli();  // Disable interrupts
    sleep_mode();
    }

void loop() {
  // put your main code here, to run repeatedly:

//DividerValue = 11648;  //145.600MHz
//DividerValue = 11654;  //145.675MHz
//SendDataWordA();
//delay(3000);
//DividerValue = 11620;  //145.250MHz
//SendDataWordA();
delay(3000);

}

/****************************************************************************
Routine om 15 bits delerwaarde in de programable divider te zetten
Valiabele is - DividerValue -

*****************************************************************************/

int SendDataWordA() {
  digitalWrite(DATA, LOW);
  digitalWrite(DLEN, HIGH);
  delay(1);
  sendbit(false);   //leading zero
  delay(1);

  sendbit(false); //Bit 16 = false = Data Word A

  bp = 14;
  do{
    sendbit(bitRead(DividerValue,bp));
    bp = bp - 1;   //loop through bit positions 14 to 0
    }while(bp>=0);
  digitalWrite(DLEN, LOW);   //Afsluiten
  sendbit(false);   //Belangrijk!!!!!! LOAD-PULS !!! Laten staan
   //einde word A

}
