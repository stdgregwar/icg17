
## Homework 2

## 2 Trackball

Trackball was achieved by using given projection formulae and the normalizing the result, taking cross product of the starting and ending vectors gave the rotation axis and arcsine of the length of the cross product gave the angle.

Zoomin was achieved by retaining old mouse y pos to get the 'delta' of the mouse movement, multiplying it with a konstant to achieve proper zoom speed, it is then just added to the right view matrix component to translate it. 

## 4 Water animation (bonus)
The bonus can be enabled and disabled by editing the constant in the grid vertex shader.

It features additive wave construction composed of several layer moving in different direction. Some whitening is done at waves top and textures coords are a bit distorted to mimic water. The waveform generator is just a absoluted sinusoid. This create some nice wave edges.
