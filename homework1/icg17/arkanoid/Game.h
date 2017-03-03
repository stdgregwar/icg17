#pragma once

#include <array>
#include "quad/quad.h"

#define BOARDW 16 //4/3 board
#define BOARDH 12
#define ROWS 7
#define BLOCKW 32 //Pixels
#define BLOCKH 16

struct Block{
    bool destroyed = false;
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
 * is just ultra-ugly and implies problems when including files.
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
