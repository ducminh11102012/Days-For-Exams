# 📘 Day For Exams (DFE)

> **Lock in. Cut distractions. Survive the deadline.**

---

## 🚀 Overview

**Day For Exams (DFE)** is a lightweight study-focused lockdown application built for students facing high-pressure exam periods.

Inspired by tools like Safe Exam Browser, DFE creates a controlled digital environment that minimizes distractions while guiding users through a structured study plan.

Instead of replacing the operating system, DFE runs as a **secure fullscreen layer**, ensuring focus without compromising system stability.

---

## 🎯 Core Concept

DFE operates on a simple idea:

> When exams get close, your computer should stop being entertainment and start being a study tool.

---

## ✨ Features

### 🧠 Smart Study Setup

* Input subject(s), exam date, and study goals
* Automatically generates a personalized study schedule
* Adapts to remaining time before exams

---

### 📁 User Profile System (.dfe)

* Stores user configuration in `.dfe` files
* Enables quick resume of study sessions
* Keeps track of preferences and goals

---

### 🔒 Focus Lock Mode

When activated:

* Blocks unauthorized applications
* Restricts system-level distractions
* Runs in fullscreen, always-on-top mode

---

### 🌐 Controlled Web Access

Only selected websites are accessible, such as:

* ChatGPT
* Google Search
* Oxford Dictionary
* Gemini

All other websites are restricted during study sessions.

---

### 🖥️ Minimal Study Interface

* Clean, distraction-free UI
* Inspired by exam environments like SEB
* Quick access to:

  * Allowed applications
  * Study materials
  * Timer and schedule

---

### ⏱️ Session & Time Management

* Built-in study timer
* Daily schedule tracking
* Optional end time (default: 23:30)

---

### 🔓 Exit System

* Secure exit option within the app
* Restores normal system behavior after session ends

---

## 🏗️ Architecture Overview

```
+----------------------+
|  Setup Module        |
|  (User Input)        |
+----------+-----------+
           |
           v
+----------------------+
|  Scheduler Engine    |
|  (Study Plan)        |
+----------+-----------+
           |
           v
+----------------------+
|  Lockdown Manager    |
|  - App Control       |
|  - Web Restriction   |
+----------+-----------+
           |
           v
+----------------------+
|  Fullscreen UI Layer |
|  (DFE Interface)     |
+----------------------+
```

---

## 🛠️ Technologies

* **Language:** C++
* **Platform:** Windows
* **Core APIs:** WinAPI
* **Optional UI Framework:** Qt
* **System Control:**

  * Process management
  * Keyboard hooks
  * Window control

---

## 📦 Project Structure

```
/DFE
 ├── core/
 ├── ui/
 ├── scheduler/
 ├── security/
 ├── config/
 └── main.cpp
```

---

## ⚠️ Limitations

* Cannot guarantee 100% prevention of advanced bypass techniques
* Requires appropriate system permissions for full functionality
* Performance may vary depending on system configuration

---

## 💡 Future Improvements

* AI-assisted study planning
* Progress analytics dashboard
* Cross-platform support
* Cloud sync for `.dfe` profiles

---

## 📌 Summary

DFE is designed to do one thing well:

> Help students stay focused when it matters most.

No distractions. No excuses. Just results.
