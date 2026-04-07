#include "dfe/AppController.hpp"

#ifdef DFE_WITH_QT
#include <QVariantMap>

#ifdef _WIN32
#include <windows.h>
#endif

namespace dfe {

AppController::AppController(const UserProfile& profile, const std::vector<StudyBlock>& plan,
                             QObject* parent)
    : QObject(parent), profile_(profile), plan_(plan) {}

QString AppController::studentName() const { return QString::fromStdString(profile_.userName); }

QString AppController::examDate() const { return QString::fromStdString(profile_.examDate); }

QString AppController::dailyEndTime() const { return QString::fromStdString(profile_.dailyEndTime); }

QStringList AppController::allowedApps() const {
  QStringList list;
  for (const auto& app : profile_.allowedApps) {
    list.append(QString::fromStdString(app));
  }
  return list;
}

QVariantList AppController::scheduleModel() const {
  QVariantList list;
  for (const auto& block : plan_) {
    QVariantMap item;
    item["startsAt"] = QString::fromStdString(block.startsAt);
    item["endsAt"] = QString::fromStdString(block.endsAt);
    item["subject"] = QString::fromStdString(block.subject);
    item["task"] = QString::fromStdString(block.task);
    list.append(item);
  }
  return list;
}

bool AppController::launchApp(const QString& appName) {
#ifdef _WIN32
  const auto wide = appName.toStdWString();
  const auto result = reinterpret_cast<intptr_t>(
      ShellExecuteW(nullptr, L"open", wide.c_str(), nullptr, nullptr, SW_SHOWNORMAL));
  return result > 32;
#else
  (void)appName;
  return false;
#endif
}

}  // namespace dfe
#endif
