cmake_minimum_required(VERSION 3.16)

project(EliteCarwash LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# --- Qt ---
find_package(Qt6 COMPONENTS
    Core Gui Widgets Sql Network Xml PrintSupport
    QUIET)

if(Qt6_FOUND)
    set(QT_PACKAGE Qt6)
else()
    find_package(Qt5 REQUIRED COMPONENTS
        Core Gui Widgets Sql Network Xml PrintSupport)
    set(QT_PACKAGE Qt5)
endif()

# --- Sources ---
file(GLOB_RECURSE APP_SOURCES
    *.cpp
    ../Base/*.cpp
    ../Cache/*.cpp
    ../Selector/*.cpp
    ../Controls/*.cpp
    ../RowEditor/*.cpp
    ../Filter/*.cpp
    ../Print/*.cpp
    ../Widgets/*.cpp
    ../GridOptionWidgets/*.cpp
    ../Xlsx/src/*.cpp
)

file(GLOB_RECURSE APP_HEADERS
    *.h
    ../Base/*.h
    ../Cache/*.h
    ../Selector/*.h
    ../Controls/*.h
    ../RowEditor/*.h
    ../Filter/*.h
    ../Print/*.h
    ../Widgets/*.h
    ../GridOptionWidgets/*.h
    ../Xlsx/src/*.h
    ../../NewTax/Src/*.h
)

file(GLOB_RECURSE APP_FORMS *.ui)

file(GLOB_RECURSE APP_RESOURCES *.qrc)

# --- Executable ---
add_executable(EliteCarwash WIN32
    ${APP_SOURCES}
    ${APP_HEADERS}
    ${APP_FORMS}
    ${APP_RESOURCES}
)

# --- Includes ---
target_include_directories(EliteCarwash PRIVATE
    .
    ../Base
    ../Cache
    ../Selector
    ../Controls
    ../RowEditor
    ../Filter
    ../Print
    ../Widgets
    ../GridOptionWidgets
    ../Xlsx/src
    C:/Development/OpenSSL-Win64/include
    C:/Development/OpenSSL-Win64/include/openssl
)

# --- Defines ---
target_compile_definitions(EliteCarwash PRIVATE
    _MODULE_="EliteCarwash"
    _ORGANIZATION_="SmartHotel"
    _APPLICATION_="SmartHotel"
    _DBDRIVER_="QMARIADB"
)

# --- Qt libs ---
target_link_libraries(EliteCarwash PRIVATE
    ${QT_PACKAGE}::Core
    ${QT_PACKAGE}::Gui
    ${QT_PACKAGE}::Widgets
    ${QT_PACKAGE}::Sql
    ${QT_PACKAGE}::Network
    ${QT_PACKAGE}::Xml
    ${QT_PACKAGE}::PrintSupport
)

add_subdirectory(C:/Development/projects/qxlsx qxlsx_build)
target_link_libraries(EliteCarwash PRIVATE QXlsx)

# --- Windows libs ---
if(WIN32)
    # Указываем путь к OpenSSL (если find_package не используем)
    target_include_directories(EliteCarwash PRIVATE
        C:/Development/OpenSSL-Win64/include
        C:/Development/OpenSSL-Win64/include/openssl
    )

    # Линковка библиотек
    target_link_directories(EliteCarwash PRIVATE
        C:/Development/OpenSSL-Win64/lib/VC/x64/MD
    )

    target_link_libraries(EliteCarwash PRIVATE
        Version.lib
        Ws2_32.lib
        libssl.lib
        libcrypto.lib
        Gdi32.lib
        User32.lib
    )

endif()
