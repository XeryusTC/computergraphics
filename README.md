Raytracer
=========

Super sampling
--------------
To configure supersampling add the following to the yaml file:

SuperSampling:
  factor: <factor> 
  mode: <mode>

Where:
- factor (integer): the number of horizontal and vertical subpixels. In total
  factor^2 rays are cast for each pixel
- mode [grid|jitter|random]: Change the subpixel selection scheme. grid creates
  a factor * factor grid of subpixels and casts rays toward the center of each
  subpixel. Jitter creates the same grid but casts rays to a random position
  within each subpixel. Random casts factor * factor rays to random positions
  within the pixel.

