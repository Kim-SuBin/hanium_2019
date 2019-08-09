#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <TlHelp32.h>
#include <string>

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK EnumWindowMIN(HWND hwnd, LPARAM lParam);

BOOL CALLBACK EnumWindowRES(HWND hwnd, LPARAM lParam);

LPCTSTR lpszClass = _T("Process Appear");

HINSTANCE hInst;

// ��ư1 ���� ���μ������� �������� �� PID �������
// ��ư2 ������ PID�� ���μ������� ������ �ڵ�(HWND) �������
// ��ư3 ������ HWND�� ��� ������ ȭ�鿡 ����

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
	WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;

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
	HANDLE hProcess;
	DWORD pid;

	switch (iMsg) {
	case WM_CREATE:
		CreateWindow(_T("button"), _T("MAX"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			20, 20, 100, 30,
			hwnd, (HMENU)0101, hInst, NULL);
		CreateWindow(_T("button"), _T("RESTORE"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			20, 60, 100, 30,
			hwnd, (HMENU)0102, hInst, NULL);
		CreateWindow(_T("button"), _T("MIN"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			20, 100, 100, 30,
			hwnd, (HMENU)0103, hInst, NULL);
		CreateWindow(_T("button"), _T("MIN ALL"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			20, 140, 100, 30,
			hwnd, (HMENU)0104, hInst, NULL);
		CreateWindow(_T("button"), _T("RESTORE ALL"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			20, 180, 100, 30,
			hwnd, (HMENU)0105, hInst, NULL);
		CreateWindow(_T("button"), _T("CLOSE"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			20, 220, 100, 30,
			hwnd, (HMENU)0106, hInst, NULL);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case 0101:
			//ShowWindow(hwnd, SW_SHOWMAXIMIZED);
			SendMessage(hwnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
			break;
		case 0102:
			SendMessage(hwnd, WM_SYSCOMMAND, SC_RESTORE, 0);
			break;
		case 0103:
			SendMessage(hwnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
			break;
		case 0104:
			// �����츦 ã�� ������ EnumWindowsProc ȣ��
			EnumWindows(EnumWindowMIN, NULL);
			break;
		case 0105:
			// �����츦 ã�� ������ EnumWindowsProc ȣ��
			EnumWindows(EnumWindowRES, NULL);
			break;
		case 0106:
			//pid = GetCurrentProcessId();
			//hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
			//CreateRemoteThread(hProcess, 0, 0, 0, ExitProcess, 0, 0);
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

BOOL CALLBACK EnumWindowMIN(HWND hwnd, LPARAM lParam) {
	if (IsWindowVisible(hwnd)) { // ã�� �����찡 ȭ�鿡 ���̴� ��������
		SendMessage(hwnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
	}
	return TRUE; // ��� �����Ϸ��� TRUE�� ��ȯ�ؾ� �Ѵ�
}

BOOL CALLBACK EnumWindowRES(HWND hwnd, LPARAM lParam) {
	if (IsWindowVisible(hwnd)) { // ã�� �����찡 ȭ�鿡 ���̴� ��������
		SendMessage(hwnd, WM_SYSCOMMAND, SC_RESTORE, 0);
	}
	return TRUE; // ��� �����Ϸ��� TRUE�� ��ȯ�ؾ� �Ѵ�
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