#pragma once
#include <iostream>
#include <windows.h>
#include <thread>

#include <d3d9.h>
#pragma comment(lib,"d3d9.lib")

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"

extern HWND main_hwnd;
extern LPDIRECT3DDEVICE9 g_pd3dDevice;
extern D3DPRESENT_PARAMETERS g_d3dpp;
extern LPDIRECT3D9 g_pD3D;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
