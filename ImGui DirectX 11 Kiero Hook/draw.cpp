#include "draw.h"
#include "SDK.hpp"
using namespace SDK;
#include <map>
void draw::DrawLine(const ImVec2& aPoint1, const ImVec2 aPoint2, ImU32 aColor, const FLOAT aLineWidth)
{
	auto vList = ImGui::GetBackgroundDrawList();
	vList->AddLine(aPoint1, aPoint2, aColor, aLineWidth);
}


void draw::DrawBox(float x, float y, float w, float h, ImColor color)
{
	DrawLine(ImVec2(x, y), ImVec2(x + w, y), color, 1.3f); // top
	DrawLine(ImVec2(x, y - 1.3f), ImVec2(x, y + h + 1.4f), color, 1.3f); // left
	DrawLine(ImVec2(x + w, y - 1.3f), ImVec2(x + w, y + h + 1.4f), color, 1.3f);  // right
	DrawLine(ImVec2(x, y + h), ImVec2(x + w, y + h), color, 1.3f);   // bottom
}

void draw::DrawCircle(const ImVec2& aPoint, const FLOAT aR, ImU32 aColor, const FLOAT aLineWidth)
{
	auto vList = ImGui::GetBackgroundDrawList();
	vList->AddCircle(aPoint, aR, aColor, 120, aLineWidth);
}
void draw::DrawCircle(float x, float y, float radius, ImVec4 color, int segments)
{
	auto vList = ImGui::GetBackgroundDrawList();
	vList->AddCircle(ImVec2(x, y), radius, ImGui::ColorConvertFloat4ToU32(color), segments, 2.5f);
}

void draw::DrawCorneredBox(float X, float Y, float W, float H, const ImU32& color, float thickness)
{
	auto vList = ImGui::GetBackgroundDrawList();

	float lineW = (W / 3);
	float lineH = (H / 3);
	//black outlines
	auto col = ImGui::GetColorU32(color);

	//corners
	vList->AddLine(ImVec2(X, Y - thickness / 2), ImVec2(X, Y + lineH), col, thickness);//top left
	vList->AddLine(ImVec2(X - thickness / 2, Y), ImVec2(X + lineW, Y), col, thickness);

	vList->AddLine(ImVec2(X + W - lineW, Y), ImVec2(X + W + thickness / 2, Y), col, thickness);//top right horizontal
	vList->AddLine(ImVec2(X + W, Y - thickness / 2), ImVec2(X + W, Y + lineH), col, thickness);

	vList->AddLine(ImVec2(X, Y + H - lineH), ImVec2(X, Y + H + (thickness / 2)), col, thickness);//bot left
	vList->AddLine(ImVec2(X - thickness / 2, Y + H), ImVec2(X + lineW, Y + H), col, thickness);

	vList->AddLine(ImVec2(X + W - lineW, Y + H), ImVec2(X + W + thickness / 2, Y + H), col, thickness);//bot right
	vList->AddLine(ImVec2(X + W, Y + H - lineH), ImVec2(X + W, Y + H + (thickness / 2)), col, thickness);
}


void draw::DrawOutlinedText(const std::string& text, const ImVec2& pos, float size, ImU32 color, bool center)
{
	auto vList = ImGui::GetBackgroundDrawList();
	//ImGui::PushFont(pFont);
	std::stringstream stream(text);
	std::string line;

	float y = 0.0f;
	int index = 0;

	while (std::getline(stream, line))
	{
		ImVec2 textSize = gc.Ruda->CalcTextSizeA(size, FLT_MAX, 0.0f, line.c_str());

		if (center)
		{
			vList->AddText(gc.Ruda, size, ImVec2((pos.x - textSize.x / 2.0f) - 1, (pos.y + textSize.y * index)), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
			vList->AddText(gc.Ruda, size, ImVec2((pos.x - textSize.x / 2.0f), (pos.y + textSize.y * index) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
			vList->AddText(gc.Ruda, size, ImVec2((pos.x - textSize.x / 2.0f) + 1, (pos.y + textSize.y * index)), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
			vList->AddText(gc.Ruda, size, ImVec2((pos.x - textSize.x / 2.0f), (pos.y + textSize.y * index) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
			vList->AddText(gc.Ruda, size, ImVec2(pos.x - textSize.x / 2.0f, pos.y + textSize.y * index), ImGui::GetColorU32(color), line.c_str());
		}
		else
		{//
			vList->AddText(gc.Ruda, size, ImVec2((pos.x) + 1, (pos.y + textSize.y * index) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
			vList->AddText(gc.Ruda, size, ImVec2((pos.x) - 1, (pos.y + textSize.y * index) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
			vList->AddText(gc.Ruda, size, ImVec2(pos.x, pos.y + textSize.y * index), ImGui::GetColorU32(color), line.c_str());
		}

		y = pos.y + textSize.y * (index + 1);
		index++;
	}
	//ImGui::PopFont();
}
void draw::FilledRect(const ImVec2& up, const ImVec2& down, ImU32 aColor)
{
	auto vList = ImGui::GetBackgroundDrawList();
	vList->AddRectFilled(up, down, aColor, 3, ImDrawCornerFlags_All);
}
auto draw::RectFilled(float x0, float y0, float x1, float y1, ImColor color, float rounding, int rounding_corners_flags) -> VOID
{
	auto vList = ImGui::GetBackgroundDrawList();
	vList->AddRectFilled(ImVec2(x0, y0), ImVec2(x1, y1), color, rounding, rounding_corners_flags);
}
void draw::nome(ImVec2 pos, const char* team, const char* name, ImColor TeamCol, float health, float shield, ImColor shieldCol)
{
	//lenght 250
	//height 50
	int healthValue = max(0, min(health, 100));

	ImColor barColor = ImColor
	(
		min(510 * (100 - healthValue) / 100, 255), min(510 * healthValue / 100, 255),
		25,
		255
	);

	if (pos.x < 0 || pos.y < 0 || !team || !name) return;

	ImVec2 reSize = ImGui::CalcTextSize(name);

	float hei = 57;
	float llen;
	float rlen;
	float heit;
	llen = pos.x + (reSize.x / 2);
	rlen = pos.x - (reSize.x / 2);
	heit = pos.y - hei;


	//FilledRect(ImVec2{ llen - reSize.x + 20	,heit + 3 },	ImVec2{ rlen + reSize.x - 5 ,heit - 15 }, ImColor(0.21f, 0.21f, 0.21f, 0.8f));
	//FilledRect(ImVec2{ llen - 5				,heit + 3 },	ImVec2{ rlen + reSize.x - 20,heit - 15 }, TeamCol);
	//FilledRect(ImVec2{ llen - 12				,heit + 3 },	ImVec2{ rlen + reSize.x - 15,heit - 15 }, ImColor{ 0.14f, 0.14f, 0.14f,1.f });
	FilledRect(ImVec2{ llen - reSize.x + 5 ,	heit - 15 }, ImVec2{ rlen + reSize.x + 20	, heit + 3 }, ImColor(0.21f, 0.21f, 0.21f, 0.8f));
	FilledRect(ImVec2{ llen - reSize.x - 20,	heit - 15 }, ImVec2{ rlen - 5				, heit + 3 }, TeamCol);
	FilledRect(ImVec2{ llen - reSize.x - 15,	heit - 15 }, ImVec2{ rlen + 12				, heit + 3 }, ImColor{ 0.14f, 0.14f, 0.14f,1.f });
	DrawOutlinedText(team, ImVec2{ rlen - 2,heit - 16 }, 20, TeamCol, true);
	DrawOutlinedText(name, ImVec2{ rlen + (reSize.x / 2) + 15,heit - 13 }, 14, ImColor{ 255,255,255 }, true);

	if (true)
	{
		auto total = (llen - reSize.x - 20) - (rlen + reSize.x + 20);
		FilledRect(ImVec2{ llen - reSize.x - 20		,heit + 3 }, ImVec2{ (rlen + reSize.x + 20)									,heit + 12 }, ImColor(0.f, 0.f, 0.f, 1.f));
		FilledRect(ImVec2{ (llen - reSize.x - 20) + 2	,heit + 4 }, ImVec2{ ((llen - reSize.x - 20) + ((health / 100) * -total) - 2)	,heit + 7 }, barColor);
		FilledRect(ImVec2{ (llen - reSize.x - 20) + 2	,heit + 8 }, ImVec2{ ((llen - reSize.x - 20) + ((shield / 100) * -total) - 2)	,heit + 11 }, shieldCol);
	}

#ifdef _Original
	FilledRect(ImVec2{ rlen - reSize.x + 5 ,heit - 15 }, ImVec2{ llen + reSize.x + 20 ,heit + 3 }, ImColor(0.21f, 0.21f, 0.21f, 0.8f));
	FilledRect(ImVec2{ rlen - reSize.x - 20, heit - 15 }, ImVec2{ llen - 5, heit + 3 }, TeamCol);
	FilledRect(ImVec2{ rlen - reSize.x - 15,heit - 15 }, ImVec2{ llen + 12 ,heit + 3 }, ImColor{ 0.14f, 0.14f, 0.14f,1.f });
	DrawOutlinedText(gc.Ruda, team, ImVec2{ llen - 2,heit - 16 }, 20, TeamCol, true);
	DrawOutlinedText(gc.Ruda, name, ImVec2{ llen + (reSize.x / 2) + 15,heit - 13 }, 14, ImColor{ 255,255,255 }, true);

	if (!t.cfg(_b::EspSeer))
	{
		auto total = (rlen - reSize.x - 20) - (llen + reSize.x + 20);
		FilledRect(ImVec2{ rlen - reSize.x - 20 ,heit + 3 }, ImVec2{ (llen + reSize.x + 20),heit + 12 }, ImColor(0.f, 0.f, 0.f, 1.f));
		FilledRect(ImVec2{ (rlen - reSize.x - 20) + 2 ,heit + 4 }, ImVec2{ ((rlen - reSize.x - 20) + ((health / 100) * -total) - 2),heit + 7 }, barColor);
		FilledRect(ImVec2{ (rlen - reSize.x - 20) + 2 ,heit + 8 }, ImVec2{ ((rlen - reSize.x - 20) + ((shield / 100) * -total) - 2),heit + 11 }, shieldCol);
	}
#endif
	//#define _NEW_NAMEBAR
#ifdef _NEW_NAMEBAR
	FilledRect({ 80,50 }, { 250,35 }, ImColor(0.21f, 0.21f, 0.21f));
	FilledRect({ 50,50 }, { 80,35 }, ImColor(0.27f, 0.45f, 0.46f));
	DrawOutlinedText(gc.Ruda, "1", { 50 + 16,33 }, 20, ImColor{ 50,255,50 }, true);
	DrawOutlinedText(gc.Ruda, "[ACKB]OfficialLee#11777337", { 50 + 115,35 }, 14, ImColor{ 50,255,50 }, true);
#endif
}

auto draw::HealthBar(float x, float y, float w, float h, int phealth, bool Outlined) -> VOID
{
	auto vList = ImGui::GetBackgroundDrawList();
	if (x <= 1 || y <= 1 || w <= 1 || h <= 1 || phealth <= 0) return;


	int healthValue = max(0, min(phealth, 100));

	ImColor barColor = ImColor
	(
		min(510 * (100 - healthValue) / 100, 255), min(510 * healthValue / 100, 255),
		25,
		255
	);
	if (Outlined)
		vList->AddRect(ImVec2(x - 1, y - 1), ImVec2(x + w + 1, y + h + 1), ImColor(0.f, 0.f, 0.f), 0.0f, 0, 1.0f);

	RectFilled(x, y + h - (int)(((float)h / 100.0f) * (float)phealth), x + w, y + h, barColor, 0.0f, 0);
}
auto draw::ShieldBar(float x, float y, float w, float h, int phealth, bool Outlined) -> VOID
{
	auto vList = ImGui::GetBackgroundDrawList();

	int healthValue = max(0, min(phealth, 100));

	ImColor barColor = ImColor
	(
		min(510 * (100 - healthValue) / 100, 255),
		25, min(510 * healthValue / 100, 255),
		255
	);
	if (Outlined)
		vList->AddRect(ImVec2(x - 1, y - 1), ImVec2(x + w + 1, y + h + 1), ImColor(0.f, 0.f, 0.f), 0.0f, 0, 1.0f);

	RectFilled(x, y, x + w, y + (int)(((float)h / 100.0f) * (float)phealth), barColor, 0.0f, 0);
}

auto draw::DrawFilledRect(const ImVec2& up, const ImVec2& down, ImU32 aColor) -> VOID
{
	auto vList = ImGui::GetBackgroundDrawList();
	vList->AddRectFilled(up, down, aColor, 0.f, ImDrawCornerFlags_None);
}

//auto draw::RadarHUD(int xAxis, int yAxis, int width, int height) -> VOID
//{
//	FVector siz;
//	siz.X = width;
//	siz.Y = height;
//	FVector pos;
//	pos.X = xAxis;
//	pos.Y = yAxis;
//	float RadarCenterX = pos.X + (siz.X / 2);
//	float RadarCenterY = pos.Y + (siz.Y / 2);
//	//DrawBox(pos.X, pos.Y, siz.X, siz.Y,ImColor{255,255,255});
//	//DrawBox(pos.X, pos.Y, siz.X, siz.Y, ImColor{ 255,255,255 });
//	DrawFilledRect({ pos.X, pos.Y }, { siz.X + xAxis + 1, siz.Y + yAxis + 1 }, ImColor{ 0.f,0.f,0.f,0.3f });
//	//DrawCircle(pos.X, pos.Y, 5, ImColor{ 255,255,0 }, 3);
//	//DrawCircle(siz.X, siz.Y, 5, ImColor{ 255,255,0 }, 3);
//	DrawBox(pos.X, pos.Y, siz.X, siz.Y, ImColor{ 0,0,0 });
//	DrawLine({ RadarCenterX, RadarCenterY, }, { pos.X, pos.Y }, ImColor{ .3,.3,.3, 1.f }, 1.f);
//	DrawLine({ RadarCenterX, RadarCenterY }, { pos.X + siz.X , pos.Y }, ImColor{ .3, .3, .3, 1.f }, 1.f);
//	DrawLine({ pos.X, RadarCenterY }, { pos.X + siz.X , RadarCenterY }, ImColor{ .3, .3, .3, 1.f }, 1.f);
//	DrawLine({ RadarCenterX, RadarCenterY }, { RadarCenterX, pos.Y + siz.Y }, ImColor{ .3, .3, .3, 1.f }, 1.f);
//	DrawCircle(RadarCenterX, RadarCenterY, 1, ImColor{ 255,255,255 }, 0);
//}

auto draw::DrawFillCircle(float x, float y, float radius, ImVec4 color, int segments) -> VOID
{
	auto vList = ImGui::GetBackgroundDrawList();
	vList->AddCircle(ImVec2(x, y), radius, ImGui::ColorConvertFloat4ToU32(color), segments, 1.f);
}

void draw::DrawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, float thickness) {
	DrawLine({ x1, y1 }, { x2, y2 }, ImColor{ 255,0,0 }, thickness);
	DrawLine({ x2, y2 }, { x3, y3 }, ImColor{ 255,0,0 }, thickness);
	DrawLine({ x3, y3 }, { x1, y1 }, ImColor{ 255,0,0 }, thickness);
}