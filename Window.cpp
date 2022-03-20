#include "Window.h"

Window* window = nullptr;

Window::Window()
{
}

/*
* Use the CallWindowProc function for window subclassing. Usually, all windows with the same class share one window procedure. 
* A subclass is a window or set of windows with the same class whose messages are intercepted and processed by another window procedure (or procedures) 
* before being passed to the window procedure of the class.
*/
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)  // Passes message information to the specified window procedure
{
    /* Syntax:
    * LRESULT CallWindowProcA(
      [in] WNDPROC lpPrevWndFunc,
      [in] HWND    hWnd,    -> A handle to the window procedure to receive the message
      [in] UINT    Msg,     -> The message
      [in] WPARAM  wParam,  -> Additional message-specific information. The contents of this parameter depend on the value of the Msg parameter
      [in] LPARAM  lParam   -> Additional message-specific information. The contents of this parameter depend on the value of the Msg parameter
    );
    */

    /*
    * Return value:
    * Type: LRESULT
    * The return value specifies the result of the message processing and depends on the message sent.
    */

    switch (msg)
    {
    case WM_CREATE:
    {
        // Event triggered when the window will be created
        window->setHWND(hwnd);
        window->onCreate();
        break;
    }
    case WM_DESTROY:
    {
        // Event triggered when the window will be destroyed
        // Window* window = (Window*)GetWindowLong(hwnd, GWL_USERDATA);
        window->onDestroy();
        ::PostQuitMessage(0);
        break;
    }
    default:
        return ::DefWindowProc(hwnd, msg, wparam, lparam);
    }

    return NULL;
}

bool Window::init()
{
    WNDCLASSEX wc;
    wc.cbClsExtra = NULL;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.cbWndExtra = NULL;
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wc.hInstance = NULL;
    wc.lpszClassName = L"MyWindowClass";
    wc.lpszMenuName = L"";
    wc.style = NULL;
    wc.lpfnWndProc = WndProc;  // A pointer to the window procedure. You must use the CallWindowProc function to call the window procedure

    if (!::RegisterClassEx(&wc))  // If the registration of class will fail, the function will return false
        return false;

    // Set this BEFORE the window creation to avoid NullPointerException
    if (!window)
        window = this;

    // Creation of the window
    m_hwnd = ::CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, L"MyWindowClass", L"DirectX Application", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768, NULL, NULL, NULL, NULL);

    // If the creation fail, return false
    if (!m_hwnd)
        return false;

    // Show up the window
    ::ShowWindow(m_hwnd, SW_SHOW);
    ::UpdateWindow(m_hwnd);

    // Set this flag to indicate that the window is initialized and running
    m_is_running = true;
    return true;
}

bool Window::broadcast()
{
    MSG msg;

    window->onUpdate();

    while(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)  // We have to do this UNTIL this queue is emptied
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    Sleep(1);  // With this, the CPU can handle the loop in a more relaxed way

    return true;
}

bool Window::release()
{
    // Destroying the window
    if (!::DestroyWindow(m_hwnd))
        return false;

    return true;
}

bool Window::isRunning()
{
    return m_is_running;
}

RECT Window::getClientWindowRect()
{
    RECT rc;
    ::GetClientRect(this->m_hwnd, &rc);
    return rc;
}

void Window::setHWND(HWND hwnd)
{
    this->m_hwnd = hwnd;
}

void Window::onCreate()
{
}

void Window::onUpdate()
{
}

void Window::onDestroy()
{
    m_is_running = false;
}

Window::~Window()
{
}
