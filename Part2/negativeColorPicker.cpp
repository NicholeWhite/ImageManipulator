/*
*  File: negativeColorPicker.cpp
*  Implements the negative color picker for CPSC 221 PA2.
*
*/

#include "negativeColorPicker.h"

NegativeColorPicker::NegativeColorPicker(PNG& inputimg)
{
  // complete your implementation below
  img = inputimg; 
}

HSLAPixel NegativeColorPicker::operator()(PixelPoint p)
{ 
  HSLAPixel color; 
  
  HSLAPixel *pixel = img.getPixel(p.x, p.y);

  color = *pixel;
  
  if(color.h - 180 < 0){
    color.h += 180;
    
  } else {
    color.h -= 180;
    
  }
  color.l = 1 - color.l;

  return color;

}
