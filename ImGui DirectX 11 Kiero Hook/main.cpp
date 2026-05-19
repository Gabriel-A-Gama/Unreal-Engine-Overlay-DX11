#include "includes.h"
#include "render.h"
#include "ruda.h"
#include "Fonte.h"
#include "kiero/minhook/include/MinHook.h"
#include <thread>
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Present oPresent;
Resize oResize = NULL;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;

void InitImGui()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	//io.Fonts->AddFontFromMemoryTTF(roboto, sizeof(roboto), 17.0f);
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	gc.Ruda = io.Fonts->AddFontFromMemoryTTF(rawData, sizeof(rawData), 14);
	gc.Roboto = io.Fonts->AddFontFromMemoryTTF(rawData, sizeof(rawData), 16);
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	if (render::bDisplay)
		ImGui::GetIO().MouseDrawCursor = ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);

	/*if (ImGui::GetIO().WantCaptureMouse)
	{
		if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
			return true;
		return false;
	}*/
	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

bool init = false;
HRESULT __stdcall hkResize(IDXGISwapChain* pThis, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags) {
	if (mainRenderTargetView)
	{
		pContext->OMSetRenderTargets(0, 0, 0);
		mainRenderTargetView->Release();
	}

	HRESULT hr = oResize(pThis, BufferCount, Width, Height, NewFormat, SwapChainFlags);
	ID3D11Texture2D* pBuffer;
	pThis->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBuffer);

	pDevice->CreateRenderTargetView(pBuffer, NULL, &mainRenderTargetView);

	pBuffer->Release();

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);

	D3D11_VIEWPORT vp;
	vp.Width = Width;
	vp.Height = Height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pContext->RSSetViewports(1, &vp);
	return hr;
}
bool init_item = false;
HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!init)
	{
		//SDK::InitGObjects();
	
		SDK::UObject::GObjects.InitGObjects();
		unrealengine::GWorld = SDK::UWorld().GetWorld();
		
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)& pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			InitImGui();
			init = true;
		}

		else
			return oPresent(pSwapChain, SyncInterval, Flags);
	}

	if ((GetAsyncKeyState(VK_INSERT) & 1) || (GetAsyncKeyState(VK_DELETE) & 1))
		render::bDisplay = !render::bDisplay;


	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	
	if (render::bDisplay)
		render::Menu();

	render::Render();
	ImGui::Render();

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return oPresent(pSwapChain, SyncInterval, Flags);
}

DWORD WINAPI MainThread(LPVOID lpReserved)
{
	//uint64_t hook_preset_sig = (uint64_t)(GetModuleHandleA("GameOverlayRenderer64.dll")) + 0x8C550;

	//uint64_t create_hook_sig = (uint64_t)(GetModuleHandleA("GameOverlayRenderer64.dll")) + 0x86170;


	/*uint64_t addr = (uint64_t)(GetModuleHandleA("DiscordHook64.dll")) + 0xE9090;

	Present* discord_present = (Present*)addr;
	
	oPresent = *discord_present;

	_InterlockedExchangePointer((volatile PVOID*)addr, hkPresent);*/

	//uintptr_t GetViewPointAddress = (uintptr_t)GetModuleHandleA("Strinova-Win64-Shipping.exe") + 0xA59480;
	bool init_hook = false;
	do
	{
		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		{
			kiero::bind(8, (void**)& oPresent, hkPresent);
			kiero::bind(13, (void**)&oResize, hkResize);
			//if (MH_Initialize() != MH_STATUS::MH_OK) return FALSE;

			//if (MH_CreateHook(reinterpret_cast<LPVOID>(GetViewPointAddress), render::GetPlayerViewPointHook, reinterpret_cast<LPVOID*>(&render::GetPlayerViewPoint)) != MH_STATUS::MH_OK) return FALSE;

			//if (MH_EnableHook(reinterpret_cast<LPVOID>(GetViewPointAddress)) != MH_STATUS::MH_OK) return FALSE;
			init_hook = true;
		}
	} while (!init_hook);
	return TRUE;
}
DWORD WINAPI AimThread(LPVOID lpReserved)
{
	while (true)
	{
		render::Aim();
	}
}
BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		AllocConsole();
		FILE* Dummy;
		freopen_s(&Dummy, "CONOUT$", "w", stdout);
		freopen_s(&Dummy, "CONIN$", "r", stdin);
		DisableThreadLibraryCalls(hMod);
		CreateThread(nullptr, 0, MainThread, hMod, 0, nullptr);
		CreateThread(nullptr, 0, AimThread, hMod, 0, nullptr);
		break;
	case DLL_PROCESS_DETACH:
		kiero::shutdown();
		break;
	}
	return TRUE;
}