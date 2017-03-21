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
# 5 Spot Light Shading
