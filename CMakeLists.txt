cmake_minimum_required(VERSION 2.6)
set(PROJECT lxqt-lightdm-greeter)
project(${PROJECT})


set ( lxqt-lightdm-greeter_HDRS
        mainwindow.h
        loginform.h
        logindata.h
)

set ( lxqt-lightdm-greeter_SRCS
        main.cpp
        mainwindow.cpp
        loginform.cpp
        logindata.cpp
)

set ( lxqt-lightdm-greeter_UIS
        loginform.ui
)

set ( lxqt-lightdm-greeter_MOCS
       mainwindow.h
       loginform.h
       logindata.h
)

set ( lxqt-lightdm-greeter_RSCS
      lxqt-lightdm-greeter.qrc
    )

find_package(Qt5Widgets REQUIRED)
find_package(lxqt-qt5 REQUIRED)
include(${LXQT_USE_FILE})

QT5_WRAP_UI(UIS ${lxqt-lightdm-greeter_UIS})
QT5_ADD_RESOURCES( RSCS ${lxqt-lightdm-greeter_RSCS})
QT5_WRAP_CPP(MOCS ${lxqt-lightdm-greeter_MOCS})


include(FindPkgConfig)
message(STATUS "Checking for lightdm-qt5-3")
pkg_check_modules(LIGHTDM_QT liblightdm-qt5-3)

if (NOT LIGHTDM_QT_INCLUDE_DIRS)
    message(FATAL "liblightdm-qt5-3 include directory not found")
endif (NOT LIGHTDM_QT_INCLUDE_DIRS)

if (NOT LIGHTDM_QT_LIBRARIES)
    message(FATAL "lightdm-qt5-3 library not found")
endif (NOT LIGHTDM_QT_LIBRARIES)


include_directories ( ${CMAKE_CURRENT_SOURCE_DIR}
                      ${CMAKE_CURRENT_BINARY_DIR}
                      ${LIGHTDM_QT_INCLUDE_DIRS}
)

message(STATUS "RSCS=${RSCS}")



if(CMAKE_BUILD_TYPE MATCHES [Dd]ebug)
    configure_file(test/test.sh test/test.sh)
    configure_file(test/lxqt-lightdm-greeter.desktop test/lxqt-lightdm-greeter.desktop)
    configure_file(test/lightdm.conf test/lightdm.conf)
endif()

add_executable ( lxqt-lightdm-greeter
                 ${lxqt-lightdm-greeter_SRCS}
                 ${lxqt-lightdm-greeter_HDRS}
                 ${lxqt-lightdm-greeter_UIS}
                 ${MOCS}
                 ${RSCS}
)

target_link_libraries ( lxqt-lightdm-greeter  
                        ${LXQT_LIBRARIES}
                        ${QTXDG_LIBRARIES}
                        ${QT_QTCORE_LIBRARY}
                        ${QT_QTGUI_LIBRARY} 
                        ${QT_QTDBUS_LIBRARY} 
                        ${QT_QTNETWORK_LIBRARY} 
                        ${LIGHTDM_QT_LIBRARIES} )

file(GLOB DESKTOP_FILES resources/*.desktop)
file(GLOB CONFIG_FILES resources/*.conf)

install(TARGETS ${PROJECT} RUNTIME DESTINATION bin)
install(FILES   ${DESKTOP_FILES}   DESTINATION /usr/share/xgreeters)

if (NOT KEEP_CONFIGS)
    install(FILES ${CONFIG_FILES} DESTINATION /etc/lightdm)
endif()

# building tarball with CPack -------------------------------------------------
include (InstallRequiredSystemLibraries)
set (CPACK_PACKAGE_VERSION_MAJOR ${LXQT_MAJOR_VERSION})
set (CPACK_PACKAGE_VERSION_MINOR ${LXQT_MINOR_VERSION})
set (CPACK_PACKAGE_VERSION_PATCH ${LXQT_PATCH_VERSION})
set (CPACK_GENERATOR TBZ2)
set (CPACK_SOURCE_GENERATOR TBZ2)
set (CPACK_SOURCE_IGNORE_FILES /build/;.gitignore;.*~;.git;.kdev4;temp)
include (CPack)
