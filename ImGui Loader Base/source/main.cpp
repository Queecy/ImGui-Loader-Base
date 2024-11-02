#include "includes.h"
#include <Windows.h>

HWND main_hwnd = nullptr;
LPDIRECT3DDEVICE9 g_pd3dDevice = nullptr;
D3DPRESENT_PARAMETERS g_d3dpp;
LPDIRECT3D9 g_pD3D = nullptr;

namespace ui {
    void init(LPDIRECT3DDEVICE9 device) {
    }

    void render() {
        ImGui::SetNextWindowSize(ImVec2(550, 350)); 
        static bool show_window = true;

        ImGui::Begin("Imgui loader base | Queecy", &show_window, ImGuiWindowFlags_NoResize);
        ImGui::Text("Imgui loader base | Queecy");

        if (ImGui::Button("Github")) {
            ShellExecute(0, 0, "https://github.com/Queecy", 0, 0, SW_SHOW);
        }
        ImGui::SameLine();  
        if (ImGui::Button("Telegram")) {
            ShellExecute(0, 0, "https://t.me/notQueecy", 0, 0, SW_SHOW);
        }

        static bool checkbox_state = false;
        ImGui::Checkbox("Checkbox", &checkbox_state);

        static char buf[256] = "";
        ImGui::InputText("input text", buf, IM_ARRAYSIZE(buf));

        static int slider_value = 0;
        ImGui::SliderInt("Slide", &slider_value, 0, 100);

        static ImVec4 color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        ImGui::ColorEdit3("select color", (float*)&color);

        const char* items[] = { "Option1", "Option2", "Option3" };
        static int item_current = 0;
        ImGui::Combo("Select option", &item_current, items, IM_ARRAYSIZE(items));

        ImGui::Separator();
        static float float_value = 0.0f;
        ImGui::SliderFloat("Float Slider", &float_value, 0.0f, 1.0f);

        ImGui::ProgressBar(0.5f, ImVec2(0.0f, 0.0f), "50%"); // Optional text
        ImGui::Text("Tooltip");
        if (ImGui::IsItemHovered()) {
            ImGui::SetTooltip("tooltip text");
        }

        ImGui::End();

        if (!show_window)
        {
            PostQuitMessage(0);
        }
// ImGuiWindowFlags_NoResize
// ImGuiWindowFlags_None
// ImGuiWindowFlags_AlwaysAutoResize
// ImGuiWindowFlags_NoTitleBar
// ImGuiWindowFlags_NoResize
// ImGuiWindowFlags_NoMove
// ImGuiWindowFlags_NoScrollbar
// ImGuiWindowFlags_NoScrollWithMouse
// ImGuiWindowFlags_NoCollapse
// ImGuiWindowFlags_AlwaysVerticalScrollbar
// ImGuiWindowFlags_AlwaysHorizontalScrollbar
// ImGuiWindowFlags_AlwaysUseWindowPadding
// ImGuiWindowFlags_NoNav
// ImGuiWindowFlags_NoBackground
// ImGuiWindowFlags_ChildWindow
// ImGuiWindowFlags_ComboBox
// ImGuiWindowFlags_NoMouseInputs
// ImGuiWindowFlags_MenuBar
// ImGuiWindowFlags_HorizontalScrollbar
// ImGuiWindowFlags_NoFocusOnAppearing
// ImGuiWindowFlags_NoBringToFrontOnFocus
// ImGuiWindowFlags_AlwaysUseWindowPadding
// ImGuiWindowFlags_NoDocking
// ImGuiWindowFlags_NoBackground
    }
}

bool CreateDeviceD3D(HWND hWnd)
{
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
        return false;

    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE; 

    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
        return false;
    return true;
}

void CleanupDeviceD3D()
{
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
}

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg) {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED) {
            g_d3dpp.BackBufferWidth = LOWORD(lParam);
            g_d3dpp.BackBufferHeight = HIWORD(lParam);
            ResetDevice();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) 
            return 0;
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, "Loader base", NULL };
    RegisterClassEx(&wc);
    main_hwnd = CreateWindow(wc.lpszClassName, "Loader base", WS_POPUP, 0, 0, 5, 5, NULL, NULL, wc.hInstance, NULL);

    if (!CreateDeviceD3D(main_hwnd)) {
        CleanupDeviceD3D();
        UnregisterClass(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    ShowWindow(main_hwnd, SW_HIDE);
    UpdateWindow(main_hwnd);

    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr; 
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; 

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplWin32_Init(main_hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    bool isActive = true; 
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            continue;
        }

        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        static bool initialized = false;
        if (!initialized) {
            ui::init(g_pd3dDevice);
            initialized = true;
        }
        ui::render();

        ImGui::EndFrame();

        g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0) {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }

        HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) {
            ResetDevice();
        }
        if (!isActive) {
            msg.message = WM_QUIT;
        }
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    DestroyWindow(main_hwnd);
    UnregisterClass(wc.lpszClassName, wc.hInstance);

    return 0;
}
