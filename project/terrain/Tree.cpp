#include "Tree.h"

#include <glm/gtc/type_ptr.hpp>

using namespace glm;

Tree::Tree() : mRand(0,1)
{

}

std::tuple<vec3,vec3,vec3,vec3> bezierBase(const Bezier<vec3>& b, float p, float p2, const vec3& hint) {
    vec3 center = b.curveAtTime(p);
    vec3 next = b.curveAtTime(p2);
    vec3 forw = normalize(next-center);
    vec3 side = cross(forw,vec3(0,1,0));
    vec3 bside = cross(side,forw);
    return tie(center,forw,side,bside);
}


void Tree::addTrunc(const Bezier<vec3> &b, float count, float res, float width) {

    auto& vertices = mTruncVerts;
    auto& indices = mTruncIndices;
    int begin = vertices.size();
    //Bezier<vec3> b({{b,b+normal,b+normal+vec3(0,0,1)*length(normal)}});
    for(float i = 0; i < count; i++) {
        float t = i*0.9999 / count;
        float twidth = width*(1-t);
        vec3 center, forw, side, bside;
        tie(center,forw,side,bside) = bezierBase(b,t,t+1/count,vec3(1,0,0));
        for(float j = 0; j <= res; j++) {
            float ang = M_PI*2*j/res;
            vec3 norm = cos(ang)*side + sin(ang)*bside;
            vec3 point = center + norm*twidth;
            vertices.push_back({
                                   point,
                                   norm,
                                   vec3(0,0,t),
                                   vec2(j/res,t)
                               });

            //Add indices
            if(i != count -1) {
                int offset = begin+ i * (res+1) + j;
                indices.push_back(offset + res + 1);
                indices.push_back(offset + 1);
                indices.push_back(offset + 0);
                indices.push_back(offset + res + 1 +1  );
                indices.push_back(offset + 1);
                indices.push_back(offset + res +1);
            }
        }
    }
}

void Tree::addLeaves(const Bezier<glm::vec3>& b, float count) {

}

void Tree::build(const glm::vec3& pos, const glm::vec3& normal, float width) {
    mMaterial.init("v_tree.glsl","f_tree.glsl");

    mEng.seed(pos.x);

    std::vector<VertexData>& vertices = mTruncVerts;
    std::vector<GLuint>& indices = mTruncIndices;

    float count = 20;
    float res = 12;
    Bezier<vec3> mainTrunc({{pos,pos+normal,pos+normal+vec3(0,0,1)*length(normal)}});
    addTrunc(mainTrunc,count,res,width);
    for(float i = 0.4; i < 1; i+=1/count) {
        vec3 center, forw, side, bside;
        float twidth = width*(1-i);
        float lenght = normal.length()*4*(1-i);
        tie(center,forw,side,bside) = bezierBase(mainTrunc,i,i+1/count,vec3(1,0,0));
        float ang = 2*M_PI*mRand(mEng);
        vec3 dir = cos(ang) * side + sin(ang)* bside - forw;
        dir*=lenght;
        Bezier<vec3> branch({{center,center+dir,center+dir+vec3(1,0,0)}});
        addTrunc(branch,4,4,twidth*0.2);
    }

    glGenVertexArrays(1, &mVertexArrayId);
    glBindVertexArray(mVertexArrayId);

    // position buffer
    glGenBuffers(1, &mVertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexData),
                 vertices.data(), GL_STATIC_DRAW);

    // vertex indices
    glGenBuffers(1, &mVertexBufferObjectIndex);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVertexBufferObjectIndex);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
                 indices.data(), GL_STATIC_DRAW);

    mTruncSize = indices.size();

    // position shader attribute
    GLuint locPosition = mMaterial.attrLocation("position");
    if(locPosition == -1) {
        throw std::runtime_error("failed to bind attrib position");
    }
    glEnableVertexAttribArray(locPosition);
    glVertexAttribPointer(locPosition, 3, GL_FLOAT, DONT_NORMALIZE,
                          sizeof(VertexData), (void*)offsetof(VertexData,pos));

    GLuint locNormal = mMaterial.attrLocation("normal");
    if(locNormal == -1) {
        throw std::runtime_error("failed to bind attrib normal");
    }
    glEnableVertexAttribArray(locNormal);
    glVertexAttribPointer(locNormal, 3, GL_FLOAT, DONT_NORMALIZE,
                           sizeof(VertexData), (void*)offsetof(VertexData,normal));

    GLuint locColor = mMaterial.attrLocation("color");
    if(locColor == -1) {
        throw std::runtime_error("failed to bind attrib color");
    }
    glEnableVertexAttribArray(locColor);
    glVertexAttribPointer(locColor, 3, GL_FLOAT, DONT_NORMALIZE,
                           sizeof(VertexData), (void*)offsetof(VertexData,col));

    GLuint locUV = mMaterial.attrLocation("uv");
    if(locUV == -1) {
        throw std::runtime_error("failed to bind attrib uv");
    }
    glEnableVertexAttribArray(locUV);
    glVertexAttribPointer(locUV, 2, GL_FLOAT, DONT_NORMALIZE,
                           sizeof(VertexData), (void*)offsetof(VertexData,uv));

}

void Tree::draw(const glm::mat4& view, const glm::mat4& proj) {
    glBindVertexArray(mVertexArrayId);
    mMaterial.bind();
    glUniformMatrix4fv(mMaterial.uniformLocation("VP"), ONE, DONT_TRANSPOSE,glm::value_ptr(proj*view));
    glDrawElements(GL_TRIANGLES, mTruncSize, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    mMaterial.unbind();
}