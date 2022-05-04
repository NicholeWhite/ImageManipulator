# ImageManipulator
### Purpose
The first part of the project 

#### Part 1
- Represents an image as a doubly linked list structure, with each node representing one pixel
- Carves the image and removes nodes based on tolerance specifications; reduces memory used for the structure
- Renders the image (either carved or original) onto an output PNG, filling any carved pixels

#### Part 2
- Colors the specified region of the image using different functors
  - ImageTileColorPicker: replacement of pixels from a different image into the destination image
  - NegativeColorPicker: fills area with negated hue and saturation values
  - CensorColorpicker: covers a circular region of the image with a blocky mosaic censor of averaged colors
- Performs a flood fill on the image based on priority conditions using BFS or DFS; the image is filled based on which functor is passed as a parameter

#### Part 3
- A binary tree representation of a binary tree (PTree)
  - The root is the whole image and each child represents half of the parent
  - Leaf nodes represent individual pixels
- Prunes the tree and reduces the memory and size of the structure based on tolerance specifications; this causes the output image to have a more 'blocky' appearance
- Renders the the leaves of the tree onto an output PNG
- If specified, the tree/image can be flipped horizontally and vertically

#### TODOs Remaining
- Resolve memory leak in part 1
- Reorganize and add comments to code
