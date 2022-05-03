/*
*  File:        ptree.cpp
*  Description: Implementation of a partitioning tree class for CPSC 221 PA3
*  Date:        2022-03-03 01:53
*
*               ADD YOUR PRIVATE FUNCTION IMPLEMENTATIONS TO THE BOTTOM OF THIS FILE
*/

#include "ptree.h"
#include "hue_utils.h" // useful functions for calculating hue averages
#include <queue>
#include <stack>


using namespace cs221util;
using namespace std;

// The following definition may be convenient, but is not necessary to use
typedef pair<unsigned int, unsigned int> pairUI;

/////////////////////////////////
// PTree private member functions
/////////////////////////////////

/*
*  Destroys all dynamically allocated memory associated with the current PTree object.
*  You may want to add a recursive helper function for this!
*  POST: all nodes allocated into the heap have been released.
*/
void PTree::Clear() {
  // add your implementation below
 // root = new Node();
 //if(root != nullptr){
    ClearHelper(root);

  //}
  
}

/*
*  Copies the parameter other PTree into the current PTree.
*  Does not free any memory. Should be called by copy constructor and operator=.
*  You may want a recursive helper function for this!
*  PARAM: other - the PTree which will be copied
*  PRE:   There is no dynamic memory associated with this PTree.
*  POST:  This PTree is a physically separate copy of the other PTree.
*/
void PTree::Copy(const PTree& other) {
  // add your implementation below
  root = operatorHelper(other.root);
}

/*
Helper for constructor
*/
Node* PTree::BuildNode(PNG& im, pair<unsigned int, unsigned int> ul, unsigned int w, unsigned int h) {
  // replace the line below with your implementation
  if (w <= 0 || h <= 0) {
    return nullptr;
  }
  
    Node* n = new Node();
    n->height = h;
    n->width = w;
    n->avg= averageColor(ul,w, h, im);
    n-> upperleft = ul;
    // n->A = NULL;

  if(w<=1 && h <=1){
    return n;
  }

  unsigned int wA = w;
  unsigned int wB = w;
  unsigned int hA = h;
  unsigned int hB = h;

  unsigned int xA = ul.first;
  unsigned int yA = ul.second;

  unsigned int xB = ul.first;
  unsigned int yB = ul.second;

  // decide whether we split horizontally or vertically
  if(w>=h){
    wA = w/2;
    wB = w - wA;
    xB += wA;
  } else{
    hA = h/2;
    hB = h - hA;
    yB += hA;
  }

  

    // n->B = NULL;
    n ->A = BuildNode(im, make_pair(xA,yA), wA, hA);
    n ->B = BuildNode( im, make_pair(xB,yB), wB, hB);

    return n;
  
}













////////////////////////////////
// PTree public member functions
////////////////////////////////

/*
*  Constructor that builds the PTree using the provided PNG.
*
*  The PTree represents the sub-image (actually the entire image) from (0,0) to (w-1, h-1) where
*  w-1 and h-1 are the largest valid image coordinates of the original PNG.
*  Each node corresponds to a rectangle of pixels in the original PNG, represented by
*  an (x,y) pair for the upper-left corner of the rectangle, and two unsigned integers for the
*  number of pixels on the width and height dimensions of the rectangular sub-image region the
*  node defines.
*
*  A node's two children correspond to a partition of the node's rectangular region into two
*  equal (or approximately equal) size regions which are either tiled horizontally or vertically.
*
*  If the rectangular region of a node is taller than it is wide, then its two children will divide
*  the region into vertically-tiled sub-regions of equal height:
*  +-------+
*  |   A   |
*  |       |
*  +-------+
*  |   B   |
*  |       |
*  +-------+
*
*  If the rectangular region of a node is wider than it is tall, OR if the region is exactly square,
*  then its two children will divide the region into horizontally-tiled sub-regions of equal width:
*  +-------+-------+
*  |   A   |   B   |
*  |       |       |
*  +-------+-------+
*
*  If any region cannot be divided exactly evenly (e.g. a horizontal division of odd width), then
*  child B will receive the larger half of the two subregions.
*
*  When the tree is fully constructed, each leaf corresponds to a single pixel in the PNG image.
* 
*  For the average colour, this MUST be computed separately over the node's rectangular region.
*  Do NOT simply compute this as a weighted average of the children's averages.
*  The functions defined in hue_utils.h and implemented in hue_utils.cpp will be very useful.
*  Computing the average over many overlapping rectangular regions sounds like it will be
*  inefficient, but as an exercise in theory, think about the asymptotic upper bound on the
*  number of times any given pixel is included in an average calculation.
* 
*  PARAM: im - reference image which will provide pixel data for the constructed tree's leaves
*  POST:  The newly constructed tree contains the PNG's pixel data in each leaf node.
*/
PTree::PTree(PNG& im) {
  
  root = BuildNode(im, make_pair(0,0), im.width(), im.height());
}


/*
*  Copy constructor
*  Builds a new tree as a copy of another tree.
*
*  PARAM: other - an existing PTree to be copied
*  POST:  This tree is constructed as a physically separate copy of other tree.
*/
PTree::PTree(const PTree& other) {
  
  // add your implementation below
  Clear();
  Copy(other);

  //operatorHelper(root, other.root);
}

/*
*  Assignment operator
*  Rebuilds this tree as a copy of another tree.
*
*  PARAM: other - an existing PTree to be copied
*  POST:  If other is a physically different tree in memory, all pre-existing dynamic
*           memory in this tree is deallocated and this tree is reconstructed as a
*           physically separate copy of other tree.
*         Otherwise, there is no change to this tree.

* Code adapted from https://stackoverflow.com/questions/35098658/memory-leak-in-assignment-operator-for-binary-search-tree
*/
PTree& PTree::operator=(const PTree& other) {
  // add your implementation below
 
   if (this != &other) {
        Clear();
        Copy(other);
    }
   return *this;
}

/*
*  Destructor
*  Deallocates all dynamic memory associated with the tree and destroys this PTree object.
*/
PTree::~PTree() {
  
  // add your implementation below
  Clear();
  
}

/*
*  Traverses the tree and puts the leaf nodes' color data into the nodes'
*  defined image regions on the output PNG.
*  For non-pruned trees, each leaf node corresponds to a single pixel that will be coloured.
*  For pruned trees, each leaf node may cover a larger rectangular region that will be
*  entirely coloured using the node's average colour attribute.
*
*  You may want to add a recursive helper function for this!
*
*  RETURN: A PNG image of appropriate dimensions and coloured using the tree's leaf node colour data
*/
PNG PTree::Render() const {
  // replace the line below with your implementation

  PNG out = PNG(root->width,root->height);
  
  queue<Node*> q{};

  q.push(root);
  
  while(!q.empty()){
    Node* current = q.front();
    if(current == nullptr){
      
      break;
    }

    if(current->A == nullptr && current->B == nullptr){//is a leaf node?
      *(out.getPixel(current->upperleft.first,current->upperleft.second)) = current->avg;
      for(int i = 0; i<current->width;i++){
        for(int j =0; j<current->height; j++){
        *(out.getPixel(current->upperleft.first+i,current->upperleft.second+j)) = current->avg;
        
        }
      }
    }else {
      q.push(current->A);
      q.push(current->B);
    }

    q.pop();
  }
  

  
     
  return out;
}

/*
*  Trims subtrees as high as possible in the tree. A subtree is pruned
*  (its children are cleared/deallocated) if ALL of its leaves have colour
*  within tolerance of the subtree root's average colour.
*  Pruning criteria should be evaluated on the original tree, and never on a pruned
*  tree (i.e. we expect that Prune would be called on any tree at most once).
*  When processing a subtree, you should determine if the subtree should be pruned,
*  and prune it if possible before determining if it has subtrees that can be pruned.
* 
*  You may want to add (a) recursive helper function(s) for this!
*
*  PRE:  This tree has not been previously pruned (and is not copied/assigned from a tree that has been pruned)
*  POST: Any subtrees (as close to the root as possible) whose leaves all have colour
*        within tolerance from the subtree's root colour will have their children deallocated;
*        Each pruned subtree's root becomes a leaf node.
*/
void PTree::Prune(double tolerance) {
  // add your implementation below

 pruneHelper(root, tolerance);


}

/*
*  Returns the total number of nodes in the tree.
*  This function should run in time linearly proportional to the size of the tree.
*
*  You may want to add a recursive helper function for this!
*/
int PTree::Size() const {
  return SizeHelper(root);
}

/*
*  Returns the total number of leaf nodes in the tree.
*  This function should run in time linearly proportional to the size of the tree.
*
*  You may want to add a recursive helper function for this!
*/
int PTree::NumLeaves() const {
  return NumLeavesHelper(root);
}

/*
*  Rearranges the nodes in the tree, such that a rendered PNG will be flipped horizontally
*  (i.e. mirrored over a vertical axis).
*  This can be achieved by manipulation of the nodes' member attribute(s).
*  Note that this may possibly be executed on a pruned tree.
*  This function should run in time linearly proportional to the size of the tree.
*
*  You may want to add a recursive helper function for this!
*
*  POST: Tree has been modified so that a rendered PNG will be flipped horizontally.
*/
void PTree::FlipHorizontal() {
  // add your implementation below
  FlipHorizontalHelper(root);
}

/*
*  Like the function above, rearranges the nodes in the tree, such that a rendered PNG
*  will be flipped vertically (i.e. mirrored over a horizontal axis).
*  This can be achieved by manipulation of the nodes' member attribute(s).
*  Note that this may possibly be executed on a pruned tree.
*  This function should run in time linearly proportional to the size of the tree.
*
*  You may want to add a recursive helper function for this!
*
*  POST: Tree has been modified so that a rendered PNG will be flipped vertically.
*/
void PTree::FlipVertical() {
  FlipVerticalHelper(root);
  // 
}

/*
    *  Provides access to the root of the tree.
    *  Dangerous in practice! This is only used for testing.
    */
Node* PTree::GetRoot() {
  return root;
}

//////////////////////////////////////////////
// PERSONALLY DEFINED PRIVATE MEMBER FUNCTIONS
//////////////////////////////////////////////

HSLAPixel PTree::averageColor(pair<unsigned int, unsigned int> ul,int w, int h, PNG& im){
  // get the average colour
  double sumX = 0; 
  double Xcount = 0;
  //vector<double> xs{};
  double sumY = 0;
  double ycount = 0;
  //vector<double> ys{};

  //vector<double> xLum{};
  double sumLum = 0;
  double lumcount = 0;

  //vector<double> xSat{};
  double sumSat = 0;
  double sumcount =0;

  double sumAlpha = 0;
  double alphacount = 0;

  for(int x = ul.first; x < ul.first+w; ++x) {
    for(int y = ul.second; y < ul.second+h; ++y) {
      sumX += (Deg2X(im.getPixel(x,y)->h));
      Xcount++;
      sumY += (Deg2Y(im.getPixel(x,y)->h));
      ycount++;
       sumLum +=  (im.getPixel(x,y)->l);
       lumcount++;
      sumSat += (im.getPixel(x,y)->s);
      sumcount++;
      sumAlpha += (im.getPixel(x,y)->a);
      alphacount++;
    }
  }
  // for (unsigned int i = 0; i < xs.size(); i++) {
  //   sumX += xs[i];
  //   sumY += ys[i];
  //   sumLum += xLum[i];
  //   sumSat += xSat[i];
  // }
  double avgX = sumX / Xcount;
  double avgY = sumY / ycount;

  double avgh = XY2Deg(avgX, avgY);

  double avgLum = sumLum / lumcount;
  double avgSat = sumSat / sumcount;
  double avgAlpha = sumAlpha / alphacount;


  return HSLAPixel(avgh, avgSat, avgLum, avgAlpha);
}

int PTree::NumLeavesHelper(Node* node) const {
 int count = 0;
  if(node == nullptr){
    return 0;
  } else if(node->A == nullptr && node->B == nullptr){
    return 1;
  } else{
    count += NumLeavesHelper(node->A);
    count += NumLeavesHelper(node->B);
  }

  return count;
} 

void PTree::FlipHorizontalHelper(Node* node){
        if (node == nullptr)
        return;
 
    FlipHorizontalHelper(node->A);
    FlipHorizontalHelper(node->B);
   node->upperleft.first = abs((( int)node->upperleft.first)- (int)root->width)-node->width;

}



void PTree::FlipVerticalHelper(Node* node){

  if (node == nullptr)
        return;
 
    FlipVerticalHelper(node->A);  
    FlipVerticalHelper(node->B);
   node->upperleft.second = abs((( int)node->upperleft.second)- (int)root->height)-node->height;
}

int PTree::SizeHelper(Node* node) const {
  if(node == nullptr){
    
    return 0;
  }
  return 1 + SizeHelper(node->A) + SizeHelper(node->B);
}


void PTree::ClearHelper(Node* node){
  if(node == nullptr){
    return;
  }



  ClearHelper(node->A);
  ClearHelper(node ->B);

  
  delete node;
  
}



// Helper adapted from: https://stackoverflow.com/questions/35098658/memory-leak-in-assignment-operator-for-binary-search-tree
Node* PTree::operatorHelper(const Node *other){
     if (other == nullptr){
       return nullptr;
    } 
    Node * current =  new Node(other->upperleft, other->width, other->height, other->avg);  
    current->A = operatorHelper(other->A);
    current->B = operatorHelper(other->B);
    return current; 
}






void PTree::pruneHelper(Node* n, double tolerance){
//use this and pass in prune helper to prune
  if (n == nullptr  ||(!n->A && !n->B)) {
    return;
  } else {
    bool left = false;
    bool right = false;
    
      
      left = toleranceCheck(n, tolerance, n->avg);
      
      if (left) {
       
        ClearHelper(n->A);
        n->A = NULL;
        ClearHelper(n->B);
        n->B = NULL;
      } else {
       
        pruneHelper(n->A, tolerance);
        pruneHelper(n->B, tolerance);
      }
    
 
   }

}

bool PTree::toleranceCheck(Node *n, double tolerance, HSLAPixel color) {
    if (n == NULL){
      return false;
    }
    double col = (double) color.dist(n->avg);
    

    if(n->A == nullptr && n->B == nullptr){
      if(col<= tolerance){
        return true;
      } else {
        return false;
      }
    }

    return toleranceCheck(n->A, tolerance, color)&&
     toleranceCheck(n->B, tolerance, color);
    
}