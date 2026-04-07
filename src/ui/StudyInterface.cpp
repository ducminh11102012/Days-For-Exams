#include "dfe/StudyInterface.hpp"

#include <algorithm>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#endif

namespace dfe {

#ifdef _WIN32
namespace {

struct UiState {
  UserProfile profile;
  std::vector<StudyBlock> plan;
};

std::wstring toWide(const std::string& text) {
  if (text.empty()) {
    return L"";
  }

  const int size = MultiByteToWideChar(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), nullptr, 0);
  std::wstring result(size, L'\0');
  MultiByteToWideChar(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), result.data(), size);
  return result;
}

std::wstring formatNowTime() {
  std::time_t now = std::time(nullptr);
  std::tm localTime{};
  localtime_s(&localTime, &now);
  std::wostringstream out;
  out << std::setfill(L'0') << std::setw(2) << localTime.tm_hour << L":" << std::setw(2)
      << localTime.tm_min << L":" << std::setw(2) << localTime.tm_sec;
  return out.str();
}

LRESULT CALLBACK DashboardProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  auto* state = reinterpret_cast<UiState*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

  switch (msg) {
    case WM_CREATE: {
      auto* create = reinterpret_cast<CREATESTRUCT*>(lParam);
      SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(create->lpCreateParams));
      SetTimer(hwnd, 1, 1000, nullptr);
      return 0;
    }

    case WM_TIMER:
      InvalidateRect(hwnd, nullptr, FALSE);
      return 0;

    case WM_KEYDOWN:
      if (wParam == VK_ESCAPE) {
        DestroyWindow(hwnd);
      }
      return 0;

    case WM_DESTROY:
      KillTimer(hwnd, 1);
      PostQuitMessage(0);
      return 0;

    case WM_PAINT: {
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(hwnd, &ps);

      RECT client{};
      GetClientRect(hwnd, &client);

      const HBRUSH bgBrush = CreateSolidBrush(RGB(18, 22, 34));
      FillRect(hdc, &client, bgBrush);
      DeleteObject(bgBrush);

      RECT header{0, 0, client.right, 92};
      const HBRUSH headerBrush = CreateSolidBrush(RGB(32, 40, 62));
      FillRect(hdc, &header, headerBrush);
      DeleteObject(headerBrush);

      SetBkMode(hdc, TRANSPARENT);
      SetTextColor(hdc, RGB(240, 244, 255));

      HFONT titleFont = CreateFontW(38, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                                    OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                                    VARIABLE_PITCH, L"Segoe UI");
      HFONT normalFont = CreateFontW(21, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                                     OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                                     VARIABLE_PITCH, L"Segoe UI");
      HFONT sectionFont = CreateFontW(25, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                                      OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                                      VARIABLE_PITCH, L"Segoe UI");

      SelectObject(hdc, titleFont);
      TextOutW(hdc, 28, 18, L"DFE Focus Mode", 14);

      std::wstring clock = L"Now: " + formatNowTime();
      SelectObject(hdc, normalFont);
      TextOutW(hdc, client.right - 240, 34, clock.c_str(), static_cast<int>(clock.size()));

      const int leftPaneWidth = 360;
      RECT leftPane{22, 116, leftPaneWidth, client.bottom - 24};
      const HBRUSH leftBrush = CreateSolidBrush(RGB(28, 34, 50));
      FillRect(hdc, &leftPane, leftBrush);
      DeleteObject(leftBrush);

      SetTextColor(hdc, RGB(223, 233, 255));
      SelectObject(hdc, sectionFont);
      TextOutW(hdc, 38, 136, L"Student", 7);

      SelectObject(hdc, normalFont);
      const std::wstring user = state ? toWide(state->profile.userName) : L"Unknown";
      TextOutW(hdc, 38, 172, user.c_str(), static_cast<int>(user.size()));

      const std::wstring exam = state ? (L"Exam: " + toWide(state->profile.examDate)) : L"Exam: -";
      TextOutW(hdc, 38, 212, exam.c_str(), static_cast<int>(exam.size()));

      const std::wstring end = state ? (L"Daily end: " + toWide(state->profile.dailyEndTime)) : L"Daily end: -";
      TextOutW(hdc, 38, 252, end.c_str(), static_cast<int>(end.size()));

      SetTextColor(hdc, RGB(168, 255, 203));
      TextOutW(hdc, 38, 316, L"Allowed apps", 12);

      if (state) {
        SetTextColor(hdc, RGB(220, 227, 247));
        int y = 350;
        for (size_t i = 0; i < state->profile.allowedApps.size() && i < 8; ++i) {
          std::wstring app = L"• " + toWide(state->profile.allowedApps[i]);
          TextOutW(hdc, 42, y, app.c_str(), static_cast<int>(app.size()));
          y += 30;
        }
      }

      RECT rightPane{leftPaneWidth + 16, 116, client.right - 20, client.bottom - 24};
      const HBRUSH rightBrush = CreateSolidBrush(RGB(24, 30, 45));
      FillRect(hdc, &rightPane, rightBrush);
      DeleteObject(rightBrush);

      SetTextColor(hdc, RGB(235, 240, 255));
      SelectObject(hdc, sectionFont);
      TextOutW(hdc, leftPaneWidth + 38, 136, L"Today's Study Roadmap", 21);

      if (state) {
        int y = 184;
        for (size_t i = 0; i < state->plan.size() && i < 8; ++i) {
          RECT card{leftPaneWidth + 36, y, client.right - 44, y + 58};
          const HBRUSH cardBrush = CreateSolidBrush(i % 2 == 0 ? RGB(45, 67, 109) : RGB(54, 78, 122));
          FillRect(hdc, &card, cardBrush);
          DeleteObject(cardBrush);

          const std::wstring row = toWide(state->plan[i].startsAt + " → " + state->plan[i].endsAt +
                                          " | " + state->plan[i].subject + " | " + state->plan[i].task);
          SetTextColor(hdc, RGB(242, 247, 255));
          SelectObject(hdc, normalFont);
          TextOutW(hdc, leftPaneWidth + 48, y + 17, row.c_str(), static_cast<int>(row.size()));
          y += 72;
        }
      }

      SetTextColor(hdc, RGB(150, 158, 185));
      const std::wstring footer = L"Press ESC to Exit Focus UI";
      TextOutW(hdc, client.right - 290, client.bottom - 44, footer.c_str(), static_cast<int>(footer.size()));

      DeleteObject(titleFont);
      DeleteObject(normalFont);
      DeleteObject(sectionFont);
      EndPaint(hwnd, &ps);
      return 0;
    }

    default:
      return DefWindowProc(hwnd, msg, wParam, lParam);
  }
}

}  // namespace
#endif

void StudyInterface::runSession(const UserProfile& profile, const std::vector<StudyBlock>& plan) const {
#ifdef _WIN32
  HINSTANCE instance = GetModuleHandle(nullptr);
  const wchar_t kClassName[] = L"DFEDashboardWindow";

  WNDCLASSEXW wc{};
  wc.cbSize = sizeof(wc);
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc = DashboardProc;
  wc.hInstance = instance;
  wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wc.lpszClassName = kClassName;

  RegisterClassExW(&wc);

  UiState state{profile, plan};

  const int width = GetSystemMetrics(SM_CXSCREEN);
  const int height = GetSystemMetrics(SM_CYSCREEN);

  HWND hwnd = CreateWindowExW(WS_EX_TOPMOST, kClassName, L"Day For Exams", WS_POPUP | WS_VISIBLE, 0, 0, width,
                              height, nullptr, nullptr, instance, &state);

  if (!hwnd) {
    std::cerr << "Cannot initialize DFE dashboard window.\n";
    return;
  }

  ShowWindow(hwnd, SW_SHOWMAXIMIZED);
  UpdateWindow(hwnd);

  MSG msg;
  while (GetMessage(&msg, nullptr, 0, 0) > 0) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
#else
  std::cout << "=== DFE Focus Session (simulation) ===\n";
  std::cout << "Student: " << profile.userName << "\n";
  std::cout << "Exam date: " << profile.examDate << "\n";
  std::cout << "Daily end time: " << profile.dailyEndTime << "\n";
  std::cout << "Allowed apps: ";
  for (const auto& app : profile.allowedApps) {
    std::cout << app << ' ';
  }
  std::cout << "\n";
  for (size_t i = 0; i < plan.size() && i < 5; ++i) {
    const auto& block = plan[i];
    std::cout << "- " << block.startsAt << " -> " << block.endsAt << " | " << block.subject << "\n";
  }
#endif
}

}  // namespace dfe
