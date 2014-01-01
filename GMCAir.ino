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
#include "eeprom.h"

CCorner CornerLR(LeftRear);
CCorner CornerRR(RightRear);
CEEprom EEProm;

const int PinTilt = 5;
const int PinDumpTank = 2;

static states_t state = RUNMANUAL;

uint32_t SampleTime;    

//the height valuse that will level the coach to the horizon
//when camping
int16_t LeftLevel;
int16_t RightLevel;

//the current height readings
int16_t LRheight;
int16_t RRheight;
int16_t SetPoint;
int16_t Tilt;

//the 2 mode bits define 3 valid states
#define MODES_LIST(macro) \
    macro(TRAVELMODE)   \
    macro(MANUALMODE)   \
    macro(AUTOMODE)     \
    macro(AUTOCALMODE)  \
    
enum modes_t
{
  MODES_LIST(ENUMIFY)
};

static modes_t mode = MANUALMODE;
static char *ModeStrs[] = {MODES_LIST(STRINGIFY)};

#define MODULE "Main"
#define MAINSTATE "MainState"

static char *StateStrs[] = {STATES_LIST(STRINGIFY)};

void setup() 
{
  Serial.begin(9600);

  Serial.println("*******GMC Air Ride Controller V1.0*********");
  
  SampleTime = millis();
  
  SetState(RUNMANUAL);
  
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

//calculate the Left and right height values that will make the coach level
void CaclulateLevel()
{
//todo...implement it!
//handle not enough suspesion rage issues
//handle travel limits, ranges stored in EEPROM
    LeftLevel = 512;
    RightLevel = 512;
}

//read all the inputs and change states accordingly
void CheckEvents()
{
    //Low travel, high manual
    switch(mode)
    {
        case TRAVELMODE: //TravelChanged())
        {
            switch(state)
            {
                //only switch states if in these states;
                case RUNMANUAL:
                case RUNHORIZON:
                case DUMPTANK:
                case DUMPINGTANK:
                  SetState(RUNTRAVEL);
                break;
                
                case RUNTRAVEL:
                case CALLIMITS:
                case CALLOW:
                case CALHIGH:
                case CALTRAVEL:
                case CALAUTO:
                break; //no change allowed
            }   
        }
        break;
        case MANUALMODE:
        { 
            switch(state)
            {
                //only switch states if in these states;
                case RUNTRAVEL:
                case RUNHORIZON:
                case DUMPTANK:
                case DUMPINGTANK:
                    SetState(RUNMANUAL);
                break;
                
                case RUNMANUAL:
                case CALLIMITS:
                case CALLOW:
                case CALHIGH:
                case CALTRAVEL:
                case CALAUTO:
                break; //no change allowed
            } 

        }
        break;
        case AUTOMODE:
        {
            switch(state)
            {
                //only switch states if in these states;
                case RUNMANUAL:
                case RUNTRAVEL:
                case DUMPTANK:
                case DUMPINGTANK:
                    SetState(RUNHORIZON);
                break;
                
                case RUNHORIZON:
                case CALLIMITS:
                case CALLOW:
                case CALHIGH:
                case CALTRAVEL:
                case CALAUTO:
                break; //no change allowed
            }
        }
        break;
    }
    
}

void loop() 
{
    LRheight = 1024 - analogRead(A0);
    RRheight = 1024 - analogRead(A2);
    SetPoint = analogRead(A3);
    Tilt = analogRead(A1)-512;
        
    //read the tilt setpoint and adjust heights as needed
    //center of pot means no tilt
    //increasing value (>512) means raise right side, lower left side
    //decreasing value (<512) means raise left side, Lover right side
    
    if(IsTimedOut(100, SampleTime))
    {      
        Log(MODULE, "LRHeight", LRheight);
        Log(MODULE, "RRHeight", RRheight);
        Log(MODULE, "SetPoint", SetPoint);
        Log(MODULE, "Tilt", Tilt);
       
        SampleTime = millis();
    }

    CheckEvents();
    
    switch(state)
    {
        //Manual leveling
        case RUNMANUAL: 
            CornerLR.Run(0-Tilt);  
            CornerRR.Run(Tilt);
            break;
            
        //Run at the calibrated travel height
        //todo change update frequency once car moves
        case RUNTRAVEL:

            CornerLR.Run(EEProm.GetLeftCal());  
            CornerRR.Run(EEProm.GetRightCal());  
            break;
            
        //Auto level to the horizon
        case RUNHORIZON:
            //read accel
            CaclulateLevel();
            CornerLR.Run(LeftLevel);  
            CornerRR.Run(RightLevel);
            break;
            
        //open all valves and dump all air from the system
        //as long as switch is closed
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
                
                SetState(RUNMANUAL);
            }
            break;
            
        //raise and lower the coach to find the upper and lower limits of travel 
        //must be in travelmode before pressing cal button
        case CALLIMITS:
        break;
        case CALLOW:
        break;
        case CALHIGH:
        break;
        //Read and save current height as the new travel position
        //must be in manual mode and at ride height before pressing cal button
        case CALTRAVEL:
        break;
        //read and save the current accelerometer position as level
        //must be in Calautomode and level before pressing button
        case CALAUTO:
        break;
        default:
            SetState(RUNMANUAL);
    }
  
}
