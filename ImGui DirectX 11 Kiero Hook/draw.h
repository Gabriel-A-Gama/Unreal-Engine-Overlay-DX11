#pragma once
#include <Windows.h>
#include <string>
#include <sstream>
#include "imgui/imgui.h"
#include "includes.h"
class draw
{
public:
	static void DrawLine(const ImVec2& aPoint1, const ImVec2 aPoint2, ImU32 aColor, const FLOAT aLineWidth);
	static void DrawBox(float x, float y, float w, float h, ImColor color);
	static void DrawTextImgui(float x, float y, float w, float h, ImColor color);
	static void DrawCircle(const ImVec2& aPoint, const FLOAT aR, ImU32 aColor, const FLOAT aLineWidth);
	static void DrawCircle(float x, float y, float radius, ImVec4 color, int segments);
	static void DrawCorneredBox(float X, float Y, float W, float H, const ImU32& color, float thickness);
	static void DrawOutlinedText(const std::string& text, const ImVec2& pos, float size, ImU32 color, bool center);
	static void RectFilled(float x0, float y0, float x1, float y1, ImColor color, float rounding, int rounding_corners_flags);
	static void nome(ImVec2 pos, const char* team, const char* name, ImColor TeamCol, float health, float shield, ImColor shieldCol);
	static void FilledRect(const ImVec2& up, const ImVec2& down, ImU32 aColor);
	static void HealthBar(float x, float y, float w, float h, int phealth, bool Outlined);
	static void ShieldBar(float x, float y, float w, float h, int phealth, bool Outlined);
	static void DrawFilledRect(const ImVec2& up, const ImVec2& down, ImU32 aColor);
	static void RadarHUD(int xAxis, int yAxis, int width, int height);
	static void DrawFillCircle(float x, float y, float radius, ImVec4 color, int segments);
	static void DrawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, float thickness);
};

