#include "FPS_GT511C3.h"
#include "SoftwareSerial.h"
#include "LedControl.h"
FPS_GT511C3 fps(4, 5);   //RX - 4; TX -5
LedControl lc=LedControl(12,10,11,2);  // Pins: DIN,CLK,CS, # of Display connected
void notfound_disp()
{
byte notfound_b[] =
{
  B10111101,    //d
  B01110110,    //n
  B00111110,    //u
  B01111110,    //o
  
  B01000111,    //f
  B01110000,    //t
  B01111110,    //o
  B01110110     //n
};
    for (int i = 0; i < 8; i++)  
  {
    lc.setRow(0,i,notfound_b[i]);
  }
}
void disp_found()
{
byte notfound_b[] =
{
  B10111101,    //d
  B01110110,    //n
  B00111110,    //u
  B01111110,    //o
  
  B01000111,    //f
  B0000000,    //t
  B00000000,    //
  B00000000     //
};
    for (int i = 0; i < 8; i++)  
  {
    lc.setRow(0,i,notfound_b[i]);
  }
}
void disp_press()
{
byte notfound_b[] =
{
  B00000000,    //d
  B00000000,    //n
  B00000000,    //u
  B00000000,    //o
  
  B01001111,    //e
  B00001110,    //l
  B00111101,    //d
  B01111000     //i
};
    for (int i = 0; i < 8; i++)  
  {
    lc.setRow(0,i,notfound_b[i]);
  }
}
void disp_null()
{
byte null_b[] =
{
  B00000000,    //d
  B00000000,    //n
  B00000000,    //u
  B00000000,    //o
  
  B00000000,    //f
  B00000000,    //t
  B00000000,    //o
  B00000000     //n
};
    for (int i = 0; i < 8; i++)  
  {
    lc.setRow(0,i,null_b[i]);
  }
}
int ledpos = 7;
int ledneg = 8;

void setup()
{
  lc.shutdown(0,false);  // Wake up displays
  lc.shutdown(1,false);
  lc.setIntensity(0,5);  // Set intensity levels
  lc.setIntensity(1,5);
  lc.clearDisplay(0);  // Clear Displays
  lc.clearDisplay(1);
  
  Serial.begin(9600); //set up Arduino's hardware serial UART
  delay(100);
  fps.Open();         //send serial command to initialize fps
  fps.SetLED(true);   //turn on LED so fps can see fingerprint
  pinMode(ledpos,OUTPUT);
  pinMode(ledneg,OUTPUT);
}

void loop()
{
  digitalWrite(ledneg,LOW);
  digitalWrite(ledpos,LOW);
  // Identify fingerprint test
  if (fps.IsPressFinger())
  {
    fps.CaptureFinger(false);
    int id = fps.Identify1_N();
    //Serial.println(id);
    if (id  < 200) 
    {
      Serial.print("Verified ID:");
      Serial.println(id);
      while( fps.IsPressFinger() )
       {
          digitalWrite(ledpos,HIGH);
          disp_found();
       }
      disp_null();     
    }
    else
    {//if unable to recognize
      Serial.println("Finger not found");
      while( fps.IsPressFinger() )
           {
            notfound_disp();
            digitalWrite(ledneg,HIGH);
           }
      disp_null();           
    }
  }
  else
  {
    Serial.println("Please press finger");
    disp_press();
  }
  //delay(100);
}
