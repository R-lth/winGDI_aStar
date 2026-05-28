#include "wndBase.h"
#include <iostream>

namespace monkeyEngine
{
	LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) // 전역 함수 초기화
	{
		switch (msg)
		{
		case WM_SIZE:
		{
			IWndBase* pWnd = (IWndBase*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			if (pWnd) { pWnd->OnResize(LOWORD(lparam), HIWORD(lparam)); }
			break;
		}
		case WM_CLOSE:
		{
			IWndBase* pWnd = (IWndBase*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			if (pWnd) { pWnd->OnClose(); }
			PostQuitMessage(0);
			break;
		}
		default:
			return::DefWindowProc(hwnd, msg, wparam, lparam);
		}
		return NULL;
	}

	bool IWndBase::Create(const wchar_t* className, const wchar_t* windowName, int width, int height)
	{
		WNDCLASSEX wc = {};
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.lpszClassName = className;
		wc.lpfnWndProc = wndProc; // (중요) 윈도우 프로시저(함수)의 포인터 등록

		ATOM classId = 0;
		if (!GetClassInfoEx(HINSTANCE(), className, &wc))
		{
			classId = RegisterClassEx(&wc);

			if (0 == classId) return false;
		}

		m_width = width;
		m_height = height;

		RECT rc = { 0, 0, width, height };
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);

		//[CHECK] AdjustWindowRect()의 의미는?
		m_hWnd = CreateWindowEx(NULL, MAKEINTATOM(classId), L"", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
			rc.right - rc.left, rc.bottom - rc.top, HWND(), HMENU(), HINSTANCE(), NULL);

		if (NULL == m_hWnd) return false;

		::SetWindowText((HWND)m_hWnd, windowName);

		//[CHECK] #5. SetWindowLongPtr()의 의미는?
		SetWindowLongPtr((HWND)m_hWnd, GWLP_USERDATA, (LONG_PTR)this);

		ShowWindow((HWND)m_hWnd, SW_SHOW);
		UpdateWindow((HWND)m_hWnd);

		return true;
	}

	void IWndBase::Destroy()
	{
		if (NULL != m_hWnd)
		{
			DestroyWindow((HWND)m_hWnd);
			m_hWnd = NULL;
		}
	}

	void IWndBase::OnResize(int width, int height)
	{
		m_width = width;
		m_height = height;
	}
}