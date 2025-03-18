#include <windows.h>
#include <thread>
#include <chrono>
#include <string>
#include <sstream>
#include <commctrl.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void AutoClick();
void RegisterHotkeys();
void HotkeyListener();
void UpdateStatusBar();

HWND hwndStart, hwndStop, hwndInterval, hwndLabel, hwndStatusBar;
bool clicking = false;
int clickInterval = 1000;
int clickType = MOUSEEVENTF_LEFTDOWN;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "AutoClickerWindow";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wc);
    
    HWND hwnd = CreateWindowEx(
        0, "AutoClickerWindow", "Auto Clicker GUI", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 350, 220,
        NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);
    
    // Start hotkey listener in a separate thread
    std::thread(HotkeyListener).detach();
    
    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE:
        hwndLabel = CreateWindow("STATIC", "Click Interval (ms):", WS_VISIBLE | WS_CHILD, 20, 20, 150, 20, hwnd, NULL, NULL, NULL);
        hwndInterval = CreateWindow("EDIT", "1000", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 180, 20, 100, 25, hwnd, NULL, NULL, NULL);
        hwndStart = CreateWindow("BUTTON", "Start", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 60, 70, 100, 35, hwnd, (HMENU)1, NULL, NULL);
        hwndStop = CreateWindow("BUTTON", "Stop", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 180, 70, 100, 35, hwnd, (HMENU)2, NULL, NULL);
        
        hwndStatusBar = CreateWindow(STATUSCLASSNAME, "Stopped", WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP, 
                                    0, 0, 0, 0, hwnd, NULL, NULL, NULL);
        
        SendMessage(hwndStart, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);
        SendMessage(hwndStop, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);
        SendMessage(hwndInterval, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);
        SendMessage(hwndLabel, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);
        break;
    
    case WM_SIZE:
        SendMessage(hwndStatusBar, WM_SIZE, 0, 0);
        break;
    
    case WM_COMMAND:
        if (LOWORD(wParam) == 1) { // Start Button Clicked
            char buffer[10];
            GetWindowText(hwndInterval, buffer, 10);
            clickInterval = atoi(buffer);
            clicking = true;
            UpdateStatusBar();
            std::thread(AutoClick).detach();
        }
        else if (LOWORD(wParam) == 2) { // Stop Button Clicked
            clicking = false;
            UpdateStatusBar();
        }
        break;

    case WM_CLOSE:
        clicking = false;
        DestroyWindow(hwnd);
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void AutoClick() {
    while (clicking) {
        mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(clickInterval));
    }
    UpdateStatusBar();
}

void HotkeyListener() {
    // Register hotkeys: F6 to start, F7 to stop
    RegisterHotKey(NULL, 1, 0, VK_F6);
    RegisterHotKey(NULL, 2, 0, VK_F7);
    
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (msg.message == WM_HOTKEY) {
            switch (msg.wParam) {
            case 1: // F6
                clicking = true;
                UpdateStatusBar();
                std::thread(AutoClick).detach();
                break;
            case 2: // F7
                clicking = false;
                UpdateStatusBar();
                break;
            }
        }
    }
}

void UpdateStatusBar() {
    SendMessage(hwndStatusBar, SB_SETTEXT, 0, (LPARAM)(clicking ? "Clicking..." : "Stopped"));
}
