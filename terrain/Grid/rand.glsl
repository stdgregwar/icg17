float rand(vec2 co)
{
   return fract(sin(dot(co.xy,vec2(12.9898,78.233))) * 43758.5453);
}

vec2 rand2(vec2 p) {
    return vec2(rand(p),rand(p.yx));
}
