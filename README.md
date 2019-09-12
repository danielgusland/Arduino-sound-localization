# Arduino-sound-localization
A repository containing the source code and description for the project presented in https://www.youtube.com/watch?v=vGGvgp6u1-M.
The source code has been "hidden" in the comment section for quite a while and i am sorry for those of you who might have missed it. 

The project was a part of a small subject during my bachelors degree and was done together with [Jonas Waaler](https://github.com/jonasswa). It has been quite some time since I have looked at it. Unfortunately the report is written in Norwegian, but some quick translation of the variables can be found in the code. 
I have made a small descrition of how the code works below.
There is also heaps and loads of improvements that can be done. 

## Description
Each axis is controlled by a separate arduino and the program itself is really simple.
The peaking output of the microphones are connected to pin4 and pin5.
Basically the program checks if either pin4 or pin5 is high and then records the time until the other is high as well. 
This is done using low-level functions to achieve a relatively high sampling rate. 

The sampling of the time delay is done in this function:
```
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
```
When the time "tid3" is recorded, which is the time delay for the signal to pass from one microphone to the other, the angle corresponding to this time delay is calculated using: 
```
#define AVSTAND 0.145
float regn_ut_radianer (float time)
//REGNER UT HVOR LYDEN KOMMER FRA
//RETURNERER SVARET I RADIANER
  {
    return (asin((time*0.000001*343.00)/(AVSTAND)));
  }
```
When this angle is calculated, it is converted to degrees and the steppers are steered towards the sound source. 
All of the above is done in the following lines:
```
 //REGN UT VINKEL OG GJØR DET OM FRA RADIANER TIL GRADER OG SEND TIL STYR_STEP FUNKSJON
 radianer = regn_ut_radianer(tid3);
 grader = fra_rad_til_grad(radianer);
 delay(20);
 styr_step(int(grader));
```
