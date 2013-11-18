#include "corner.h"

CCorner CornerLR(LeftRear);
CCorner CornerRR(RightRear);
const int PinTilt = 5;
const int PinDumpTank = 6;
enum states {RUNHEIGHT, DUMPTANK, DUMPINGTANK};
//bool Dumping = false;
states state = RUNHEIGHT;
uint32_t SampleTime;
int32_t LRfilter_reg; 
int32_t RRfilter_reg; 
int32_t LRFiltered;
int32_t RRFiltered;

void setup() 
{
  Serial.begin(9600);

  Serial.println("*******GMC Air Ride Controller V1.0*********");
  
  SampleTime = millis();
  
  LRfilter_reg 		= (1024 - analogRead(A0)) << FILTER_SHIFT;
  RRfilter_reg 		= (1024 - analogRead(A2)) << FILTER_SHIFT;
}

//return true if it's been period ms since start
bool IsTimedOut(uint32_t period, uint32_t start)
{
  uint32_t current = millis();
  return(current - start > period);
}

void loop() 
{
    //read the tilt setpoint and adjust heights as needed
    //center of pot means no tilt
    //increasing value (>512) means raise right side, lower left side
    //decreasing value (<512) means raise left side, Lover right side
    
    //int16_t tilt = analogRead(PinTilt) - 512;
    
   
    //on first press of dump switch transition to dump tanks state
    //if((LOW == digitalRead(PinDumpTank)) && (false == Dumping))
    //{
    //    Dumping = true;
    //    state = DUMPTANK;
    //}
    
    if(IsTimedOut(100, SampleTime))
    {
        int LRheight = 1024 - analogRead(A0);
        int RRheight = 1024 - analogRead(A2);
        
    	
        // Update filter with current sample.
		LRfilter_reg = LRfilter_reg - (LRfilter_reg >> FILTER_SHIFT) + LRheight;
        RRfilter_reg = RRfilter_reg - (RRfilter_reg >> FILTER_SHIFT) + RRheight;
        // Scale output for unity gain.
        LRFiltered = (LRfilter_reg >> FILTER_SHIFT);
        RRFiltered = (RRfilter_reg >> FILTER_SHIFT);
        
        Serial.print(HEIGHT_NOMINAL); //nominal
        Serial.print(",");
        Serial.print(HEIGHT_NOMINAL - HOLD_DEAD_BAND); //low
        Serial.print(",");
        Serial.print(HEIGHT_NOMINAL + HOLD_DEAD_BAND); //high
        Serial.print(",");
        Serial.print(LRheight);
        Serial.print(",");
        Serial.print(RRheight);
        Serial.print(",");
        Serial.print(LRFiltered);
        Serial.print(",");
        Serial.print(RRFiltered);
        Serial.println("");
        
        SampleTime = millis();
    }

    switch(state)
    {
        case RUNHEIGHT: 
            CornerLR.Run(0); //tilt);
            CornerRR.Run(0); //-tilt);
            break;
        case DUMPTANK:
            //open all valves to dump all air from system
            //CornerLR.Fill(Open);
            //CornerLR.Dump(Open);
        
            //CornerRR.Fill(Open);
            //CornerRR.Dump(Open);
            
            state = DUMPINGTANK;
            break;
         case DUMPINGTANK:
            //wait for user to release button, then close all valves
            if(HIGH == digitalRead(PinDumpTank))
            {
              //  CornerLR.Fill(Closed);
              //  CornerLR.Dump(Closed);
            
              //  CornerRR.Fill(Closed);
              //  CornerRR.Dump(Closed);
                
                state = RUNHEIGHT;
            }
            break;
        default:
            state = RUNHEIGHT;
    }
  
}
