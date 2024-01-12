#include <windows.h>

typedef struct {
    int x;
    int y;
    int size;
    int blockNumber;
    COLORREF color;
} Block;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static Block blocks[100][100]; // Matriz de bloques

    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            int blockSize = 10; // px alto y ancho

                HBRUSH hBackgroundBrush = CreateSolidBrush(RGB(0, 147, 57)); // Cambia el color de fondo según tus preferencias
                FillRect(hdc, &ps.rcPaint, hBackgroundBrush);
                DeleteObject(hBackgroundBrush);
            for (int y = 0; y < 100; y++) {
                for (int x = 0; x < 100; x++) {
                    blocks[y][x].x = x * blockSize;
                    blocks[y][x].y = y * blockSize;
                    blocks[y][x].size = blockSize;
                    blocks[y][x].blockNumber = y * 100 + x;

                    // Determinar el color basado en el bloque que tenga encima
                    if (y > 0 && blocks[y - 1][x].color == RGB(255, 255, 255)) {
                       
                        blocks[y][x].color = RGB(0, 0, 0);
                    } else {
                        // Si es la primera fila, asigna un color inicial
                        blocks[y][x].color = RGB(255, 255, 255); // Blanco
                    }

                    // Dibuja el cubo con el color asignado
                    HBRUSH hBrush = CreateSolidBrush(blocks[y][x].color);
                    SelectObject(hdc, hBrush);
                    SetBkMode(hdc, TRANSPARENT);
                    RECT rect = { blocks[y][x].x, blocks[y][x].y, blocks[y][x].x + blockSize, blocks[y][x].y + blockSize };
                    FillRect(hdc, &rect, hBrush);

                    DeleteObject(hBrush); // Limpia el pincel creado

                }
            }

            EndPaint(hwnd, &ps);
            return 0;
        }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


int main() {
    // Registro de la clase de la ventana
    WNDCLASSW wc = {0}; // Usa WNDCLASSW para literales de cadena amplios
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"VentanaClase"; // Usa el prefijo L para literales de cadena amplios

    if (!RegisterClassW(&wc)) { // Usa RegisterClassW para literales de cadena amplios
        MessageBoxW(NULL, L"Error al registrar la clase de la ventana", L"Error", MB_ICONERROR); // Usa MessageBoxW
        return 1;
    }

    // Creación de la ventana
    HWND hwnd = CreateWindowExW(
        0,
        L"VentanaClase",
        L"Ventana en Blanco y Negro",
        WS_OVERLAPPEDWINDOW,
        100, // Posición x
        100, // Posición y
        1000,
        1000,
        NULL,
        NULL,
        GetModuleHandle(NULL),
        NULL
    );

    if (!hwnd) {
        MessageBoxW(NULL, L"Error al crear la ventana", L"Error", MB_ICONERROR); // Usa MessageBoxW
        return 1;
    }
 

    
    ShowWindow(hwnd, SW_SHOWNORMAL);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

  

    // Eliminar la clase de ventana registrada
    UnregisterClassW(L"VentanaClase", GetModuleHandle(NULL)); // Usa UnregisterClassW

    return 0;
}
