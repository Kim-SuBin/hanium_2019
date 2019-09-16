#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <time.h>
#include "resource.h"

//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndEmpty(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndScroll(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK EnumWindowMinimize(HWND hwnd, LPARAM lParam);

BOOL CALLBACK EnumWindowRestore(HWND hwnd, LPARAM lParam);

BOOL CALLBACK EnumWindowCapture(HWND hwnd, LPARAM lParam);

HBITMAP WindowCapture(HWND hwnd);

HBITMAP ScreenCapture(HWND hwnd);

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
	WndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;

	RegisterClass(&WndClass);

	WndClass.lpfnWndProc = WndEmpty;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WndClass.lpszClassName = _T("DisplayWindow");

	RegisterClass(&WndClass);

	WndClass.lpfnWndProc = WndScroll;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszClassName = _T("ScrollWindow");

	RegisterClass(&WndClass);

	hwnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
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

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;

	switch (iMsg) {
	case WM_CREATE:
		CreateWindow(_T("button"), _T("MAX"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			20, 20, 100, 30,
			hwnd, (HMENU)01, hInst, NULL);
		CreateWindow(_T("button"), _T("RESTORE"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			20, 60, 100, 30,
			hwnd, (HMENU)02, hInst, NULL);
		CreateWindow(_T("button"), _T("MIN"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			20, 100, 100, 30,
			hwnd, (HMENU)03, hInst, NULL);
		CreateWindow(_T("button"), _T("MIN ALL"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			20, 140, 100, 30,
			hwnd, (HMENU)04, hInst, NULL);
		CreateWindow(_T("button"), _T("RESTORE ALL"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			20, 180, 100, 30,
			hwnd, (HMENU)05, hInst, NULL);
		CreateWindow(_T("button"), _T("CAPTURE ALL"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			20, 220, 100, 30,
			hwnd, (HMENU)06, hInst, NULL);
		CreateWindow(_T("button"), _T("SCROLL TEST"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			20, 260, 100, 30,
			hwnd, (HMENU)07, hInst, NULL);
		CreateWindow(_T("button"), _T("CLOSE"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			20, 300, 100, 30,
			hwnd, (HMENU)10, hInst, NULL);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case 01:
			// ShowWindow(hwnd, SW_SHOWMAXIMIZED);
			SendMessage(hwnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
			break;
		case 02:
			// ShowWindow(hwnd, SW_RESTORE);
			SendMessage(hwnd, WM_SYSCOMMAND, SC_RESTORE, 0);
			break;
		case 03:
			//ShowWindow(hwnd, SW_SHOWMINIMIZED);
			SendMessage(hwnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
			break;
		case 04:
			// �����츦 ã�� ������ EnumWindowsProc ȣ��
			EnumWindows(EnumWindowMinimize, NULL);
			break;
		case 05:
			// �����츦 ã�� ������ EnumWindowsProc ȣ��
			EnumWindows(EnumWindowRestore, NULL);
			break;
		case 06:
			// �����츦 ã�� ������ EnumWindowsProc ȣ��
			EnumWindows(EnumWindowCapture, NULL);
			break;
		case 07:
			CreateWindow("ScrollWindow", "Scroll Test", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
				CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
				NULL, (HMENU)NULL, hInst, NULL);
			break;
		case 10:
			// pid = GetCurrentProcessId();
			// Process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
			// CreateRemoteThread(hProcess, 0, 0, 0, ExitProcess, 0, 0);
			SendMessage(hwnd, WM_SYSCOMMAND, SC_CLOSE, 0);
			break;
		}
	case WM_SIZE:
		switch (wParam) {
		case SIZE_MAXIMIZED:
			SetWindowText(hwnd, "Ŀ���� !");
			break;
		case SIZE_MINIMIZED:
			SetWindowText(hwnd, "�۾����� !");
			break;
		case SIZE_RESTORED:
			SetWindowText(hwnd, lpszClass);
			break;
		}
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

HBITMAP hBitmap = NULL;

LRESULT CALLBACK WndEmpty(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	HDC hdc, memdc;
	BITMAP bmp;
	HBITMAP hOld;
	PAINTSTRUCT ps;

	switch (iMsg) {
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		if (hBitmap != NULL) {
			memdc = CreateCompatibleDC(hdc);
			hOld = (HBITMAP)SelectObject(memdc, hBitmap);
			GetObject(hBitmap, sizeof(BITMAP), &bmp);
			BitBlt(hdc, 0, 0, bmp.bmWidth, bmp.bmHeight, memdc, 0, 0, SRCCOPY);
			SelectObject(memdc, hOld);
			//DeleteObject(memdc);
			DeleteDC(memdc);
		}
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		if (hBitmap != NULL)
			DeleteObject(hBitmap);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

HBITMAP hBimo = NULL;
HBITMAP hOldBimo = NULL;

RECT crt, rt;

int inc;
int pos; // �������� ���� ���� ��� ��ũ�� �̺�Ʈ �߻�X
int max; // �������� ���� ���� ��� â�� ���� �����ϰ� ��� �Ʒ��� ������

LRESULT CALLBACK WndScroll(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	HDC hdc, memdc;
	PAINTSTRUCT ps;
	BITMAP bmp;

	int lines;
	TCHAR str[128];

	switch (iMsg) {
	case WM_CREATE:
		hBimo = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		break;
	case WM_SIZE:
		max = 20 * 100 - HIWORD(lParam); // â�� �ϴܿ� ���缭 ��ũ�� �� ����(max = 20*100)
		SetScrollRange(hwnd, SB_VERT, 0, max, TRUE); // ��ũ�� ���� ����
		SetScrollPos(hwnd, SB_VERT, 0, TRUE); // ��ũ�� �ʱⰪ ����
		break;
	case WM_LBUTTONDOWN:
		InvalidateRect(hwnd, &rt, TRUE);
		UpdateWindow(hwnd);
		break;
	case WM_MOUSEWHEEL:
		SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &lines, 0);
		for (int i = 0; i < lines; i++) {
			if ((SHORT)HIWORD(wParam) > 0) // ���� ������ ���ư� ��
				inc = -lines * 10; // ���� �ѹ� �ø� �� 10�� ��ŭ inc ����
			//MessageBox(hScroll, _T("���콺 �ø�"), _T("Wheel Test"), NULL);
			else if ((SHORT)HIWORD(wParam) < 0)
				inc = lines * 20; // ���� �ѹ� ���� �� 10�� ��ŭ inc ����
			//MessageBox(hScroll, _T("���콺 ����"), _T("Wheel Test"), NULL);
		}

		// ���� ������ ������ �ʰԲ� ������
		if (pos + inc < 0)
			inc = -pos;
		if (pos + inc > max)
			inc = max - pos;
		pos += inc; // ���� ��ġ�� inc ��ŭ ����

		GetClientRect(hwnd, &crt); // ���� �������� RECT ����ü ����
		SetRect(&rt, 100, 100, crt.right, crt.bottom); // RECT ����ü�� width, height�� ���� ����
		ScrollWindow(hwnd, 0, -inc, &rt, &rt); // SetRect�� ������ ������ �����ϰ� ��ũ�� �� �� �ְ� ��
		SetScrollPos(hwnd, SB_VERT, pos, TRUE); // ���� ��ġ�� pos�� ��ġ�� ���缭 ����
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		memdc = CreateCompatibleDC(hdc);
		hOldBimo = (HBITMAP)SelectObject(memdc, hBimo);
		GetObject(hBimo, sizeof(BITMAP), &bmp);

		// pos�� ���� ��ġ�� ���� �̹����� �׷���(��ũ�ѽ� ȭ���� �ö󰡰ų� �������� ��ó�� ���̴� ȿ�� �߻�)
		BitBlt(hdc, 0, -pos, bmp.bmWidth, bmp.bmHeight, memdc, 0, 0, SRCCOPY); 
		SelectObject(memdc, hOldBimo);
		DeleteDC(memdc);
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		if (hBitmap != NULL)
			DeleteObject(hBitmap);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

BOOL CALLBACK EnumWindowMinimize(HWND hwnd, LPARAM lParam) {
	char caption[255];
	GetWindowText(hwnd, caption, sizeof(caption));
	if (IsWindowVisible(hwnd) && strcmp(caption, _T("MainWindow"))) { // ã�� �����찡 ȭ�鿡 ���̴� ��������
		SendMessage(hwnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
	}
	return TRUE; // ��� �����Ϸ��� TRUE�� ��ȯ�ؾ� �Ѵ�
}

BOOL CALLBACK EnumWindowRestore(HWND hwnd, LPARAM lParam) {
	if (IsWindowVisible(hwnd)) { // ã�� �����찡 ȭ�鿡 ���̴� ��������
		SendMessage(hwnd, WM_SYSCOMMAND, SC_RESTORE, 0);
	}
	return TRUE; // ��� �����Ϸ��� TRUE�� ��ȯ�ؾ� �Ѵ�
}

HWND g_hwnd;

RECT rct;

BOOL CALLBACK EnumWindowCapture(HWND hwnd, LPARAM lParam) {
	HWND l_hwnd;
	char caption[255];
	int len = GetWindowTextLength(hwnd);

	GetWindowText(hwnd, caption, sizeof(caption));

	if (IsWindowVisible(hwnd) && (len > 0) && (!IsIconic(hwnd)) && strcmp(caption, _T("MainWindow"))
		&& strcmp(caption, _T("NVIDIA GeForce Overlay")) && strcmp(caption, _T("Microsoft Store"))
		&& strcmp(caption, _T("Program Manager"))) {
		//SetWindowPos(hwnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_HIDEWINDOW);
		g_hwnd = FindWindow(NULL, _T(caption));
		GetClientRect(g_hwnd, &rct); // ���� �������� ��ǥ���� �޾ƿ´�
		l_hwnd = CreateWindow("DisplayWindow", caption, WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			CW_USEDEFAULT, CW_USEDEFAULT, rct.right, rct.bottom,
			NULL, (HMENU)NULL, hInst, NULL);
		hBitmap = WindowCapture(g_hwnd);
		//SetWindowPos(g_hwnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_HIDEWINDOW);
		//SendMessage(g_hwnd, WM_SYSCOMMAND, SC_MINIMIZE, NULL);
		//ShowWindow(g_hwnd, SW_HIDE);
		//InvalidateRect(g_hwnd, NULL, FALSE);
		//UpdateWindow(l_hwnd);
		Sleep(100);
	}
	return TRUE; // ��� �����Ϸ��� TRUE�� ��ȯ�ؾ� �Ѵ�
}

HBITMAP WindowCapture(HWND hwnd) {
	HDC hScrdc = GetDC(hwnd);
	HDC hMemdc = CreateCompatibleDC(hScrdc); // hdc�� ȣȯ�Ǵ� memdc ����
	HBITMAP hBit = NULL;
	HBITMAP hOld = NULL;

	// ��Ʈ���� ���� ������ �����
	if (hBit) DeleteObject(hBit);

	// hdc�� ȣȯ�Ǵ� ��Ʈ�� hBit ����
	hBit = CreateCompatibleBitmap(hScrdc, rct.right, rct.bottom);
	hOld = (HBITMAP)SelectObject(hMemdc, hBit); // memdc�� hBit ������ �׸��� �׸��� ���ؼ� ���

	BitBlt(hMemdc, 0, 0, rct.right, rct.bottom, hScrdc, 0, 0, SRCCOPY);
	//SetStretchBltMode(hScrdc, COLORONCOLOR);
	//StretchBlt(hMemdc, 0, 0, rct.right-rct.left, rct.bottom-rct.top, hScrdc, 0, 0, rct.right - rct.left, rct.bottom - rct.top, SRCCOPY);
	//PrintWindow(hwnd, hMemdc, PW_CLIENTONLY);

	SelectObject(hMemdc, hOld);
	ReleaseDC(hwnd, hScrdc);
	DeleteDC(hMemdc);

	return hBit;
}

HBITMAP ScreenCapture(HWND hwnd) {
	int ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	int ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	HDC hScrdc = CreateDC("DISPLAY", NULL, NULL, NULL);
	HDC hMemdc = CreateCompatibleDC(hScrdc);
	HBITMAP hBit = CreateCompatibleBitmap(hScrdc, ScreenWidth, ScreenHeight);
	HBITMAP hOld = (HBITMAP)SelectObject(hMemdc, hBit);

	BitBlt(hMemdc, 0, 0, ScreenWidth, ScreenHeight, hScrdc, 0, 0, SRCCOPY);

	SelectObject(hMemdc, hOld);
	ReleaseDC(hwnd, hScrdc);
	DeleteDC(hMemdc);

	return hBit;
}