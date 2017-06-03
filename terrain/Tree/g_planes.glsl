#version 330

#define gtiles 6

uniform float time;
uniform mat4 VP;
uniform mat4 V;

uniform vec3 eyePos;
uniform float dist;

layout(points) in;
layout(triangle_strip, max_vertices=4) out;

in vData {
    int tex;
    vec3 pos;
    vec3 dir;
    vec3 view_dir;
} vertices[];

out vData {
    vec2 uv;
    vec3 normal;
    mat3 tSpace;
} vertex;

const vec3 up = vec3(0,0,1);

vec2 uvFrame(vec2 base, int i) {
    return vec2(float(i)/gtiles + base.x/gtiles,base.y);
}

void main() {
    if(distance(vertices[0].pos, eyePos) < dist) return;


    vec3 view = vertices[0].view_dir;
    int tex = vertices[0].tex;

    vec3 top = vertices[0].dir;
    vec3 side = normalize(cross(top,view));
    side*=length(top)*0.5;
    vec3 center = vertices[0].pos;

    //Make quad
    vertex.normal = normalize(mat3(V)*up);
    vertex.tSpace[0] = normalize(side);
    vertex.tSpace[1] = normalize(top);
    vertex.tSpace[2] = normalize(-view);

    vertex.tSpace = mat3(V)*vertex.tSpace;

    vec3 pos;

    vertex.uv = uvFrame(vec2(0,1),tex);
    pos = center+top-side;
    gl_Position = VP*vec4(pos,1);
    EmitVertex();


    vertex.uv = uvFrame(vec2(1,1),tex);
    pos = center+top+side;
    gl_Position = VP*vec4(pos,1);
    EmitVertex();

    vertex.uv = uvFrame(vec2(0,0),tex);
    pos = center-side;
    gl_Position = VP*vec4(pos,1);
    EmitVertex();

    vertex.uv = uvFrame(vec2(1,0),tex);
    pos = center+side;
    gl_Position = VP*vec4(pos,1);
    EmitVertex();
    EndPrimitive();
}
