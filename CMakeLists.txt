cmake_minimum_required(VERSION 3.20)
project(DFE VERSION 0.2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(DFE_ENABLE_QT_UI "Enable Qt/QML premium UI if Qt6 is available" ON)

add_library(dfe_core
  src/core/ProfileStore.cpp
  src/core/DateTime.cpp
  src/scheduler/SchedulerEngine.cpp
  src/security/PolicyEngine.cpp
  src/security/LockdownManager.cpp
  src/ui/StudyInterface.cpp
)

target_include_directories(dfe_core PUBLIC include)

set(DFE_APP_SOURCES
  src/main.cpp
)

set(DFE_WITH_QT OFF)
if(DFE_ENABLE_QT_UI)
  find_package(Qt6 COMPONENTS Quick Qml QUIET)
  if(Qt6_FOUND)
    message(STATUS "Qt6 found: enabling QML dashboard UI")
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_AUTOUIC ON)

    list(APPEND DFE_APP_SOURCES
      src/backend/AppController.cpp
      qml/qml.qrc
    )

    add_executable(dfe ${DFE_APP_SOURCES})
    target_link_libraries(dfe PRIVATE dfe_core Qt6::Quick Qt6::Qml)
    target_compile_definitions(dfe PRIVATE DFE_WITH_QT)
    target_compile_definitions(dfe_core PRIVATE DFE_WITH_QT)
    set(DFE_WITH_QT ON)
  endif()
endif()

if(NOT DFE_WITH_QT)
  message(STATUS "Qt6 not found (or disabled): building fallback non-Qt runtime")
  add_executable(dfe ${DFE_APP_SOURCES})
  target_link_libraries(dfe PRIVATE dfe_core)
endif()

if (WIN32)
  target_compile_definitions(dfe PRIVATE NOMINMAX UNICODE _UNICODE)
  target_compile_definitions(dfe_core PRIVATE NOMINMAX UNICODE _UNICODE)
endif()

enable_testing()
add_executable(dfe_scheduler_tests tests/SchedulerTests.cpp)
target_link_libraries(dfe_scheduler_tests PRIVATE dfe_core)
add_test(NAME scheduler_tests COMMAND dfe_scheduler_tests)
