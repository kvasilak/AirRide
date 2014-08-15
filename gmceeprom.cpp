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
**    eeprom.cpp
**************************************************************************/
#include <EEPROM.h>
#include "gmceeprom.h"

CEEprom::CEEprom():
LeftTravelHeightAddr(0),
RightTravelHeightAddr(2),
LeftAutoHeightAddr(4),
RightAutoHeightAddr(6),
LeftLowAddr(8),
LeftHighAddr(10),
RightLowAddr(12),
RightHightAddr(14)
{
}

int16_t CEEprom::GetLeftTravel()
{
    int16_t val = ReadInt(LeftTravelHeightAddr);
    
    if( val == 0xFFFF)
    {
        val = 512;
    }
}

int16_t CEEprom::GetRightTravel()
{
    int16_t val = ReadInt(RightTravelHeightAddr);
    
    if( val == 0xFFFF)
    {
        val = 512;
    }
}

int16_t CEEprom::GetLeftAuto()
{
    int16_t val = ReadInt(LeftAutoHeightAddr);
    
    if( val == 0xFFFF)
    {
        val = 512;
    }
}

int16_t CEEprom::GetRightAuto()
{
    int16_t val = ReadInt(RightAutoHeightAddr);
    
    if( val == 0xFFFF)
    {
        val = 512;
    }
}

void CEEprom::GetLimits(int16_t *leftlow, int16_t *lefthigh, int16_t *rightlow, int16_t *righthigh)
{
    *leftlow = ReadInt(LeftLowAddr);   
    if( *leftlow == 0xFFFF)
    {
        *leftlow = 512;
    }
    
    *lefthigh = ReadInt(LeftHighAddr);
    if( *lefthigh == 0xFFFF)
    {
        *lefthigh = 512;
    }
    
    *rightlow = ReadInt(RightLowAddr);
    if( *rightlow == 0xFFFF)
    {
        *rightlow = 512;
    }
    
    *righthigh = ReadInt(RightHightAddr);
    if( *righthigh == 0xFFFF)
    {
        *righthigh = 512;
    }
}

/////Saves//////////////////////////////////////////////////////////////////////
void CEEprom::SaveLeftTravel(int16_t v)
{
    SaveInt(LeftTravelHeightAddr, v);
}

void CEEprom::SaveRightTravel(int16_t v)
{
    SaveInt(RightTravelHeightAddr, v);
}

void CEEprom::SaveLeftAuto(int16_t v)
{
    SaveInt(LeftAutoHeightAddr, v);
}

void CEEprom::SaveRightAuto(int16_t v)
{
    SaveInt(RightAutoHeightAddr, v);
}

void CEEprom::SaveLimits(int16_t leftlow, int16_t lefthigh, int16_t rightlow, int16_t righthigh)
{
    SaveInt(LeftLowAddr, leftlow);
    SaveInt(LeftHighAddr, lefthigh);
    SaveInt(RightLowAddr, rightlow);
    SaveInt(RightHightAddr, righthigh);
}

//low byte into address
//hi byte into address +1
void CEEprom::SaveInt(int address, int16_t v)
{
    EEPROM.write(address, (v & 0xFF));
    
    EEPROM.write(address+1, (v >> 8));
}

int16_t CEEprom::ReadInt(int address)
{
    int16_t val = EEPROM.read(address);
    
    int16_t valh = (EEPROM.read(address+1) << 8);
    
    return valh + val;;
}