//VEDLEGG 1

//***************************************************************************
//Laget av Daniel Gusland og Jonas S. Waaler
//Digsysprosjekt, vår 2014
//Program som lokaliserer lyd og flytter en steppermotor deretter
//
//
//
//______________________________NOTATER_______________________________________
//STEP: RØD BLÅ GRØNN SVART
//KABLING: HVIT-LILLA:GUL-LILLA, HVIT-GUL:GUL-GRØNN,HVIT-BLÅ:GRØNN-MØRKEGRØNN 
//____________________________________________________________________________


//INKLUDERER BIBLIOTEK OG DEFINERER NOEN KONSTANTER
#include <math.h>
#define AVSTAND 0.145
#define PAUSE 500

//STEPPER KONSTANTER FOR ARDUINO SHIELD
const int Skru_paa_1 = 3;
const int Skru_paa_2 = 11;
const int Spole_1_side_A = 12;
const int BremseaAV = 9;
const int Spole_2_side_A = 13;
const int BremseBAV = 8;
const int C_S_A = 0;
const int C_S_B= 1;

//HASTIGHET PÅ MOTOR
int MotorDelay  = 30;

//GLOBALE VARIABLE BRUKT I UTREGNINGER OG TELLELØKKER
float tid1;
float tid2;
float tid3;
float mellom;
int step_steg=1;
float radianer;
float grader;
int i;

void setup() 
{
  
  //SETTER UT OG INNGANGER I DDRD
  DDRD = B11001111;
  Serial.begin(9600);
  
  //SETTER UTGANGER BRUKT TIL STEPPER
  pinMode(Skru_paa_1, OUTPUT);
  pinMode(Skru_paa_2, OUTPUT);
  pinMode (Spole_1_side_A, OUTPUT);
  pinMode (Spole_2_side_A, OUTPUT);
  pinMode (BremseBAV, OUTPUT);
  pinMode (BremseaAV, OUTPUT);
  
}
 
//PROGRAMLØKKE
void loop() 
{
  //NULLSTILL/SETT TIDSVARIABLER TIL NULL
   tid1 = 0;
   tid2 = 0;
   tid3 = 0;
   
   //HVIS LOGISK OG PIN 5 OG PIND, NOTER TID
   if((PIND & B00100000)!=0)
   {
     tid1 = micros();
     //VENT TIL PIN 4 MOTTAR SIGNAL
     while((PIND & B00010000)==0)
     {}
     //NOTER TID TIL PIN 4
     tid2 = micros();
   
   //REGNER UT TIDSDIFFERANSE
   tid3 = tid2-tid1;
   
   //GJØR NEGATIVT DA DETTE ER VENSTRE SIDE
   tid3 = -1*(tid2-tid1);
   
   //HVIS VERDIEN IKKE ER BRUKBAR, HOPP OVER
   if(abs(tid3) >1000)
   {}
   else
   {
     //REGN UT VINKEL OG GJØR DET OM FRA RADIANER TIL GRADER OG SEND TIL STYR_STEP FUNKSJON
     radianer = regn_ut_radianer(tid3);
     grader = fra_rad_til_grad(radianer);
     delay(20);
     styr_step(int(grader));    
     
   }
   delay(PAUSE);
   }
   
   //HVIS LOGISK OG PIN 4 OG PIND, NOTER TID
   if((PIND & B00010000)!=0)
   {
     tid1 = micros();
     //VENT TIL PIN 5 MOTTAR SIGNAL
     while((PIND & B00100000)==0)
     {}
     //NOTER TID
     tid2 = micros();
   
   //REGN UT TIDSDIFFERANSE
   tid3 = tid2-tid1;
   
   //HVIS UBRUKELIG TID, HOPP OVER
   if(abs(tid3) >1000)
   {}
   else
   {
     //REGN UT VINKEL OG GJØR DET OM FRA RADIANER TIL GRADER OG SEND TIL STYR_STEP FUNKSJON
     radianer = regn_ut_radianer(tid3);
     grader = fra_rad_til_grad(radianer);
     delay(20);
     styr_step(int(grader));
     
   }
   delay(PAUSE);
   }

}

void styr_step(int grader)
//STYRER STEP MOTOR FREM ELLER TILBAKE
{
  //DEKLARERER VARIABLE
  int ant_step = 0;
  
  //SKALERER FRA GRADER TIL ANT STEP
  ant_step = map(abs(grader),0,180,0,100);
  
  //HVIS NEGATIVT, BEVEG MOTOR BAKOVER
  if (grader < 0 )
  {
    Step_bak(ant_step);
  }
  //HVIS POSITIV, BEVEG MOTOR FRAMOVER
  else if (grader > 0)
  {
    Step_frem(ant_step);
  }
  //ELLER
  else
  {}
}

void Step_frem(int ant_step)
//KJØRER MOTOR FREMOVER
{
    int i = 0;
    for (i=0;i<ant_step;i++)
    {
      digitalWrite(Skru_paa_1,HIGH);
      digitalWrite (Skru_paa_2,HIGH);
     
      digitalWrite(BremseaAV,LOW);
      digitalWrite(BremseBAV,LOW);
     
      //GÅR GJENNOM BITMØNSTER, OG TAR VARE PÅ POSISJON I MØNSTERET
      switch (step_steg)
        {
        case 1:
        digitalWrite(Spole_1_side_A,HIGH);
        digitalWrite(Spole_2_side_A,HIGH);
        delay(MotorDelay);
        step_steg=2;
        i++;
        
        case 2:
        digitalWrite(Spole_1_side_A,LOW);
        delay(MotorDelay);
        step_steg=3;
        i++;
        
        case 3:
        digitalWrite(Spole_2_side_A,LOW);
        delay(MotorDelay);
        step_steg=4;
        i++;
        
        case 4:
        digitalWrite(Spole_1_side_A,HIGH);
        delay(MotorDelay);
        step_steg = 1;
        break;
        }
       
      digitalWrite(Skru_paa_1,HIGH);
      digitalWrite (Skru_paa_2,HIGH);
     
      }
 
}
 
  

void Step_bak(int ant_step)
//KJØRER MOTOR BAKOVER
{
  int i = 0;
  
  for (i=0;i<ant_step;i++)
    {
    digitalWrite(Skru_paa_1,HIGH);
    digitalWrite (Skru_paa_2,HIGH);
   
    digitalWrite(BremseaAV,LOW);
    digitalWrite(BremseBAV,LOW);
   
    //GÅR GJENNOM BITMØNSTER, OG TAR VARE PÅ POSISJON I MØNSTERET
    switch (step_steg)
      {
      case 1:
      digitalWrite(Spole_1_side_A,HIGH);
      digitalWrite(Spole_2_side_A,LOW);
      delay(MotorDelay);
      step_steg=2;
      i++;
      
      case 2:
      digitalWrite(Spole_1_side_A,LOW);
      delay(MotorDelay);
      step_steg=3;
      i++;
      
      case 3:
      digitalWrite(Spole_2_side_A,HIGH);
      delay(MotorDelay);
      step_steg=4;
      i++;
      
      case 4:
      digitalWrite(Spole_1_side_A,HIGH);
      delay(MotorDelay);
      step_steg = 1;
      break;
      }
     
    digitalWrite(Skru_paa_1,HIGH);
    digitalWrite (Skru_paa_2,HIGH);
   
   }
 
}

float regn_ut_radianer (float time)
//REGNER UT HVOR LYDEN KOMMER FRA
//RETURNERER SVARET I RADIANER
  {
    return (asin((time*0.000001*343.00)/(AVSTAND)));
  }

float fra_rad_til_grad (float radianer)
//REGNER OM FRA RADIANER TIL GRADER
  {
    return ((radianer/(2*PI))*360);
  }
