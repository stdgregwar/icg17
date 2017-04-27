#version 330


#define amount 12
#define dist 20
#define adist 10

layout(triangles) in;
layout(triangle_strip, max_vertices=201) out;

in vData {
    vec2 uv;
    vec3 normal_m;
    vec3 w_pos;
    float base_color;
} vertices[];

out vData {
    vec2 uv;
    float alpha;
} vertex;

uniform mat4 VP;
uniform mat4 iV;

uniform float time;

float rand(vec2 co)
{
   return fract(sin(dot(co.xy,vec2(12.9898,78.233))) * 43758.5453);
}

vec2 rand2(vec2 p) {
    return vec2(rand(p),rand(p.yx));
}

void bladeAt(vec3 bpos,vec3 dir,float size) {
    float ph = rand(bpos.xy);
    vec3 top = dir*size+vec3(rand2(bpos.xy),0)*sin((time+ph)*20*ph)*0.2;
    vec3 side = normalize((iV*vec4(0.1,0,0,0)).xyz);
    side*=0.1;

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


void main()
{
  vec3 vPos = (iV*vec4(0,0,0,1)).xyz;
  vec3 cpos = (vertices[0].w_pos + vertices[1].w_pos + vertices[2].w_pos)/3.f;
  vec3 v1 = vertices[1].w_pos - vertices[0].w_pos;
  vec3 v2 = vertices[2].w_pos - vertices[0].w_pos;
  vec3 normal0 = vertices[0].normal_m;
  vec3 normal1 = vertices[1].normal_m;
  vec3 normal2 = vertices[2].normal_m;



  //const int amount = 8;

  vec4 ccpos = VP*vec4(cpos,1);
  vec3 nccpos = ccpos.xyz /ccpos.w;
  const float l = sqrt(2)+0.2;
  if(distance(vec3(0,0,0),nccpos) > l) {
      return;
  }


  int tamount = int(amount-clamp(distance(cpos,vPos)*0.5,0,amount));
  float fac = 1.f/tamount;

  v1 *= fac;
  v2 *= fac;

  //v1 = vec3(0.1,0,0);
  //v2 = vec3(0,0.1,0);

  const float baseSize = 1;


  for(int i = 0; i < tamount; i++) {
      for(int j = 0; j < tamount; j++) {
          if(j+i <= tamount-1) {
	      float ifa = float(i)/tamount;
	      float jfa = float(j)/tamount;
	      vec3 bpos = vertices[0].w_pos + v1 * i + v2 * j;
	      vec3 normal = normal0 * (1-jfa) * (1-ifa) + normal1 * ifa + normal2 * jfa;
	      normal = normalize(normal);
	      float normalFac = 1-pow(1-dot(normal,vec3(0,0,1)),4);
	      float bdist = distance(bpos,vPos);
	      if(bdist < dist && bpos.z > 0) {
		float alpha = clamp((dist-bdist)/(dist-adist),0,1);
		vertex.alpha = alpha;
                bladeAt(bpos/*+vec3(rand2(bpos.xy)*0.5,0)*/,normal,baseSize*normalFac);
	      }
	  }
      }
  }
}
