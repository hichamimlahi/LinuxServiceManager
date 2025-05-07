cmake_minimum_required(VERSION 3.5)

project(LinuxServiceManager LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(LinuxServiceManager
    main.cpp
    mainwindow.cpp
    mainwindow.h
    servicemanager.cpp
    servicemanager.h
)

target_link_libraries(LinuxServiceManager PRIVATE Qt5::Widgets)

install(TARGETS LinuxServiceManager
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
