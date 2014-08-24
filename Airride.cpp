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
#include "common.h"
#include "gmceeprom.h"
#include "airride.h"

CAirRide::CAirRide()
{
}

void CAirRide::Init()
{
    Serial.println(F(">AirRide,msg,GMC Air Ride Controller V1.0<"));

    SampleTime = millis();

    SetState(ENTERMANUAL);

    pinMode(PINMODE1, INPUT_PULLUP);
    pinMode(PINMODE2, INPUT_PULLUP);
    pinMode(PINCAL, INPUT_PULLUP);
  
    CornerL.Init(LeftRear); 
    CornerR.Init(RightRear);
    
    EEProm.GetLimits(&LeftLowLimit, &LeftHighLimit, &RightLowLimit, &RightHighLimit);
    
    //debug, force travel height to known value
    //EEProm.SaveLeftTravel(642); 
    //EEProm.SaveRightTravel(698);

    LTravelHeight = EEProm.GetLeftTravel(); //642
    RTravelHeight = EEProm.GetRightTravel(); //698
   
    CornerL.Limits(LeftLowLimit, LeftHighLimit);
    CornerR.Limits(RightLowLimit, RightHighLimit);
    
    Serial.print(F(">AirRide,msg,limts (Lt, Rt, Ll Lh,Rl Rh) "));
    Serial.print(LTravelHeight);
    Serial.print(F(","));
    Serial.print(RTravelHeight);
    Serial.print(F(","));
    Serial.print(LeftLowLimit);
    Serial.print(F(","));
    Serial.print(LeftHighLimit);
    Serial.print(F(","));
    Serial.print(RightLowLimit);
    Serial.print(F(","));
    Serial.print(RightHighLimit);
    Serial.println(F("<"));
    
    CheckEvents();
  
}

void CAirRide::SetState(states_t s)
{
    static states_t laststate = LASTSTATE;
    //const char * const statestrs[] PROGMEM = {STATES_LIST(STRINGIFY)};
    
    static const char s0[] PROGMEM = "RUNAUTOCAL";
    static const char s1[] PROGMEM = "RUNCAMP";
    static const char s2[] PROGMEM = "ENTERMANUAL";
    static const char s3[] PROGMEM = "ENTERTRAVEL";
    static const char s4[] PROGMEM = "CALLIMITS";
    static const char s5[] PROGMEM = "CALLOW";
    static const char s6[] PROGMEM = "CALWAITHIGH";
    static const char s7[] PROGMEM = "CALHIGH";
    static const char s8[] PROGMEM = "CALSAVELIMITS";
    static const char s9[] PROGMEM = "CALDONELED";
    static const char s10[] PROGMEM = "CALDONE";
    static const char s11[] PROGMEM = "CALCOMPLETE";
    static const char s12[] PROGMEM = "RUNMANUAL";
    static const char s13[] PROGMEM = "STARTTRAVEL";
    static const char s14[] PROGMEM = "RUNTRAVEL";
    static const char s15[] PROGMEM = "CALTRAVEL";
    static const char s16[] PROGMEM = "CALTRAVELDONE";
    static const char s17[] PROGMEM = "LASTSTATE";
    

    static const char * const statestrs[] PROGMEM = {s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13, s14, s15, s16, s17};
    
    if(s != laststate)
    {
        char buffer[15];
        strcpy_P(buffer, (char*)pgm_read_word(&(statestrs[s]))); // copy strings out of program space
    
        Serial.print(F("AirRide,MainState,"));
        Serial.print(buffer);
        Serial.println(F("<"));
        laststate = s;
    }
    state = s;
}

//calculate the Left and right height values that will make the coach level
void CAirRide::CaclulateLevel()
{
//todo...implement it!
//handle not enough suspension range issues
//handle travel limits, ranges stored in EEPROM
    LeftLevel = 512;
    RightLevel = 512;
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
    
    //N.c. switch
    //goes high when pressed, true == HIGH == Pressed
    bool pressed = (HIGH == digitalRead(PINCAL));
     
    switch(calstate)
    {
        case 0: 
            //don't start timer until button is not pressed
            if(!pressed)
            {
                Serial.println(F(">AirRide, CalState, NotPressed<"));
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
                    Serial.println(F(">AirRide, CalState, Pressed<"));
                    //Button un pressed long enough!
                    //Now wait another 5 seconds
                    pressstart = millis();
                    calstate++;
                }
                else //button pressed too soon, restart timer
                {
                    Serial.println(F(">AirRide, CalState, Released<"));
                    
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
                    Serial.println(F(">AirRide, CalState, Calibrate<"));
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
//   0        0       Camp calibrate 
//   0        1       Camp ( horizon level )
//   1        0       Manual
//   1        1       Travel 
void CAirRide::GetMode()
{
    static const char s0[] PROGMEM = "AUTOCALMODE";
    static const char s1[] PROGMEM = "CAMPMODE";
    static const char s2[] PROGMEM = "MANUALMODE";
    static const char s3[] PROGMEM = "TRAVELMODE";
    static const char * const modestrs[] PROGMEM = {s0, s1, s2, s3};
    
    static modes_t LastMode = CAMPMODE;//should always be different the first test
    //static const char *states[] = {MODES_LIST(STRINGIFY)};

    int m = digitalRead(PINMODE1);
    m |= digitalRead(PINMODE2) <<1;
  
    if(LastMode != m)
    {        
        mode = (modes_t)m;
        
        char buffer[15];
        strcpy_P(buffer, (char*)pgm_read_word(&(modestrs[m]))); // copy strings out of program space
        
        Serial.print(F(">AirRide, Mode"));
        Serial.print(buffer);
        Serial.println(F("<"));
        
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
        case STARTTRAVEL:
            switch(mode)
            {
                case MANUALMODE:
                case CAMPMODE:
                case AUTOCALMODE:
                SetState((states_t)mode);
                break;
                //No mode change
                case TRAVELMODE:
                if(Calibrate())
                {
                    //Set limits to full range so cal can find the new limits
                    //CornerL.Limits(0, 1024);
                    //CornerR.Limits(0, 1024);
            
                    //SetState(CALLIMITS);
                    
                    //stop managing height so user can set height manually with remote
                    SetState(CALTRAVEL);
                    MoveTimeOut = millis();
                }
                break;
            }
            break;
        case CALTRAVEL:
            if(Calibrate())
            {
                SetState(CALTRAVELDONE);
            }
            break;
        case RUNMANUAL:
            switch(mode)
            {
                case TRAVELMODE:
                case CAMPMODE:
                case AUTOCALMODE:
                
                SetState((states_t)mode);
                break;
                case MANUALMODE:
                //find the high and low travel limits
                if(Calibrate())
                {
                    CornerL.Limits(0, 1024);
                    CornerR.Limits(0, 1024);
            
                    SetState(CALLIMITS);
                                       
                    //SetState(CALDONELED);
                }
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
                break;
            }
            break;
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
        case CALTRAVELDONE:
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



//turn on Cal LED
void CAirRide::CalLED( bool on)
{
    if(on)
    {
        //make output and turn on LED
        pinMode(PINCAL, OUTPUT);
        digitalWrite(PINCAL, HIGH);
    }
    else
    {
        digitalWrite(PINCAL, LOW);
        pinMode(PINCAL, INPUT_PULLUP);
        
    }
}

void CAirRide::Run() 
{
    SetPoint = analogRead(PINSETPOINT);
    Tilt = analogRead(PINTILT)-512;
        
    //read the tilt setpoint and adjust heights as needed
    //center of pot means no tilt
    //increasing value (>512) means raise right side, lower left side
    //decreasing value (<512) means raise left side, Lover right side
    
    if(IsTimedOut(250, SampleTime))
    {      
        Serial.print(F("AirRide,SetPoint,"));
        Serial.print(SetPoint);
        Serial.println(F("<"));
        
        Serial.print(F("AirRide,Tilt,"));
        Serial.print(Tilt);
        Serial.println(F("<"));
               
        SampleTime = millis();
    }

    CheckEvents();
    
    switch(state)
    {
        //Manual leveling
        //need to pass in setpoint as read from set point pot
        case ENTERMANUAL:
            CornerL.SetLongFilter(false);
            CornerR.SetLongFilter(false);
            
            SetState(RUNMANUAL);
            break;
        case RUNMANUAL: 
            CornerL.Run(SetPoint - Tilt);  
            CornerR.Run(SetPoint + Tilt);
            break;
            
        //Run at the calibrated travel height
        //need to pass in travel height as read from EEPROM
        case ENTERTRAVEL:
            CornerL.SetLongFilter(false);
            CornerR.SetLongFilter(false);
            
            //reset the at height flag
            CornerL.AtHeight(false);
            CornerR.AtHeight(false);
            
            SetState(STARTTRAVEL);
            break;
            
        case STARTTRAVEL:
            //Start travel mode in a fast reacting mode
            //until height is reached
            if( CornerL.AtHeight() && CornerR.AtHeight() )
            {
                CornerL.SetLongFilter(true);
                CornerR.SetLongFilter(true);    

                SetState(RUNTRAVEL);
            }
            else
            {
                CornerL.Run(LTravelHeight);  
                CornerR.Run(RTravelHeight);
            }
        case RUNTRAVEL:
            CornerL.Run(LTravelHeight);  
            CornerR.Run(RTravelHeight);  
            break;
            
        //Auto level to the  for camping
        case RUNCAMP:
            //read accel
            CaclulateLevel();
            CornerL.Run(512);  
            CornerR.Run(512);
            break;
            
        //manually level the coach to the horizon
        //then press caibrate to save the acellerometer values
        //to use for autoleveling
        case RUNAUTOCAL:
            CornerL.Run(0);  
            CornerR.Run(0);
            break;
        //raise and lower the coach to find the upper and lower limits of travel 
        //must be in travelmode before pressing cal button
        case CALLIMITS:
            CornerL.Run(0);  
            CornerR.Run(0);
            
            if(CornerL.GetHeight() > 200)
            {
                if(CornerL.IsMoving() || CornerR.IsMoving() )
                {
                    MoveTimeOut = millis();
                    
                    SetState(CALLOW);
                }
                else
                {
                    //wait 10 seconds if no movement cancel calibration
                    if(true == IsTimedOut(10000, MoveTimeOut) )
                    {
                        Serial.println(F("AirRide,msg,Cal failed to Lower<"));
                        MoveTimeOut = millis();
                        
                        //SetState(CALDONELED);
                    }
                }
            }
            else //we may be all the way down already
            {
                MoveTimeOut = millis();
                    
                SetState(CALLOW);
            }
            break;
        case CALLOW:
            CornerL.Run(0);  
            CornerR.Run(0);
            
            //wait for car to be all the way down
            if(CornerL.IsMoving() || CornerR.IsMoving())
            {
                //keep resetting timeout until both corners stop moving
                MoveTimeOut = millis();
            }
            else
            {
                    //wait 10 seconds to see if we get any more movement
                if(IsTimedOut(10000, MoveTimeOut))
                {
                    Serial.println(F("AirRide,msg,All Down<"));
                    
                    MoveTimeOut = millis();
                    
                    LeftLowLimit = CornerL.GetHeight();
                    RightLowLimit = CornerR.GetHeight();
                    
                    Serial.print(F("AirRide,msg,Left low cal; "));
                    Serial.print(LeftLowLimit);
                    Serial.println(F("<")); 
                    
                    Serial.print(F("AirRide,msg,Right low cal; "));
                    Serial.print(RightLowLimit);
                    Serial.println(F("<"));

                    SetState(CALWAITHIGH);
                }

            }
            break;
        case CALWAITHIGH:
            CornerL.Run(1024);  
            CornerR.Run(1024);
            
            //wait for the car to start rising
            if(CornerL.IsMoving() || CornerR.IsMoving() )   
            {
                MoveTimeOut = millis();
                
                SetState(CALHIGH);
            }
            else
            {
                //wait 10 seconds if no movement cancel calibration
                if(IsTimedOut(20000, MoveTimeOut))
                {
                    Serial.println(F("AirRide,msg,Cal failed to rise<"));
                    MoveTimeOut = millis();
                    
                    SetState(CALDONELED);
                }
            }
            break;
        case CALHIGH:
            CornerL.Run(1024);  
            CornerR.Run(1024);
            
            if(CornerL.IsMoving() || CornerR.IsMoving() )
            {
                MoveTimeOut = millis();
            }
            else
            {
                //wait 10 seconds to see if we get any more movement
                if(IsTimedOut(20000, MoveTimeOut) )
                {
                    Serial.println(F("AirRide,msg,All up<"));
     
                    MoveTimeOut = millis();
                    
                    LeftHighLimit = CornerL.GetHeight();
                    RightHighLimit = CornerR.GetHeight();
                    
                    Serial.print(F("AirRide,msg,Left hi cal; "));
                    Serial.print(RightHighLimit);
                    Serial.println(F("<"));
                    
                    Serial.print(F("AirRide,msg,Right hi cal; "));
                    Serial.print(RightLowLimit);
                    Serial.println(F("<"));
                    
                    SetState(CALSAVELIMITS);
                }
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
            if(IsTimedOut(1000, MoveTimeOut))
            {
                CalLED(false);
                SetState(CALCOMPLETE);
            }
            break;
        case CALCOMPLETE:
            break;
            
        case CALTRAVEL:
            //dont adjust height, let the user adjust the height with the remote
            CornerL.DoHeight(CornerL.GetHeight(), 512);
            CornerR.DoHeight(CornerR.GetHeight(), 512);
            break;
        case CALTRAVELDONE:
            //save heights
            //return to where we were
            LTravelHeight = CornerL.GetHeight(); 
            RTravelHeight = CornerR.GetHeight(); 
    
            EEProm.SaveLeftTravel(LTravelHeight); 
            EEProm.SaveRightTravel(RTravelHeight);
                    
            Serial.print(F("AirRide,msg,Left Travel Height; "));
            Serial.print(LTravelHeight);
            Serial.println(F("<"));

            Serial.print(F("AirRide,msg,Right Travel Height; "));
            Serial.print(RTravelHeight);
            Serial.println(F("<"));                    

            break;
        default:
            SetState(RUNMANUAL);
    }
  
}
