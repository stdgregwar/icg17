#version 330

#define gtiles 3

uniform float time;
uniform mat4 VP;
uniform mat4 V;

layout(points) in;
layout(triangle_strip, max_vertices=4) out;

in vData {
    int tex;
    vec3 pos;
    vec3 color;
    vec3 view_dir;
    float size;
} vertices[];

out vData {
    vec2 uv;
    vec3 color;
    vec3 normal;
} vertex;

const vec3 up = vec3(0,0,1);

vec2 uvFrame(vec2 base, int i) {
    return vec2(float(i)/gtiles + base.x/gtiles,base.y);
}

void main() {
    float phi = sin(time*9+vertices[0].color.g*3)*0.2;

    vec3 view = vertices[0].view_dir;
    int tex = vertices[0].tex;

    vec3 rot = phi*view;

    vec3 top = normalize(up+cross(up,rot));
    vec3 side = normalize(cross(top,view));
    top = normalize(cross(view,side));
    top*=vertices[0].size;
    side*=vertices[0].size;
    vec3 center = vertices[0].pos;

    //Make quad
    vertex.normal = normalize((V*vec4(up,0)).xyz);
    vertex.color = vertices[0].color;

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
    pos = center-top-side;
    gl_Position = VP*vec4(pos,1);
    EmitVertex();

    vertex.uv = uvFrame(vec2(1,0),tex);
    pos = center-top+side;
    gl_Position = VP*vec4(pos,1);
    EmitVertex();
    EndPrimitive();
}
