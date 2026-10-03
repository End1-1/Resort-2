cmake_minimum_required(VERSION 3.16)

project(EliteCarwash LANGUAGES CXX RC)

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
file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS
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
)

file(GLOB_RECURSE FILTER_FORMS CONFIGURE_DEPENDS ../Filter/*.ui)

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
    ../../NewTax/Src/*.h
)

file(GLOB_RECURSE APP_FORMS CONFIGURE_DEPENDS *.ui)
set(APP_FORMS ${APP_FORMS} ${FILTER_FORMS})

file(GLOB_RECURSE APP_RESOURCES *.qrc)

# --- Executable ---
add_executable(EliteCarwash WIN32
    ${APP_SOURCES}
    ${APP_HEADERS}
    ${APP_FORMS}
    ${APP_RESOURCES}
)

if(WIN32)
    target_sources(EliteCarwash PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/res.rc
    )
endif()

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
    C:/Development/OpenSSL-Win64/include
    C:/Development/OpenSSL-Win64/include/openssl
)

# --- Defines ---
target_compile_definitions(EliteCarwash PRIVATE
    _MODULE_="EliteCarwash"
    _ORGANIZATION_="SmartHotel"
    _APPLICATION_="SmartHotel"
    _DBDRIVER_="QMARIADB"
    RESORT_AUDIT_LOG
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

target_sources(EliteCarwash PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../Filter/dlgasexportconstants.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../Filter/dlgasexportconstants.ui
)

find_program(LRELEASE_EXECUTABLE NAMES lrelease lrelease-qt6
    HINTS
        ${Qt6_DIR}/../../../bin
        ${Qt5_DIR}/../../../bin
        "C:/Development/Qt/5.15.2/msvc2019/bin"
)

if(LRELEASE_EXECUTABLE)
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/Resort.qm
        COMMAND ${LRELEASE_EXECUTABLE}
            ${CMAKE_CURRENT_SOURCE_DIR}/Resort.ts
            -qm ${CMAKE_CURRENT_SOURCE_DIR}/Resort.qm
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Resort.ts
        COMMENT "Generating Resort.qm"
    )
    add_custom_target(resort_qm DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Resort.qm)
    add_dependencies(EliteCarwash resort_qm)

    add_custom_command(TARGET EliteCarwash POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/Resort.qm
            $<TARGET_FILE_DIR:EliteCarwash>/Resort.qm
        COMMENT "Copy Resort.qm next to executable"
    )
endif()
