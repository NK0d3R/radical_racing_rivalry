// Copyright 2018 Catalin G. Manciu

#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include "defs.h"

class Level;
class SpriteRenderer;

class GameObject {
 protected:
    // World coordinates
    FP32        xPos;
    FP32        zPos;
    // Screen coordinates
    int16_t     screenX;
    int16_t     screenY;
    uint8_t     screenW;

    Level*      parent;
 public:
    GameObject(Level* p, FP32 initialX, FP32 initialY, uint8_t scrW):
        parent(p), xPos(initialX), zPos(initialY), screenW(scrW) {}
    void updateScreenX();
    void updateScreenY();
    bool isVisible();
    FP32& getX()            { return xPos; }
    FP32& getZ()            { return zPos; }
    virtual void update(int16_t dt) = 0;
    virtual void draw(SpriteRenderer* renderer) = 0;
};

#endif  // GAMEOBJECT_H_
