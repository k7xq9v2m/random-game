#include <cmath>
#include "constant.hpp"

float highGravity = windowHeight * 600.f / 600;
float lowGravity = windowHeight * 300.f / 600;
float Jumpforce = windowHeight * 450.f / 600;
float friction = .3f;


float degToRad(float degree){
    return degree * (3.14 / 180.0);
}

void jump(Player& player){
    player.xvelocity = -cos(degToRad(player.angle)) * Jumpforce;
    player.yvelocity = -sin(degToRad(player.angle)) * Jumpforce;
}

void yVelocityUpdate(float& velocity, float delta){
    if(velocity < 0){
        velocity += highGravity * delta;
    }else velocity += lowGravity * delta;
}
void xVelocityUpdate(float& velocity, float delta){
    velocity *= (1 - friction * delta);
}

