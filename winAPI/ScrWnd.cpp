#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <time.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

LPCTSTR lpszClass = _T("MainWindow");

HINSTANCE hInst;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow) {
	HWND hwnd;
	MSG msg;
	WNDCLASS WndClass;
	hInst = hInstance;

	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;

	RegisterClass(&WndClass);

	hwnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW | WS_VSCROLL,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

RECT crt, rt;

int inc = 0;
int pos = 0; // �������� ���� ���� ��� ��ũ�� �̺�Ʈ �߻�X
int max = 20 * 100; // �������� ���� ���� ��� â�� ���� �����ϰ� ��� �Ʒ��� ������

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	TCHAR str[128];
	HDC hdc;
	PAINTSTRUCT ps;

	int lines;

	switch (iMsg) {
	case WM_CREATE: // ��ũ���� ���� �� �ʱⰪ ������
		CreateWindow("button", "��ư1", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			20, 20, 100, 25, hwnd, (HMENU)0, hInst, NULL);
		CreateWindow("button", "��ư2", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			20, 50, 100, 25, hwnd, (HMENU)0, hInst, NULL);
		//SetScrollRange(hwnd, SB_VERT, 0, ymax, TRUE);
		//SetScrollPos(hwnd, SB_VERT, 0, TRUE);
		break;
	case WM_SIZE:
		max = 20*100 - HIWORD(lParam); // â�� �ϴܿ� ���缭 ��ũ�� �� ����
		SetScrollRange(hwnd, SB_VERT, 0, max, TRUE); // ��ũ�� ���� ����
		SetScrollPos(hwnd, SB_VERT, 0, TRUE); // ��ũ�� �ʱⰪ ����
		break;
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_UP:
			SendMessage(hwnd, WM_VSCROLL, MAKELONG(SB_PAGEUP, 0), 0);
			break;
		case VK_DOWN:
			SendMessage(hwnd, WM_VSCROLL, MAKELONG(SB_PAGEDOWN, 0), 0);
			break;
		}
	case WM_MOUSEWHEEL:
		SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &lines, 0);
		for (int i = 0; i < lines; i++) {
			if ((SHORT)HIWORD(wParam) > 0) // ���� ������ ���ư� ��
				inc = -lines * 20;
				//MessageBox(hwnd, _T("���콺 �ø�"), _T("Wheel Test"), NULL);
			else if ((SHORT)HIWORD(wParam) < 0) // ���� �ڷ� ���ư� ��
				inc = lines * 20;
				//MessageBox(hwnd, _T("���콺 ����"), _T("Wheel Test"), NULL);
		}

		if (pos + inc < 0)
			inc = -pos;
		if (pos + inc > max)
			inc = max - pos;
		pos = pos + inc;

		GetClientRect(hwnd, &crt);
		SetRect(&rt, 150, 100, crt.right, crt.bottom);
		ScrollWindow(hwnd, 0, -inc, &rt, &rt);
		SetScrollPos(hwnd, SB_VERT, pos, TRUE); // ǥ�� ��ũ�ѹ�
		//SetScrollPos(hScroll, SB_CTL, scroll, TRUE); // ��ũ�ѹ� ��Ʈ��
		break;
	case WM_VSCROLL:
		inc = 0;
		switch (LOWORD(wParam)) {
		case SB_LINEUP:
			inc = -20;
			break;
		case SB_LINEDOWN:
			inc = 20;
			break;
		case SB_PAGEUP:
			inc = -200;
			break;
		case SB_PAGEDOWN:
			inc = 200;
			break;
		case SB_THUMBTRACK:
			inc = HIWORD(wParam) - pos;
			break;
		}
		
		if (pos + inc < 0)
			inc = -pos;
		if (pos + inc > max)
			inc = max - pos;
		pos = pos + inc;

		GetClientRect(hwnd, &crt);
		SetRect(&rt, 150, 100, crt.right, crt.bottom);
		ScrollWindow(hwnd, 0, -inc, &rt, &rt);
		SetScrollPos(hwnd, SB_VERT, pos, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		for (int i = 0; i < 100; i++) {
			wsprintf(str, _T("Line Number : %d"), i);
			TextOut(hdc, 150, i * 20 - pos, str, lstrlen(str));
		}
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}