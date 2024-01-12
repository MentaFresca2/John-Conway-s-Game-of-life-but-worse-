#include <windows.h>
#include <windowsx.h>


typedef struct {
    int x;
    int y;
    int size;
    int blockNumber;
    COLORREF color;
    BOOL reves;
} Block;

// Variable global para almacenar el estado del clic
BOOL isMouseDown = FALSE;

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

            HBRUSH hBackgroundBrush = CreateSolidBrush(RGB(0, 147, 57));
            FillRect(hdc, &ps.rcPaint, hBackgroundBrush);
            DeleteObject(hBackgroundBrush);

            for (int y = 0; y < 100; y++) {
                for (int x = 0; x < 100; x++) {
                    blocks[y][x].x = x * blockSize;
                    blocks[y][x].y = y * blockSize;
                    blocks[y][x].size = blockSize;
                    blocks[y][x].blockNumber = y * 100 + x;

                    
                                                if (blocks[y][x].reves){
                            blocks[y][x].color = RGB(255, 255, 255);
                        }else{
                            blocks[y][x].color = RGB(0, 0, 0);
                            }
                        
                 
                        if (blocks[y][x].reves){
                            blocks[y][x].color = RGB(0, 0, 0);
                        }else{
                            blocks[y][x].color = RGB(255, 255, 255);
                            }
                        
                    

                    HBRUSH hBrush = CreateSolidBrush(blocks[y][x].color);
                    SelectObject(hdc, hBrush);
                    SetBkMode(hdc, TRANSPARENT);
                    RECT rect = { blocks[y][x].x, blocks[y][x].y, blocks[y][x].x + blockSize, blocks[y][x].y + blockSize };
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
                blocks[yBlock][xBlock].reves = (blocks[yBlock][xBlock].reves == TRUE) ? FALSE : TRUE;
                InvalidateRect(hwnd, NULL, TRUE);
            }

            return 0;
        }
        case WM_LBUTTONUP: {
            isMouseDown = FALSE;
            return 0;
        }
        case WM_MOUSEMOVE: {
            // Si el botón está presionado, cambiar el color mientras se mueve
            if (isMouseDown) {
                int xPos = GET_X_LPARAM(lParam);
                int yPos = GET_Y_LPARAM(lParam);

                int xBlock = xPos / 10;
                int yBlock = yPos / 10;

                if (xBlock >= 0 && xBlock < 100 && yBlock >= 0 && yBlock < 100) {
                    blocks[yBlock][xBlock].reves = (blocks[yBlock][xBlock].reves == TRUE) ? FALSE : TRUE;
                    InvalidateRect(hwnd, NULL, TRUE);
                }
            }

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
