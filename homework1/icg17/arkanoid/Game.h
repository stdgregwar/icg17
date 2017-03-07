#pragma once

#include <array>
#include "quad/quad.h"

#define BOARDW 16 //4/3 board
#define BOARDH 12
#define ROWS 5

struct Block{
    bool destroyed = true;
    static Quad sprite;
    glm::mat4 transform;
};

struct Ball{
    Quad sprite;
    glm::vec2 pos;
    glm::vec2 speed;
};

struct Pal{
    Quad sprite;
    glm::vec2 pos;
    glm::vec2 speed;
    float width;
    float height;
};

struct iCoord{
    int i,j;
    iCoord up() {
        return {i,(j-1+BOARDH)%BOARDH};
    }
    iCoord down() {
        return {i,(j+1)%BOARDH};
    }
    iCoord left() {
        return {(i-1+BOARDW)%BOARDW,j};
    }
    iCoord right() {
        return {(i+1)%BOARDW,j};
    }
};

typedef std::array<Block,BOARDH> Column;
typedef std::array<Column,BOARDW> Board;

enum STATE {
    START,
    PLAYING
};

/**
 * @brief The Game class
 *
 * This class is implemented correctly in descent C++ fashion,
 * this is exactly what you, makers of this course and project, should
 * promote when releasing handouts and template code. Doing everything in headers
 * is just ultra-ugly and implies problems when including files. G.H.
 */
class Game
{
public:
    Game(int width,int height);
    void tick(float time_s);
    void left();
    void right();
    void space();
    void draw();
    iCoord coordFor(const glm::vec2& pos);
    Block& blockAt(const iCoord& coord);
private:
    float mLastTime;
    int mWidth;
    int mHeight;
    Ball mBall;
    Pal mPal;
    glm::mat4 mView;
    Board mBoard;
    STATE mState;
};
