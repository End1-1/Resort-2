cmake_minimum_required(VERSION 3.21)

project(Restaurant LANGUAGES CXX)

# ---------- C++ ----------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------- Qt automoc ----------
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# ---------- Qt modules ----------
find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    Sql
    Network
    Xml
    PrintSupport
    LinguistTools
)

qt_standard_project_setup()

set(RESTAURANT_QM "${CMAKE_CURRENT_SOURCE_DIR}/Restaurant.am.qm")
add_custom_command(
    OUTPUT "${RESTAURANT_QM}"
    COMMAND "${CMAKE_COMMAND}" -E echo "Updating Restaurant.am.qm"
    COMMAND powershell -NoProfile -ExecutionPolicy Bypass
            -File "${CMAKE_CURRENT_SOURCE_DIR}/compile_restaurant_qm.ps1"
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/Restaurant.am.ts"
            "${CMAKE_CURRENT_SOURCE_DIR}/compile_restaurant_qm.ps1"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    VERBATIM
)

# ---------- SOURCES ----------
set(SOURCES
    main.cpp
    ../Resort/database2.cpp
    ../Resort/logwriter.cpp
    ../Resort/message.cpp
    ../Resort/storeoutput.cpp
    ../Filter/giftcartstore.cpp
    branchstoremap.cpp
    customerdisplay.cpp
    winprinternames.cpp
    dlgpassword.cpp
    dlgprintmultiplefiscal.cpp
    dlgsalarytotal.cpp
    rawmessage.cpp
    rface.cpp
    rlogin.cpp
    rmessage.cpp
    session.cpp
    sessiondebtclose.cpp
    debtpay.cpp
    orderlog.cpp
    talonservice.cpp
    dlgpaydebt.cpp
    dlgsessionopen.cpp
    ../Base/utils.cpp
    ../Base/base.cpp
    ../Base/basedialog.cpp
    ../Base/database.cpp
    ../Base/defines.cpp
    ../Base/preferences.cpp
    ../Base/user.cpp
    hall.cpp
    rdesk.cpp
    dishestable.cpp
    splash.cpp
    rchangelanguage.cpp
    rchangemenu.cpp
    rchangehall.cpp
    rkeyboard.cpp
    rdishcomment.cpp
    dlgsmile.cpp
    rtools.cpp
    rnumbers.cpp
    rmodifiers.cpp
    ../Controls/ecolorlineedit.cpp
    ../Controls/eqcheckbox.cpp
    ../Controls/eqdateedit.cpp
    ../Controls/eqlineedit.cpp
    ../Controls/eqtextedit.cpp
    ../Controls/eqtimeedit.cpp
    ../Controls/trackcontrol.cpp
    dlgreservation.cpp
    ../Cache/cachebase.cpp
    ../Cache/cachecreditcard.cpp
    ../Cache/cacheguest.cpp
    ../Cache/cacheguesttitle.cpp
    ../Cache/cacheone.cpp
    ../Cache/cachepaymentmode.cpp
    ../Cache/cacherestdishmod.cpp
    ../Cache/cacherestdishpart.cpp
    ../Cache/cacherestdishtype.cpp
    ../Cache/cacheresthall.cpp
    ../Cache/cacherestmenu.cpp
    ../Cache/cacherestprinters.cpp
    ../Cache/cachereststore.cpp
    ../Cache/cacheresttable.cpp
    ../Cache/cacherights.cpp
    ../Cache/cachetrackingtables.cpp
    ../Cache/cacheusers.cpp
    ../Cache/cacheusersgroups.cpp
    ../Cache/cacheusersstate.cpp
    ../Cache/cachevatmode.cpp
    ../Controls/eqspinbox.cpp
    ../Controls/eqtablewidget.cpp

    ../Controls/epushbutton.cpp
    ../Print/c5printing.cpp
    restaurantc5print.cpp
    ../Base/logging.cpp
    reportprint.cpp
    ../Controls/eqcombobox.cpp
    ../Cache/cachsex.cpp
    ../Base/command.cpp
    dlgbanketitems.cpp
    dlglist.cpp
    dlgdate.cpp
    ../Controls/edateedit.cpp
    ../Base/databaseresult.cpp
    dlgvoidback.cpp
    ../Restaurant2/pprintreceipt.cpp
    ../Base/baseuid.cpp
    dlggettext.cpp
    dlgconnecttoserver.cpp
    ../Cache/cachecoupontype.cpp
    ../Base/defrest.cpp
    ../Base/baseorder.cpp
    dlgcarselection.cpp
    ../Cache/cachecar.cpp
    dlgpayment.cpp
    dlgdeptholder.cpp
    dlgtalonredeem.cpp

    ../Cache/cachedocpayment.cpp
    dlgprinttaxnew.cpp
    ../Cache/cachecouponseria.cpp
    dlgsalary.cpp
    ../Base/appconfig.cpp
    baseextendeddialog.cpp
    printtaxno.h
    printtaxno.cpp
    sslsocket.cpp
)

set(HEADERS
    rface.h
    ../Resort/database2.h
    ../Resort/logwriter.h
    branchstoremap.h
    customerdisplay.h
    winprinternames.h
    dlgpassword.h
    version.h
)

set(FORMS
    rface.ui
    dlgpassword.ui
    rlogin.ui
    dlgsessionopen.ui
    dlgpaydebt.ui
    splash.ui
)

set(RESOURCES res.qrc)

# ---------- EXECUTABLE ----------
qt_add_executable(Restaurant WIN32
    ${SOURCES}
    ${HEADERS}
    ${FORMS}
    ${RESOURCES}
    ${RESTAURANT_QM}
    printtaxno.cpp printtaxno.h
    restaurantc5print.h restaurantc5print.cpp
    checktime.h checktime.cpp
    dlgcalc.h dlgcalc.cpp dlgcalc.ui
)

# ---------- DEFINES ----------
target_compile_definitions(Restaurant PRIVATE
    _WASH_
    _RESTAURANT_
    _APPLICATION_="ELITEWASH"
    _MODULE_="SALES"
    _DBDRIVER_="QMARIADB"
)

# ---------- INCLUDE PATHS ----------
target_include_directories(Restaurant PRIVATE
    C:/Development/projects/elite.carwash/base
    C:/Development/projects/elite.carwash/Controls
    C:/Development/projects/elite.carwash/Cache
    C:/Development/projects/elite.carwash/Print
    C:/Development/projects/elite.carwash/Resort
    C:/Development/projects/elite.carwash/Filter
    C:/Development/projects/elite.carwash/Selector
    C:/Development/projects/desktop.newtax/Src
    C:/Development/OpenSSL-Win64/include

)

# ---------- LIB PATH ----------
target_link_directories(Restaurant PRIVATE
    C:/Development/OpenSSL-Win64/lib/VC/x64/MD
)

# ---------- LINK ----------
target_link_libraries(Restaurant PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Sql
    Qt6::Network
    Qt6::Xml
    Qt6::PrintSupport
    Version
    Ws2_32
    libssl
    libcrypto
    Gdi32
    User32
)
