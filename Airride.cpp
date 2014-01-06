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
#include "eeprom.h"
#include "airride.h"


#define MODULE "AirRide"
#define MAINSTATE "MainState"

CAirRide::CAirRide() :
PinTilt(5),
PinDumpTank(A5),
Mode1(5),
Mode2(6),
CalPin(7)
{

}

void CAirRide::Init()
{
    Serial.println("*******GMC Air Ride Controller V1.0*********");

    SampleTime = millis();

    SetState(RUNMANUAL);

    pinMode(2, INPUT_PULLUP);
    pinMode(Mode1, INPUT_PULLUP);
    pinMode(Mode2, INPUT_PULLUP);
    pinMode(CalPin, INPUT_PULLUP);
  
    CornerLR.Init(LeftRear); 
    CornerRR.Init(RightRear);
  
}

void CAirRide::SetState(states_t s)
{
    static states_t laststate = DUMPINGTANK;
    static char *statestrs[] = {STATES_LIST(STRINGIFY)};
    
    if(s != laststate)
    {
        Log(MODULE, "MainState", statestrs  [s]);
        laststate = s;
    }
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
    int a = analogRead(A4);
    bool pressed = false;
    
    if( a < 512 )
    {
        pressed = true;
    }
}

//only called when appropriate
//button must transition as follows
//not pressed for at least 5 seconds
//pressed for 5 seconds
//released
//once released were in calibrate mode
bool CAirRide::Calibrate()
{
    static int calstate = 0;
    bool docal = false;
    static uint32_t pressstart;
    
    bool pressed = (LOW == digitalRead(CalPin));

    
    switch(calstate)
    {
        case 0: 
            //don't start timer until button is not pressed
            if(!pressed)
            {
                pressstart = millis();
                calstate++;
            }
            break;
        case 1: 
            if(pressed)
            {
                if(IsTimedOut(5000, pressstart))
                {
                    //Button not pressed long enough!
                    //Now wait another 5 seconds
                    pressstart = millis();
                    calstate++;
                }
                else
                {
                    //button pressed too soon, restart timer
                    calstate = 0;
                }
            }
        
        case 2:
            if(!pressed)
            {
                if(IsTimedOut(5000, pressstart))
                {
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

void CAirRide::GetMode()
{
    int m = digitalRead(Mode1);
    
    m &= digitalRead(Mode2) << 2;
    
    mode = (states_t)m;
    
    Log(MODULE, "Mode", mode);
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
                case AUTOMODE:
                case AUTOCALMODE:
                SetState(mode);
                break;
                //No mode change
                case TRAVELMODE:
                //find the high and low limits
                if(Calibrate())
                {
                    SetState(CALLIMITS);
                }
                //what are the chances of dumptank and calibrate being pressed at the exact same time?
                if(DumpTank())
                {
                    SetState(DUMPTANK);
                }
                break;
            }
            break;
        case RUNMANUAL:
            switch(mode)
            {
                case TRAVELMODE:
                case AUTOMODE:
                case AUTOCALMODE:
                SetState(mode);
                break;
                case MANUALMODE:
                //save the current height as the travel height
                if(Calibrate())
                {
                    EEProm.SaveLeftHeight(LeftHeight); 
                    EEProm.SaveRightHeight(RightHeight);
                    EEProm.WriteEEprom();
                    SetState(CALDONELED);
                }
                
                if(DumpTank())
                {
                    SetState(DUMPTANK);
                }
                break;
            }
            break;
        case RUNAUTO:
            switch(mode)//no calibration to do
            {
                case TRAVELMODE:
                case MANUALMODE:
                case AUTOCALMODE:
                SetState(mode);
                break;
                case AUTOMODE:
                if(DumpTank())
                {
                    SetState(DUMPTANK);
                }
                break;
            }
            break;
        case RUNAUTOCAL:
            switch(mode)
            {
                case TRAVELMODE:
                case MANUALMODE:
                case AUTOMODE:
                SetState(mode);
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
                
                if(DumpTank())
                {
                    SetState(DUMPTANK);
                }
                break;
            }
            break;
        //no mode change allowed until the user lets go of the dump button, or we'd just end up back here
        case DUMPTANK:
        case DUMPINGTANK:
        //no change allowed during limit calibration
        case CALLIMITS: 
        case CALLOW:
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
                case AUTOMODE:
                case AUTOCALMODE:
                SetState(mode);
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
bool CAirRide::AllDown(int left, int right)
{
    static int oldleft = left;
    static int oldright = right;
    static uint32_t timeout = millis();
    static uint32_t mintime = millis();
     
    bool leftdown = false;
    bool rightdown = false;
    bool alldown = false;
    
    if(IsTimedOut(1000, timeout))
    {
        //less than 2 counts in 1 second means we're all the way down
        if(oldleft - left < 2)
        {
            leftdown = true;
        }
        
        if(oldright - right < 2)
        {
            rightdown = true;
        }
        
        //both heights stopped moving and we have waited at least 10 seconds
        if((leftdown && rightdown)&& IsTimedOut(10000, mintime))
        {
            alldown = true;
            LeftLowLimit = left;
            RightLowLimit = right;
        }
        
        timeout = millis();
        oldleft = left;
        oldright = right;
    }
    return alldown;
}

//see all down comments
bool CAirRide::AllUp(int left, int right)
{
    static int oldleft = left;
    static int oldright = right;
    static uint32_t timeout = millis();
    static uint32_t mintime = millis();
    
    bool leftup = false;
    bool rightup = false;
    bool allup = false;
    
    if(IsTimedOut(1000, timeout))
    {
        //less than 2 counts in 1 second means we're all the way down
        if(left - oldleft < 2)
        {
            leftup = true;
        }
        
        if(right - oldright < 2)
        {
            rightup = true;
        }
        
        //both heights stopped moving and we have waited at least 10 seconds
        if((leftup && rightup) && IsTimedOut(10000, mintime))
        {
            allup = true;
            LeftHighLimit = left;
            RightHighLimit = right;
        }
        
        timeout = millis();
        oldleft = left;
        oldright = right;
    }
    return allup;
}

//turn on Cal LED
void CAirRide::CalLED( bool on)
{
    if(on)
    {
        //make output and turn on LED
        pinMode(CalPin, OUTPUT);
        digitalWrite(CalPin, HIGH);
    }
    else
    {
        digitalWrite(CalPin, LOW);
        pinMode(CalPin, INPUT_PULLUP);
        
    }
}

void CAirRide::Run() 
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
        //todo change update frequency 
        case RUNTRAVEL:
            CornerLR.Run(EEProm.GetLeftCal());  
            CornerRR.Run(EEProm.GetRightCal());  
            break;
            
        //Auto level to the horizon
        case RUNAUTO:
            //read accel
            CaclulateLevel();
            CornerLR.Run(LeftLevel);  
            CornerRR.Run(RightLevel);
            break;
            
        //manually level the coach to the horizon
        //then press caibrate to save the acellerometer values
        //to use for autoleveling
        case RUNAUTOCAL:
            CornerLR.Run(0-Tilt);  
            CornerRR.Run(Tilt);
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
            if(!DumpTank())
            {
                CornerLR.Fill(Closed);
                CornerLR.Dump(Closed);
            
                CornerRR.Fill(Closed);
                CornerRR.Dump(Closed);
                
                //allow the event handler to switch us back to the current state
                SetState(CALCOMPLETE);
            }
            break;
            
        //raise and lower the coach to find the upper and lower limits of travel 
        //must be in travelmode before pressing cal button
        case CALLIMITS:
            //turn on Cel LED at start of calibration cycle
            CalLED(true);
            
            CornerLR.Dump(Open);
            CornerLR.Dump(Open);
            SetState(CALLOW);
            break;
        case CALLOW:
            //wait for height to stop changing
            if(AllDown(LRheight, RRheight))
            {
                CornerLR.Dump(Closed);
                CornerLR.Dump(Closed);
                CornerLR.Fill(Open);     
                CornerRR.Fill(Open);
                
                SetState(CALHIGH);
            }
            break;
        case CALHIGH:
            if(AllUp(LRheight, RRheight))
            {
                CornerLR.Fill(Closed);     
                CornerRR.Fill(Closed);
                
                SetState(CALSAVELIMITS);
            }
            break;
        case CALSAVELIMITS:
            EEProm.SaveLimits(LeftLowLimit, LeftHighLimit, RightLowLimit, RightHighLimit);
           
            CornerLR.Limits(LeftLowLimit, LeftHighLimit);
            CornerRR.Limits(RightLowLimit, RightHighLimit);
            
            SetState(CALDONE);
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
