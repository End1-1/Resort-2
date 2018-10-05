#-------------------------------------------------
#
# Project created by QtCreator 2017-07-15T15:15:16
#
#-------------------------------------------------

QT       += core gui sql network xml printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Restaurant
TEMPLATE = app

RC_FILE = res.rc

INCLUDEPATH += $$PWD/../Base
INCLUDEPATH += $$PWD/../Controls
INCLUDEPATH += $$PWD/../Cache
INCLUDEPATH += $$PWD/../Print
INCLUDEPATH += $$PWD/../Resort
INCLUDEPATH += $$PWD/../Selector

SOURCES += main.cpp\
    rface.cpp \
    rlogin.cpp \
    rmessage.cpp \
    ../Base/utils.cpp \
    ../Base/base.cpp \
    ../Base/basedialog.cpp \
    ../Base/database.cpp \
    ../Base/defines.cpp \
    ../Base/preferences.cpp \
    baseextendeddialog.cpp \
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
    ../Cache/cacheactiveroom.cpp \
    ../Cache/cachebase.cpp \
    ../Cache/cachecardex.cpp \
    ../Cache/cachecityledger.cpp \
    ../Cache/cachecreditcard.cpp \
    ../Cache/cacheguest.cpp \
    ../Cache/cacheguesttitle.cpp \
    ../Cache/cacheinvoiceitem.cpp \
    ../Cache/cacheone.cpp \
    ../Cache/cachepaymentmode.cpp \
    ../Cache/cacheredreservation.cpp \
    ../Cache/cachereservation.cpp \
    ../Cache/cachereservestate.cpp \
    ../Cache/cachereservestatus.cpp \
    ../Cache/cacherestdishmod.cpp \
    ../Cache/cacherestdishpart.cpp \
    ../Cache/cacherestdishtype.cpp \
    ../Cache/cacheresthall.cpp \
    ../Cache/cacherestmenu.cpp \
    ../Cache/cacherestprinters.cpp \
    ../Cache/cachereststore.cpp \
    ../Cache/cacheresttable.cpp \
    ../Cache/cacherights.cpp \
    ../Cache/cacheroom.cpp \
    ../Cache/cacheroomarrangment.cpp \
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
    ../Print/printtax.cpp \
    ../Controls/eqcombobox.cpp \
    ../Cache/cachsex.cpp \
    ../Cache/cachereminderstate.cpp \
    dlgselecttaxcashmode.cpp \
    dlgbreakfast.cpp \
    ../Base/command.cpp \
    ../Resort/broadcastthread.cpp \
    dlgbanket.cpp \
    dlgbanketitems.cpp \
    dlglist.cpp \
    dlgdate.cpp \
    ../Controls/edateedit.cpp \
    ../Base/databaseresult.cpp \
    dlgvoidback.cpp \
    ../Print/pprintreceipt.cpp \
    dlgorders.cpp \
    dlgonebreakfasat.cpp \
    ../Base/baseuid.cpp \
    dlggettext.cpp \
    dlgconnecttoserver.cpp \
    ../Cache/cachebed.cpp \
    ../Cache/cacheroomcategory.cpp \
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
    ../Cache/cachecouponseria.cpp

HEADERS  += rface.h \
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
    baseextendeddialog.h \
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
    ../Cache/cacheactiveroom.h \
    ../Cache/cachebase.h \
    ../Cache/cachecardex.h \
    ../Cache/cachecityledger.h \
    ../Cache/cachecreditcard.h \
    ../Cache/cacheguest.h \
    ../Cache/cacheguesttitle.h \
    ../Cache/cacheinvoiceitem.h \
    ../Cache/cacheone.h \
    ../Cache/cachepaymentmode.h \
    ../Cache/cacheredreservation.h \
    ../Cache/cachereservation.h \
    ../Cache/cachereservestate.h \
    ../Cache/cachereservestatus.h \
    ../Cache/cacherestdishmod.h \
    ../Cache/cacherestdishpart.h \
    ../Cache/cacherestdishtype.h \
    ../Cache/cacheresthall.h \
    ../Cache/cacherestmenu.h \
    ../Cache/cacherestprinters.h \
    ../Cache/cachereststore.h \
    ../Cache/cacheresttable.h \
    ../Cache/cacherights.h \
    ../Cache/cacheroom.h \
    ../Cache/cacheroomarrangment.h \
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
    ../Print/printtax.h \
    ../Controls/eqcombobox.h \
    ../Cache/cachsex.h \
    ../Cache/cachereminderstate.h \
    dlgselecttaxcashmode.h \
    dlgbreakfast.h \
    ../Base/command.h \
    ../Resort/broadcastthread.h \
    dlgbanket.h \
    dlgbanketitems.h \
    ../Base/vauchers.h \
    dlglist.h \
    dlgdate.h \
    ../Controls/edateedit.h \
    ../Base/databaseresult.h \
    dlgvoidback.h \
    ../Print/pprintreceipt.h \
    dlgorders.h \
    dlgonebreakfasat.h \
    ../Base/baseuid.h \
    dlggettext.h \
    dlgconnecttoserver.h \
    ../Cache/cachebed.h \
    ../Cache/cacheroomcategory.h \
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
    ../Cache/cachecouponseria.h

FORMS    += rface.ui \
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
    ../Print/printtax.ui \
    dlgselecttaxcashmode.ui \
    dlgbreakfast.ui \
    dlgbanket.ui \
    dlgbanketitems.ui \
    dlglist.ui \
    dlgdate.ui \
    dlgvoidback.ui \
    dlgorders.ui \
    dlgonebreakfasat.ui \
    dlggettext.ui \
    dlgconnecttoserver.ui \
    dlgcarselection.ui \
    dlgpayment.ui \
    dlgdeptholder.ui \
    dlgprinttaxnew.ui

LIBS += -lVersion

RESOURCES += \
    res.qrc

DISTFILES += \
    stylesheet.qss \
    css

