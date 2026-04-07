#include <filesystem>
#include <iostream>
#include <string>

#include "dfe/LockdownManager.hpp"
#include "dfe/ProfileStore.hpp"
#include "dfe/SchedulerEngine.hpp"
#include "dfe/StudyInterface.hpp"

#ifdef DFE_WITH_QT
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "dfe/AppController.hpp"
#endif

namespace {

dfe::UserProfile promptProfile() {
  dfe::UserProfile profile;
  std::cout << "Welcome to Day For Exams (DFE)\n";
  std::cout << "Your name: ";
  std::getline(std::cin, profile.userName);
  std::cout << "Exam date (YYYY-MM-DD): ";
  std::getline(std::cin, profile.examDate);
  std::cout << "Daily end time [23:30]: ";
  std::getline(std::cin, profile.dailyEndTime);
  if (profile.dailyEndTime.empty()) {
    profile.dailyEndTime = "23:30";
  }

  profile.goals = {{"Math", 3, 20}, {"English", 2, 10}, {"Physics", 1, 8}};
  profile.allowedApps = {"dfe.exe", "WINWORD.EXE", "chrome.exe", "explorer.exe"};
  profile.allowedWebsites = {"chatgpt.com", "gemini.google.com", "google.com", "oxfordlearnersdictionaries.com"};
  return profile;
}

}  // namespace

int main(
#ifdef DFE_WITH_QT
    int argc, char* argv[]
#else

#endif
) {
  const std::filesystem::path profilePath = "user_profile.dfe";

  dfe::ProfileStore store;
  auto profile = store.load(profilePath);
  if (!profile) {
    auto created = promptProfile();
    if (!store.save(profilePath, created)) {
      std::cerr << "Cannot save .dfe profile file.\n";
      return 1;
    }
    profile = created;
    std::cout << "Profile created: " << profilePath << "\n";
  }

  dfe::SchedulerEngine scheduler;
  const auto plan = scheduler.buildPlan(*profile);

  dfe::PolicyEngine policy(*profile);
  dfe::LockdownManager lockdown(std::move(policy));

#ifdef DFE_WITH_QT
  QGuiApplication app(argc, argv);
  QQmlApplicationEngine engine;

  dfe::AppController controller(*profile, plan);
  engine.rootContext()->setContextProperty("backend", &controller);

  const QUrl mainUrl(QStringLiteral("qrc:/qml/Main.qml"));
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreated, &app,
      [mainUrl](QObject* object, const QUrl& objectUrl) {
        if (!object && mainUrl == objectUrl) {
          QCoreApplication::exit(-1);
        }
      },
      Qt::QueuedConnection);
  engine.load(mainUrl);

  lockdown.start();
  const int code = app.exec();
  lockdown.stop();
  return code;
#else
  dfe::StudyInterface ui;

  lockdown.start();
  ui.runSession(*profile, plan);

  std::cout << "Press Enter to exit focus mode...\n";
  std::string input;
  std::getline(std::cin, input);

  lockdown.stop();
  std::cout << "Focus mode exited. Restoring normal desktop behavior.\n";
  return 0;
#endif
}
