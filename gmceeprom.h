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
**    EEprom.h
**************************************************************************/
#ifndef INCLUDE_EEPROM_H
#define INCLUDE_EEPROM_H

#include <stdint.h>

class CEEprom
{
    public:
    CEEprom();
    
    int16_t GetLeftTravel();
    int16_t GetRightTravel();
    int16_t GetLeftAuto();
    int16_t GetRightAuto();
    void GetLimits(int16_t *leftlow, int16_t *lefthigh, int16_t *rightlow, int16_t *righthigh);
    
    void SaveLeftTravel(int16_t v);
    void SaveRightTravel(int16_t v);
    void SaveLeftAuto(int16_t v);
    void SaveRightAuto(int16_t v);
    void SaveLimits(int16_t leftlow, int16_t lefthigh, int16_t rightlow, int16_t righthigh);
    
    private:
    void SaveInt(int address, int16_t v);
    int16_t ReadInt(int address);
    
    const int LeftTravelHeightAddr;
    const int RightTravelHeightAddr;
    const int LeftAutoHeightAddr;
    const int RightAutoHeightAddr;
    const int LeftLowAddr;
    const int LeftHighAddr;
    const int RightLowAddr;
    const int RightHightAddr;
};
#endif
