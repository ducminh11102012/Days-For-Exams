# 📘 Day For Exams (DFE)

> **Lock in. Cut distractions. Survive the deadline.**

DFE là ứng dụng tập trung học thi theo phong cách SEB-like: tạo môi trường học fullscreen, giảm xao nhãng và bám sát kế hoạch đến ngày thi.

## 🚀 Stack hiện tại

- **Core logic:** C++20
- **UI chính (premium):** Qt 6 + QML (khi môi trường có Qt)
- **Fallback UI:** C++ session simulation (không cần Qt)
- **Build system:** CMake
- **CI:** GitHub Actions build Windows `.exe` artifact

## ✅ Tính năng đã có

- Profile `.dfe` (load/save)
- Sinh lịch học tự động theo ngày thi + mức ưu tiên môn
- Policy whitelist app/web
- Lockdown monitor (MVP) cho nhóm app gây xao nhãng
- Dashboard QML fullscreen (TopBar, SchedulePanel, Dock)

## 🧱 Kiến trúc

```text
+----------------------+
| Setup/Profile Loader |
+----------+-----------+
           v
+----------------------+
| Scheduler Engine     |
+----------+-----------+
           v
+----------------------+
| Policy + Lockdown    |
+----------+-----------+
           v
+----------------------+           +----------------------+
| AppController (Qt)   | <-------> | QML UI (Main/Widgets)|
+----------------------+           +----------------------+
```

## 📁 Cấu trúc

```text
.
├── include/dfe
├── src
│   ├── backend
│   │   └── AppController.cpp
│   ├── core
│   ├── scheduler
│   ├── security
│   ├── ui
│   └── main.cpp
├── qml
│   ├── Main.qml
│   ├── components/
│   ├── pages/
│   └── styles/
├── tests
└── .github/workflows
```

## 🛠️ Build

### 1) Fallback (không Qt)

```bash
cmake -S . -B build -DDFE_ENABLE_QT_UI=OFF
cmake --build build
ctest --test-dir build --output-on-failure
```

### 2) Qt + QML UI

Yêu cầu Qt6 (Quick + Qml):

```bash
cmake -S . -B build
cmake --build build
```

Nếu CMake tìm thấy Qt6, binary `dfe` sẽ chạy QML dashboard.

## 🔒 Lưu ý

- Bản này là nền tảng production-ready theo hướng module hóa, có thể mở rộng thêm hardening web restriction/network layer.
- Không thay shell Windows (`explorer.exe`).
