#include <stdio.h>
#include <windows.h>

// Procedimiento de ventana (Window Procedure)
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

int main() {
    // Paso 1: Registrar la clase de la ventana
    WNDCLASSW wc = { 0 };  // Utiliza WNDCLASSW para cadenas de caracteres de ancho
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandleW(NULL);  // Utiliza GetModuleHandleW para cadenas de caracteres de ancho
    wc.hbrBackground = (HBRUSH)(GetStockObject(BLACK_BRUSH)); // Establece el fondo a negro
    wc.lpszClassName = L"MiVentanaClase";

    if (!RegisterClassW(&wc)) {  // Utiliza RegisterClassW para cadenas de caracteres de ancho
        MessageBoxW(NULL, L"Error al registrar la clase de la ventana", L"Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    // Paso 2: Crear la ventana
    HWND hwnd = CreateWindowExW(
        0,
        L"MiVentanaClase",
        L"Mi Ventana",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL, NULL, GetModuleHandleW(NULL), NULL);  // Utiliza GetModuleHandleW para cadenas de caracteres de ancho

    if (!hwnd) {
        MessageBoxW(NULL, L"Error al crear la ventana", L"Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    // Paso 3: Mostrar y actualizar la ventana
    ShowWindow(hwnd, SW_SHOWNORMAL);
    UpdateWindow(hwnd);

    // Paso 4: Bucle de mensajes
    MSG msg;
    while (GetMessageW(&msg, NULL, 0, 0)) {  // Utiliza GetMessageW para cadenas de caracteres de ancho
        TranslateMessage(&msg);
        DispatchMessageW(&msg);  // Utiliza DispatchMessageW para cadenas de caracteres de ancho
    }

    return 0;
}
