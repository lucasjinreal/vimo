cmake_minimum_required(VERSION 3.8)
project(apps)


set(CMAKE_CXX_STANDARD 11)  # for c++11


set(XINPUTSIMULATOR_DIR xinputsimulator)
# Linux
if (UNIX AND NOT APPLE)
    find_library(X_11 X11)
    find_library(X_TST Xtst)
    set(EXTRA_LIBS ${X_11} ${X_TST})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    set(PLATFORM_SOURCE_FILES
            ${XINPUTSIMULATOR_DIR}/xinputsimulatorimpl.cpp
            ${XINPUTSIMULATOR_DIR}/xinputsimulatorimpllinux.cpp
            ${XINPUTSIMULATOR_DIR}/xinputsimulatorimpllinux.h
            ${XINPUTSIMULATOR_DIR}/xinputsimulator.cpp)
endif (UNIX AND NOT APPLE)

# Apple
if (APPLE)
    find_library(APP_SERVICES ApplicationServices)
    find_library(CARBON Carbon)
    find_library(CORE_FOUNDATION CoreFoundation)
    set(EXTRA_LIBS ${APP_SERVICES_LIBRARY} ${CARBON} ${CORE_FOUNDATION})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    set(PLATFORM_SOURCE_FILES ${XINPUTSIMULATOR_DIR}/xinputsimulatorimplmacos.cpp ${XINPUTSIMULATOR_DIR}/xinputsimulatorimplmacos.h)
endif (APPLE)

# Windows
if (WIN32)
    #find_library(USER_32 User32.Lib)
    #set(EXTRA_LIBS ${USER_32})
    set(PLATFORM_SOURCE_FILES ${XINPUTSIMULATOR_DIR}/xinputsimulatorimplwin.cpp ${XINPUTSIMULATOR_DIR}/xinputsimulatorimplwin.h)
endif (WIN32)


if (UNIX AND NOT APPLE)
    add_executable(explo_linux explo_linux.cc ${PLATFORM_SOURCE_FILES})
    target_link_libraries(explo_linux ${EXTRA_LIBS})
elseif(APPLE)
    add_executable(explo_macos explo_macos.cc ${PLATFORM_SOURCE_FILES})
    target_link_libraries(explo_macos ${EXTRA_LIBS})

elseif(WIN32)
    add_executable(explo_win explo_win.cc)
    target_link_libraries(explo_win ${EXTRA_LIBS})
endif()
