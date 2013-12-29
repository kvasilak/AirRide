/*************************************************************************
**   Air ride controller for the MSGPIO hardware
**
** Copyright 2013 Keith Vasilakes
**
** This file is part of MegaRide an Air ride controller.
**
** MegaRide is free software: you can redistribute it and/or modify it under the terms of the GNU General Public
** License as published by the Free Software Foundation, either version 3 of the License, or (at your option) 
** any later version.
**
** MegaRide is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the 
** implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public 
** License for more details.
**
** You should have received a copy of the GNU General Public License along with MegaRide. If not, see
** http://www.gnu.org/licenses/.
**
**    GMCAit.ino
**************************************************************************/
#include "corner.h"
#include "debug.h"
#include "common.h"

CCorner CornerLR(LeftRear);
CCorner CornerRR(RightRear);
const int PinTilt = 5;
const int PinDumpTank = 2;

static states_t state = RUNHEIGHT;

bool IsDumping = false;
uint32_t SampleTime;
int32_t LRfilter_reg; 
int32_t RRfilter_reg; 
int32_t LRFiltered;
int32_t RRFiltered;

#define MODULE "Main"
#define MAINSTATE "MainState"

static char *StateStrs[] = {STATES_LIST(STRINGIFY)};

void setup() 
{
  Serial.begin(9600);

  Serial.println("*******GMC Air Ride Controller V1.0*********");
  
  SampleTime = millis();
  
  LRfilter_reg 		= (1024 - analogRead(A0)) << FILTER_SHIFT;
  RRfilter_reg 		= (1024 - analogRead(A2)) << FILTER_SHIFT;
  
  SetState(RUNHEIGHT);
  
  pinMode(2, INPUT_PULLUP);
}

//return true if it's been period ms since start
bool IsTimedOut(uint32_t period, uint32_t start)
{
  uint32_t current = millis();
  return(current - start > period);
}


void SetState(states_t s)
{
    static states_t laststate = DUMPINGTANK;
    
    if(s != laststate)
    {
        Log(MODULE, "MainState", StateStrs[s]);
        laststate = s;
    }
}

void loop() 
{
    //read the tilt setpoint and adjust heights as needed
    //center of pot means no tilt
    //increasing value (>512) means raise right side, lower left side
    //decreasing value (<512) means raise left side, Lover right side
    
    //int16_t tilt = analogRead(PinTilt) - 512;  
    
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
        
        
        Log(MODULE, "LRHeight", LRheight);
        Log(MODULE, "RRHeight", LRheight);
        Log(MODULE, "LRHeightFilt", LRFiltered);
        Log(MODULE, "RRHeightFilt", LRheight);
        
        Log(MODULE, "MainState", StateStrs[state]);
        
        SampleTime = millis();
    }

    
    switch(state)
    {
        case RUNHEIGHT: 
            CornerLR.Run(0); //tilt);
            CornerRR.Run(0); //-tilt);
            
            if(LOW == digitalRead(PinDumpTank))
            {
                SetState(DUMPTANK);
            }
            break;
        case DUMPTANK:
            //open all valves to dump all air from system
            CornerLR.Fill(Open);
            CornerLR.Dump(Open);
        
            CornerRR.Fill(Open);
            CornerRR.Dump(Open);
            
            SetState(DUMPINGTANK);
            break;
         case DUMPINGTANK:
            //wait for user to release button, then close all valves
            if(HIGH == digitalRead(PinDumpTank))
            {
                CornerLR.Fill(Closed);
                CornerLR.Dump(Closed);
            
                CornerRR.Fill(Closed);
                CornerRR.Dump(Closed);
                
                SetState(RUNHEIGHT);
            }
            break;
        default:
            SetState(RUNHEIGHT);
    }
  
}
