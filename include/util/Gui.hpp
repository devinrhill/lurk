// Devin Hill 2026

#pragma once

#include <raylib.h>
#include "../geo/AABB2D.hpp"
#include "../geo/Vec2.hpp"

using namespace lvk::geo;

namespace lvk::util {

bool isGuiBoxPressedLeft(Vec2 origin, Vec2 size) {
    return IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && AABB2D::fromCenterHalfSize(origin, size).contains(GetMousePosition());
}

bool isGuiBoxPressedRight(Vec2 origin, Vec2 size) {
	return IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && AABB2D::fromCenterHalfSize(origin, size).contains(GetMousePosition());
}

bool isGuiBoxReleasedLeft(Vec2 origin, Vec2 size) {
	return IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && AABB2D::fromCenterHalfSize(origin, size).contains(GetMousePosition());
}

bool isGuiBoxReleasedRight(Vec2 origin, Vec2 size) {
	return IsMouseButtonReleased(MOUSE_RIGHT_BUTTON) && AABB2D::fromCenterHalfSize(origin, size).contains(GetMousePosition());
}

bool isGuiBoxHover(Vec2 origin, Vec2 size) {
    return !IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && AABB2D::fromCenterHalfSize(origin, size).contains(GetMousePosition());
}

bool isGuiBoxDownLeft(Vec2 origin, Vec2 size) {
	return IsMouseButtonDown(MOUSE_LEFT_BUTTON) && AABB2D::fromCenterHalfSize(origin, size).contains(GetMousePosition());
}

bool isGuiBoxDownRight(Vec2 origin, Vec2 size) {
	return IsMouseButtonDown(MOUSE_RIGHT_BUTTON) && AABB2D::fromCenterHalfSize(origin, size).contains(GetMousePosition());
}

bool isGuiBoxUpLeft(Vec2 origin, Vec2 size) {
	return IsMouseButtonUp(MOUSE_LEFT_BUTTON) && AABB2D::fromCenterHalfSize(origin, size).contains(GetMousePosition());
}

bool isGuiBoxUpRight(Vec2 origin, Vec2 size) {
	return IsMouseButtonUp(MOUSE_RIGHT_BUTTON) && AABB2D::fromCenterHalfSize(origin, size).contains(GetMousePosition());
}

} // namespace lvk::util
