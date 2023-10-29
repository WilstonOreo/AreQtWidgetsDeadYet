
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        widget.cpp
        widget.h
        widgets/widgets.qrc
)

qt_add_executable(AreQtWidgetsDeadYet
    MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )


target_link_libraries(AreQtWidgetsDeadYet PRIVATE Qt6::Widgets)

set_target_properties(AreQtWidgetsDeadYet PROPERTIES
    WIN32_EXECUTABLE TRUE
)

qt_finalize_executable(AreQtWidgetsDeadYet)
