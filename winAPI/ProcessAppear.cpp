#include <stdio.h>
#include <tchar.h>
#include <windows.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndEmpty(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK EnumWindowMinimize(HWND hwnd, LPARAM lParam);

BOOL CALLBACK EnumWindowRestore(HWND hwnd, LPARAM lParam);

BOOL CALLBACK EnumWindowCapture(HWND hwnd, LPARAM lParam);

HBITMAP WindowCapture(HWND hwnd);

LPCTSTR lpszClass = _T("MainWindow");

HINSTANCE hInst;

HWND g_hwnd;

HBITMAP hBit = NULL;

RECT rct;

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
	PAINTSTRUCT ps;
	//HANDLE hProcess;
	//DWORD pid;

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
		CreateWindow(_T("button"), _T("EMPTY"),
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
			//PostMessage(hwnd, WM_SHOWWINDOW, FALSE, SW_OTHERUNZOOM);
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
			// ĸ�Ǹ��� �Է��ϸ� �׿� ���� ������ ȣ��
			//g_hwnd = FindWindow(NULL, _T("WinAPI - Microsoft Visual Studio"));
			//GetClientRect(g_hwnd, &rct); // ���� �������� ��ǥ���� �޾ƿ´�
			//CreateWindow("DisplayWindow", "DisplayWindow", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			//	CW_USEDEFAULT, CW_USEDEFAULT, rct.right - rct.left, rct.bottom - rct.top,
			//	NULL, (HMENU)NULL, hInst, NULL);
			//hBit = WindowCapture(g_hwnd);
			break;
		case 10:
			// pid = GetCurrentProcessId();
			// Process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
			// CreateRemoteThread(hProcess, 0, 0, 0, ExitProcess, 0, 0);
			// �θ� �ƴ� �������� ���迩�� �ݱ�� ����
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
		BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

LRESULT CALLBACK WndEmpty(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	HDC hdc, memdc;
	BITMAP bmp;
	HBITMAP hOld;
	PAINTSTRUCT ps;

	InvalidateRect(hwnd, NULL, TRUE);
	UpdateWindow(hwnd);

	switch (iMsg) {
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		if (hBit != NULL) {
			memdc = CreateCompatibleDC(hdc);
			hOld = (HBITMAP)SelectObject(memdc, hBit);
			GetObject(hBit, sizeof(BITMAP), &bmp);
			BitBlt(hdc, 0, 0, bmp.bmWidth, bmp.bmHeight, memdc, 0, 0, SRCCOPY);
			SelectObject(memdc, hOld);
			ReleaseDC(hwnd, hdc);
			DeleteDC(memdc);
		}
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		if (hBit != NULL)
			DeleteObject(hBit);
		PostQuitMessage(0);
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

BOOL CALLBACK EnumWindowMinimize(HWND hwnd, LPARAM lParam) {
	char caption[255];
	GetWindowText(hwnd, caption, sizeof(caption));
	if (IsWindowVisible(hwnd) && strcmp(caption, _T("MainWindow"))) { // ã�� �����찡 ȭ�鿡 ���̴� ��������
		//ShowWindow(hwnd, SW_SHOWMINIMIZED);
		//PostMessage(hwnd, WM_SHOWWINDOW, FALSE, SW_OTHERUNZOOM);
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

BOOL CALLBACK EnumWindowCapture(HWND hwnd, LPARAM lParam) {
	char caption[255];
	int len = GetWindowTextLength(hwnd);
	
	GetWindowText(hwnd, caption, sizeof(caption));

	if (IsWindowVisible(hwnd) && (len > 0) && (!IsIconic(hwnd)) && strcmp(caption, _T("Program Manager"))
		&& strcmp(caption, _T("NVIDIA GeForce Overlay")) && strcmp(caption, _T("MainWindow"))) {
		SetWindowPos(hwnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
		g_hwnd = FindWindow(NULL, _T(caption));
		GetClientRect(g_hwnd, &rct); // ���� �������� ��ǥ���� �޾ƿ´�
		CreateWindow("DisplayWindow", caption, WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			CW_USEDEFAULT, CW_USEDEFAULT, rct.right - rct.left, rct.bottom - rct.top,
			NULL, (HMENU)NULL, hInst, NULL);
		hBit = WindowCapture(g_hwnd);
		SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
		SendMessage(hwnd, WM_SYSCOMMAND, SC_MINIMIZE, NULL);
	}
	return TRUE; // ��� �����Ϸ��� TRUE�� ��ȯ�ؾ� �Ѵ�
}

HBITMAP WindowCapture(HWND hwnd) {
	HDC hScrdc = GetDC(hwnd);
	HDC hMemdc = CreateCompatibleDC(hScrdc); // hdc�� ȣȯ�Ǵ� memdc ����

	// hdc�� ȣȯ�Ǵ� ��Ʈ�� hBit ����

	HBITMAP hBitmap = CreateCompatibleBitmap(hScrdc, rct.right, rct.bottom);
	HBITMAP hOldmap = (HBITMAP)SelectObject(hMemdc, hBitmap); // memdc�� hBit ������ �׸��� �׸��� ���ؼ� ���

	BitBlt(hMemdc, 0, 0, rct.right - rct.left, rct.bottom - rct.top, hScrdc, 0, 0, SRCCOPY);
	//SetStretchBltMode(hScrdc, COLORONCOLOR);
	//StretchBlt(hMemdc, 0, 0, rct.right-rct.left, rct.bottom-rct.top, hScrdc, 0, 0, rct.right - rct.left, rct.bottom - rct.top, SRCCOPY);
	//PrintWindow(hwnd, hMemdc, PW_CLIENTONLY);

	SelectObject(hMemdc, hOldmap);
	ReleaseDC(hwnd, hScrdc);
	DeleteDC(hMemdc);
	
	return hBitmap;
}

/*
void FindPID(void) {
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE) {
		printf("Invalid Snapshot Handle\n");
		exit(EXIT_FAILURE);
	}
	PROCESSENTRY32 PE32; // ���μ����� ���� ������ ��� ����
	PE32.dwSize = sizeof(PE32); // PE32�� ����ü ũ�� ����
	if (Process32First(hSnapshot, &PE32)) {
		do {
			printf("%s, [%d]\n", PE32.szExeFile, PE32.th32ProcessID); // name of process, PID
		} while (Process32Next(hSnapshot, &PE32));
	}
	CloseHandle(hSnapshot);
	return;
}
*/