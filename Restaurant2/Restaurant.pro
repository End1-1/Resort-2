#-------------------------------------------------
#
# Project created by QtCreator 2017-07-15T15:15:16
#
#-------------------------------------------------

QT       += core gui sql network xml printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Restaurant
TEMPLATE = app

DEFINES += _WASH_
DEFINES += _RESTAURANT_

RC_FILE = res.rc

INCLUDEPATH += c:/projects/resort2/Base
INCLUDEPATH += c:/projects/resort2/Controls
INCLUDEPATH += c:/projects/resort2/Cache
INCLUDEPATH += c:/projects/resort2/Print
INCLUDEPATH += c:/projects/resort2/Resort
INCLUDEPATH += c:/projects/resort2/Selector
INCLUDEPATH += c:/projects/Cafe5/Classes
INCLUDEPATH += C:/Projects/NewTax/Src/
INCLUDEPATH += c:/OpenSSL-Win32/include
INCLUDEPATH += c:/OpenSSL-Win32/include/openssl

SOURCES += main.cpp\
    ../../Cafe5/Classes/notificationwidget.cpp \
    ../Resort/message.cpp \
    dlgcheckcard.cpp \
    rface.cpp \
    rlogin.cpp \
    rmessage.cpp \
    ../Base/utils.cpp \
    ../Base/base.cpp \
    ../Base/basedialog.cpp \
    ../Base/database.cpp \
    ../Base/defines.cpp \
    ../Base/preferences.cpp \
    ../Base/user.cpp \
    hall.cpp \
    rdesk.cpp \
    dishestable.cpp \
    splash.cpp \
    rchangelanguage.cpp \
    rchangemenu.cpp \
    rpaymenttype.cpp \
    rselecttable.cpp \
    rchangehall.cpp \
    rkeyboard.cpp \
    rdishcomment.cpp \
    dlgsmile.cpp \
    rtools.cpp \
    rnumbers.cpp \
    rmodifiers.cpp \
    ../Controls/ecolorlineedit.cpp \
    ../Controls/eqcheckbox.cpp \
    ../Controls/eqdateedit.cpp \
    ../Controls/eqlineedit.cpp \
    ../Controls/eqtextedit.cpp \
    ../Controls/eqtimeedit.cpp \
    ../Controls/trackcontrol.cpp \
    dlgreservation.cpp \
    ../Cache/cachebase.cpp \
    ../Cache/cachecreditcard.cpp \
    ../Cache/cacheguest.cpp \
    ../Cache/cacheguesttitle.cpp \
    ../Cache/cacheinvoiceitem.cpp \
    ../Cache/cacheone.cpp \
    ../Cache/cachepaymentmode.cpp \
    ../Cache/cacherestdishmod.cpp \
    ../Cache/cacherestdishpart.cpp \
    ../Cache/cacherestdishtype.cpp \
    ../Cache/cacheresthall.cpp \
    ../Cache/cacherestmenu.cpp \
    ../Cache/cacherestprinters.cpp \
    ../Cache/cachereststore.cpp \
    ../Cache/cacheresttable.cpp \
    ../Cache/cacherights.cpp \
    ../Cache/cachetrackingtables.cpp \
    ../Cache/cacheusers.cpp \
    ../Cache/cacheusersgroups.cpp \
    ../Cache/cacheusersstate.cpp \
    ../Cache/cachevatmode.cpp \
    ../Controls/eqspinbox.cpp \
    ../Controls/eqtablewidget.cpp \
    dlgcomplexdish.cpp \
    ../Controls/epushbutton.cpp \
    ../Print/pimage.cpp \
    ../Print/pprintpreview.cpp \
    ../Print/pprintscene.cpp \
    ../Print/ptextrect.cpp \
    ../Base/logging.cpp \
    reportprint.cpp \
    ../Controls/eqcombobox.cpp \
    ../Cache/cachsex.cpp \
    dlgselecttaxcashmode.cpp \
    dlgbreakfast.cpp \
    ../Base/command.cpp \
    ../Resort/broadcastthread.cpp \
    dlgbanketitems.cpp \
    dlglist.cpp \
    dlgdate.cpp \
    ../Controls/edateedit.cpp \
    ../Base/databaseresult.cpp \
    dlgvoidback.cpp \
    ../Print/pprintreceipt.cpp \
    dlgorders.cpp \
    ../Base/baseuid.cpp \
    dlggettext.cpp \
    dlgconnecttoserver.cpp \
    ../Cache/cachecoupontype.cpp \
    ../Base/defrest.cpp \
    ../Base/baseorder.cpp \
    dlgcarselection.cpp \
    ../Cache/cachecar.cpp \
    dlgpayment.cpp \
    dlgdeptholder.cpp \
    ../Cache/cacherestdebtholder.cpp \
    ../Cache/cachedocpayment.cpp \
    dlgprinttaxnew.cpp \
    ../Cache/cachecouponseria.cpp \
    dlgsalary.cpp \
    ../Base/appconfig.cpp \
    baseextendeddialog.cpp \
    c:/projects/NewTax/Src/printtaxn.cpp \
    ../../Cafe5/Classes/c5printing.cpp

HEADERS  += rface.h \
    ../../Cafe5/Classes/notificationwidget.h \
    c:/projects/NewTax/Src/printtaxn.h \
    dlgcheckcard.h \
    rlogin.h \
    rmessage.h \
    ../Base/base.h \
    ../Base/basedialog.h \
    ../Base/database.h \
    ../Base/defines.h \
    ../Base/preferences.h \
    ../Base/utils.h \
    ../Base/user.h \
    hall.h \
    rdesk.h \
    dishestable.h \
    splash.h \
    rchangelanguage.h \
    rchangemenu.h \
    rpaymenttype.h \
    rselecttable.h \
    rchangehall.h \
    rkeyboard.h \
    rdishcomment.h \
    dlgsmile.h \
    rtools.h \
    rnumbers.h \
    rmodifiers.h \
    ../Controls/ecolorlineedit.h \
    ../Controls/eqcheckbox.h \
    ../Controls/eqdateedit.h \
    ../Controls/eqlineedit.h \
    ../Controls/eqtextedit.h \
    ../Controls/eqtimeedit.h \
    ../Controls/trackcontrol.h \
    dlgreservation.h \
    ../Cache/cachebase.h \
    ../Cache/cachecreditcard.h \
    ../Cache/cacheguest.h \
    ../Cache/cacheguesttitle.h \
    ../Cache/cacheinvoiceitem.h \
    ../Cache/cacheone.h \
    ../Cache/cachepaymentmode.h \
    ../Cache/cacherestdishmod.h \
    ../Cache/cacherestdishpart.h \
    ../Cache/cacherestdishtype.h \
    ../Cache/cacheresthall.h \
    ../Cache/cacherestmenu.h \
    ../Cache/cacherestprinters.h \
    ../Cache/cachereststore.h \
    ../Cache/cacheresttable.h \
    ../Cache/cacherights.h \
    ../Cache/cachetrackingtables.h \
    ../Cache/cacheusers.h \
    ../Cache/cacheusersgroups.h \
    ../Cache/cacheusersstate.h \
    ../Cache/cachevatmode.h \
    ../Controls/eqspinbox.h \
    ../Controls/eqtablewidget.h \
    dlgcomplexdish.h \
    ../Controls/epushbutton.h \
    ../Print/pdefaults.h \
    ../Print/pimage.h \
    ../Print/pprintpreview.h \
    ../Print/pprintscene.h \
    ../Print/ptextrect.h \
    ../Base/logging.h \
    reportprint.h \
    ../Controls/eqcombobox.h \
    ../Cache/cachsex.h \
    dlgselecttaxcashmode.h \
    dlgbreakfast.h \
    ../Base/command.h \
    ../Resort/broadcastthread.h \
    dlgbanketitems.h \
    ../Base/vauchers.h \
    dlglist.h \
    dlgdate.h \
    ../Controls/edateedit.h \
    ../Base/databaseresult.h \
    dlgvoidback.h \
    ../Print/pprintreceipt.h \
    dlgorders.h \
    ../Base/baseuid.h \
    dlggettext.h \
    dlgconnecttoserver.h \
    ../Cache/cachecoupontype.h \
    ../Base/defrest.h \
    ../Base/baseorder.h \
    dlgcarselection.h \
    ../Cache/cachecar.h \
    dlgpayment.h \
    dlgdeptholder.h \
    ../Cache/cacherestdebtholder.h \
    ../Cache/cachedocpayment.h \
    dlgprinttaxnew.h \
    ../Cache/cachecouponseria.h \
    dlgsalary.h \
    ../Base/appconfig.h \
    ../Resort/message.h \
    baseextendeddialog.h \
     c:/projects/Cafe5/Classes/c5printing.h

FORMS    += rface.ui \
    ../../Cafe5/Classes/notificationwidget.ui \
    dlgcheckcard.ui \
    rlogin.ui \
    rmessage.ui \
    rdesk.ui \
    splash.ui \
    rchangelanguage.ui \
    rchangemenu.ui \
    rpaymenttype.ui \
    rselecttable.ui \
    rchangehall.ui \
    rkeyboard.ui \
    rdishcomment.ui \
    dlgsmile.ui \
    rtools.ui \
    rnumbers.ui \
    rmodifiers.ui \
    dlgreservation.ui \
    dlgcomplexdish.ui \
    ../Print/pprintpreview.ui \
    dlgselecttaxcashmode.ui \
    dlgbreakfast.ui \
    dlgbanketitems.ui \
    dlglist.ui \
    dlgdate.ui \
    dlgvoidback.ui \
    dlgorders.ui \
    dlggettext.ui \
    dlgconnecttoserver.ui \
    dlgcarselection.ui \
    dlgpayment.ui \
    dlgdeptholder.ui \
    dlgprinttaxnew.ui \
    dlgsalary.ui \
    ../Resort/message.ui \

LIBS += -lVersion
LIBS += -LC:/OpenSSL-Win32/lib
LIBS += -lopenssl
LIBS += -llibcrypto
LIBS +=  -lwsock32

RESOURCES += \
    res.qrc

DISTFILES += \
    stylesheet.qss \
    css \
    images/add.png

