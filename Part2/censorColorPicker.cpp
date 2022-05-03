/*
*  File: censorColorPicker.cpp
*  Implements the censor color picker for CPSC 221 PA2.
*
*/

#include "censorColorPicker.h"

#include <math.h> // gives access to sqrt function

/*
*  Useful function for computing the Euclidean distance between two PixelPoints
*/
double PointDistance(PixelPoint a, PixelPoint b) {
  unsigned int diff_x, diff_y;
  if (a.x > b.x)
    diff_x = a.x - b.x;
  else
    diff_x = b.x - a.x;
  if (a.y > b.y)
    diff_y = a.y - b.y;
  else
    diff_y = b.y - a.y;
  return sqrt(diff_x * diff_x + diff_y * diff_y);
}




 /*
    *  Constructs a new CensorColorPicker.
    *
    *  PARAM: b_width - width of each mosaic block in pixels.
    *  PARAM: ctr - middle/center point of mosaic area
    *  PARAM: rad - maximum pixel distance away from center where the mosaic color will be applied
    *  PARAM: inputimage - Original input image used to determine average color for a block
    */

CensorColorPicker::CensorColorPicker(unsigned int b_width, PixelPoint ctr, unsigned int rad, PNG& inputimage)
{
  blockwidth = b_width; // width of a mosaic block in pixels.
  center = ctr;       // middle/center point of mosaic area
  radius = rad;     // maximum pixel distance away from center where the mosaic color will be applied
  img = inputimage;                 // Original image used in flood fill algorithm
  blockyimg = inputimage;     // fully block-ified version of the original image, computed once at creation time
  
}







/*
    *  Picks the color for pixel (x, y). There is a circular region in which the fill is "active".
    *  Outside that region, it simply returns the colour from the original image. Inside the active
    *  region, this instead returns a color picked from the given (x, y) location in a blocky
    *  "mosaic image" that you should precompute in the constructor above. (So, THIS function's
    *  main job is just deciding which image to find the pixel in based on the circular active region.)
    *
    *  The mosaic image is divided into blockwidth x blockwidth square blocks, where the H/S/L/A values
    *  (x, y) will be set as the average H/S/L/A values of all pixels in the block of the
    *  original image where (x, y) resides.
    *
    *  Each square region has its upper left corner at a point where its
    *  image coordinates are divisible by blockwidth,
    *  e.g. for blockwidth = 5, the upper left corners of the blocks will be at
    *       (0,0),  (5,0),  (10,0), ...
    *       (0,5),  (5,5),  (10,5), ...
    *       (0,10), (5,10), (10,10), etc.
    *
    *  NOTE: Unlike PA1, do not be concerned about using the minimum angular interval(s)
    *        for computing hue average. Just use a simple arithmetic mean.
    *
    *  PARAM:  p - The point for which you're picking a color
    *  RETURN: The color chosen for (p).
    */

HSLAPixel CensorColorPicker::operator()(PixelPoint p)
{ 
  // complete your implementation below
  HSLAPixel* pixel = blockyimg.getPixel(p.x,p.y);

  if(PointDistance(p, center)> radius ){
    return *pixel;
  }

  int startX = p.x-p.x%blockwidth;

  int startY = p.y-p.y%blockwidth;

  double total_hue = 0;
  double total_lum = 0;
  double total_a = 0;
  double total_s = 0;
  
  HSLAPixel* get_hue = new HSLAPixel();
  
  for(int y = startY; y < startY+blockwidth; y ++){
      for(int x = startX; x< startX+blockwidth; x++){
        total_hue += img.getPixel(x, y)->h;
        total_lum += img.getPixel(x, y)->l;
        total_a += img.getPixel(x, y)->a;
        total_s += img.getPixel(x, y)->s;

      }
  }
  total_hue =(double) total_hue/ (blockwidth*blockwidth);
  total_lum =(double) total_lum/ (blockwidth*blockwidth);
  total_s =(double) total_s/ (blockwidth*blockwidth);
  total_a =(double) total_a/ (blockwidth*blockwidth);

  pixel->h = total_hue;

   pixel->l = total_lum;
  
   pixel->s = total_s;
   pixel->a = total_a;
  return *pixel;
  
  
}
