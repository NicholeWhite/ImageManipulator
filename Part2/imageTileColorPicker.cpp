/*
*  File: imageTileColorPicker.cpp
*  Implements the image tile color picker for CPSC 221 PA2.
*
*/

#include "imageTileColorPicker.h"

ImageTileColorPicker::ImageTileColorPicker(PNG& otherimage) {
  // complete your implementation below
  img_other = otherimage;
}

HSLAPixel ImageTileColorPicker::operator()(PixelPoint p) {
  // complete your implementation below
  HSLAPixel color; 
  
  int x = p.x % img_other.width();
  int y = p.y % img_other.height();
  
  HSLAPixel *pixel = img_other.getPixel(x, y);
  color = *pixel;

  return color;
}