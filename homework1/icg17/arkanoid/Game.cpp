#include "Game.h"
#include <iostream>

using namespace glm;


Quad Block::sprite = Quad();

Game::Game(int width, int height) : mWidth(width),mHeight(height), mState(START)
{
    mPal.width = 120;
    mPal.height = 29;
    mPal.pos = {width/2,height-32};
    Block::sprite.Init("block.png"); //init shared sprite
    mView = mat4();
    mView = translate(mView,vec3(-1,1,0));
    mView = scale(mView,vec3(2.f/(mWidth),-2.f/(mHeight),1));

    for(int x = 1; x < BOARDW-1; x++) {
        for(int y = 1; y < BOARDH-1; y++) {
            Block &b = mBoard[x][y];
            if(y < ROWS) {
                b.destroyed = false;
            }
        }
    }
    mBall.sprite.Init("ball.png");
    mPal.sprite.Init("pal.png");
}

void Game::draw() {
    mat4 palModel = translate(mat4(),vec3(mPal.pos,.0f));
    palModel = scale(palModel,vec3(mPal.width,20.f,1));
    mPal.sprite.Draw(palModel,mView);

    float blockWidth = mWidth/BOARDW;
    float blockHeight = mHeight/BOARDH;
    mat4 blockSize = scale(mat4(),vec3(blockWidth+10,blockHeight+10,1));
    for(int x = 0; x < BOARDW; x++) {
        for(int y = 0; y < BOARDH; y++) {
            Block &b = mBoard[x][y];
            if(!b.destroyed) {
                mat4 blockPos = translate(mat4(),vec3(x*blockWidth,y*blockHeight,0));
                b.sprite.Draw(blockPos*blockSize,mView);
            }
        }
    }

    mat4 ballModel = translate(mat4(),vec3(mBall.pos-vec2{13,13},0.f));
    ballModel = scale(ballModel,vec3(26,26,1));
    mBall.sprite.Draw(ballModel,mView);
}

void Game::tick(float time_s) {
    float dt = time_s-mLastTime;
    mLastTime = time_s;
    mPal.pos += dt*mPal.speed;
    mPal.pos.x = max(0.f,min(mWidth-mPal.width,mPal.pos.x));
    float bounce = -1.05f;
    switch(mState) {
    case PLAYING:
    {
        //integrate move
        mBall.pos += dt*mBall.speed;
        //Constraints
        //Pal
        if(mBall.pos.y >= mPal.pos.y) {
            if(mBall.pos.x > mPal.pos.x && mBall.pos.x < mPal.pos.x+mPal.width) {
                mBall.speed.y *= bounce;
                mBall.speed += 0.5f*mPal.speed; //Bounce on pal
            } else {
                mState = START;
            }
        }
        //Borders
        if(mBall.pos.x > mWidth || mBall.pos.x < 0) {
            mBall.speed.x *= bounce;
        }
        if(mBall.pos.y < 0) {
            mBall.speed.y *= bounce;
        }
        //Blocks
        iCoord c = coordFor(mBall.pos);
        Block& up = blockAt(c.up());
        Block& down = blockAt(c.down());
        Block& left = blockAt(c.left());
        Block& right = blockAt(c.right());

        if(!up.destroyed && mBall.speed.y<0) {
            up.destroyed = true;
            mBall.speed.y*= bounce;
        }
        if(!down.destroyed && mBall.speed.y>0) {
            down.destroyed = true;
            mBall.speed.y*= bounce;
        }
        if(!left.destroyed && mBall.speed.x<0) {
            left.destroyed = true;
            mBall.speed.x*= bounce;
        }
        if(!right.destroyed && mBall.speed.x>0) {
            right.destroyed = true;
            mBall.speed.x*= bounce;
        }
        break;
    }
    case START:
        mBall.pos = {mPal.pos.x+mPal.width/2,mPal.pos.y-16};
        break;
    }
}

iCoord Game::coordFor(const glm::vec2& pos) {
    float blockWidth = mWidth/BOARDW;
    float blockHeight = mHeight/BOARDH;
    return {pos.x / blockWidth,pos.y/blockHeight};
}

Block& Game::blockAt(const iCoord& coord) {
    return mBoard[coord.i][coord.j];
}

void Game::space() {
    if(mState == START) {
        mState = PLAYING;
        mBall.speed = {mPal.speed.x, -300};
    }
}

static const float palSpeed = 400;

void Game::left() {
    mPal.speed.x -= palSpeed;
}

void Game::right() {
    mPal.speed.x += palSpeed;
}
