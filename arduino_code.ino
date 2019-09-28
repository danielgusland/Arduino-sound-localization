//VEDLEGG 1

//***************************************************************************
//ENG:
//Made my Daniel Gusland and Jonas S. Waaler
//Made in 2014 at OsloMet, Oslo
//This program localizes sound, and moves a step motor thereafter.
//
//NOR:
//Laget av Daniel Gusland og Jonas S. Waaler
//Digsysprosjekt, vår 2014
//Program som lokaliserer lyd og flytter en steppermotor deretter
//
//KEEP IN MIND THAT THERE ARE MANY IMPROVEMENTS THAT CAN BE DONE
//THIS PROJECT WAS DONE OVER A SMALL TIME PERIOD
//(APPROX. TWO WEEKS? - DON'T REMEMBER)
//______________________________NOTATER_______________________________________
//(notes specific for our motor we had at the time)
//STEP: RØD BLÅ GRØNN SVART
//KABLING: HVIT-LILLA:GUL-LILLA, HVIT-GUL:GUL-GRØNN,HVIT-BLÅ:GRØNN-MØRKEGRØNN 
//____________________________________________________________________________


//TRANSLATION OF VARIABLE NAMES
//AVSTAND = DISTANCE
//PAUSE = DELAY
//INKLUDERER BIBLIOTEK OG DEFINERER NOEN KONSTANTER
#include <math.h>
#define AVSTAND 0.145
#define PAUSE 500

//CONSTANTS USED FOR THE ARDUINO SHIELD
//STEPPER KONSTANTER FOR ARDUINO SHIELD

//TURN ON 1
const int Skru_paa_1 = 3;
//TURN ON 2
const int Skru_paa_2 = 11;
//COIL 1 SIDE A
const int Spole_1_side_A = 12;
//BREAK A OF
const int BremseaAV = 9;
//COIL 2 SIDE A
const int Spole_2_side_A = 13;
//BREAK B OF
const int BremseBAV = 8;
const int C_S_A = 0;
const int C_S_B= 1;


//SPEED OF MOTOR
//HASTIGHET PÅ MOTOR
int MotorDelay  = 30;

//GLOBAL VARIABLES USED FOR CALCULATING AND COUNTING
//GLOBALE VARIABLE BRUKT I UTREGNINGER OG TELLELØKKER

//TIME 1
float tid1;
//TIME 2
float tid2;
//TIME 3
float tid3;
//BETWEEN
float mellom;
//STEPPER MOTOR STEPS
int step_steg=1;
//RADIANS
float radianer;
//DEGREES
float grader;
int i;

void setup() 
{
  //DEFINING OUT AND IN - DDRD
  //SETTER UT OG INNGANGER I DDRD
  DDRD = B11001111;
  Serial.begin(9600);
  
  //INITIALIZING OUT USED FOR STEPPER
  //SETTER UTGANGER BRUKT TIL STEPPER
  pinMode(Skru_paa_1, OUTPUT);
  pinMode(Skru_paa_2, OUTPUT);
  pinMode (Spole_1_side_A, OUTPUT);
  pinMode (Spole_2_side_A, OUTPUT);
  pinMode (BremseBAV, OUTPUT);
  pinMode (BremseaAV, OUTPUT);
  
}
 
//PROGRAMLOOP
//PROGRAMLØKKE
void loop() 
{
   //FLUSH
   //NULLSTILL/SETT TIDSVARIABLER TIL NULL
   tid1 = 0;
   tid2 = 0;
   tid3 = 0;
   
   //IF PIN 5 AND PIND, NOTE TIME
   //HVIS LOGISK OG PIN 5 OG PIND, NOTER TID
   if((PIND & B00100000)!=0)
   {
     tid1 = micros();
     //WAIT FOR PIN 4
     //VENT TIL PIN 4 MOTTAR SIGNAL
     while((PIND & B00010000)==0)
     {}
     //NOTE TIME PIN 4
     //NOTER TID TIL PIN 4
     tid2 = micros();

   //CALCULATE TIME DIFFERENCE
   //REGNER UT TIDSDIFFERANSE
   tid3 = tid2-tid1;
   
   //LEFT SIDE = NEGATIVE
   //GJØR NEGATIVT DA DETTE ER VENSTRE SIDE
   tid3 = -1*(tid2-tid1);
   
   //IF THE TIME IS MORE THAN 1000, SKIP
   //HVIS VERDIEN IKKE ER BRUKBAR, HOPP OVER
   if(abs(tid3) >1000)
   {}
   else
   {
     //CALCULATE ANGLE, AND CONVERT FROM RAD TO DEG AND SEND TO STYR_STEP FUNCTION
     //REGN UT VINKEL OG GJØR DET OM FRA RADIANER TIL GRADER OG SEND TIL STYR_STEP FUNKSJON
     radianer = regn_ut_radianer(tid3);
     grader = fra_rad_til_grad(radianer);
     delay(20);
     styr_step(int(grader));    
     
   }
   delay(PAUSE);
   }
   //IF PIN4 AND PIND, NOTE TIME
   //HVIS LOGISK OG PIN 4 OG PIND, NOTER TID
   if((PIND & B00010000)!=0)
   {
     tid1 = micros();
     //WAIT FOR PIN 5
     //VENT TIL PIN 5 MOTTAR SIGNAL
     while((PIND & B00100000)==0)
     {}
     //NOTE TIME
     //NOTER TID
     tid2 = micros();
   //CALCULATE TIME DIFFERENCE
   //REGN UT TIDSDIFFERANSE
   tid3 = tid2-tid1;
   
   //IF TIME IS MORE THAN 1000, SKIP
   //HVIS UBRUKELIG TID, HOPP OVER
   if(abs(tid3) >1000)
   {}
   else
   { //CALCULATE ANGLE AND CONVERT FROM RAD TO DEG AND SEND TO STYR_STEP FUNCTION
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
//CONTROL STEP MOTOR
//STYRER STEP MOTOR FREM ELLER TILBAKE
{
  
  //DEKLARERER VARIABLE
  
  //NR OF STEPS
  int ant_step = 0;
  
  //SCALE FROM DEGREES TO NR OF STEPS
  //SKALERER FRA GRADER TIL ANT STEP
  ant_step = map(abs(grader),0,180,0,100);
  
  //IF NEGATIVE, MOVE MOTOR BACK
  //HVIS NEGATIVT, BEVEG MOTOR BAKOVER
  if (grader < 0 )
  {
    Step_bak(ant_step);
  }
  //IF POSITIV, MOVE MOTOR FORWARD
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
//STEP FORWARD
//KJØRER MOTOR FREMOVER
{
    int i = 0;
    for (i=0;i<ant_step;i++)
    {
      digitalWrite(Skru_paa_1,HIGH);
      digitalWrite (Skru_paa_2,HIGH);
     
      digitalWrite(BremseaAV,LOW);
      digitalWrite(BremseBAV,LOW);
     
      //GO THROUGH BIT-PATTERN, AND KEEP POSITION
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
//STEP BACKWARD
//KJØRER MOTOR BAKOVER
{
  int i = 0;
  
  for (i=0;i<ant_step;i++)
    {
    digitalWrite(Skru_paa_1,HIGH);
    digitalWrite (Skru_paa_2,HIGH);
   
    digitalWrite(BremseaAV,LOW);
    digitalWrite(BremseBAV,LOW);
    
    //GO THROUGH BIT-PATTERN, AND KEEP POSITION
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
//CALCULATE WHERE AUDIO IS COMING FROM
//REGNER UT HVOR LYDEN KOMMER FRA

//RETURN ANS. IN RADIANS
//RETURNERER SVARET I RADIANER
  {
    return (asin((time*0.000001*343.00)/(AVSTAND)));
  }

float fra_rad_til_grad (float radianer)
//CONVERTS FROM RADIANS TO DEGREES
//REGNER OM FRA RADIANER TIL GRADER
  {
    return ((radianer/(2*PI))*360);
  }
