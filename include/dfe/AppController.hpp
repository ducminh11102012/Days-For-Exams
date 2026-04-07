#pragma once

#ifdef DFE_WITH_QT
#include <QObject>
#include <QString>
#include <QStringList>
#include <QVariantList>
#endif

#include <vector>

#include "dfe/Models.hpp"

namespace dfe {

#ifdef DFE_WITH_QT
class AppController : public QObject {
  Q_OBJECT
  Q_PROPERTY(QString studentName READ studentName CONSTANT)
  Q_PROPERTY(QString examDate READ examDate CONSTANT)
  Q_PROPERTY(QString dailyEndTime READ dailyEndTime CONSTANT)

 public:
  explicit AppController(const UserProfile& profile, const std::vector<StudyBlock>& plan,
                         QObject* parent = nullptr);

  QString studentName() const;
  QString examDate() const;
  QString dailyEndTime() const;

  Q_INVOKABLE QStringList allowedApps() const;
  Q_INVOKABLE QVariantList scheduleModel() const;
  Q_INVOKABLE bool launchApp(const QString& appName);

 private:
  UserProfile profile_;
  std::vector<StudyBlock> plan_;
};
#endif

}  // namespace dfe
