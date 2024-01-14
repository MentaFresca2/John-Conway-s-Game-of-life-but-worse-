#include <windows.h>
#include <windowsx.h>
typedef struct {
    int x;
    int y;
    int size;
    COLORREF color;
} Block;

// Matriz de bloques
Block blocks[100][100];

// Variable global que almacena el estado del juego
BOOL empezado = FALSE;
// Variable global para almacenar el estado del clic
BOOL isMouseDown = FALSE;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    RECT rect; 
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Dibujo de bloques aquí
            for (int y = 0; y < 100; y++) {
                for (int x = 0; x < 100; x++) {
                    HBRUSH hBrush = CreateSolidBrush(blocks[y][x].color);
                    SelectObject(hdc, hBrush);
                    SetBkMode(hdc, TRANSPARENT);
                    RECT rect = { blocks[y][x].x, blocks[y][x].y, blocks[y][x].x + blocks[y][x].size, blocks[y][x].y + blocks[y][x].size };
                    FillRect(hdc, &rect, hBrush);
                    DeleteObject(hBrush);
                }
            }

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_LBUTTONDOWN: {
            
            isMouseDown = TRUE;
            int xPos = GET_X_LPARAM(lParam);
            int yPos = GET_Y_LPARAM(lParam);

            // Identificar el bloque clicado
            int xBlock = xPos / 10; // tamaño del bloque
            int yBlock = yPos / 10;

            // Cambiar el color del bloque clicado
            if (xBlock >= 0 && xBlock < 100 && yBlock >= 0 && yBlock < 100) {
                blocks[yBlock][xBlock].color = (blocks[yBlock][xBlock].color == RGB(255, 255, 255)) ? RGB(0, 0, 0) : RGB(255, 255, 255);
                InvalidateRect(hwnd, &rect, TRUE);
            }

            return 0;
        }

        case WM_KEYDOWN: {
            if (wParam == VK_SPACE) {
                empezado = TRUE;
            }
            return 0;
        }

        case WM_LBUTTONUP: {
            isMouseDown = FALSE;
            return 0;
        }

        case WM_MOUSEMOVE: {
            if (isMouseDown) {
                int xPos = GET_X_LPARAM(lParam);
                int yPos = GET_Y_LPARAM(lParam);

                int xBlock = xPos / 10;
                int yBlock = yPos / 10;

                if (xBlock >= 0 && xBlock < 100 && yBlock >= 0 && yBlock < 100) {
                    blocks[yBlock][xBlock].color = (blocks[yBlock][xBlock].color == RGB(255, 255, 255)) ? RGB(0, 0, 0) : RGB(255, 255, 255);
                    InvalidateRect(hwnd, &rect, TRUE);
                }
            }

            return 0;
        }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int main() {
    // Registro de la clase de la ventana
    WNDCLASSW wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"VentanaClase";

    if (!RegisterClassW(&wc)) {
        MessageBoxW(NULL, L"Error al registrar la clase de la ventana", L"Error", MB_ICONERROR);
        return 1;
    }

    // Creación de la ventana
    HWND hwnd = CreateWindowExW(
        0,
        L"VentanaClase",
        L"Ventana en Blanco y Negro",
        WS_OVERLAPPEDWINDOW,
        100, 100, 1000, 1000,
        NULL,
        NULL,
        GetModuleHandle(NULL),
        NULL
    );

    if (!hwnd) {
        MessageBoxW(NULL, L"Error al crear la ventana", L"Error", MB_ICONERROR);
        return 1;
    }

    // Inicialización de la matriz de bloques
    for (int y = 0; y < 100; y++) {
        for (int x = 0; x < 100; x++) {
            blocks[y][x].x = x * 10;
            blocks[y][x].y = y * 10;
            blocks[y][x].size = 10;
            blocks[y][x].color = RGB(0, 0, 0);
        }
    }

    // Mostrar la ventana
    ShowWindow(hwnd, SW_SHOWNORMAL);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Eliminar la clase de ventana registrada
    UnregisterClassW(L"VentanaClase", GetModuleHandle(NULL));

    return 0;
}
