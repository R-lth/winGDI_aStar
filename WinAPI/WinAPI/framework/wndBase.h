#pragma once

#include "pch.h"
#include "inc_windows.h"
#include "resource.h"

LRESULT CALLBACK wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); // 주 창의 메시지를 처리합니다.

namespace monkeyEngine 
{
	class IWndBase
	{
	public:
		IWndBase() = default;
		virtual ~IWndBase() = default;

		// 복사 생성자, 대입 불가 -> 자식한테 주기
		// WndBase(const WndBase&) = delete;
		// WndBase& operator=(const WndBase&) = delete;
		// 이동 생성자, 대입 불가
		// WndBase(WndBase&&) = delete;
		// WndBase& operator=(WndBase&&) = delete;

	protected: // __super 키워드 용
		bool Create(const wchar_t* className, const wchar_t* windowName, int width, int height);
		void Destroy();

		void* GetHandle() const { return m_hWnd; }

		int GetWidth() const { return m_width; }
		int GetHeight() const { return m_height; }

	protected:
		// 메세지 프로시저
		virtual void OnResize(int width, int height);
		virtual void OnClose() = 0; // 객체 생성 불가

	protected:
		friend LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	protected:
		HWND m_hWnd = HWND();
		int m_width = 0;
		int m_height = 0;
	};
}



