
## Homework 2

## 1 Perspective Projection

We simply implemented the given matrix (with a few hiccups due to the matrices being column-indexed rather than row-indexed), and we could simplify a few entries because the volume is symmetric. The result of this is that the top is equal to -bottom, and right is equal to -left. Everything was written on the website, given by the link on the slides. 

## 2 Trackball

Trackball was achieved by using given projection formulae and the normalizing the result, taking cross product of the starting and ending vectors gave the rotation axis and arcsine of the length of the cross product gave the angle.

Zoomin was achieved by retaining old mouse y pos to get the 'delta' of the mouse movement, multiplying it with a konstant to achieve proper zoom speed, it is then just added to the right view matrix component to translate it. 

## 4 Water animation (bonus)
The bonus can be enabled and disabled by editing the constant in the grid vertex shader. The boolean SINE is true for the 3rd part, and false for the bonus. 

It features additive wave construction composed of several layer moving in different direction. Some whitening is done at waves top and textures coords are a bit distorted to mimic water. The waveform generator is just a absoluted sinusoid. This create some nice wave edges.
