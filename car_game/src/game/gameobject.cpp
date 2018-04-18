// Copyright 2018 Catalin G. Manciu

#include "gameobject.h"
#include "level.h"

bool GameObject::isVisible() {
    int32_t minX = screenX - (screenW >> 1);
    int32_t maxX = minX + screenW;
    return !(maxX < 0 || minX >= SCREEN_W);
}

void GameObject::updateScreenX() {
    screenX = parent->worldToScreenX(xPos, zPos);
}

void GameObject::updateScreenY() {
    screenY = parent->worldToScreenY(xPos, zPos);
}
