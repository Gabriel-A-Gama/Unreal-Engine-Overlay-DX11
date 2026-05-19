#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include "kiero/kiero.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef HRESULT(__stdcall* Resize)(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
typedef uintptr_t PTR;

template<typename T>
class Singleton
{
protected:
	Singleton() {}
	~Singleton() {}

	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;

	Singleton(Singleton&&) = delete;
	Singleton& operator=(Singleton&&) = delete;
public:
	static T& Get()
	{
		static T inst{};
		return inst;
	}
};


inline namespace Game
{
	class Variables : public Singleton<Variables>
	{
	public:
		float box2dCol[4] = { 1.f, 1.f, 1.f, 1.f };
		ImFont* Ruda = NULL;
		ImFont* Roboto = NULL;
		//bool
		bool EnableESP = false;
		bool PlayerVisual = false;
		bool ItemVisual = false;
		bool AiVisual = false;

		//ai
		bool AiEnableOutline = false;


		float maxDist = 300.f;
	};
#define gc Game::Variables::Get()
}