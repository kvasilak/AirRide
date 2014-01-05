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
**    common.h
**************************************************************************/
#ifndef INCLUDE_AIRRIDE_H
#define INCLUDE_AIRRIDE_H

#include "debug.h"
#include "corner.h"
#include "common.h"
#include "eeprom.h"

class CAirRide
{
    public:
    CAirRide();
    
    void Init();
    void SetState(states_t s);
    void CaclulateLevel();
    bool DumpTank();
    bool Calibrate();
    void GetMode();
    void CheckEvents();
    void Run();
    
    private:
    CCorner CornerLR; //LeftRear);
    CCorner CornerRR; //RightRear);
    CEEprom EEProm;

    const int PinTilt;// = 5;
    const int PinDumpTank;// = A5;
    const int Mode1;// = 5;
    const int Mode2;// = 6;

    states_t state;// = RUNMANUAL;

    uint32_t SampleTime;    
    uint32_t CalDoneTime;

    //the height valuse that will level the coach to the horizon
    //when camping
    int16_t LeftLevel;
    int16_t RightLevel;

    //the current height readings
    int16_t LRheight;
    int16_t RRheight;
    int16_t SetPoint;
    int16_t Tilt;
    int16_t LeftHeight;
    int16_t RightHeight;
    int16_t LeftAuto;
    int16_t RightAuto;
    
    states_t mode;// = MANUALMODE;

    //char *StateStrs[] = {STATES_LIST(STRINGIFY)};

};

#endif