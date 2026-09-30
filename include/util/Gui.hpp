#pragma once

#include <raylib.h>
#include "Raylib.hpp"

namespace util {

bool isGuiBoxPressedLeft(Vector2 origin, Vector2 size) {
    return IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && isAABB2D(aabb2D(origin, size), GetMousePosition());
}

bool isGuiBoxPressedRight(Vector2 origin, Vector2 size) {
    return IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && isAABB2D(aabb2D(origin, size), GetMousePosition());
}

bool isGuiBoxReleasedLeft(Vector2 origin, Vector2 size) {
    return IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && isAABB2D(aabb2D(origin, size), GetMousePosition());
}

bool isGuiBoxReleasedRight(Vector2 origin, Vector2 size) {
    return IsMouseButtonReleased(MOUSE_RIGHT_BUTTON) && isAABB2D(aabb2D(origin, size), GetMousePosition());
}

bool isGuiBoxHover(Vector2 origin, Vector2 size) {
    return !IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && isAABB2D(aabb2D(origin, size), GetMousePosition());
}

bool isGuiBoxDownLeft(Vector2 origin, Vector2 size) {
    return IsMouseButtonDown(MOUSE_LEFT_BUTTON) && isAABB2D(aabb2D(origin, size), GetMousePosition());
}

bool isGuiBoxDownRight(Vector2 origin, Vector2 size) {
    return IsMouseButtonDown(MOUSE_RIGHT_BUTTON) && isAABB2D(aabb2D(origin, size), GetMousePosition());
}

bool isGuiBoxUpLeft(Vector2 origin, Vector2 size) {
    return IsMouseButtonUp(MOUSE_LEFT_BUTTON) && isAABB2D(aabb2D(origin, size), GetMousePosition());
}

bool isGuiBoxUpRight(Vector2 origin, Vector2 size) {
    return IsMouseButtonUp(MOUSE_RIGHT_BUTTON) && isAABB2D(aabb2D(origin, size), GetMousePosition());
}

}
