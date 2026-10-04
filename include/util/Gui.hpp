#pragma once

#include <raylib.h>
#include "Raylib.hpp"
#include "../geo/Vec2.hpp"

using namespace geo;

namespace util {

bool isGuiBoxPressedLeft(Vec2 origin, Vec2 size) {
    return IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && isAABB2D(aabb2D(origin.raylib(), size.raylib()), GetMousePosition());
}

bool isGuiBoxPressedRight(Vec2 origin, Vec2 size) {
    return IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && isAABB2D(aabb2D(origin.raylib(), size.raylib()), GetMousePosition());
}

bool isGuiBoxReleasedLeft(Vec2 origin, Vec2 size) {
    return IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && isAABB2D(aabb2D(origin.raylib(), size.raylib()), GetMousePosition());
}

bool isGuiBoxReleasedRight(Vec2 origin, Vec2 size) {
    return IsMouseButtonReleased(MOUSE_RIGHT_BUTTON) && isAABB2D(aabb2D(origin.raylib(), size.raylib()), GetMousePosition());
}

bool isGuiBoxHover(Vec2 origin, Vec2 size) {
    return !IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && isAABB2D(aabb2D(origin.raylib(), size.raylib()), GetMousePosition());
}

bool isGuiBoxDownLeft(Vec2 origin, Vec2 size) {
    return IsMouseButtonDown(MOUSE_LEFT_BUTTON) && isAABB2D(aabb2D(origin.raylib(), size.raylib()), GetMousePosition());
}

bool isGuiBoxDownRight(Vec2 origin, Vec2 size) {
    return IsMouseButtonDown(MOUSE_RIGHT_BUTTON) && isAABB2D(aabb2D(origin.raylib(), size.raylib()), GetMousePosition());
}

bool isGuiBoxUpLeft(Vec2 origin, Vec2 size) {
    return IsMouseButtonUp(MOUSE_LEFT_BUTTON) && isAABB2D(aabb2D(origin.raylib(), size.raylib()), GetMousePosition());
}

bool isGuiBoxUpRight(Vec2 origin, Vec2 size) {
    return IsMouseButtonUp(MOUSE_RIGHT_BUTTON) && isAABB2D(aabb2D(origin.raylib(), size.raylib()), GetMousePosition());
}

}
