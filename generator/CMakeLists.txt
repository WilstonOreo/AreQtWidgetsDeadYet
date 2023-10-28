
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        analogclock.h
        analogclock.cpp
        wigglywidget.h
        wigglywidget.cpp
)

qt_add_executable(WidgetImageGenerator
    MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        stepinterface.h
    )


target_link_libraries(WidgetImageGenerator PRIVATE Qt6::Widgets)

set_target_properties(WidgetImageGenerator PROPERTIES
    WIN32_EXECUTABLE TRUE
)

qt_finalize_executable(AreQtWidgetsDeadYet)
