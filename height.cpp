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
**    height.cpp
**************************************************************************/



CHeight::CHeight()
{
}

//This function returns the 100 point moving average
//the average is not calculated untill needed
//and only if a new sample has been added
int32_t CHeight::FastHeight(Position p)
{
    if(p == LeftRear)
    {
        if(NewFastLeft)
        {
            NewFastLeft = false;
            LeftFastHeight = CalcAvg(Left100);
        }
        else
        {
            return LeftFastHeight;
        }
    }
    else //RightRear:
    {
        if(NewFastRight)
        {
            NewFastRight = false;
            RightFastHeight = CalcAvg(Right100);
        }
        else
        {
            return RightFastHeight;
        }
    }
}

int32_t CHeight::SlowHeight(Position p)
{
    if(p == LeftRear)
    {
        if(NewSlowLeft)
        {
            NewSlowLeft = false;
            LeftSlowHeight = CalcSlowAvg(p, FastHeight(p));
        }
        else
        {
            return LeftSlowHeight;
        }
    }
    else //RightRear:
    {
        if(NewSlowRight)
        {
            NewSlowRight = false;
            RightSlowHeight = CalcSlowAvg(p, FastHeight(p));
        }
        else
        {
            return RightSlowHeight;
        }
        return RightSlowHeight;
    }
}

int32_t CHeight::SetPoint(Position p)
{
    if(p == LeftRear)
    {
        if(NewLeftSet)
        {
            NewLeftSet = false;
            LeftSetPoint = 
        }
        return LeftSetPoint;
    }
    else //RightRear:
    {
        return RightSetPoint;
    }
}

//Change filter constans based on Mode
//longer if in travel mode for instance
void Mode(states_t mode)
{

}

void CHeight::Run()
{
    //read height and add to 100 point filter once every 100 ms
    if(IstimedOut(100, SampleTime)
    {
//===== Left Fast
         Left100[Left100Count++] = (int32_t)analogRead(PINLRHEIGHT); 
         if(Left100Count >-100) Left100Count = 0;
         //FIR filter
         LeftFastHeight = CalcAvg(Left100, 100);

         
////====== Right Fast
         Right100[Right100Count++] = (int32_t)analogRead(PINRRHEIGHT);
         if(Right100Count >=100) Right100Count = 0;
         
         RightFastHeight = CalcAvg(Right100, 100);
                 
//====== Left Slow
             //IIR filter the FIR filter
             Leftfilter_reg = Leftfilter_reg - (Leftfilter_reg >> FILTER_SHIFT) + LeftFastHeight;
            // Scale output for unity gain.
            LeftSlowHeight = (Leftfilter_reg >> FILTER_SHIFT);

////====== Right Slow
             //IIR filter the FIR filter
             Rightfilter_reg = Rightfilter_reg - (Rightfilter_reg >> FILTER_SHIFT) + RightFastHeight;
            // Scale output for unity gain.
            RightSlowHeight = (Rightfilter_reg >> FILTER_SHIFT);
        
//====== Setpoint  
        //Read setpoint and add to filter
        SetPoint[SetCount++] = (int32_t)analogRead(PINSETPOINT);
        if(SetCount >=100) SetCount = 0;
        
        SmoothSetPoint = CalcAvg(Setpoint, 10);
        
//====== Tilt
        Tilt[TiltCount++] = (int32_t)analogRead(PINSETPOINT);
        if(TiltCount >=100) TiltCount = 0;
        
        smoothTilt = CalcAvg(Tilt, 10);
        
//====== LeftSet
        LeftSetPoint = SmoothSetPoint - smoothTilt;

//====== Right Set
        RightSetPoint = SmoothSetPoint + smoothTilt;
    }

}