#version 330

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

in vData {
    vec2 uv;
    vec3 normal_m;
    vec3 w_pos;
    float base_color;
} vertices[];

out vData {
    vec2 uv;
    vec3 normal_m;
    vec3 w_pos;
    float base_color;
} vertex;

uniform mat4 VP;
uniform mat4 iV;

void main()
{
  vec3 vPos = (iV*vec4(0,0,0,1)).xyz;
  vec3 bpos = (vertices[0].w_pos + vertices[1].w_pos + vertices[2].w_pos)/3.f;

  if(distance(bpos,vPos) < 3) {
      vec3 top = vec3(0,0,1);
      vec3 side = vec3(0.1,0,0);

      vec3 pos = bpos + side;
      gl_Position = VP * vec4(pos,1.0);
      EmitVertex();

      pos = bpos + top;
      gl_Position = VP * vec4(pos,1.0);
      EmitVertex();

      pos = bpos -side;
      gl_Position = VP * vec4(pos,1.0);
      EmitVertex();

      EndPrimitive();
  }
}
