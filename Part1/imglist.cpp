// File:        imglist.cpp
// Date:        2022-01-27 10:21
// Description: Contains partial implementation of ImgList class
//              for CPSC 221 2021W2 PA1
//              Function bodies to be completed by yourselves
//
// ADD YOUR FUNCTION IMPLEMENTATIONS IN THIS FILE
//

#include "imglist.h"

#include <math.h> // provides fmin and fabs functions

# define M_PI           3.14159265358979323846;

/**************************
* MISCELLANEOUS FUNCTIONS *
**************************/

/*
* This function is NOT part of the ImgList class,
* but will be useful for one of the ImgList functions.
* Returns the "difference" between two hue values.
* PRE: hue1 is a double between [0,360).
* PRE: hue2 is a double between [0,360).
* 
* The hue difference is the absolute difference between two hues,
* but takes into account differences spanning the 360 value.
* e.g. Two pixels with hues 90 and 110 differ by 20, but
*      two pixels with hues 5 and 355 differ by 10.
*/
double HueDiff(double hue1, double hue2) {
    return fmin(fabs(hue1 - hue2), fabs(360 + fmin(hue1, hue2) - fmax(hue1, hue2)));
}

HSLAPixel* averageColor(ImgNode *west, ImgNode *east) {
  if (west != NULL && east != NULL) {
    // double radian = 180 * M_PI;
    // double westcos = (west->colour.h) / radian ;
    // westcos = cos(westcos);
    // double westsin = (west->colour.h)/ radian ;
    // westsin = sin(westsin);
    // double eastcos = (east->colour.h) / radian ;
    // eastcos = cos(eastcos);
    // double eastsin =  (east->colour.h) / radian ;
    // eastsin = sin(eastsin);
    // double w = westcos + eastcos;
    // double e = eastsin + westsin;
    // w /=2.0;
    // e /=2.0;
    // double h1 = atan2(e,w) * radian;
    // double h2 = (east->colour.h + west->colour.h)/2;
    // double h = 0.0;
    // if (h1 < h2) {
    //   h = h1;
    // } else {
    //   h = h2;
    // }
    double h1 = (east->colour.h + west->colour.h)/2;
    //double h1 = (atan2(e,w) * radian;)
    double diff = (west->colour.h -east->colour.h + 180 +360);
    diff = fmod(diff, 360);
    diff -= 180;
    double h = (360 + east->colour.h + (diff / 2 ));
    h = fmod(h, 360);
   // double h2 = (west->colour.h + (360 - west->colour.h + east->colour.h) / 2);
    // double h = 0.0;
    if (( west->colour.h >= east ->colour.h - 180 ) && (west->colour.h <= east->colour.h + 180)) {
      h = h1;
    }
    double s =(west->colour.s + east->colour.s)/2.0;
    double l = (west->colour.l + east->colour.l)/2.0;
    double a = (west->colour.a + east->colour.a)/2.0;
    HSLAPixel *color = new HSLAPixel(h,s,l,a);
    return color;
  } else if (west != NULL && east == NULL){
    HSLAPixel *color = new HSLAPixel(west->colour.h,west->colour.s,west->colour.l,west->colour.a);
    return color;
  } else if (east != NULL && west == NULL){
    HSLAPixel *color = new HSLAPixel(east->colour.h,east->colour.s,east->colour.l,east->colour.a);
    return color;
  } else {
    HSLAPixel *color = new HSLAPixel();
    return color;
  }
  
}

/*********************
* CONSTRUCTORS, ETC. *
*********************/

/*
* Default constructor. Makes an empty list
*/
ImgList::ImgList()
{
  ImgNode* nodeList = new ImgNode();
}

/*
* Creates a list from image data
* PRE: img has dimensions of at least 1x1
*/
ImgList::ImgList(PNG& img) {
  // build the linked node structure and set the member attributes appropriately
  ImgNode* currNode;
  ImgNode* northNode;
  ImgNode* eastNode = NULL; 
  ImgNode* southNode = NULL; //?
  ImgNode* westNode = NULL; 

  ImgNode* temp;
  
  for(unsigned x = 0; x < img.width(); x++) {
    
    for (unsigned y = 0; y < img.height(); y++){
        
        currNode = new ImgNode();

        HSLAPixel* pixel = img.getPixel(x,y);
        currNode -> colour = *(pixel);

        if(y == 0 && x == 0){
          

          northwest = currNode;

          currNode -> west = NULL;
          currNode -> north = NULL;

         
          
          northNode = currNode;
          
          westNode = currNode;
          temp = northwest;


        } else if (y!= 0 && x == 0){
          currNode -> west = NULL;
          northNode -> south = currNode;
          currNode -> north = northNode;

          northNode = currNode;

          if(y == img.height()-1){
            currNode -> south = NULL;
            northNode = NULL;
          }
          


        } else if (y == 0 & x != 0){
          currNode -> north = NULL;
          currNode -> west = westNode;
          westNode -> east = currNode;

          northNode = currNode;
          
          westNode = westNode -> south;
          temp = currNode;

          if(x == img.width()-1){
            currNode -> east = NULL;
          }


        } else {
          if(y == img.height()-1 && x == img.width()-1){
            southeast = currNode;
            currNode -> east = NULL;
            currNode -> south = NULL;
            currNode -> north = northNode;
            currNode -> west = westNode;

            northNode -> south = currNode;
            westNode -> east = currNode;
            

          } else if (y == img.height()-1 && x != img.width()-1){
            currNode -> south = NULL;
            currNode -> north = northNode;
            northNode -> south = currNode;
            currNode -> west = westNode;
            westNode -> east = currNode;

            northNode = NULL;
            westNode = temp;


          } else if (y != img.height()-1 && x == img.width()-1){
            currNode -> north = northNode;
            currNode -> east = NULL;
            currNode -> west = westNode;
            westNode -> east = currNode;
            northNode -> south = currNode;

            westNode = westNode -> south;
            northNode = currNode;

          } else {
            currNode -> north = northNode;
            currNode -> west = westNode;
            northNode -> south = currNode;
            westNode -> east = currNode;

            westNode = westNode -> south;
            northNode = currNode;
            
          }
        }
        //delete currNode;
        //currNode = NULL;

    }
   }

  // Clear();
   
  cout << "FINISHED CONSTRUCTOR NODE \n";
}


/*
* Copy constructor.
* Creates this this to become a separate copy of the data in otherlist
*/
ImgList::ImgList(const ImgList &otherlist)
{
  // build the linked node structure using otherlist as a template
  Copy(otherlist);
}

/*
* Assignment operator. Enables statements such as list1 = list2;
*   where list1 and list2 are both variables of ImgList type.
* POST: the contents of this list will be a physically separate copy of rhs
*/
ImgList &ImgList::operator=(const ImgList &rhs)
{
  // Re-build any existing structure using rhs as a template

  if (this != &rhs)
  { // if this list and rhs are different lists in memory
    // release all existing heap memory of this list
    Clear();

    // and then rebuild this list using rhs as a template
    Copy(rhs);
  }

  return *this;
}

/*
* Destructor.
* Releases any heap memory associated with this list.
*/
ImgList::~ImgList()
{
  // Ensure that any existing heap memory is deallocated
  Clear();
}

/************
* ACCESSORS *
************/

/*
* Returns the horizontal dimension of this list (counted in nodes)
* Note that every row will contain the same number of nodes, whether or not
*   the list has been carved.
* We expect your solution to take linear time in the number of nodes in the
*   x dimension.
*/
unsigned int ImgList::GetDimensionX() const
{
  // replace the following line with your implementation
  int i = 0;
  ImgNode *current = northwest;

  while (current != NULL)
  {
    current = current->east;
    i++;
  }
  return i;
}

/*
* Returns the vertical dimension of the list (counted in nodes)
* It is useful to know/assume that the grid will never have nodes removed
*   from the first or last columns. The returned value will thus correspond
*   to the height of the PNG image from which this list was constructed.
* We expect your solution to take linear time in the number of nodes in the
*   y dimension.
*/
unsigned int ImgList::GetDimensionY() const
{
  // replace the following line with your implementation
  int i = 0;
  ImgNode *current = northwest;

  while (current != NULL)
  {
    
    current = current->south;
    i++;
  }
  return i;
}

/*
* Returns the horizontal dimension of the list (counted in original pixels, pre-carving)
* The returned value will thus correspond to the width of the PNG image from
*   which this list was constructed.
* We expect your solution to take linear time in the number of nodes in the
*   x dimension.
*/
unsigned int ImgList::GetDimensionFullX() const
{
  // replace the following line with your implementation
  unsigned int i = 0;
  unsigned int skipVals;
  ImgNode* current = northwest;

  while (current != NULL)
  {
    skipVals = current->skipright;
    current = current->east;
    i++;
    i += skipVals;
  }
  return i;
}

/*
* Returns a pointer to the node which best satisfies the specified selection criteria.
* The first and last nodes in the row cannot be returned.
* PRE: rowstart points to a row with at least 3 physical nodes
* PRE: selectionmode is an integer in the range [0,1]
* PARAM: rowstart - pointer to the first node in a row
* PARAM: selectionmode - criterion used for choosing the node to return
*          0: minimum luminance across row, not including extreme left or right nodes
*          1: node with minimum total of "hue difference" with its left neighbour and with its right neighbour.
*        In the (likely) case of multiple candidates that best match the criterion,
*        the left-most node satisfying the criterion (excluding the row's starting node)
*        will be returned.
* A note about "hue difference": For PA1, consider the hue value to be a double in the range [0, 360).
* That is, a hue value of exactly 360 should be converted to 0.
* The hue difference is the absolute difference between two hues,
* but be careful about differences spanning the 360 value.
* e.g. Two pixels with hues 90 and 110 differ by 20, but
*      two pixels with hues 5 and 355 differ by 10.
*/
ImgNode *ImgList::SelectNode(ImgNode *rowstart, int selectionmode)
{
  ImgNode *current = rowstart->east;
  //ImgNode *next = rowstart->east->east;
  ImgNode *nodeToReturn = current;
  double lum = current->colour.l  ;
  
  double hueDiff = HueDiff(current->colour.h, current->east->colour.h) + HueDiff(current->colour.h, current->west->colour.h) ;
  double hueDifference = 0.0 ;
  while (current->east != NULL) {
    
    hueDifference = HueDiff(current->colour.h, current->east->colour.h) + HueDiff(current->colour.h, current->west->colour.h);
    if (selectionmode == 0 && current->colour.l < lum) {
        lum = current->colour.l;
        nodeToReturn = current;
    } else if (selectionmode == 1 && hueDifference < hueDiff)
      {
        hueDiff = hueDifference;
        nodeToReturn = current;
      }
      current = current->east;
      //next = next->east;
    }
    
 return nodeToReturn;
  //return NULL;
}

/*
* Renders this list's pixel data to a PNG, with or without filling gaps caused by carving.
* PRE: fillmode is an integer in the range of [0,2]
* PARAM: fillgaps - whether or not to fill gaps caused by carving
*          false: render one pixel per node, ignores fillmode
*          true: render the full width of the original image,
*                filling in missing nodes using fillmode
* PARAM: fillmode - specifies how to fill gaps
*          0: solid, uses the same colour as the node at the left of the gap
*          1: solid, using the averaged values (all channels) of the nodes at the left and right of the gap
*             Note that "average" for hue will use the closer of the angular distances,
*             e.g. average of 10 and 350 will be 0, instead of 180.
*             Average of diametric hue values will use the smaller of the two averages
*             e.g. average of 30 and 210 will be 120, and not 300
*                  average of 170 and 350 will be 80, and not 260
*          
*          2: *** OPTIONAL - FOR BONUS ***
*             linear gradient between the colour (all channels) of the nodes at the left and right of the gap
*             e.g. a gap of width 1 will be coloured with 1/2 of the difference between the left and right nodes
*             a gap of width 2 will be coloured with 1/3 and 2/3 of the difference
*             a gap of width 3 will be coloured with 1/4, 2/4, 3/4 of the difference, etc.
*             Like fillmode 1, use the smaller difference interval for hue,
*             and the smaller-valued average for diametric hues
*/
PNG ImgList::Render(bool fillgaps, int fillmode) const {
  // Add/complete your implementation below
  PNG outpng; //this will be returned later. Might be a good idea to resize it at some point.
  HSLAPixel* pixel = new HSLAPixel(200,1,0.5,1);
  

  PNG* out = new PNG(100,100);


  ImgNode* curr = northwest;
  ImgNode* temp = northwest -> south;
  
  out -> resize(GetDimensionX(),GetDimensionY());
  

  for(int y = 0; y < GetDimensionY(); y++){
    
    
    int xDim = GetDimensionX();

    for(int x = 0; x<= GetDimensionFullX(); x ++){
      
      if(curr == NULL){
        break;
      } 
      
      //delete pixel;
         
      *(pixel) = curr ->colour;
      *(out -> getPixel(x,y)) = *(pixel);
      
      if(fillgaps && curr -> skipright > 0){
        

        out -> resize(GetDimensionFullX(),GetDimensionY());
        
        if(fillmode == 0){
          
          *(out -> getPixel(x+1,y)) = *(pixel);
          
          if(curr -> skipright == 2){
            
            *(out -> getPixel(x+2,y)) = *(pixel);
            x++;
          }
          x++;

        } else if(fillmode == 1){
          
          HSLAPixel* prevColor;
          HSLAPixel* newColor;

          *(pixel) = *(out -> getPixel(x,y));
          curr -> colour = *(pixel);
          HSLAPixel* p = averageColor(curr, curr->east);
          *(pixel) = *(p);
           delete p;
           p = NULL;
          //newColor = averageColor(curr, curr->east);
           
          *(out -> getPixel(x+1,y)) = *(pixel);
          
          if(curr -> skipright == 2){
            
            *(out -> getPixel(x+2,y)) = *(pixel);
            x++;
          } 
          x++;

          

          } 
      }
      
      curr = curr -> east;

    }

    curr = temp;

    if(curr == NULL){
      break;
    }
  
    temp = temp -> south;
  }
  delete temp;
  temp = NULL;
  delete curr;
  curr = NULL;
  delete pixel;
  pixel = NULL;
  outpng = *(out);
  delete out;
  out = NULL;
  //Clear();
  return outpng;
  //return outpng;
}

/************
* MODIFIERS *
************/

/*
* Removes exactly one node from each row in this list, according to specified criteria.
* The first and last nodes in any row cannot be carved.
* PRE: this list has at least 3 nodes in each row
* PRE: selectionmode is an integer in the range [0,1]
* PARAM: selectionmode - see the documentation for the SelectNode function.
* POST: this list has had one node removed from each row. Neighbours of the created
*       gaps are linked appropriately, and their skip values are updated to reflect
*       the size of the gap.
*/
void ImgList::Carve(int selectionmode)
{

  ImgNode *current = northwest;
  ImgNode *removeNode = NULL;

  ImgNode *northNode = NULL;
  ImgNode *eastNode = NULL;
  ImgNode *southNode = NULL;
  ImgNode *westNode = NULL;
  
  while (current != NULL)
  {
    //if (current->east != NULL) {
      removeNode = SelectNode(current, selectionmode);
      northNode = removeNode->north;
      eastNode = removeNode->east;
      southNode = removeNode->south;
      westNode = removeNode->west;
    
    westNode->east = eastNode;
    westNode->skipright += 1;
    westNode->skipright += removeNode->skipright;

    eastNode->west = westNode;
    eastNode->skipleft += 1;
    eastNode->skipleft += removeNode->skipleft;
      
    if(southNode != NULL){
      if(northNode == NULL){
        southNode->north = NULL;
        southNode -> skipup += 1;
        
      } else {
        southNode -> north = northNode;
        southNode -> skipup += 1;
        southNode -> skipup += removeNode -> skipup;
      }
    }

    if(northNode != NULL){
      if(southNode == NULL){
        northNode -> south = NULL;
        northNode -> skipdown += 1;
      } else {
        northNode->south = southNode;
        northNode -> skipdown += 1;
        northNode -> skipdown += removeNode -> skipdown;
      }
    }

    removeNode->east = NULL;
    removeNode->west = NULL;
    removeNode->north = NULL;
    removeNode->south = NULL;

      delete removeNode;
      removeNode = NULL;
      current = current->south;
     
  }
    
}


// note that a node on the boundary will never be selected for removal
/*
* Removes "rounds" number of nodes (up to a maximum of node width - 2) from each row,
* based on specific selection criteria.
* Note that this should remove one node from every row, repeated "rounds" times,
* and NOT remove "rounds" nodes from one row before processing the next row.
* PRE: selectionmode is an integer in the range [0,1]
* PARAM: rounds - number of nodes to remove from each row
*        If rounds exceeds node width - 2, then remove only node width - 2 nodes from each row.
*        i.e. Ensure that the final list has at least two nodes in each row.
* POST: this list has had "rounds" nodes removed from each row. Neighbours of the created
*       gaps are linked appropriately, and their skip values are updated to reflect
*       the size of the gap.
*/
void ImgList::Carve(unsigned int rounds, int selectionmode)
{
  int maxRounds =  GetDimensionFullX() - 2;
  maxRounds = maxRounds;
  if (rounds > maxRounds) {
    rounds = maxRounds;
  }
  for (int i = 0; i < rounds; ++i)
    {
      Carve(selectionmode);
    }
   
    
}

/*
* Helper function deallocates all heap memory associated with this list,
* puts this list into an "empty" state. Don't forget to set your member attributes!
* POST: this list has no currently allocated nor leaking heap memory,
*       member attributes have values consistent with an empty list.
*/
void ImgList::Clear()
{
  ImgNode *head = northwest;
  ImgNode *current = head;
  ImgNode *next = NULL;

  while (current != NULL) {
    next = current->east;
    if (next == NULL) {
      delete current;
      current = head->south;
      head = head->south;
    } else {
      delete current;
      current = next;
      
    }
  }
  delete next;
  next = NULL;
  delete head; 
  head = NULL;
  delete current;
  current = NULL;
  
}

/* ************************
*  * OPTIONAL - FOR BONUS *
** ************************
* Helper function copies the contents of otherlist and sets this list's attributes appropriately
* PRE: this list is empty
* PARAM: otherlist - list whose contents will be copied
* POST: this list has contents copied from by physically separate from otherlist
*/
void ImgList::Copy(const ImgList &otherlist)
{
  // add your implementation here
}

/*************************************************************************************************
* IF YOU DEFINED YOUR OWN PRIVATE FUNCTIONS IN imglist.h, YOU MAY ADD YOUR IMPLEMENTATIONS BELOW */