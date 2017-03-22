# 1 Phong Shading
In the vertex shader, we projected normal into camera coordinates, the light vector is just
light_pos - point in camera coordinates, the view vector simply is the point,
since the view point is at position 0,0,0 in view coordinates.
In the fragment shader, we normalized all the previously computed vectors, then
we used the given formulas to compute the three component (ambient, diffuse,
specular), we also made sure to clamp any result of dot product not between 0
and 1.
# 2 Toon Shading
We bound the texture in mesh.
We used the same vertex shader code as in Phong Shading.
In the fragment shader, we used the same formulas as in Phong, but we mapped the
results of the dot products to the given texture.
# 3 Artistic Shading
Very similar to toon shading, the only difference is that we used the diffuse
and specular components as coordinates for the texture.
# 4 Flat Shading
Flat shading is a particular case of Phong shading where the normal is not interpolated from
vertex attribute but computed on the fly in the fragment shader based on the local derivative
of the surface in screen space (dFdx,dFdy), this technique is somehow costlier than the
previous ones. And additional cost of evalutating the derivates could be avoided by the use
of interpolation modifiers such as 'flat'. But apprently this was'nt the point of the exercise
here.
# 5 Spot Light Shading
Spot light shading is also derived from Phong shading but with a fallof multiplicative term
added to the light intensity formula. Taking one more uniform argument for the light direction
we achieved directionnal lighting by inputing the divergence of the view vector in the fallof
fonction and multiplying final light color by the obtained scalar.
