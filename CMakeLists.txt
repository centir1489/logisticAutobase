cmake_minimum_required(VERSION 3.16)
project(autobase VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)


find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql Network Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql Network Core)


find_package(PostgreSQL)
if(PostgreSQL_FOUND)
    message(STATUS "PostgreSQL found, building with database support")
    set(HAVE_POSTGRESQL TRUE)
else()
    message(WARNING "PostgreSQL not found, building without database support")
    set(HAVE_POSTGRESQL FALSE)
endif()


add_executable(Client
    Client/header/client.h
    Client/src/client.cpp

    Client/src/main.cpp

    Client/src/authorizationDialog.cpp
    Client/src/mainwindow.cpp
    Client/src/parcelCard.cpp
    Client/src/userTable.cpp

    Client/header/logisticAutobase.h
    Client/header/authorizationDialog.h
    Client/header/mainwindow.h
    Client/header/parcelCard.h
    Client/header/userTable.h

    Client/header/parcelDialogAdd.h
    Client/header/parcelDialogDelete.h
    Client/header/parcelDialogEdit.h
    Client/header/parcelDialogMove.h
    Client/header/parcelDialogOpen.h

    Client/header/tableDialogAdd.h
    Client/header/tableDialogDelete.h
    Client/header/tableDialogExport.h
    Client/header/tableDialogView.h
    Client/header/tableDialogEdit.h
    Client/header/tableDialogOpen.h
    Client/header/tableDialogRename.h

    Client/resources.qrc
)

target_include_directories(Client PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Client/header
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(Client PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Core
)


add_executable(Server
    Server/src/main.cpp
    Server/src/server.cpp
    Server/src/sqlwork.cpp

    Server/header/server.h
    Server/header/sqlwork.h
)

target_include_directories(Server PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Server/header
)

target_link_libraries(Server PRIVATE
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Sql
)

if(HAVE_POSTGRESQL)
    target_link_libraries(Server PRIVATE PostgreSQL::PostgreSQL)
    target_compile_definitions(Server PRIVATE HAVE_POSTGRESQL)
else()
    target_compile_definitions(Server PRIVATE NO_POSTGRESQL)
endif()


install(TARGETS Client Server
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)


if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Client)
    qt_finalize_executable(Server)
endif()
