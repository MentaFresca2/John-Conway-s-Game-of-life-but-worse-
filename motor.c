#include <windows.h>
#include <windowsx.h>

typedef struct {
    int x;
    int y;
    int size;
    int blockNumber;
    COLORREF color;
    COLORREF color2;
} Block;

// Matriz de blocks
static Block blocks[100][100];

BOOL empezado = FALSE;
BOOL isMouseDown = FALSE;
UINT_PTR timerID;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    

    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            int blockSize = 10;

            HBRUSH hBackgroundBrush = CreateSolidBrush(RGB(0, 0, 0));
            FillRect(hdc, &ps.rcPaint, hBackgroundBrush);
            DeleteObject(hBackgroundBrush);

            for (int y = 0; y < 100; y++) {
                for (int x = 0; x < 100; x++) {
                    if(empezado){
                        if (blocks[y][x].color == RGB(0, 0, 0)){
                            int vivos = 0;
                            if(blocks[y - 1][x].color == RGB(255, 255, 255)){
                            vivos++;
                            }
                            if(blocks[y + 1][x].color == RGB(255, 255, 255)){
                            vivos++;
                            }
                            if(blocks[y][x - 1].color == RGB(255, 255, 255)){
                            vivos++;
                            }
                            if(blocks[y][x + 1].color == RGB(255, 255, 255)){
                            vivos++;
                            }
                            if(blocks[y + 1][x + 1].color == RGB(255, 255, 255)){
                            vivos++;
                            }
                            if(blocks[y - 1][x - 1].color == RGB(255, 255, 255)){
                            vivos++;
                            }
                            if(blocks[y + 1][x - 1].color == RGB(255, 255, 255)){
                            vivos++;
                            }
                            if(blocks[y - 1][x + 1].color == RGB(255, 255, 255)){
                            vivos++;
                            }
                            if(vivos == 3){
                                blocks[y][x].color2 = RGB(255, 255, 255);
                            }else{
                                blocks[y][x].color2 = RGB(0, 0, 0);
                                }
                        }else{
                            int vivos = 0;
                            if(blocks[y - 1][x].color == RGB(255, 255, 255)){
                            vivos++;
                            }
                            if(blocks[y + 1][x].color == RGB(255, 255, 255)){
                            vivos++;
                            }
                            if(blocks[y][x - 1].color == RGB(255, 255, 255)){
                            vivos++;
                            }
                            if(blocks[y][x + 1].color == RGB(255, 255, 255)){
                            vivos++;
                            }
                            if(blocks[y + 1][x + 1].color == RGB(255, 255, 255)){
                            vivos++;
                            }
                            if(blocks[y - 1][x - 1].color == RGB(255, 255, 255)){
                            vivos++;
                            }
                            if(blocks[y + 1][x - 1].color == RGB(255, 255, 255)){
                            vivos++;
                            }
                            if(blocks[y - 1][x + 1].color == RGB(255, 255, 255)){
                            vivos++;
                            }
                            if(vivos == 3){
                                blocks[y][x].color2 = RGB(255, 255, 255);
                            }
                            if(vivos == 2){
                                blocks[y][x].color2 = RGB(255, 255, 255);
                            }
                            if(vivos >= 4){
                                blocks[y][x].color2 = RGB(0, 0, 0);
                            }
                            if(vivos <= 1){
                                blocks[y][x].color2 = RGB(0, 0, 0);
                            }
                            }
                        
                 
                              

                    }
                    if(!empezado){
      
                        HBRUSH hBrush = CreateSolidBrush(blocks[y][x].color);
                        SelectObject(hdc, hBrush);
                        SetBkMode(hdc, TRANSPARENT);

                        RECT rect = { blocks[y][x].x, blocks[y][x].y, blocks[y][x].x + blockSize, blocks[y][x].y + blockSize };

                        // Convierte el RECT a un objeto Rectangle
                        Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);

                        // Define un nuevo RECT para el borde
                        RECT borderRect = { rect.left - 1, rect.top - 1, rect.right + 1, rect.bottom + 1 };

                        // Dibuja el borde gris
                        FrameRect(hdc, &borderRect, GetSysColorBrush(COLOR_GRAYTEXT));

                        FillRect(hdc, &rect, hBrush);
                        DeleteObject(hBrush);
                    }
                }
            }
        if(empezado){
            for (int y = 0; y < 100; y++) {
                for (int x = 0; x < 100; x++) {
                
                blocks[y][x].color = blocks[y][x].color2;
                HBRUSH hBrush = CreateSolidBrush(blocks[y][x].color2);
                SelectObject(hdc, hBrush);
                SetBkMode(hdc, TRANSPARENT);
                RECT rect = { blocks[y][x].x, blocks[y][x].y, blocks[y][x].x + blockSize, blocks[y][x].y + blockSize };
                FillRect(hdc, &rect, hBrush);
                DeleteObject(hBrush);



                    
                }
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
            if (wParam == VK_RETURN) {
                empezado = TRUE;
                timerID = SetTimer(hwnd, 1, 200, NULL);
            }
            if (wParam == VK_BACK) {
                 empezado = FALSE;
                 KillTimer(hwnd, timerID);
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
    int blockSize = 10;

    for (int y = 0; y < 100; y++) {
                for (int x = 0; x < 100; x++) {
                    blocks[y][x].x = x * blockSize;
                    blocks[y][x].y = y * blockSize;
                    blocks[y][x].size = blockSize;
                    blocks[y][x].blockNumber = y * 100 + x;
                }
            }

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
