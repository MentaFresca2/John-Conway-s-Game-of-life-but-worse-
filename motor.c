#include <windows.h>
#include <windowsx.h>

typedef struct {
    int x;
    int y;
    int size;
    int blockNumber;
    COLORREF color;
} Block;

BOOL empezado = FALSE;
BOOL isMouseDown = FALSE;
UINT_PTR timerID;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static Block blocks[100][100];

    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            int blockSize = 10;

            HBRUSH hBackgroundBrush = CreateSolidBrush(RGB(0, 147, 57));
            FillRect(hdc, &ps.rcPaint, hBackgroundBrush);
            DeleteObject(hBackgroundBrush);

            for (int y = 0; y < 100; y++) {
                for (int x = 0; x < 100; x++) {
                    blocks[y][x].x = x * blockSize;
                    blocks[y][x].y = y * blockSize;
                    blocks[y][x].size = blockSize;
                    blocks[y][x].blockNumber = y * 100 + x;

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

            int xBlock = xPos / 10;
            int yBlock = yPos / 10;

            if (xBlock >= 0 && xBlock < 100 && yBlock >= 0 && yBlock < 100) {
                blocks[yBlock][xBlock].color = (blocks[yBlock][xBlock].color == RGB(000, 000, 000)) ? RGB(255, 255, 255) : RGB(000, 000, 000);
                InvalidateRect(hwnd, NULL, TRUE);
            }

            return 0;
        }
        case WM_KEYDOWN:
            if (wParam == VK_SPACE) {
                empezado = TRUE;
                timerID = SetTimer(hwnd, 1, 200, NULL);
            }
            break;
        case WM_TIMER:
            // Cambiar el color de manera más continua
            blocks[0][0].color = RGB(GetRValue(blocks[0][0].color) ^ 255, GetGValue(blocks[0][0].color) ^ 255, GetBValue(blocks[0][0].color) ^ 255);
            InvalidateRect(hwnd, NULL, TRUE);
            break;

           
           
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
                    blocks[yBlock][xBlock].color = (blocks[yBlock][xBlock].color == RGB(000, 000, 000)) ? RGB(255, 255, 255) : RGB(000, 000, 000);
                    InvalidateRect(hwnd, NULL, TRUE);
                }
            }

            return 0;
        }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int main() {
    WNDCLASSW wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"VentanaClase";

    if (!RegisterClassW(&wc)) {
        MessageBoxW(NULL, L"Error al registrar la clase de la ventana", L"Error", MB_ICONERROR);
        return 1;
    }

    HWND hwnd = CreateWindowExW(
        0,
        L"VentanaClase",
        L"Ventana en Blanco y Negro",
        WS_OVERLAPPEDWINDOW,
        100,
        100,
        1000,
        1000,
        NULL,
        NULL,
        GetModuleHandle(NULL),
        NULL
    );

    if (!hwnd) {
        MessageBoxW(NULL, L"Error al crear la ventana", L"Error", MB_ICONERROR);
        return 1;
    }

    ShowWindow(hwnd, SW_SHOWNORMAL);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnregisterClassW(L"VentanaClase", GetModuleHandle(NULL));

    return 0;
}
