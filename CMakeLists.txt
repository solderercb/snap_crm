cmake_minimum_required(VERSION 3.5)

project(snap_crm VERSION 0.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets LinguistTools Sql Gui Core5Compat REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets LinguistTools Sql Gui Core5Compat REQUIRED)

set(TS_FILES SNAP_CRM_ru_RU.ts)

set(PROJECT_SOURCES
        SSetComboBox/ssetcombobox.cpp
        SSetComboBox/ssetcombobox.h
        appver.h
        bottoolbarwidget.cpp
        bottoolbarwidget.h
        bottoolbarwidget.ui
        chooseofficewindow.cpp
        chooseofficewindow.h
        chooseofficewindow.ui
        com_sql_queries.h
        loginwindow.cpp
        loginwindow.h
        loginwindow.ui
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        mylineedit.cpp
        mylineedit.h
        repairstablemodel.cpp
        repairstablemodel.h
        tabclients.cpp
        tabclients.h
        tabclients.ui
        tabrepair.cpp
        tabrepair.h
        tabrepair.ui
        tabrepairnew.cpp
        tabrepairnew.h
        tabrepairnew.ui
        tabrepairs.cpp
        tabrepairs.h
        tabrepairs.ui
        windowsdispatcher.cpp
        windowsdispatcher.h
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(snap
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET SNAP_CRM APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(snap-crm SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(snap
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(snap PUBLIC Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Core5Compat)

set_target_properties(snap PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

target_sources(snap PRIVATE resources.rc)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(snap)
endif()
