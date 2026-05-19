#include "render.h"
#include "draw.h"
#include <fstream>
#include <vector>
#include <format>
//#include "imgui/imgui_internal.h"
bool hook = false;
bool esp = false;

bool skeleton = false;
bool name = false;
bool impostor = false;
int tipo_cor = 0;
bool espfov = false;
bool box = false;
bool corneredbox = false;
bool inf_estamina = false;
bool avent_esp = false;
bool bchange_colot = false;
bool gas_esp = false;
bool dropped_item_esp = false;
bool battery_esp = false;
bool alimbox_esp = false;
bool screwdriver = false;
bool filtervents = false;
bool delivery_esp = false;
bool scanner_esp = false;
bool container_esp = false;
bool hp = false;
bool line = false;
bool distance = false;
bool prediction = false;
bool aimbot = false;
bool espname = false;
bool silent_aimbot = false;
bool silent_aim_ativado = false;
int teclaaim = 0;
float fovaim = 26;
float smoothing = 1;
int rot[2];
SDK::FVector OriginalLocation(0, 0, 0);
SDK::FRotator OriginalRotation(0, 0, 0);
namespace utl
{
	struct box {
		SDK::FVector2D position;
		double width;
		double height;
	};
	/*inline std::tuple<float, float> health(SDK::AMec_C *pawn)
	{
		if (!pawn) return std::tuple<float, float>{};

		return std::tuple<float, float>{pawn->CurrentHealth, pawn->MaxHealth};
	}*/
	inline std::tuple<bool, box> get_box(SDK::APlayerController* local_player_controller, SDK::AMec_C* pawn)
	{
		box ret_box{};

		std::tuple<bool, box> return_tuple{ false, ret_box };

		if (!pawn) return return_tuple;

		auto rootcmp = pawn->RootComponent;
		if (!rootcmp) return return_tuple;

		auto location = rootcmp->RelativeLocation;

		SDK::FVector origin;
		SDK::FVector extent;
		pawn->GetActorBounds(true, &origin, &extent, 0);

		SDK::FVector2D headpos;
		if (!local_player_controller->ProjectWorldLocationToScreen({ origin.X,origin.Y,location.Z + extent.Z + 40.f }, &headpos, 0))
			return return_tuple;

		SDK::FVector2D footpos;
		if (!local_player_controller->ProjectWorldLocationToScreen({ location.X,location.Y,location.Z - extent.Z }, &footpos, 0))
			return return_tuple;

		ret_box.height = (footpos.Y - headpos.Y);
		ret_box.width = ret_box.height * 0.4f;
		ret_box.position = SDK::FVector2D{ headpos.X - ((float)ret_box.width / 2.f),headpos.Y };

		return std::tuple<bool, box>(true, ret_box);
	}
	bool initall()
	{
		ue->World = ue->GWorld;
		if (!ue->World) {
			return false;
		};

		ue->GameInstance = ue->World->OwningGameInstance;
		if (!ue->GameInstance) {
			return false;
		};

		ue->LocalPlayer = ue->GameInstance->LocalPlayers[0];
		if (!ue->LocalPlayer) {
			return false;
		};

		ue->LocalPlayerController = ue->LocalPlayer->PlayerController;
		if (!ue->LocalPlayerController) {
			return false;
		};

		ue->PlayerCameraManager = ue->LocalPlayerController->PlayerCameraManager;
		if (!ue->PlayerCameraManager) {
			return false;
		};

		ue->LocalPawn = (SDK::AMec_C*)ue->LocalPlayerController->AcknowledgedPawn;
		if (!ue->LocalPawn) {
			return false;
		};

		ue->RootComponent = ue->LocalPawn->RootComponent;
		if (!ue->RootComponent) {
			return false;
		};

		ue->GameState = ue->World->GameState;
		if (!ue->GameState) {
			return false;
		};



		/*	auto PrivateMy = ue->LocalPlayerController->K2_GetPawn();
			 ue->me = (SDK::ACharacter*)PrivateMy;
			if (!ue->me) { return false; };*/


		return true;
	}
}
bool render::bDisplay = true;


void interfaceimgui()
{
	//const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
	//ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 20, main_viewport->WorkPos.y + 42), ImGuiCond_FirstUseEver);
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4* colors = style.Colors;

	// Definindo o alinhamento do tC-tulo da janela
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);

	// Definindo um novo estilo de cores
	ImGui::StyleColorsDark();

	// Ajustando as cores para o design preto e azul bebC*
	colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.WindowRounding = 3.0f;
	style.FrameRounding = 2.0f;
	style.ScrollbarRounding = 4.0f;
	style.WindowBorderSize = 1.0f;

	ImVec4 aguaColor = ImVec4(0.0f, 238.0f / 255.0f, 238.0f / 255.0f, 1.0f);

	// Definindo as cores de fundo das janelas
	colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);

	// Definindo as cores de grip e barras de rolagem
	colors[ImGuiCol_ResizeGrip] = aguaColor;
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.25f, 0.25f, 0.70f, 0.60f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.25f, 0.25f, 0.70f, 0.90f);

	// Definindo as cores dos botC5es
	colors[ImGuiCol_Button] = aguaColor;
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.40f, 0.80f, 1.00f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.30f, 0.70f, 1.00f, 1.00f);

	// Definindo as cores do menu e das tabs
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.15f, 0.20f, 1.00f);
	colors[ImGuiCol_Tab] = ImVec4(0.10f, 0.15f, 0.20f, 0.90f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.20f, 0.25f, 0.30f, 0.80f);
	colors[ImGuiCol_TabActive] = ImVec4(0.25f, 0.40f, 0.80f, 0.80f);

	// Definindo as cores dos checkboxes
	colors[ImGuiCol_CheckMark] = ImVec4(0.25f, 0.40f, 0.80f, 0.80f);

	// Definindo as cores do fundo dos frames
	colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.25f, 0.30f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.40f, 0.80f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.25f, 0.40f, 0.80f, 0.67f);

	// Definindo as cores do tC-tulo da janela e das bordas
	colors[ImGuiCol_TitleBg] = aguaColor;
	colors[ImGuiCol_TitleBgActive] = aguaColor;
	colors[ImGuiCol_TitleBgCollapsed] = aguaColor;
	colors[ImGuiCol_Border] = aguaColor;
	colors[ImGuiCol_BorderShadow] = aguaColor;

}

// FunC'C#o para verificar se a posiC'C#o estC! dentro do FOV
bool IsWithinFOV(const SDK::FVector2D& screenPosition, float windowWidth, float windowHeight, float fFOV)
{
	return (screenPosition.X >= (windowWidth / 2) - fFOV &&
		screenPosition.X <= (windowWidth / 2) + fFOV &&
		screenPosition.Y >= (windowHeight / 2) - fFOV &&
		screenPosition.Y <= (windowHeight / 2) + fFOV);
}

static const char* KeyNames[] =
{
	"NENHUMA",
	"Mouse Left",
	"Mouse Right",
	"Cancel",
	"Middle Center",
	"MouseSide1",
	"MouseSide2",
	"",
	"Backspace",
	"Tab",
	"",
	"",
	"Clear",
	"Enter",
	"",
	"",
	"Shift",
	"Ctrl",
	"Alt",
	"Pause",
	"CapsLock",
	"",
	"",
	"",
	"",
	"",
	"",
	"Escape",
	"",
	"",
	"",
	"",
	"Space",
	"Page Up",
	"Page Down",
	"End",
	"Home",
	"Left",
	"Up",
	"Right",
	"Down",
	"",
	"",
	"",
	"Print",
	"Insert",
	"Delete",
	"",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"",
	"",
	"",
	"",
	"",
	"Numpad 0",
	"Numpad 1",
	"Numpad 2",
	"Numpad 3",
	"Numpad 4",
	"Numpad 5",
	"Numpad 6",
	"Numpad 7",
	"Numpad 8",
	"Numpad 9",
	"Multiply",
	"Add",
	"",
	"Subtract",
	"Decimal",
	"Divide",
	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"F11",
	"F12",
};

static bool KeyBinding;
bool IsKeyDown(int VK)
{
	return (GetAsyncKeyState(VK) & 0x8000) != 0;
}
void KeyBinder(int& target_key)
{

	bool flag = false;

	// KeyBinder
	while (true)
	{
		for (int i = 0; i < 0x87; i++)
		{
			if (i == VK_LWIN || i == VK_RWIN)
				continue;

			if (IsKeyDown(i))
			{
				if (i == VK_ESCAPE)
				{
					target_key = NULL;
					flag = true;
				}
				else
				{
					target_key = i;
					flag = true;
				}

				break;
			}
		}

		if (flag)
			break;
	}
	KeyBinding = false;
}


enum class Tab {
	PLAYER,
	VISUAL_ESP,
	AIMBOT
};
int hook_par = 0;
ImColor cRainbow;
float HSV_RAINBOW_SPEED = 0.0010;
static float HSV_RAINBOW_HUE = 0;
void SV_RAINBOW(float saturation, float value, float opacity)
{
	HSV_RAINBOW_HUE -= HSV_RAINBOW_SPEED;
	if (HSV_RAINBOW_HUE < -1.f) HSV_RAINBOW_HUE += 1.f;
	for (int i = 0; i < 860; i++)
	{
		float hue = HSV_RAINBOW_HUE + (1.f / (float)860) * i;
		if (hue < 0.f) hue += 1.f;
		cRainbow = ImColor::HSV(hue, (saturation / 255), (value / 255), 1);
	}
}
static ImVec4 randomColor = ImColor(255, 255, 255);

ImColor randomRainbowColor;
float randomSpeed = 0.0010;
static float randomHue = 0;
void menustyle() {

	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4* colors = ImGui::GetStyle().Colors;


	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);

	ImGui::StyleColorsClassic();


	ImGui::PushStyleColor(ImGuiCol_BorderShadow, ImVec4(cRainbow));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(cRainbow));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(cRainbow));
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(cRainbow));

	//	colors[ImGuiCol_Text] = ImVec4(2.0f, 2.28f, 2.45f, 1.00f);
	colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // Branco

	style.WindowRounding = 5.4f;
	style.FrameRounding = 2.4f;
	style.ScrollbarRounding = 2.0f;
	style.WindowBorderSize = 2.0f;

	colors[ImGuiCol_WindowBg] = ImColor(2, 0, 0, 255);

	colors[ImGuiCol_ResizeGrip] = ImColor(0, 0, 0, 0);
	colors[ImGuiCol_ResizeGripActive] = ImColor(0, 0, 0, 0);
	colors[ImGuiCol_ResizeGripHovered] = ImColor(0, 0, 0, 0);


	// colors[ImGuiCol_Button] = ImVec4(1.00f, 0.19f, 0.19f, 0.40f);
	colors[ImGuiCol_Button] = ImColor(99, 12, 12, 255);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_Tab] = ImVec4(1.00f, 0.19f, 0.19f, 0.40f);

	colors[ImGuiCol_TabHovered] = ImColor(84, 96, 227, 90);
	colors[ImGuiCol_TabActive] = ImColor(55, 90, 250, 98);

	// colors[ImGuiCol_ButtonHovered] = ImVec4(0.86f, 0.23f, 0.43f, 0.67f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.86f, 0.23f, 0.43f, 0.87f);
	colors[ImGuiCol_CheckMark] = ImColor(33, 250, 20, 98);
	//colors[ImGuiCol_CheckMark] = ImVec4(0.25f, 0.40f, 0.80f, 0.80f);
	//colors[ImGuiCol_FrameBg] = ImColor(24, 24, 24);
	//colors[ImGuiCol_FrameBg] = ImColor(33, 250, 20, 98);

	/*colors[ImGuiCol_FrameBgActive] = ImColor(26, 26, 26);
	colors[ImGuiCol_FrameBgHovered] = ImColor(149, 228, 253, 98);*/
	colors[ImGuiCol_FrameBgActive] = ImColor(33, 250, 20, 98);
	colors[ImGuiCol_FrameBgHovered] = ImColor(149, 228, 253, 98);
}

//void MyCheckBox2(const char* str_id, bool* v)
//{
//	ImVec2 p = ImGui::GetCursorScreenPos();
//	ImDrawList* DrawList = ImGui::GetWindowDrawList();
//	float Height = ImGui::GetFrameHeight();
//	float Width = Height * 1.7f;
//	float Radius = Height / 2 - 2;
//
//	ImGui::InvisibleButton(str_id, ImVec2(Width, Height));
//	if (ImGui::IsItemClicked())
//		*v = !(*v);
//
//	float t = *v ? 1.0f : 0.f;
//	ImGuiContext& g = *GImGui;
//	float AnimationSpeed = 0.15f;
//	if (g.LastActiveId == g.CurrentWindow->GetID(str_id))
//	{
//		float T_Animation = ImSaturate(g.LastActiveIdTimer / AnimationSpeed);
//		t = *v ? (T_Animation) : (1.0f - T_Animation);
//	}
//	ImU32 Color;
//
//	ImVec4 darkGreen(0.2f, 0.5f, 0.1f, 1.0f);
//	ImVec4 darkRed(0.5f, 0.1f, 0.1f, 1.0f);
//	ImVec4 darkRedmais(0.3f, 0.0f, 0.0f, 1.0f);
//	ImVec4 darkGreenmais(0.1f, 0.4f, 0.1f, 1.0f);
//
//	if (ImGui::IsItemHovered())
//		Color = ImGui::GetColorU32(ImLerp(darkRedmais, darkGreenmais, t));
//	else
//		Color = ImGui::GetColorU32(ImLerp(darkRed, darkGreen, t));
//	DrawList->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + Width, p.y + Height), Color, 360);
//	DrawList->AddCircleFilled(ImVec2(p.x + Radius + 2 + t * (Width - (Radius + 2) * 2), p.y + Radius + 2), Radius + 2, IM_COL32(255, 255, 255, 255), 360);
//	DrawList->AddCircleFilled(ImVec2(p.x + Radius + t * (Width - Radius * 2) + (t == 0 ? 2 : -2), p.y + Radius + 2), Radius, IM_COL32(230, 230, 230, 255), 360);
//	if (*v)
//		DrawList->AddText(ImVec2(p.x + 45, p.y + 2), ImColor{ 255,255,255,255 }, str_id);
//	else
//		DrawList->AddText(ImVec2(p.x + 45, p.y + 2), ImColor{ 185,185,185,255 }, str_id);
//
//}

//void DisplayPlayerTab();
ImColor ESP_LINE = { 1.0f, 0.0f, 0.0f, 1.0f };
// ImColor FOV_User = { 1.f, 1.f, 1.f, 1.f };
ImColor ESP_NAME = { 0.f, 1.f, 1.f, 1.f };
ImColor ESP_IMPOSTOR = { 1.0f, 0.0f, 0.0f, 1.0f };
ImColor color_cross = { 1.f, 1.f, 1.f, 1.f };
ImColor color_head = { 0.f, 1.f, 1.f, 1.f };

void DisplayVisualESPTab();
void DisplayAimbotTab();
Tab currentTab = Tab::VISUAL_ESP;
#include "xor.h"
#include <thread>
#include "particles.h"
SDK::FRotator AimRotation, oldRotation = SDK::FRotator{ 0, 0, 0 };
SDK::FVector LockedPawnHead = SDK::FVector{ 0, 0, 0 };
std::vector<std::string> MenuFCList = { _("Visual"), _("Visual Items"), _("Misc") };
void render::Menu()
{
	interfaceimgui();
	menustyle();
	if (hook_par == 0)
	{
		InitializeParticles();

		hook_par = 1;
	}
	static int Index = 0;
	ImGui::SetNextWindowSize(ImVec2(700.f, 450.f));
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.2f, 0.2f, 1.0f)); // Cor do texto branca
	ImGui::Begin("wolfloaders.xyz (Lockdown Protocol)", &bDisplay, ImGuiWindowFlags_NoDecoration);
	UpdateParticles(0.04);
	RenderParticles();
	//render_background();
	ImGui::Text(_("wolfloaders.xyz (Lockdown Protocol)"));
	ImGui::Separator();
	ImGui::PopStyleColor();

	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 6.0f);
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(33 / 255.0f, 33 / 255.0f, 33 / 255.0f, 1.0f));


	ImGui::BeginChild("MenuButtons", ImVec2(150, 220), false);
	ImGui::Indent(10);
	{
		ImGui::Dummy(ImVec2(0.0f, 10.0f));
		{
			for (int i = 0; i < MenuFCList.size(); i++)
			{
				static float width = ImGui::GetContentRegionAvail().x - 10.f;
				if (ImGui::Button(MenuFCList[i].c_str(), ImVec2(width, 35.f)))
					Index = i;
			}
		}
	}
	ImGui::EndChild();



	ImGui::SameLine();

	//---// Left //--------------------------------------//
	ImGui::BeginChild(_("##LeftChild"), ImVec2(ImGui::GetContentRegionAvail().x / 2.f - 16.f, ImGui::GetContentRegionAvail().y), false);
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	ImGui::Indent(5);

	//original
	//for (int i = 0; i < MenuFCList.size(); i++)
	//{
	//	static float width = (ImGui::GetContentRegionAvail().x / 4.f) - 6.f;
	//	if (ImGui::Button(MenuFCList[i].c_str(), ImVec2(width, 35.f)))
	//		Index = i;

	//	if (i != MenuFCList.size() - 1)
	//		ImGui::SameLine();
	//}

	//ImGui::Spacing();

	////---// Left //--------------------------------------//
	//ImGui::BeginChild(_("##LeftChild"), ImVec2(ImGui::GetContentRegionAvail().x / 2.f - 16.f, ImGui::GetContentRegionAvail().y), false);
	//original
	switch (Index)
	{
	case 0: // visual
		ImGui::Text(_("Visual Player"));
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::PushFont(gc.Roboto);
		ImGui::Checkbox("Enable ESP (In Match)", &hook);

		ImGui::NewLine();
		ImGui::Spacing();

		/*	ImGui::Text(_("ESP Options"));
			ImGui::Separator();
			ImGui::Spacing();
			MyCheckBox2(_("Box"), &g.ESP_Box);
			ImGui::SameLine();
			ImGui::Combo(_("##ESPBoxStyle"), &g.tipobox, _("Normal\0CornerBox\0"));*/

		//ImGui::Checkbox("ESP Box Cornered", &corneredbox);
		//ImGui::Checkbox("ESP Box", &box);
		ImGui::Checkbox("ESP Line", &line);
		ImGui::Checkbox("ESP Name", &name);
		ImGui::Checkbox("ESP Impostor", &impostor);
		ImGui::Checkbox("ESP Distance", &distance);
		ImGui::PopFont();
		break;
	case 1:
		ImGui::Text(_("ESP World"));
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::PushFont(gc.Roboto);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(233 / 255.0f, 148 / 255.0f, 12 / 255.0f, 1.0f)); // Vermelho
		ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(200 / 255.0f, 234 / 255.0f, 52 / 255.0f, 1.0f)); // Cor da linha
		ImGui::Text(_("ESP ITENS"));
		ImGui::Separator();
		ImGui::PopStyleColor(2);
		ImGui::Checkbox("ESP Dropped Item", &dropped_item_esp);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(233 / 255.0f, 148 / 255.0f, 12 / 255.0f, 1.0f)); // Vermelho
		ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(200 / 255.0f, 234 / 255.0f, 52 / 255.0f, 1.0f)); // Cor da linha
		ImGui::Text(_("ESP TASKS"));
		ImGui::Separator();
		ImGui::PopStyleColor(2);
		ImGui::Checkbox("ESP Gas Bottle", &gas_esp);
		ImGui::Checkbox("ESP Battery", &battery_esp);
		ImGui::Checkbox("ESP Vents", &avent_esp);
		ImGui::Checkbox("ESP Container", &container_esp);
		ImGui::Checkbox("ESP Scanner", &scanner_esp);
		ImGui::Checkbox("ESP Delivery", &delivery_esp);
		ImGui::Checkbox("ESP Alimbox", &alimbox_esp);
		ImGui::Checkbox("ESP ScrewDriver", &screwdriver);
		ImGui::Checkbox("ESP Vents Filter", &filtervents);
		ImGui::NewLine();
		ImGui::Spacing();
		ImGui::PopFont();
		break;
	case 2:
		ImGui::Text(_("Misc"));
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::PushFont(gc.Roboto);
		ImGui::Checkbox("Stamina Infinita", &inf_estamina);
		ImGui::Checkbox("Change Color", &bchange_colot);
		//ImGui::Combo(_("##changecolor"), &tipo_cor, _("Red\0Blue\0Yellow\0Green\0White\0Pink\0Cyan\0Orange\0gray"));
		//ImGui::Text("Press F2 to respawn from the dead");
		//ImGui::Text("Press F3 to detonate C4");
		//ImGui::Text("Press F4 to spawn pistol item");
		//ImGui::Text("Press F5 to spawn C4 item");
		//ImGui::Text("Press F6 to spawn C4 item at player's location");
		//ImGui::Text("Press F9 to spawn pistol or revolver item");
		//ImGui::Text("Press F10 to detonate C4 at saved actor's location");
		//ImGui::Text("Press RIGHT arrow to change color to the next option");
		//ImGui::Text("Press LEFT arrow to change color to the previous option");
		//ImGui::Text("Press HOME to clear saved items");
		//ImGui::Text("Press F10 to change color");
		ImGui::PopFont();
		break;
	default:
		break;
	}

	ImGui::EndChild();
	//---------------------------------------------------//
	ImGui::SameLine();
	//---// Right //--------------------------------------//
	ImGui::BeginChild(_("##RightChild"), ImVec2(ImGui::GetContentRegionAvail()), false);
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	ImGui::Indent(5);
	switch (Index)
	{
	
	case 0: // visual


		ImGui::Text(_("ESP Colors"));
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::ColorEdit4(_("Line"), &ESP_LINE.Value.x);
		ImGui::ColorEdit4(_("Name"), &ESP_NAME.Value.x);
		ImGui::ColorEdit4(_("Impostor"), &ESP_IMPOSTOR.Value.x);
		//ImGui::ColorEdit4(_("CrossHair"), &color_cross.Value.x);
		//ImGui::ColorEdit4(_("FOV AIMBOT"), &FOV_User.Value.x);
		break;
	case 2: // visual

		ImGui::Text(_("Shortcuts"));
		ImGui::Separator();
		ImGui::Spacing();
		//ImGui::PushFont(gc.Roboto);
		ImGui::Text("Press F2 para renascer dos mortos");
		ImGui::Text("Press F3 para detonar C4");
		ImGui::Text("Press F4 para criar pistola");
		ImGui::Text("Press F5 para criar C4");
		//ImGui::Text("Press F6 para criar C4");
		ImGui::Text("Press F9 para criar pistola ou revolver");
		ImGui::Text("Press F10 para detonar C4 nos players");
		ImGui::Text("Press seta DIREITA prox cor");
		ImGui::Text("Press seta ESQUERDA cor anterior");
		ImGui::Text("Press HOME para limpar itens salvos");
		//ImGui::Text("Press F10 para mudar de cor");
		//ImGui::Text("Press F2 to respawn from the dead");
		//ImGui::Text("Press F3 to detonate C4");
		//ImGui::Text("Press F4 to spawn pistol item");
		//ImGui::Text("Press F5 to spawn C4 item");
		//ImGui::Text("Press F6 to spawn C4 item at player's location");
		//ImGui::Text("Press F9 to spawn pistol or revolver item");
		//ImGui::Text("Press F10 to detonate C4 at saved actor's location");
		//ImGui::Text("Press RIGHT arrow to change color to the next option");
		//ImGui::Text("Press LEFT arrow to change color to the previous option");
		//ImGui::Text("Press HOME to clear saved items");
		//ImGui::Text("Press F10 to change color");
		//ImGui::PopFont();
		break;
	}
	ImGui::EndChild();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();

	//---------------------------------------------------//
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	SV_RAINBOW(160, 160, 200);
	ImGui::End();
	//	ImGui::BeginGroup();
	//	{
	//		UpdateParticles(0.04);
	//		RenderParticles();
	//		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.1f, 0.1f, 0.1f, 0.95f));
	//		ImGui::BeginChild("TabButtons", ImVec2(180, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize);
	//		{
	//
	//		//	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.2f, 0.2f, 1.0f)); // Cor do texto branca
	//			if (ImGui::Button("Visual ESP", ImVec2(160, 50)))
	//				currentTab = Tab::VISUAL_ESP;

	//			if (ImGui::Button("Aimbot", ImVec2(160, 50)))
	//				currentTab = Tab::AIMBOT;

	//		//	ImGui::PopStyleColor();
	//		}
	//		ImGui::EndChild();
	//		ImGui::PopStyleColor();
	//	}
	//	ImGui::EndGroup();

	//	ImGui::SameLine();

	//	ImGui::BeginGroup();
	//	{
	//		switch (currentTab)
	//		{
	//		case Tab::VISUAL_ESP:
	//			DisplayVisualESPTab();
	//			break;
	//		case Tab::AIMBOT:
	//			DisplayAimbotTab();
	//			break;
	//		default:
	//			break;
	//		}
	//	}
	//	ImGui::EndGroup();
	//	ImGui::PopStyleColor();
	//	ImGui::PopStyleColor();
	//	ImGui::PopStyleColor();
	//	ImGui::PopStyleColor();
	//	SV_RAINBOW(160, 160, 200);
	//ImGui::End();
}

void DisplayVisualESPTab()
{
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.1f, 0.1f, 0.1f, 0.95f));
	ImGui::BeginChild("PlayerChild", ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize);
	UpdateParticles(0.04);
	RenderParticles();
	ImGui::Indent(10);
	{
		ImGui::Text("PLAYER:");
		ImGui::Spacing();
		ImGui::Checkbox("Enable ESP (In Match)", &hook);

		if (hook)
		{
			//	ImGui::Checkbox("ESP Skeleton", &skeleton);
			ImGui::Checkbox("ESP Box Cornered", &corneredbox);
			ImGui::Checkbox("ESP Box", &box);
			ImGui::Checkbox("ESP Line", &line);
			ImGui::Checkbox("ESP Name", &name);
			ImGui::Checkbox("ESP Impostor", &impostor);
			//ImGui::Checkbox("ESP Health", &hp);
			ImGui::Checkbox("ESP Distance", &distance);
			ImGui::Text(_("Misc"));
			ImGui::Separator();
			ImGui::Checkbox("Stamina Infinita", &inf_estamina);
		}
	}
	ImGui::EndChild();
	ImGui::PopStyleColor();
}
void drawFov()
{

	ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
	ImVec2 screen_size = ImGui::GetIO().DisplaySize;

	float radius = fovaim;
	float x = screen_size.x / 2.0f;
	float y = screen_size.y / 2.0f;
	ImVec2 center(x, y);

	draw_list->AddCircle(center, radius, ImColor{ 255,255,255 }, 30, 0.1);

}
enum BoneID
{
	HEAD = 7,
	CHEST = 5, // Bip001_Spine2
	FEET = 113, // Bip001_L_Foot ou Bip001_R_Foot
	PELVIS = 2 // Bip001_Pelvis
};
int selectedBoneIndex = 0;
// VariC!vel para armazenar o osso selecionado
int selectedBone = HEAD;

void DisplayAimbotTab()
{
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.1f, 0.1f, 0.1f, 0.95f));
	ImGui::BeginChild("AimbotChild", ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize);
	UpdateParticles(0.04);
	RenderParticles();
	ImGui::Indent(10);
	{
		ImGui::Text("AIMBOT:");
		ImGui::Spacing();
		ImGui::Checkbox("Cross Aimbot", &aimbot);
		ImGui::Checkbox("Silent Aimbot", &silent_aimbot);
		const char* KeyBindStringaim = KeyBinding ? _("< press any key >") : KeyNames[teclaaim];
		ImGui::Text(_("KEY AIMBOT: "));
		ImGui::SameLine();
		if (ImGui::Button(KeyBindStringaim, ImVec2(180.f, 20.f)))
		{
			KeyBinding = true;
			//g.BindingID = 1;
			std::thread([&]() {
				KeyBinder(teclaaim);
				}).detach();
		}
		ImGui::Checkbox("Esp Fov", &espfov);
		if (espfov)
			drawFov();

		ImGui::SliderFloat("Fov", &fovaim, 26.0f, 300.0f);
		ImGui::SliderFloat("Smoothing", &smoothing, 1.0f, 10.0f);
		const char* boneNames[] = { "HEAD", "CHEST", "FOOT", "PELVIS" };
		if (ImGui::Combo("Bone Selection", &selectedBoneIndex, boneNames, IM_ARRAYSIZE(boneNames)))
		{
			// Ajustando o ID do osso com base na seleC'C#o
			switch (selectedBoneIndex)
			{
			case 0:
				selectedBone = HEAD;
				break;
			case 1:
				selectedBone = CHEST;
				break;
			case 2:
				selectedBone = FEET;
				break;
			case 3:
				selectedBone = PELVIS;
				break;
			}
		}
		if (aimbot)
			silent_aimbot = false;
		else if (silent_aimbot)
			aimbot = false;
	}
	ImGui::EndChild();
	ImGui::PopStyleColor();
	   ImGui::PopStyleColor();
    ImGui::PopStyleVar();
}


float InitSpeed = 0;
ImVec4 ColorBranco = { 1, 1, 1, 1 }; // branco
ImU32 branco = ImColor(ColorBranco);

void dump_bones(SDK::AMec_C* pawn);
bool bKeyRightPressed = false;
bool bKeyLeftPressed = false;
bool IsKeyJustPressed(int key, bool& keyState)
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		if (!keyState)
		{
			keyState = true;
			return true;
		}
	}
	else
	{
		keyState = false;
	}
	return false;
}
bool F10WasPressed = false;
bool F9WasPressed = false;
bool F6WasPressed = false;
bool F5WasPressed = false;
bool F4WasPressed = false;
//void aimbot_function(SDK::AMec_C* pawn, int id_bone, int index_aim);
//void aimbot_function2(SDK::AMec_C* pawn, int id_bone, int index_aim);
//void silent_aim(SDK::AMec_C* pawn, int id_bone);
//void GetPlayerViewPointHook(SDK::APlayerController* this_PlayerController, SDK::FVector* Location, SDK::FRotator* Rotation, SDK::AMec_C* pawn, int id_bone);
int inimigoTravado = -1;
SDK::AMec_C* localpawn = 0;
SDK::FVector2D winCenter{ GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN) };
std::vector<SDK::AVent_C*> avent;
std::vector<SDK::AAlimBox_C*> alimbox;
std::vector<SDK::ADeliveryCase_C*> delivery;
std::vector<SDK::AScanner_Machine_C*> scanner;
std::vector<SDK::AContainerCleaner_C*> container;
std::vector<SDK::AMec_C*> savedActors;
SDK::UData_Item_C* itemsavedatac4;
SDK::FStr_ItemState ItemStatedatac4;
SDK::UData_Item_C* itemsavedatapistol;
SDK::FStr_ItemState ItemStatedatapistol;
SDK::UGameplayStatics* GPStatics = NULL;
void render::Render()
{
	ImGui::Begin("##DrawFunction", 0, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoInputs);
	if (hook)
	{
		unrealengine::GWorld = SDK::UWorld().GetWorld();
		//auto actors = ue->World->GameState->PlayerArray;
		//	utl::initall();
		SDK::UObject::GObjects.InitGObjects();
		SDK::UWorld* GWorldPtr = SDK::UWorld().GetWorld(); // Obtém o ponteiro para o objeto SDK::UWorld

		ue->World = GWorldPtr;
		if (!ue->World)
		{
			//printf("aqui 1");
			return;
		}

		ue->GameInstance = ue->World->OwningGameInstance;
		if (!ue->GameInstance) {
			//printf("aqui 2");
			return;
		}

		ue->LocalPlayer = ue->GameInstance->LocalPlayers[0];
		if (!ue->LocalPlayer) {
			//printf("aqui 3");
			return;
		}

		ue->LocalPlayerController = ue->LocalPlayer->PlayerController;
		if (!ue->LocalPlayerController) {
			//printf("aqui 4");
			return;
		}

			/*ue->PlayerCameraManager = ue->LocalPlayerController->PlayerCameraManager;
			if (!ue->PlayerCameraManager)  return  

			ue->LocalPawn = (SDK::AMec_C*)ue->LocalPlayerController->AcknowledgedPawn;
			if (!ue->LocalPawn)  return;*/

			//ue->RootComponent = ue->LocalPawn->RootComponent;
			//if (!ue->RootComponent)  return;

			//ue->GameState = ue->World->GameState;
			//if (!ue->GameState)  return;
		savedActors.clear();
		SDK::TArray<SDK::AActor*> PlayerArray{};
		 GPStatics = (SDK::UGameplayStatics*)SDK::UGameplayStatics::StaticClass();
		 GPStatics->GetAllActorsOfClass(GWorldPtr, SDK::AMec_C::StaticClass(), &PlayerArray);
		
		//GPStatics->GetAllActorsOfClass(ue->World, SDK::AMec_C::StaticClass(), &PlayerArray);

		/*SDK::UGameplayStatics().GetDefaultObj()->GetAllActorsOfClass(ue->World, SDK::AMec_C::StaticClass(), &PlayerArray);*/

		SDK::TArray<SDK::AActor*> Vent{}, AlimboxC{}, DeliverycaseC{}, ScannerMachine{}, aContainerclean{}, agasbottle{}, ascrewdriver{}, aventsfilter{}, battery{};
		SDK::TArray<SDK::AActor*> worditem{};
		SDK::UGameplayStatics().GetDefaultObj()->GetAllActorsOfClass(ue->World, SDK::AVent_C::StaticClass(), &Vent);
		SDK::UGameplayStatics().GetDefaultObj()->GetAllActorsOfClass(ue->World, SDK::AAlimBox_C::StaticClass(), &AlimboxC);
		SDK::UGameplayStatics().GetDefaultObj()->GetAllActorsOfClass(ue->World, SDK::ADeliveryCase_C::StaticClass(), &DeliverycaseC);
		SDK::UGameplayStatics().GetDefaultObj()->GetAllActorsOfClass(ue->World, SDK::AScanner_Machine_C::StaticClass(), &ScannerMachine);
		SDK::UGameplayStatics().GetDefaultObj()->GetAllActorsOfClass(ue->World, SDK::AContainerCleaner_C::StaticClass(), &aContainerclean);
		SDK::UGameplayStatics().GetDefaultObj()->GetAllActorsOfClass(ue->World, SDK::ABottleSlot_C::StaticClass(), &agasbottle);
		SDK::UGameplayStatics().GetDefaultObj()->GetAllActorsOfClass(ue->World, SDK::AScrew_C::StaticClass(), &ascrewdriver);
		SDK::UGameplayStatics().GetDefaultObj()->GetAllActorsOfClass(ue->World, SDK::AFilterCleaner_C::StaticClass(), &aventsfilter);
		SDK::UGameplayStatics().GetDefaultObj()->GetAllActorsOfClass(ue->World, SDK::ABatteryCharger_C::StaticClass(), &battery);
		SDK::UGameplayStatics().GetDefaultObj()->GetAllActorsOfClass(ue->World, SDK::AWorldItem_C::StaticClass(), &worditem);
		SDK::AMec_C* pawn_local = 0;

		if (PlayerArray.Num() > 0)
		{
			for (int i = 0; i < PlayerArray.Num(); i++)
			{
				if (!PlayerArray.IsValidIndex(i)) continue;


				auto actor = (SDK::AMec_C*)PlayerArray[i];
				if (!actor) continue;

				auto pawn = actor;

				/*	if (actor->IsA(SDK::AMec_C::StaticClass())) {
						auto* pawn = (SDK::AMec_C*)actor;*/

						//if (!pawn) continue;
				if (!pawn)
				{
					continue;
				}
				if (pawn->IsLocallyControlled())
				{
					localpawn = pawn;
					if (GetAsyncKeyState(VK_F2) & 0x8000)
					{
						pawn->Respawn();
					}
					if (GetAsyncKeyState(VK_F3) & 0x8000)
					{
						//pawn->Detonate_C4();

						pawn->Detonate_C4();

					}					
					if (inf_estamina)
					{
						pawn->Stamina_CD = 0;
						actor->Stamina = 1;

						if (!pawn->Stamina_Regenering)
							pawn->Stamina_Regenering = true;

						pawn->Exhausted_Effect = 0;
						if (pawn->Exhausted)
							pawn->Exhausted = false;
					}
					if (bchange_colot)
					{
						if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
						{
							if (!bKeyRightPressed) // Verifica se a tecla nC#o estava pressionada anteriormente
							{
								tipo_cor++;
								if (tipo_cor > 9) // Se ultrapassar o C-ndice 9
								{
									tipo_cor = 1; // Reseta para o primeiro C-ndice
								}
								pawn->Set_Player_Index(tipo_cor);
								pawn->Skin_Color = tipo_cor;
								bKeyRightPressed = true; // Marca a tecla como pressionada
							}
						}
						else
						{
							bKeyRightPressed = false; // Reseta o estado quando a tecla C) liberada
						}

						if (GetAsyncKeyState(VK_LEFT) & 0x8000)
						{
							if (!bKeyLeftPressed) // Verifica se a tecla nC#o estava pressionada anteriormente
							{
								tipo_cor--;
								if (tipo_cor < 1) // Se passar do C-ndice 1
								{
									tipo_cor = 9; // Volta para o C:ltimo C-ndice
								}
								pawn->Set_Player_Index(tipo_cor);
								pawn->Skin_Color = tipo_cor;
								bKeyLeftPressed = true; // Marca a tecla como pressionada
							}
						}
						else
						{
							bKeyLeftPressed = false; // Reseta o estado quando a tecla C) liberada
						}
					}
					if (IsKeyJustPressed(VK_F5, F5WasPressed))
					{
						SDK::FVector playerLocation = localpawn->K2_GetActorLocation();

						playerLocation.Z += 20.0f;
						SDK::FVector Mec_Velocity_0(0.f, 0.f, 0.f);
						SDK::FVector Drop_Direction(0.f, 0.f, 1.f);
						/*SDK::UData_Item_C* ItemData = actor->Data;
						SDK::FStr_ItemState ItemState = actor->Item_State;*/
						SDK::UData_Item_C* ItemData;
						SDK::FStr_ItemState ItemState;
						
							if (itemsavedatac4)
							{
								ItemData = itemsavedatac4;
								ItemState = ItemStatedatac4;
								localpawn->Net_Spawn_NewItem(playerLocation, Mec_Velocity_0, Drop_Direction, ItemData, ItemState);
							}
						
					}
					if (IsKeyJustPressed(VK_F4, F4WasPressed))
						{
						SDK::FVector playerLocation = localpawn->K2_GetActorLocation();
						playerLocation.Z += 20.0f;
						SDK::FVector Mec_Velocity_0(0.f, 0.f, 0.f);
						SDK::FVector Drop_Direction(0.f, 0.f, 1.f);
						/*SDK::UData_Item_C* ItemData = actor->Data;
						SDK::FStr_ItemState ItemState = actor->Item_State;*/
						SDK::UData_Item_C* ItemData;
						SDK::FStr_ItemState ItemState;
						
							if (itemsavedatapistol)
							{
								ItemData = itemsavedatapistol;
								ItemState = ItemStatedatapistol;
								localpawn->Net_Spawn_NewItem(playerLocation, Mec_Velocity_0, Drop_Direction, ItemData, ItemState);
							}
						}
					if (GetAsyncKeyState(VK_HOME) & 0x8000)
					{
						itemsavedatapistol = 0;						
							itemsavedatac4 = 0;
					}
					continue;
				}
				savedActors.push_back(pawn);

				//	auto basebone = pawn->Mesh->GetSocketLocation(pawn->Mesh->GetBoneName(0));
				auto basebone = actor->K2_GetActorLocation();


				auto [good, boxb] = utl::get_box(ue->LocalPlayerController, pawn);
				if (good)
				{
					if (box)
					{
						if (boxb.width != 0 && boxb.height != 0)
							draw::DrawBox(boxb.position.X, boxb.position.Y, boxb.width, boxb.height, ue->LocalPlayerController->LineOfSightTo(pawn, { 0,0,0 }, false) ? ImColor{ 55,255,0 } : ImColor{ 255,255,255 });

					}
					if (corneredbox)
					{
						if (boxb.width != 0 && boxb.height != 0)
							draw::DrawCorneredBox(boxb.position.X, boxb.position.Y, boxb.width, boxb.height, ue->LocalPlayerController->LineOfSightTo(pawn, { 0,0,0 }, false) ? ImColor{ 55,255,0 } : ImColor{ 255,255,255 }, 1.5f);

					}
					if (line)
					{
						if (pawn)
						{

							SDK::FVector2D pes;

							ImVec2 lineStart(winCenter.X / 2.0f, 0.0f);
							if (ue->LocalPlayerController->ProjectWorldLocationToScreen(basebone, &pes, false))
							{
								draw::DrawLine(lineStart, { static_cast<float>(pes.X), static_cast<float>(pes.Y) },
									ue->LocalPlayerController->LineOfSightTo(pawn, { 0,0,0 }, false)
									? ImColor{ 55.0f, 255.0f, 0.0f }
								: ESP_LINE, 1.5f);
							}
						}

					}
					//if (hp)
					//{
					//	float healthBarX = boxb.position.X - 5; // Ajuste conforme necessC!rio
					//	float healthBarY = boxb.position.Y;
					//	float healthBarWidth = 3; // Ajuste conforme necessC!rio
					//	float healthBarHeight = boxb.height;

					//	// Desenhar a barra de vida
					//	draw::HealthBar(healthBarX, healthBarY, healthBarWidth, healthBarHeight, health, true);
					//}
					if (name)
					{
						if (pawn)
						{

							SDK::FVector2D pes;
							auto nameget = pawn->PlayerState->PlayerNamePrivate;
							if (ue->LocalPlayerController)
							{
								if (ue->LocalPlayerController->ProjectWorldLocationToScreen(basebone, &pes, false))
								{

									float textX = pes.X;
									float textY = pes.Y + 10;
									ImColor branco = ImColor(255, 255, 255);

									// Adicionar texto com ImGui
									ImGui::GetForegroundDrawList()->AddText(ImVec2(textX, textY), ESP_NAME, nameget.IsValid() ? nameget.ToString().c_str() : " ");
								}
							}
						}
					}
					if (impostor)
					{
						if (pawn)
						{

							SDK::FVector2D pes;

							auto role = pawn->Player_Role;
							SDK::FString nameinfo = L"";
							if (role == SDK::E_PlayerRole::NewEnumerator0)
							{
								nameinfo = L"";
							}
							if (role == SDK::E_PlayerRole::NewEnumerator1)
							{
								nameinfo = L"";
							}
							if (role == SDK::E_PlayerRole::NewEnumerator2)
							{
								nameinfo = L"";
							}
							if (role == SDK::E_PlayerRole::NewEnumerator3)
							{
								nameinfo = L"Impostor";
							}
							if (role == SDK::E_PlayerRole::NewEnumerator4)
							{
								nameinfo = L"Looby";
							}
							if (role == SDK::E_PlayerRole::E_MAX)
							{
								nameinfo = L"";
							}
							if (ue->LocalPlayerController)
							{
								if (ue->LocalPlayerController->ProjectWorldLocationToScreen(basebone, &pes, false))
								{

									float textX = pes.X;
									float textY = pes.Y + 40;
									ImColor branco = ImColor(255, 255, 255);

									// Adicionar texto com ImGui
									ImGui::GetForegroundDrawList()->AddText(ImVec2(textX, textY), ESP_IMPOSTOR, nameinfo.IsValid() ? nameinfo.ToString().c_str() : " ");
								}
							}
						}
					}
					if (distance)
					{
						if (pawn)
						{

							SDK::FVector2D pes;
							auto distanceValue = localpawn->GetDistanceTo(pawn);
							distanceValue = distanceValue / 100.f; // Convertendo para metros ou qualquer unidade desejada
							int distanceValueInt = static_cast<int>(distanceValue); // Convertendo para inteiro
							if (ue->LocalPlayerController)
							{
								if (ue->LocalPlayerController->ProjectWorldLocationToScreen(basebone, &pes, false))
								{
									float textX = pes.X;
									float textY = pes.Y + 24;
									ImColor branco = ImColor(255, 255, 255);
									std::string displayText = " [" + std::to_string(distanceValueInt) + "m]";


									ImGui::GetForegroundDrawList()->AddText(ImVec2(textX, textY), branco, displayText.c_str());
								}
							}
						}
					}

				}
			}
			if (dropped_item_esp)
			{
				if (worditem.Num() > 0)
				{
					for (int i = 0; i < worditem.Num(); i++)
					{
						if (!worditem.IsValidIndex(i)) continue;

						auto actor = (SDK::AWorldItem_C*)worditem[i];
						if (!actor) continue;

						if (actor)
						{
							//int itemCountBefore = worditem.Num();
							// Obtendo o nome do item
							auto data = actor->Data->Name_Data_Item_C.ToString();
							auto basebone = actor->K2_GetActorLocation();

							SDK::FVector2D pes;
							if (ue->LocalPlayerController)
							{
								if (ue->LocalPlayerController->ProjectWorldLocationToScreen(basebone, &pes, false))
								{
									// Calculando a distC"ncia do item atC) o jogador
									auto distanceValue = localpawn->GetDistanceTo(actor) / 100.f;
									int distanceValueInt = static_cast<int>(distanceValue);

									float textX = pes.X;
									float textY = pes.Y + 10;

									ImColor itemColor = ImColor(255, 255, 255); // Cor padrC#o C) branco

									// Definindo a cor com base no nome do item
									std::string itemName = data.c_str();
									if (itemName == "PISTOL")
									{
										itemColor = ImColor(0, 0, 255); // Azul
										/*if (!itemsavedatapistol)
										{
											itemsavedatapistol = actor->Data;
											ItemStatedatapistol = actor->Item_State;
										}*/
									}
									else if (itemName == "C4")
									{
										itemColor = ImColor(255, 0, 0); // Vermelho
										if (!itemsavedatac4)
										{
											itemsavedatac4 = actor->Data;
											ItemStatedatac4 = actor->Item_State;
										}										
									}
									else if (itemName == "REVOLVER")
									{
										itemColor = ImColor(0, 0, 255); // Azul
										if (!itemsavedatapistol)
										{
											itemsavedatapistol = actor->Data;
											ItemStatedatapistol = actor->Item_State;
										}
									}
									else if (itemName == "KNIFE")
									{
										itemColor = ImColor(255, 255, 0); // Amarelo
									}
									else if (itemName == "SCREW DRIVER")
									{
										itemColor = ImColor(245, 92, 32); // Amarelo
									}

									std::string distanceText = itemName + "[" + std::to_string(distanceValueInt) + "m]";
									ImGui::GetForegroundDrawList()->AddText(ImVec2(textX, textY), itemColor, distanceText.c_str());

									if (itemName == "C4" && IsKeyJustPressed(VK_F10, F10WasPressed))
									{
										for (auto& savedActor : savedActors)
										{
											if (savedActor)
											{
												SDK::FVector actorLocation = savedActor->K2_GetActorLocation();
												actorLocation.Z += 10.0f;

												SDK::FVector Mec_Velocity_0(0.f, 0.f, 0.f);
												SDK::FVector Drop_Direction(0.f, 0.f, 1.f);
												SDK::UData_Item_C* ItemData = actor->Data;
												SDK::FStr_ItemState ItemState = actor->Item_State;
												localpawn->Net_Spawn_NewItem(actorLocation, Mec_Velocity_0, Drop_Direction, ItemData, ItemState);
												actor->Detonate_C4();
												localpawn->Detonate_C4();
											}
										}
										
									}
									if ((itemName == "PISTOL" || itemName == "REVOLVER")&& IsKeyJustPressed(VK_F9, F9WasPressed))
									{
										SDK::FVector playerLocation = localpawn->K2_GetActorLocation();

										playerLocation.Z += 10.0f;
										SDK::FVector Mec_Velocity_0(0.f, 0.f, 0.f);
										SDK::FVector Drop_Direction(0.f, 0.f, 1.f);
										SDK::UData_Item_C* ItemData = actor->Data;
										SDK::FStr_ItemState ItemState = actor->Item_State;
										localpawn->Net_Spawn_NewItem(playerLocation, Mec_Velocity_0, Drop_Direction, ItemData, ItemState);
									}
									/*if (itemName == "C4" && IsKeyJustPressed(VK_F6, F6WasPressed))
									{
										SDK::FVector playerLocation = localpawn->K2_GetActorLocation();

										playerLocation.Z += 20.0f;
										SDK::FVector Mec_Velocity_0(0.f, 0.f, 0.f);
										SDK::FVector Drop_Direction(0.f, 0.f, 1.f);
										SDK::UData_Item_C* ItemData = actor->Data;
										SDK::FStr_ItemState ItemState = actor->Item_State;
										localpawn->Net_Spawn_NewItem(playerLocation, Mec_Velocity_0, Drop_Direction, ItemData, ItemState);

										
									}		*/							
								}
							}
						}
					}
				}
			}			
			if (avent_esp)
			{
				if (Vent.Num() > 0)
				{
					for (int i = 0; i < Vent.Num(); i++)
					{
						if (!Vent.IsValidIndex(i)) continue;


						auto actor = (SDK::ABottleSlot_C*)Vent[i];
						if (!actor) continue;

						if (actor)
						{
							auto basebone = actor->K2_GetActorLocation();
							SDK::FVector2D pes;
							if (ue->LocalPlayerController)
							{
								if (ue->LocalPlayerController->ProjectWorldLocationToScreen(basebone, &pes, false))
								{
									auto distanceValue = localpawn->GetDistanceTo(actor);
									distanceValue = distanceValue / 100.f;
									int distanceValueInt = static_cast<int>(distanceValue);

									SDK::FString nameget = L"Vent";

									float textX = pes.X;
									float textY = pes.Y + 10;
									ImColor branco = ImColor(255, 255, 255);

									float textX2 = pes.X;
									float textY2 = pes.Y + 24;
									std::string displayText = " [" + std::to_string(distanceValueInt) + "m]";
									ImGui::GetForegroundDrawList()->AddText(ImVec2(textX2, textY2), branco, displayText.c_str());
									ImGui::GetForegroundDrawList()->AddText(ImVec2(textX, textY), branco, nameget.IsValid() ? nameget.ToString().c_str() : " ");
								}
							}
						}
					}
				}
			}
			if (container_esp)
			{
				if (aContainerclean.Num() > 0)
				{
					for (int i = 0; i < aContainerclean.Num(); i++)
					{

						if (!aContainerclean.IsValidIndex(i)) continue;


						auto actor = (SDK::AContainerCleaner_C*)aContainerclean[i];
						if (!actor) continue;

						if (actor)
						{
							auto basebone = actor->K2_GetActorLocation();
							SDK::FVector2D pes;
							if (ue->LocalPlayerController)
							{
								if (ue->LocalPlayerController->ProjectWorldLocationToScreen(basebone, &pes, false))
								{

									auto distanceValue = localpawn->GetDistanceTo(actor);
									distanceValue = distanceValue / 100.f;
									int distanceValueInt = static_cast<int>(distanceValue);

									SDK::FString nameget = L"Container";

									float textX = pes.X;
									float textY = pes.Y + 10;
									ImColor branco = ImColor(255, 255, 255);

									float textX2 = pes.X;
									float textY2 = pes.Y + 24;
									std::string displayText = " [" + std::to_string(distanceValueInt) + "m]";
									ImGui::GetForegroundDrawList()->AddText(ImVec2(textX2, textY2), branco, displayText.c_str());
									ImGui::GetForegroundDrawList()->AddText(ImVec2(textX, textY), branco, nameget.IsValid() ? nameget.ToString().c_str() : " ");
								}
							}
						}
					}
				}
			}
			if (scanner_esp)
			{
				if (ScannerMachine.Num() > 0)
				{
					for (int i = 0; i < ScannerMachine.Num(); i++)
					{
						if (!ScannerMachine.IsValidIndex(i)) continue;


						auto actor = (SDK::AScanner_Machine_C*)ScannerMachine[i];
						if (!actor) continue;
						if (actor)
						{
							auto basebone = actor->K2_GetActorLocation();
							SDK::FVector2D pes;
							if (ue->LocalPlayerController)
							{
								if (ue->LocalPlayerController->ProjectWorldLocationToScreen(basebone, &pes, false))
								{
									auto distanceValue = localpawn->GetDistanceTo(actor);
									distanceValue = distanceValue / 100.f;
									int distanceValueInt = static_cast<int>(distanceValue);

									SDK::FString nameget = L"Scanner";

									float textX = pes.X;
									float textY = pes.Y + 10;
									ImColor branco = ImColor(255, 255, 255);

									float textX2 = pes.X;
									float textY2 = pes.Y + 24;



									std::string displayText = " [" + std::to_string(distanceValueInt) + "m]";

									ImGui::GetForegroundDrawList()->AddText(ImVec2(textX2, textY2), branco, displayText.c_str());
									ImGui::GetForegroundDrawList()->AddText(ImVec2(textX, textY), branco, nameget.IsValid() ? nameget.ToString().c_str() : " ");
								}
							}
						}
					}
				}
			}
			if (delivery_esp)
			{
				if (DeliverycaseC.Num() > 0)
				{
					for (int i = 0; i < DeliverycaseC.Num(); i++)
					{
						if (!DeliverycaseC.IsValidIndex(i)) continue;


						auto actor = (SDK::ADeliveryCase_C*)DeliverycaseC[i];
						if (!actor) continue;

						auto basebone = actor->K2_GetActorLocation();
						SDK::FVector2D pes;
						if (ue->LocalPlayerController)
						{
							if (ue->LocalPlayerController->ProjectWorldLocationToScreen(basebone, &pes, false))
							{
								auto distanceValue = localpawn->GetDistanceTo(actor);
								distanceValue = distanceValue / 100.f;
								int distanceValueInt = static_cast<int>(distanceValue);

								SDK::FString nameget = L"Delivery";

								float textX = pes.X;
								float textY = pes.Y + 10;
								ImColor branco = ImColor(255, 255, 255);

								float textX2 = pes.X;
								float textY2 = pes.Y + 24;
								std::string displayText = " [" + std::to_string(distanceValueInt) + "m]";
								ImGui::GetForegroundDrawList()->AddText(ImVec2(textX2, textY2), branco, displayText.c_str());
								ImGui::GetForegroundDrawList()->AddText(ImVec2(textX, textY), branco, nameget.IsValid() ? nameget.ToString().c_str() : " ");
							}
						}
					}
				}
			}
			if (alimbox_esp)
			{
				if (AlimboxC.Num() > 0)
				{
					for (int i = 0; i < AlimboxC.Num(); i++)
					{
						if (!AlimboxC.IsValidIndex(i)) continue;


						auto actor = (SDK::AAlimBox_C*)AlimboxC[i];
						if (!actor) continue;
						if (actor)
						{
							auto basebone = actor->K2_GetActorLocation();
							SDK::FVector2D pes;
							if (ue->LocalPlayerController)
							{
								if (ue->LocalPlayerController->ProjectWorldLocationToScreen(basebone, &pes, false))
								{
									auto distanceValue = localpawn->GetDistanceTo(actor);
									distanceValue = distanceValue / 100.f;
									int distanceValueInt = static_cast<int>(distanceValue);

									SDK::FString nameget = L"AlimBox";

									float textX = pes.X;
									float textY = pes.Y + 10;
									ImColor branco = ImColor(255, 255, 255);

									float textX2 = pes.X;
									float textY2 = pes.Y + 24;



									std::string displayText = " [" + std::to_string(distanceValueInt) + "m]";

									ImGui::GetForegroundDrawList()->AddText(ImVec2(textX2, textY2), branco, displayText.c_str());
									ImGui::GetForegroundDrawList()->AddText(ImVec2(textX, textY), branco, nameget.IsValid() ? nameget.ToString().c_str() : " ");
								}
							}
						}
					}
				}
			}
			if (gas_esp)
			{
				if (battery.Num() > 0)
				{
					for (int i = 0; i < battery.Num(); i++)
					{
						if (!battery.IsValidIndex(i)) continue;


						auto actor = (SDK::ABatteryCharger_C*)battery[i];
						if (!actor) continue;
						if (actor)
						{
							auto basebone = actor->K2_GetActorLocation();
							SDK::FVector2D pes;
							if (ue->LocalPlayerController)
							{
								if (ue->LocalPlayerController->ProjectWorldLocationToScreen(basebone, &pes, false))
								{
									auto distanceValue = localpawn->GetDistanceTo(actor);
									distanceValue = distanceValue / 100.f;
									int distanceValueInt = static_cast<int>(distanceValue);

									SDK::FString nameget = L"Gas Bottle";

									float textX = pes.X;
									float textY = pes.Y + 10;
									ImColor branco = ImColor(255, 255, 255);

									float textX2 = pes.X;
									float textY2 = pes.Y + 24;



									std::string displayText = " [" + std::to_string(distanceValueInt) + "m]";

									ImGui::GetForegroundDrawList()->AddText(ImVec2(textX2, textY2), branco, displayText.c_str());
									ImGui::GetForegroundDrawList()->AddText(ImVec2(textX, textY), branco, nameget.IsValid() ? nameget.ToString().c_str() : " ");
								}
							}
						}
					}
				}
			}
			if (screwdriver)
			{
				if (ascrewdriver.Num() > 0)
				{
					for (int i = 0; i < ascrewdriver.Num(); i++)
					{
						if (!ascrewdriver.IsValidIndex(i)) continue;


						auto actor = (SDK::AScrew_C*)ascrewdriver[i];
						if (!actor) continue;
						if (actor)
						{
							auto basebone = actor->K2_GetActorLocation();
							SDK::FVector2D pes;
							if (ue->LocalPlayerController)
							{
								if (ue->LocalPlayerController->ProjectWorldLocationToScreen(basebone, &pes, false))
								{
									auto distanceValue = localpawn->GetDistanceTo(actor);
									distanceValue = distanceValue / 100.f;
									int distanceValueInt = static_cast<int>(distanceValue);

									SDK::FString nameget = L"Screw Driver";

									float textX = pes.X;
									float textY = pes.Y + 10;
									ImColor branco = ImColor(255, 255, 255);

									float textX2 = pes.X;
									float textY2 = pes.Y + 24;



									std::string displayText = " [" + std::to_string(distanceValueInt) + "m]";

									ImGui::GetForegroundDrawList()->AddText(ImVec2(textX2, textY2), branco, displayText.c_str());
									ImGui::GetForegroundDrawList()->AddText(ImVec2(textX, textY), branco, nameget.IsValid() ? nameget.ToString().c_str() : " ");
								}
							}
						}
					}
				}
			}
			if (filtervents)
			{
				if (aventsfilter.Num() > 0)
				{
					for (int i = 0; i < aventsfilter.Num(); i++)
					{
						if (!aventsfilter.IsValidIndex(i)) continue;


						auto actor = (SDK::AFilterCleaner_C*)aventsfilter[i];
						if (!actor) continue;
						if (actor)
						{
							auto basebone = actor->K2_GetActorLocation();
							SDK::FVector2D pes;
							if (ue->LocalPlayerController)
							{
								if (ue->LocalPlayerController->ProjectWorldLocationToScreen(basebone, &pes, false))
								{
									auto distanceValue = localpawn->GetDistanceTo(actor);
									distanceValue = distanceValue / 100.f;
									int distanceValueInt = static_cast<int>(distanceValue);

									SDK::FString nameget = L"Filters Vent";

									float textX = pes.X;
									float textY = pes.Y + 10;
									ImColor branco = ImColor(255, 255, 255);

									float textX2 = pes.X;
									float textY2 = pes.Y + 24;



									std::string displayText = " [" + std::to_string(distanceValueInt) + "m]";

									ImGui::GetForegroundDrawList()->AddText(ImVec2(textX2, textY2), branco, displayText.c_str());
									ImGui::GetForegroundDrawList()->AddText(ImVec2(textX, textY), branco, nameget.IsValid() ? nameget.ToString().c_str() : " ");
								}
							}
						}
					}
				}
			}
			if (aimbot)
			{
				//aimbot_function(pawn, selectedBone, i);
			}
		}
	}
	ImGui::End();
}

//void dump_bones(SDK::AMec_C* pawn)
//{
//	FILE* log = fopen("c:\\bones.txt", "w");
//	if (log)
//	{
//		auto meshname = pawn->Mesh->SkeletalMesh->Name;
//		auto bonetree = pawn->Mesh->SkeletalMesh->Skeleton->BoneTree;
//
//		for (int i = 0; i < bonetree.Num(); i++)
//		{
//			auto autobonename = pawn->Mesh->GetBoneName(i);
//
//			fprintf(log, "%s = %d,\n", autobonename.GetRawString().c_str(), i);
//		}
//		fclose(log);
//	}
//}


//
//void aimbot_function(SDK::AMec_C* pawn, int id_bone, int index_aim)
//{
//	SDK::FVector2D head;
//	auto basebone = pawn->Mesh->GetSocketLocation(pawn->Mesh->GetBoneName(id_bone));
//	SDK::FVector2D winCenter{ GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN) };
//	float closestDist = INFINITY;
//	if (!ue->LocalPlayerController->ProjectWorldLocationToScreen(basebone, &head, false))
//		return;
//
//	if (IsWithinFOV(head, winCenter.X, winCenter.Y, fovaim))
//	{
//
//		SDK::FRotator currentAngles = ue->LocalPlayerController->PlayerCameraManager->GetCameraRotation();
//		SDK::FRotator targetAngles = ue->CalcAngle(ue->LocalPlayerController->PlayerCameraManager->GetCameraLocation(), basebone, currentAngles, smoothing); // 10.0f C) o fator de suavizaC'C#o, ajuste conforme necessC!rio
//		if (GetAsyncKeyState(teclaaim) != 0)
//		{
//			if (inimigoTravado == -1)
//			{
//				inimigoTravado = index_aim;
//			}
//
//			if (inimigoTravado == index_aim)
//			{
//				ue->LocalPlayerController->SetControlRotation(targetAngles);
//			}
//		}
//		else
//		{
//			inimigoTravado = -1;
//		}
//	}
//}
//
#define M_PI 3.14159265358979323846
void render::Aim()
{

}
