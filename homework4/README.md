## Homework 4

# 1 Fast Gaussian blur

Since gaussian blur has a separable kernel we where able to speed up it's implementation by splitting it in two passes.
The pass 0 renders the scene in the first framebuffer, the pass 1 renders the first framebuffer in the second with horizontal blur while the pass 2
renders the horizontal blurred frambuffer on screen, blurring it vertically, thus achieving fast gaussian blur.

# 2 Mirror floor

Using the framebuffer to render the scene flipped vertically in the first pass. We then render the scene normally with the floor having pass 0 as a texture.
The screen space texture is then mixed with the diffuse one. Note : to achieve such an effect on old hardware one could have used stencils buffers to render upsidedown
scene only in the floor and the render transparent floor.

# 3 Motion blur

By using multi-target rendering we were able to output a 'speed-map' that we then could use as an input to the post-processing stage. This way, we could control the
blur direction and amount of each pixel in the image.

# 4 Refractive cube

By using the Mirror example as a base. We inversed the role of the floor and the cube. This time we render the scene with the floor being fully diffuse and the cube gets the
frame buffer as an input texture in the second stage. We then generate a tangent space in the fragment shader using dfd{x,y} and then add procedural normal to each face of the
cube. The view vector is then refracted by the normal and this refraction serve as an offset in the framebuffer texture lookup. This way more diffracted area would indeed 
get more distortions from the 'glass cube'. 
