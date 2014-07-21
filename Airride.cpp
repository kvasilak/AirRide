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
**    Airride.cpp
**************************************************************************/
#include <Arduino.h>
#include "corner.h"
#include "debug.h"
#include "common.h"
#include "gmceeprom.h"
#include "airride.h"


//#define MODULE "AirRide"

static char MODULE[] = {"AirRide"};
static char CALSTATE[] = {"CalState"};

CAirRide::CAirRide()
{
}

void CAirRide::Init()
{
    Serial.println("*******GMC Air Ride Controller V1.0*********");

    SampleTime = millis();

    SetState(RUNMANUAL);

    pinMode(PINMODE1, INPUT_PULLUP);
    pinMode(PINMODE2, INPUT_PULLUP);
    pinMode(PINCAL, INPUT_PULLUP);
  
    CornerL.Init(LeftRear); 
    CornerR.Init(RightRear);
    
    EEProm.GetLimits(&LeftLowLimit, &LeftHighLimit, &RightLowLimit, &RightHighLimit);
    
    //TODO read from EEPROM also
    LTravelHeight = 586;
    RTravelHeight = 434;
   
    CornerL.Limits(LeftLowLimit, LeftHighLimit);
    CornerR.Limits(RightLowLimit, RightHighLimit);
    
    CheckEvents();
  
}

void CAirRide::SetState(states_t s)
{
    static states_t laststate = DUMPINGTANK;
    static char *statestrs[] = {STATES_LIST(STRINGIFY)};
    
    if(s != laststate)
    {
        Log(MODULE, "MainState", statestrs[s]);
        laststate = s;
    }
    state = s;
}

//calculate the Left and right height values that will make the coach level
void CAirRide::CaclulateLevel()
{
//todo...implement it!
//handle not enough suspesion rage issues
//handle travel limits, ranges stored in EEPROM
    LeftLevel = 512;
    RightLevel = 512;
}

//input is analog
//if at zero (<512) button is pressed
bool CAirRide::DumpTank()
{
    int a = analogRead(PINDUMP);
    bool pressed = false;
    //Log(MODULE, "DumpTank", a);
    
    if( a < 512 )
    {
        Log(MODULE, "DumpTank", "UnDumping");
        
    }
    else
    {
        Log(MODULE, "DumpTank", "Dumping");
        pressed = true;
    }
    
    return pressed;
}

//only called when appropriate
//button must transition as follows
//not pressed for at least 5 seconds
//pressed for 5 seconds
//released
//once released we're in calibrate mode
bool CAirRide::Calibrate()
{
    static int calstate = 0;
    bool docal = false;
    static uint32_t pressstart;
    
    //N.O. switch
    bool pressed = (HIGH == digitalRead(PINCAL));
   
   Log(MODULE, "CalButton", digitalRead(PINCAL));
    
    switch(calstate)
    {
        case 0: 
            //don't start timer until button is not pressed
            if(!pressed)
            {
                Log(MODULE, CALSTATE, "NotPressed");
                pressstart = millis();
                calstate++;
            }
            break;
        case 1: 
            if(pressed)
            {
                //don't press button for 5 seconds
                if(IsTimedOut(5000, pressstart))
                {
                    Log(MODULE, CALSTATE, "Pressed");
                    //Button un pressed long enough!
                    //Now wait another 5 seconds
                    pressstart = millis();
                    calstate++;
                }
                else //button pressed too soon, restart timer
                {
                    Log(MODULE, CALSTATE, "Released");
                    
                    pressstart = millis();
                    calstate = 0;
                }
            }
            break;
        case 2:
            if(!pressed)
            {
                //Hold button down for 5 seconds
                if(IsTimedOut(5000, pressstart))
                {
                    Log(MODULE, CALSTATE, "Calibrate");
                    //user completed the magic sequence, calibrate!
                    docal = true;
                    calstate=0;
                }
                else
                {
                    //user let go too soon
                    calstate = 0;
                }
            }  
            break;
    }
    
    return docal;
}

// mode 1   mode 2  state
//   0        0       Manual
//   1        0       Travel
//   0        1       Autolevel
//   1        1       Autolevel calibrate
void CAirRide::GetMode()
{
    static modes_t LastMode = CAMPMODE;//should always be different the first test
    static char *states[] = {MODES_LIST(STRINGIFY)};
    //modes_t themode=CAMPMODE;
    
    //input truth table
    //Logic is inverted
    //11 Auto mode
    //10 Travel mode
    //01 Manual mode
    //00 Auto Calibrate
    int m = digitalRead(PINMODE1);
    m |= digitalRead(PINMODE2) <<1;
  
    if(LastMode != m)
    {        
        mode = (modes_t)m;
        
        Log(MODULE, "Mode", states[m]);
        
        LastMode = (modes_t)m;
    }
    
}

//read all the inputs and change states accordingly
void CAirRide::CheckEvents()
{
    GetMode();
    
    switch(state)
    {
        //only switch states if in these states;
        case RUNTRAVEL:
            switch(mode)
            {
                case MANUALMODE:
                case CAMPMODE:
                case AUTOCALMODE:
                SetState((states_t)mode);
                break;
                //No mode change
                case TRAVELMODE:
                //find the high and low limits
                if(Calibrate())
                {
                    SetState(CALLIMITS);
                }
                
                //what are the chances of dumptank and calibrate being pressed at the exact same time?
                //if(DumpTank())
                //{
                //    SetState(DUMPTANK);
                //}
                break;
            }
            break;
        case RUNMANUAL:
            switch(mode)
            {
            //Log(MODULE, "DumpTank", "Check!!");
            
                case TRAVELMODE:
                case CAMPMODE:
                case AUTOCALMODE:
                
                //Log(MODULE, "DumpTank", "Check%");
                SetState((states_t)mode);
                break;
                case MANUALMODE:
                //save the current height as the travel height
                if(Calibrate())
                {
                    EEProm.SaveLeftTravel(LeftHeight); 
                    EEProm.SaveRightTravel(RightHeight);
                    SetState(CALDONELED);
                }
                
                //Log(MODULE, "DumpTank", "Check");
                //if(DumpTank())
                //{
                //    SetState(DUMPTANK);
                //}
                break;
            }
            break;
        case RUNCAMP:
            switch(mode)//no calibration to do
            {
                case TRAVELMODE:
                case MANUALMODE:
                case AUTOCALMODE:
                SetState((states_t)mode);
                break;
                case CAMPMODE:
                //if(DumpTank())
                //{
                //    SetState(DUMPTANK);
                //}
                break;
            }
            break;
        case RUNAUTOCAL:
            switch(mode)
            {
                case TRAVELMODE:
                case MANUALMODE:
                case CAMPMODE:
                SetState((states_t)mode);
                break;
                case AUTOCALMODE:
                //Save the accelerometer values for level
                //Assumes the coach is currently level
                if(Calibrate())
                { 
                    EEProm.SaveLeftAuto(LeftAuto);
                    EEProm.SaveRightAuto(RightAuto);
                    SetState(CALDONELED);
                }
                
                //if(DumpTank())
                //{
                //    SetState(DUMPTANK);
                //}
                break;
            }
            break;
        //no mode change allowed until the user lets go of the dump button, or we'd just end up back here
        case DUMPTANK:
        case DUMPINGTANK:
        //no change allowed during limit calibration
        case CALLIMITS: 
        case CALLOW:
        case CALWAITHIGH:   
        case CALHIGH:
        case CALDONELED:
        case CALDONE:
        break; 
        //now back to our current mode
        case CALCOMPLETE:
            switch(mode)
            {
                case TRAVELMODE:
                case MANUALMODE:
                case CAMPMODE:
                case AUTOCALMODE:
                SetState((states_t)mode);
                break;
            }
        break;
    }   
}

//used to calibrate height limits.
//the coach may not have travel from 0 to 1024
//wait for both heights to be all the way down
//determined by the height changing less that 2 counts in 1 second
//Waits 10 seconds for the cases where it is slow to start or stop moving
bool CAirRide::AllDown(int *oldleft, int *oldright, int left, int right)
{
    bool leftdown = false;
    bool rightdown = false;
    bool alldown = false;
    
    //sample height change once a second
    if(IsTimedOut(1000, timeout))
    {
        //less than 2 counts in 1 second means we're all the way down
        if(*oldleft - left <= 2)
        {
            //no change for 10 seconds
            if(IsTimedOut(10000, LeftMinTime))
            {
                leftdown = true;
            }
        }
        else
        {
            LeftMinTime = millis();
        }
        
        if(*oldright - right <= 2)
        {
            if(IsTimedOut(10000, RightMinTime))
            {
                rightdown = true;
            }
        }
        else
        {
            RightMinTime = millis();
        }

        //reset timer for another second
        timeout = millis();
        
        //don't update once we stop moving
        if(!leftdown)
        {
            *oldleft = left;
        }
        
        if(!rightdown)
        {
            *oldright = right;
        }
    }

    if(leftdown && rightdown)
    {
        //both heights stopped moving for at least 10 seconds
        alldown = true;
        LeftLowLimit = left;
        RightLowLimit = right;
    }

    return alldown;
}


//Wait for the car to start rising
bool CAirRide::IsRising(int *oldleft, int *oldright, int left, int right)
{
    bool leftmoved = false; 
    bool rightmoved = false;
    bool allmoved = false;
    
    if(IsTimedOut(1000, timeout))
    {
        //wait for car to go up 10 counts
        if(*oldleft - left > 10)
        {
            leftmoved = true;
        }
        
        if(*oldright - right > 10)
        {
            rightmoved = true;
        }
        
        //don't update once we start moving
        if(!leftmoved)
        {
            *oldleft = left;
        }
        
        if(!rightmoved)
        {
            *oldright = right;
        }
    }
    
    if(leftmoved && rightmoved)
    {
        //both heights stopped moving for at least 10 seconds
        allmoved = true;
    }

    return allmoved;
}

//see all down comments
bool CAirRide::AllUp(int *oldleft, int *oldright, int left, int right)
{ 
    bool leftup = false;
    bool rightup = false;
    bool allup = false;
    
    if(IsTimedOut(1000, timeout))
    {
        //less than 2 counts in 1 second means we're all the way down
        if(*oldleft - left < 2)
        {
            //no change for 10 seconds
            if(IsTimedOut(10000, LeftMinTime))
            {
                leftup = true;
            } 
        }
        
        if(*oldright - right < 2)
        {
            //no change for 10 seconds
            if(IsTimedOut(10000, RightMinTime))
            {
                rightup = true;
            } 
        }
        
        //both heights stopped moving and we have waited at least 10 seconds
        if(leftup && rightup)
        {
            allup = true;
        }
        
        timeout = millis();
        
        if(!leftup)
        {
            *oldleft = left;
        }
        
        if(!rightup)
        {
            *oldright = right;
        }
    }
    
    if(leftup && rightup)
    {
        //both heights stopped moving for at least 10 seconds
        allup = true;
        LeftHighLimit = left;
        RightHighLimit = right;
    }
    
    
    return allup;
}

//turn on Cal LED
void CAirRide::CalLED( bool on)
{
    if(on)
    {
        //make output and turn on LED
   //     pinMode(PINCAL, OUTPUT);
   //     digitalWrite(PINCAL, HIGH);
    }
    else
    {
   //     digitalWrite(PINCAL, LOW);
   //     pinMode(PINCAL, INPUT_PULLUP);
        
    }
}

void CAirRide::Run() 
{
    //LRheight = 1024 - analogRead(A0);
    //RRheight = 1024 - analogRead(A2);
    SetPoint = analogRead(PINSETPOINT);
    Tilt = analogRead(PINTILT)-512;
        
    //read the tilt setpoint and adjust heights as needed
    //center of pot means no tilt
    //increasing value (>512) means raise right side, lower left side
    //decreasing value (<512) means raise left side, Lover right side
    
    if(IsTimedOut(100, SampleTime))
    {      
        Log(MODULE, "SetPoint", SetPoint);
        Log(MODULE, "Tilt", Tilt);
       
        SampleTime = millis();
    }

    CheckEvents();
    
    switch(state)
    {
        //Manual leveling
        //need to pass in setpoint as read from set point pot
        case RUNMANUAL: 
            CornerL.Run(SetPoint-Tilt);  
            CornerR.Run(SetPoint+Tilt);
            break;
            
        //Run at the calibrated travel height
        //todo change update frequency 
        //need to pass in travel height as read from EEPROM
        case RUNTRAVEL:
            CornerL.Run(LTravelHeight);  //568 adc counts
            CornerR.Run(RTravelHeight);  //434 adc counts
            break;
            
        //Auto level to the  for camping
        case RUNCAMP:
            //read accel
            CaclulateLevel();
            CornerL.Run(0);  
            CornerR.Run(0);
            break;
            
        //manually level the coach to the horizon
        //then press caibrate to save the acellerometer values
        //to use for autoleveling
        case RUNAUTOCAL:
            CornerL.Run(0);  
            CornerR.Run(0);
            break;
            
        //open all valves and dump all air from the system
        //as long as switch is closed
        case DUMPTANK:
            //open all valves to dump all air from system
            CornerL.Fill(Open);
            CornerL.Dump(Open);
        
            CornerR.Fill(Open);
            CornerR.Dump(Open);
            
            SetState(DUMPINGTANK);
            break;
            
         case DUMPINGTANK:
            //wait for user to release button, then close all valves
            if(!DumpTank())
            {
                CornerL.Fill(Closed);
                CornerL.Dump(Closed);
            
                CornerR.Fill(Closed);
                CornerR.Dump(Closed);
                
                //allow the event handler to switch us back to the current state
                SetState(CALCOMPLETE);
            }
            break;
            
        //raise and lower the coach to find the upper and lower limits of travel 
        //must be in travelmode before pressing cal button
        case CALLIMITS:
            //turn on Cel LED at start of calibration cycle
            CalLED(true);
            
            CornerL.Dump(Open);
            CornerL.Dump(Open);
            
            timeout = millis();
            LeftMinTime = millis();
            RightMinTime = millis();
            OldLeft = LRheight;
            OldRight = RRheight;
    
            SetState(CALLOW);
            break;
        case CALLOW:
            //wait for car to be all the way down
            if(AllDown(&OldLeft, &OldRight, LRheight, RRheight))
            {
                CornerL.Dump(Closed);
                CornerL.Dump(Closed);
                CornerL.Fill(Open);     
                CornerR.Fill(Open);
                
                timeout = millis();
                LeftMinTime = millis();
                RightMinTime = millis();
                OldLeft = LRheight;
                OldRight = RRheight;
                SetState(CALWAITHIGH);
            }
            break;
        case CALWAITHIGH:
            //wait for the car to start rising
            if(IsRising(&OldLeft, &OldRight, LRheight, RRheight))
            {
                timeout = millis();
                LeftMinTime = millis();
                RightMinTime = millis();
                OldLeft = LRheight;
                OldRight = RRheight;
                SetState(CALHIGH);
            }
            break;
        case CALHIGH:
            if(AllUp(&OldLeft, &OldRight, LRheight, RRheight))
            {
                CornerL.Fill(Closed);     
                CornerR.Fill(Closed);
                
                SetState(CALSAVELIMITS);
            }
            break;
        case CALSAVELIMITS:
            EEProm.SaveLimits(LeftLowLimit, LeftHighLimit, RightLowLimit, RightHighLimit);
           
            CornerL.Limits(LeftLowLimit, LeftHighLimit);
            CornerR.Limits(RightLowLimit, RightHighLimit);
            
            SetState(CALDONELED);
            break;
         //done with cal, show cal LED for 1 sec 
        case CALDONELED:
            CalLED(true);
            SetState(CALDONE);
            break;
        case CALDONE:
            if(IsTimedOut(1000, CalDoneTime))
            {
                CalLED(false);
                SetState(CALCOMPLETE);
            }
            break;
        case CALCOMPLETE:
            break;
        default:
            SetState(RUNMANUAL);
    }
  
}
