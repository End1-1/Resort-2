#-------------------------------------------------
#
# Project created by QtCreator 2017-06-04T01:03:26
#
#-------------------------------------------------

QT       += core gui sql network xml printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SmartHotel
TEMPLATE = app

RC_FILE = res.rc

DEFINES += _RESORT_
DEFINES += _HOTEL_
#DEFINES += _WASH_

SOURCES += main.cpp\
    ../Filter/wsalary.cpp \
        mainwindow.cpp \
    login.cpp \
    loginsettings.cpp \
    message.cpp \
    databasesconnections.cpp \
    databaseconnection.cpp \
    databasepasswordchange.cpp \
    wmaindesk.cpp \
    wreservation.cpp \
    wguest.cpp \
    winvoice.cpp \
    wreportgrid.cpp \
    wwelcome.cpp \
    wcontacts.cpp \
    tablemodel.cpp \
    wreportroom.cpp \
    excel.cpp \
    dlgcontact.cpp \
    wnotes.cpp \
    roweditordialog.cpp \
    dlgnotes.cpp \
    about.cpp \
    wguests.cpp \
    wreservations.cpp \
    dlgfiltervalues.cpp \
    dlgroom.cpp \
    wreservationroomtab.cpp \
    dlgchartdaterange.cpp \
    basewidget.cpp \
    baseextendeddialog.cpp \
    ../Base/base.cpp \
    ../Base/basedialog.cpp \
    ../Base/database.cpp \
    ../Base/defines.cpp \
    ../Base/preferences.cpp \
    ../Base/utils.cpp \
    dlgtransferroom.cpp \
    wusers.cpp \
    wusersgroups.cpp \
    dlgusergroup.cpp \
    wroomview.cpp \
    dlgroomview.cpp \
    dlgendofday.cpp \
    wcardex.cpp \
    dlgcardex.cpp \
    dwmaindeskhint.cpp \
    ../Base/command.cpp \
    broadcastthread.cpp \
    dwreservationitemsize.cpp \
    dlgpostingcharges.cpp \
    ../Cache/cacheinvoiceitem.cpp \
    ../Selector/dwselector.cpp \
    ../Cache/cacheactiveroom.cpp \
    ../Cache/cachebase.cpp \
    dlgpaymentsdetails.cpp \
    ../Cache/cachepaymentmode.cpp \
    ../Controls/eqcheckbox.cpp \
    ../Cache/cachevatmode.cpp \
    ../Selector/dwselectorvatmode.cpp \
    ../Selector/dwselectoractiveroom.cpp \
    ../Selector/dwselectorinvoiceitem.cpp \
    ../Cache/cacheguesttitle.cpp \
    ../Selector/dwselectorguesttitle.cpp \
    ../Cache/cachecardex.cpp \
    ../Selector/dwselectorcardex.cpp \
    ../Cache/cachecityledger.cpp \
    ../Selector/dwselectorcityledger.cpp \
    ../RowEditor/reresthall.cpp \
    ../Cache/cacheresthall.cpp \
    ../Selector/dwselectorhall.cpp \
    ../Cache/cacherestmenu.cpp \
    ../Selector/dwselectorrestmenu.cpp \
    ../RowEditor/reresttable.cpp \
    ../Cache/cacheresttable.cpp \
    ../RowEditor/rerestmenunames.cpp \
    ../RowEditor/rerestmenupart.cpp \
    ../Cache/cacherestdishpart.cpp \
    ../Selector/dwselectordishpart.cpp \
    ../RowEditor/rerestdishtype.cpp \
    ../Controls/ecolorlineedit.cpp \
    ../Cache/cacherestdishtype.cpp \
    ../Selector/dwselectordishtype.cpp \
    ../Controls/eqtextedit.cpp \
    ../RowEditor/rerestdish.cpp \
    ../Cache/cacherestprinters.cpp \
    ../Selector/dwselectorrestprinter.cpp \
    ../Cache/cachereststore.cpp \
    ../Selector/dwselectorreststore.cpp \
    ../Cache/cacherestdishmod.cpp \
    ../Selector/dwselectordishmod.cpp \
    ../RowEditor/rerestdishmod.cpp \
    ../RowEditor/rereststore.cpp \
    ../RowEditor/rerestprinter.cpp \
    ../RoomChart/roomscene.cpp \
    ../RoomChart/dayitemdelegate.cpp \
    ../RoomChart/monthitemdelegate.cpp \
    ../RoomChart/roomitemdelegate.cpp \
    ../Cache/cacheroom.cpp \
    ../Cache/cacheone.cpp \
    ../RoomChart/reserverect.cpp \
    ../RoomChart/chardefaults.cpp \
    ../RoomChart/reservewidget.cpp \
    ../Cache/cachereservation.cpp \
    ../RoomChart/reservationinfo.cpp \
    ../RoomChart/dlgreservequickinfo.cpp \
    ../Controls/eqlineedit.cpp \
    ../Controls/trackcontrol.cpp \
    ../Controls/eqdateedit.cpp \
    ../Cache/cachereservestate.cpp \
    ../Cache/cachereservestatus.cpp \
    ../Selector/dwselectorreservestatus.cpp \
    ../Cache/cacheroomarrangment.cpp \
    ../Selector/dwselectorroomarrangment.cpp \
    ../Controls/eqcombobox.cpp \
    ../Controls/cacheforcombobox.cpp \
    ../Selector/dwselectorroom.cpp \
    ../Filter/wfilterbase.cpp \
    ../Filter/ftrackchanges.cpp \
    ../Controls/eqtimeedit.cpp \
    ../Cache/cachetrackingtables.cpp \
    ../Selector/dwselectortrackingtables.cpp \
    ../Cache/cacheusers.cpp \
    ../Selector/dwselectorusers.cpp \
    ../Cache/cacheusersgroups.cpp \
    ../Selector/dwselectorusersgroups.cpp \
    ../RowEditor/reusers.cpp \
    ../Cache/cacheusersstate.cpp \
    ../Cache/cacherights.cpp \
    ../Print/pprintpreview.cpp \
    ../Print/pprintscene.cpp \
    ../Print/ptextrect.cpp \
    ../Cache/cacheguest.cpp \
    ../Selector/dwselectorguest.cpp \
    ../Cache/cachecreditcard.cpp \
    ../Selector/dwselectorcreditcard.cpp \
    ../RowEditor/recreditcard.cpp \
    ../Filter/fcityledgerbalance.cpp \
    dlguserpasswords.cpp \
    dlgadvanceentry.cpp \
    ../Selector/dwselectorpaymentmode.cpp \
    ../Cache/cacheredreservation.cpp \
    ../Selector/dwselectorredreservation.cpp \
    ../RoomChart/roomstate.cpp \
    ../Cache/cacheroomstate.cpp \
    ../Selector/dwselectorroomstate.cpp \
    ../Controls/eqtablewidget.cpp \
    dlginvoicecancelation.cpp \
    dlgraiseuser.cpp \
    wglobaldbconfig.cpp \
    ../Controls/eqspinbox.cpp \
    dlggposorderinfo.cpp \
    taxprint.cpp \
    ../RowEditor/rerestdishcomplex.cpp \
    ../RowEditor/reroomcategory.cpp \
    ../RowEditor/reroombed.cpp \
    ../RowEditor/reroomcategoryrate.cpp \
    ../RowEditor/recustomaction.cpp \
    reroomcategoryraterow.cpp \
    ../Cache/cachebed.cpp \
    ../Selector/dwselectorbed.cpp \
    dlgtransfer.cpp \
    ../Filter/finvoices.cpp \
    waccinvoice.cpp \
    dlgtracking.cpp \
    ../Cache/cachemenudishes.cpp \
    ../Selector/deselectormenudishes.cpp \
    ../Controls/epushbutton.cpp \
    wsynchronize.cpp \
    dlgcreategroupreservation.cpp \
    ../Filter/frestauranttotal.cpp \
    ../Filter/fdailymovement.cpp \
    ../Filter/freservationscommon.cpp \
    ../RoomChart/reservewidgetaction.cpp \
    ../Selector/dwselectorreservestate.cpp \
    ../Print/pimage.cpp \
    dlgdiscount.cpp \
    ../Print/pprintinvoice.cpp \
    ../Filter/fcategorytosell.cpp \
    ../Cache/cachecurrencies.cpp \
    ../RowEditor/recurrency.cpp \
    ../Filter/fforecastoccupancycategory.cpp \
    ../Filter/faccmonthlyreport.cpp \
    dlgtransferinvoiceamount.cpp \
    timerblya.cpp \
    ../Print/dlgprintreservation.cpp \
    ../Cache/cachsex.cpp \
    ../Cache/cachenation.cpp \
    ../Selector/dwselectornation.cpp \
    ../Cache/cacheroomcategory.cpp \
    ../Selector/dwselectorroomcategory.cpp \
    ../Cache/cacheroomview.cpp \
    ../Selector/dwselectorroomview.cpp \
    ../Cache/cachereminderstate.cpp \
    ../Base/logging.cpp \
    ../Print/pprintheader.cpp \
    dlgtaxprintsetup.cpp \
    dlginvoiceprintoption.cpp \
    ../RowEditor/reinvoiceitem.cpp \
    ../Cache/cacheinvoiceitemgroup.cpp \
    ../Selector/dwselectorinvoiceitemgroup.cpp \
    ../Filter/finhouseguest.cpp \
    ../Filter/fexpectedarrivals.cpp \
    ../Controls/edateedit.cpp \
    ../Widgets/wrecheckin.cpp \
    ../Filter/fexpectedarrivalsimple.cpp \
    ../Filter/fcashreportsummary.cpp \
    dlgreceiptvaucher.cpp \
    ../Filter/fcityledgerdetailedbalance.cpp \
    ../RGDoubleClick/rgdoubleclickdelegate.cpp \
    ../RGDoubleClick/rgcityledgerdetailedbalance.cpp \
    ../Filter/fcashreport.cpp \
    ../Cache/cachecardexgroup.cpp \
    ../Selector/dwselectorcardexgroup.cpp \
    ../RowEditor/recityledger.cpp \
    ../Filter/fvauchers.cpp \
    ../Base/vauchers.cpp \
    dlguserrights.cpp \
    ../Selector/dwselectorvaucher.cpp \
    ../Selector/dwtemplateselector.cpp \
    ../Cache/cachevaucher.cpp \
    ../Print/pprintvaucher.cpp \
    ../Selector/dwselectorreservationcardex.cpp \
    dlgreservationremarks.cpp \
    ../Filter/fevents.cpp \
    ../Cache/cacheorderstate.cpp \
    ../Selector/dwselectororderstate.cpp \
    ../Selector/dwselectorresttable.cpp \
    ../Selector/dwselectordish.cpp \
    ../Cache/cachedish.cpp \
    ../Cache/cachedishstate.cpp \
    ../Selector/dwselectordishstate.cpp \
    dlgpretax.cpp \
    dlgprinttaxsideoption.cpp \
    ../Filter/fdailymovementcommon.cpp \
    ../Charts/chsample.cpp \
    dlgsearchinvoice.cpp \
    ../RowEditor/rebanquetcomment.cpp \
    ../Filter/fcityledgerbalanceextended.cpp \
    ../Filter/fcallhistory.cpp \
    dlgtaxback.cpp \
    ../Filter/fcardexsales.cpp \
    dlgadvance.cpp \
    ../Filter/fcommonfilterbydate.cpp \
    dlgprintrandomtax.cpp \
    dlgtaxreference.cpp \
    ../Widgets/wvauchereditor.cpp \
    ../Widgets/wsyncinvoices.cpp \
    ../RowEditor/recomplimentarycomment.cpp \
    ../Base/databaseresult.cpp \
    dlgexportsinglevaucher.cpp \
    ../Controls/etoolbarbutton.cpp \
    dlgwelcomebuttonconfig.cpp \
    ../Widgets/wreportbuilder.cpp \
    ../Widgets/wremarks.cpp \
    ../Controls/efilterlineedit.cpp \
    ../Print/pprintreceipt.cpp \
    ../Widgets/whotelstatus.cpp \
    dlgclinitialbalance.cpp \
    ../Filter/flistsourcereserve.cpp \
    ../Filter/fyearlyfinancialreport.cpp \
    ../Widgets/wcustomreports.cpp \
    dlgreportbuildergroups.cpp \
    ../Controls/ecomboboxcompleter.cpp \
    ../Widgets/wreportssetold.cpp \
    ../Controls/eradiobutton.cpp \
    ../Controls/ecombomonth.cpp \
    ../Base/baseuid.cpp \
    dlgviewinvoicecorrections.cpp \
    dlgperemovereason.cpp \
    dlgutils.cpp \
    dlgdishhistory.cpp \
    ../Cache/cachecheckoutinvoice.cpp \
    ../Selector/dwselectorcheckoutinvoice.cpp \
    dlgremotinvoices.cpp \
    dlggroupparams.cpp \
    dlgopengrouporsingle.cpp \
    dlgexportother.cpp \
    dlgreserveconfirmstatus.cpp \
    ../Filter/freservaionremarks.cpp \
    dlgchangeexportvaucher.cpp \
    dlggroupreservationfuck.cpp \
    dlgsimpleselectroom.cpp \
    fnousedadvance.cpp \
    ../RoomChart/dlgooroomprop.cpp \
    fonlinerest.cpp \
    ../Cache/cachereservegroup.cpp \
    ../Selector/dwselectorreservationgroup.cpp \
    ../Filter/freservegroups.cpp \
    ../RowEditor/recardexgroup.cpp \
    ../RowEditor/reguesttitle.cpp \
    ../RowEditor/renationalityfile.cpp \
    dlghelp.cpp \
    ../Filter/froomstate.cpp \
    ../Filter/fcanceledreservations.cpp \
    ../Widgets/wweb.cpp \
    ../Filter/freservebycreate.cpp \
    ../Widgets/storedoc.cpp \
    ../RowEditor/restorepartner.cpp \
    ../Cache/cachestorepartners.cpp \
    ../Selector/dwselectorstorepartners.cpp \
    ../Cache/cachestoredoc.cpp \
    ../Cache/cachestoredocstate.cpp \
    ../Selector/dwselectorstoredoctype.cpp \
    ../Selector/dwselectorstoredocstate.cpp \
    ../Filter/fstoredocs.cpp \
    ../Filter/fmaterialsinstore.cpp \
    ../Filter/fstoremovement.cpp \
    ../RowEditor/recoupon.cpp \
    ../Cache/cachecoupontype.cpp \
    dlgcalculateoutputofrestaurant.cpp \
    wwelcomerest.cpp \
    ../Base/defrest.cpp \
    dlgback.cpp \
    fonlinebreakfast.cpp \
    ../Filter/fmonthlyoccperc.cpp \
    fnatbyperiod.cpp \
    dlgcl.cpp \
    dlgpaymentmode.cpp \
    dlgchangeclofvaucher.cpp \
    dlgnoshow.cpp \
    ../Filter/fnoshowcancelationfee.cpp \
    ../Filter/frestsalebystore.cpp \
    dlgcheckadvancebeforecancel.cpp \
    dlgrefundvaucher.cpp \
    dlgeditservtax.cpp \
    ../Cache/cachetaxmap.cpp \
    dlgreserveshortinfo.cpp \
    dlgnewvaucheroption.cpp \
    ../Filter/fallguestindaterange.cpp \
    ../RowEditor/remodelofcars.cpp \
    ../RowEditor/recarclient.cpp \
    ../Cache/cachecar.cpp \
    ../Selector/dwselectorcar.cpp \
    ../Cache/cacherestdebtholder.cpp \
    ../Filter/freportbypayment.cpp \
    ../Cache/cachedocpayment.cpp \
    fpartnersdebt.cpp \
    dlgcouponsales.cpp \
    fcouponsales.cpp \
    ../RowEditor/recouponseria.cpp \
    ../Cache/cachecouponseria.cpp \
    ../Selector/dwselectorcouponseria.cpp \
    ../Cache/cachecashdesk.cpp \
    ../Selector/dwselectorcashdesk.cpp \
    ../Widgets/wcashentry.cpp \
    ../Cache/cachecashdoc.cpp \
    ../Filter/fcash.cpp \
    ../RowEditor/recashdesk.cpp \
    ../Filter/fstoreentry.cpp \
    ../Widgets/wstoreentry.cpp \
    dlgpartnerpayment.cpp \
    dlgcashop.cpp \
    ../Selector/dwselectorcashdoctype.cpp \
    ../Filter/fcouponstatistics.cpp \
    ../Filter/fdebtofcostumers.cpp \
    ../Cache/cachedebtcostumer.cpp \
    ../Selector/dwselectordebtcostumer.cpp \
    ../Filter/fbreakfast.cpp \
    ../Filter/fsalary.cpp \
    ../Filter/fdiscountreport.cpp \
    ../Filter/fsalesbycar.cpp \
    ../Filter/fdiscounttotal.cpp \
    dlgtaxback2.cpp \
    ../Filter/fdebtholders.cpp \
    dlgpostbreakfast.cpp \
    ../Controls/eqdoubleedit.cpp \
    ../Cache/cacherestfullmenu.cpp \
    ../Selector/dwselectorrestfullmenu.cpp \
    dlghouseitem.cpp \
    ../Filter/fhouseitems.cpp \
    dlgdebtpay.cpp \
    dlgcityadvance.cpp \
    ../Vouchers/basevoucher.cpp \
    ../Vouchers/voucher_tc.cpp \
    ../Cache/cachecladvance.cpp \
    ../Selector/dwselectorcladvance.cpp \
    ../Filter/fcladvance.cpp \
    ../Print/pprintcheckin.cpp \
    ../Filter/fcitytrayledger.cpp \
    ../Filter/finhousedetailbalance.cpp \
    ../Filter/favailablerooms.cpp \
    ../Widgets/wdate2.cpp \
    ../Base/paymentmode.cpp \
    dlgconfiggrid.cpp \
    ../Base/appconfig.cpp \
    ../Cache/cacheroominventory.cpp \
    ../Selector/dwselectorroominventory.cpp \
    ../RowEditor/reroominventory.cpp \
    ../RowEditor/reroominventorystate.cpp \
    ../Cache/cacheroominventorystate.cpp \
    ../Print/pexportinvoicetoexcel.cpp \
    dlggroupreserveoption.cpp \
    ../Cache/cachegroupreservations.cpp \
    ../Selector/dwselectorgroupreservation.cpp \
    ../GridOptionWidgets/gowidget.cpp \
    ../GridOptionWidgets/godaily.cpp \
    ../Base/demo.cpp \
    ../Vouchers/voucher_av.cpp \
    dlgexport.cpp \
    goexpextedarrivals.cpp \
    smtp.cpp \
    dlgemail.cpp \
    ../Print/pprintstoredoc.cpp \
    ../Print/pprintstoreentry.cpp \
    ../Filter/guestcheckin.cpp \
    ../Filter/flengthofstay.cpp \
    ../Filter/fexportreservation.cpp \
    dlgreservationguests.cpp \
    ../Base/doubledatabase.cpp \
    ../Cache/cachediscounttype.cpp \
    ../Selector/dwselectordiscounttype.cpp \
    ../Filter/fdiscountfullstat.cpp \
    ../Cache/cacheunit.cpp \
    ../Selector/dwselectorunit.cpp \
    ftstorereport.cpp \
    ../Filter/fbalanceoncard.cpp

HEADERS  += mainwindow.h \
    ../Filter/wsalary.h \
    login.h \
    loginsettings.h \
    message.h \
    databasesconnections.h \
    databaseconnection.h \
    databasepasswordchange.h \
    wmaindesk.h \
    wreservation.h \
    wguest.h \
    winvoice.h \
    wreportgrid.h \
    wwelcome.h \
    wcontacts.h \
    tablemodel.h \
    wreportroom.h \
    excel.h \
    dlgcontact.h \
    wnotes.h \
    roweditordialog.h \
    dlgnotes.h \
    about.h \
    rc.h \
    wguests.h \
    wreservations.h \
    dlgfiltervalues.h \
    dlgroom.h \
    wreservationroomtab.h \
    dlgchartdaterange.h \
    basewidget.h \
    baseextendeddialog.h \
    ../Base/base.h \
    ../Base/basedialog.h \
    ../Base/database.h \
    ../Base/defines.h \
    ../Base/preferences.h \
    ../Base/utils.h \
    wusers.h \
    wusersgroups.h \
    dlgusergroup.h \
    wroomview.h \
    dlgroomview.h \
    dlgendofday.h \
    wcardex.h \
    dlgcardex.h \
    dwmaindeskhint.h \
    ../Base/command.h \
    broadcastthread.h \
    dwreservationitemsize.h \
    dlgpostingcharges.h \
    ../Cache/cachebase.h \
    ../Cache/cacheinvoiceitem.h \
    ../Selector/dwselector.h \
    ../Cache/cacheactiveroom.h \
    dlgpaymentsdetails.h \
    ../Cache/cachepaymentmode.h \
    ../Controls/eqcheckbox.h \
    ../Cache/cachevatmode.h \
    ../Selector/dwselectorvatmode.h \
    ../Selector/dwselectoractiveroom.h \
    ../Selector/dwselectorinvoiceitem.h \
    ../Cache/cacheguesttitle.h \
    ../Selector/dwselectorguesttitle.h \
    ../Cache/cachecardex.h \
    ../Selector/dwselectorcardex.h \
    ../Cache/cachecityledger.h \
    ../Selector/dwselectorcityledger.h \
    ../RowEditor/reresthall.h \
    ../Cache/cacheresthall.h \
    ../Selector/dwselectorhall.h \
    ../Cache/cacherestmenu.h \
    ../Selector/dwselectorrestmenu.h \
    ../RowEditor/reresttable.h \
    ../Cache/cacheresttable.h \
    ../RowEditor/rerestmenunames.h \
    ../RowEditor/rerestmenupart.h \
    ../Cache/cacherestdishpart.h \
    ../Selector/dwselectordishpart.h \
    ../RowEditor/rerestdishtype.h \
    ../Controls/ecolorlineedit.h \
    ../Cache/cacherestdishtype.h \
    ../Selector/dwselectordishtype.h \
    ../Controls/eqtextedit.h \
    ../RowEditor/rerestdish.h \
    ../Cache/cacherestprinters.h \
    ../Selector/dwselectorrestprinter.h \
    ../Cache/cachereststore.h \
    ../Selector/dwselectorreststore.h \
    ../Cache/cacherestdishmod.h \
    ../Selector/dwselectordishmod.h \
    ../RowEditor/rerestdishmod.h \
    ../RowEditor/rereststore.h \
    ../RowEditor/rerestprinter.h \
    ../RoomChart/roomscene.h \
    ../RoomChart/dayitemdelegate.h \
    ../RoomChart/chardefaults.h \
    ../RoomChart/monthitemdelegate.h \
    ../RoomChart/roomitemdelegate.h \
    ../Cache/cacheroom.h \
    ../Cache/cacheone.h \
    ../RoomChart/reserverect.h \
    ../RoomChart/reservewidget.h \
    ../Cache/cachereservation.h \
    ../RoomChart/reservationinfo.h \
    ../RoomChart/dlgreservequickinfo.h \
    ../Controls/eqlineedit.h \
    ../Controls/trackcontrol.h \
    ../Controls/eqdateedit.h \
    ../Cache/cachereservestate.h \
    ../Cache/cachereservestatus.h \
    ../Selector/dwselectorreservestatus.h \
    ../Cache/cacheroomarrangment.h \
    ../Selector/dwselectorroomarrangment.h \
    ../Controls/eqcombobox.h \
    ../Controls/cacheforcombobox.h \
    ../Selector/dwselectorroom.h \
    ../Filter/wfilterbase.h \
    ../Filter/ftrackchanges.h \
    ../Controls/eqtimeedit.h \
    ../Cache/cachetrackingtables.h \
    ../Selector/dwselectortrackingtables.h \
    ../Cache/cacheusers.h \
    ../Selector/dwselectorusers.h \
    ../Cache/cacheusersgroups.h \
    ../Selector/dwselectorusersgroups.h \
    ../RowEditor/reusers.h \
    ../Cache/cacheusersstate.h \
    ../Cache/cacherights.h \
    ../Print/pprintpreview.h \
    ../Print/pprintscene.h \
    ../Print/ptextrect.h \
    ../Print/pdefaults.h \
    ../Cache/cacheguest.h \
    ../Selector/dwselectorguest.h \
    ../Cache/cachecreditcard.h \
    ../Selector/dwselectorcreditcard.h \
    ../RowEditor/recreditcard.h \
    ../Filter/fcityledgerbalance.h \
    dlguserpasswords.h \
    dlgadvanceentry.h \
    ../Selector/dwselectorpaymentmode.h \
    ../Cache/cacheredreservation.h \
    ../Selector/dwselectorredreservation.h \
    ../RoomChart/roomstate.h \
    ../Cache/cacheroomstate.h \
    ../Selector/dwselectorroomstate.h \
    ../Controls/eqtablewidget.h \
    dlginvoicecancelation.h \
    dlgraiseuser.h \
    wglobaldbconfig.h \
    ../Controls/eqspinbox.h \
    dlggposorderinfo.h \
    taxprint.h \
    ../RowEditor/rerestdishcomplex.h \
    ../RowEditor/reroomcategory.h \
    ../RowEditor/reroombed.h \
    ../RowEditor/reroomcategoryrate.h \
    ../RowEditor/recustomaction.h \
    reroomcategoryraterow.h \
    ../Cache/cachebed.h \
    ../Selector/dwselectorbed.h \
    dlgtransfer.h \
    ../Filter/finvoices.h \
    waccinvoice.h \
    dlgtracking.h \
    ../Cache/cachemenudishes.h \
    ../Selector/deselectormenudishes.h \
    ../Controls/epushbutton.h \
    wsynchronize.h \
    dlgcreategroupreservation.h \
    ../Filter/frestauranttotal.h \
    ../Filter/fdailymovement.h \
    ../Filter/freservationscommon.h \
    ../RoomChart/reservewidgetaction.h \
    ../Selector/dwselectorreservestate.h \
    ../Print/pimage.h \
    dlgdiscount.h \
    ../Print/pprintinvoice.h \
    ../Filter/fcategorytosell.h \
    ../Cache/cachecurrencies.h \
    ../RowEditor/recurrency.h \
    ../Filter/fforecastoccupancycategory.h \
    ../Filter/faccmonthlyreport.h \
    dlgtransferinvoiceamount.h \
    timerblya.h \
    ../Print/dlgprintreservation.h \
    ../Cache/cachsex.h \
    ../Cache/cachenation.h \
    ../Selector/dwselectornation.h \
    ../Cache/cacheroomcategory.h \
    ../Selector/dwselectorroomcategory.h \
    ../Cache/cacheroomview.h \
    ../Selector/dwselectorroomview.h \
    ../Cache/cachereminderstate.h \
    ../Base/logging.h \
    ../Print/pprintheader.h \
    dlgtaxprintsetup.h \
    dlginvoiceprintoption.h \
    ../RowEditor/reinvoiceitem.h \
    ../Cache/cacheinvoiceitemgroup.h \
    ../Selector/dwselectorinvoiceitemgroup.h \
    ../Filter/finhouseguest.h \
    ../Filter/fexpectedarrivals.h \
    ../Controls/edateedit.h \
    ../Widgets/wrecheckin.h \
    ../Filter/fexpectedarrivalsimple.h \
    ../Filter/fcashreportsummary.h \
    dlgreceiptvaucher.h \
    ../Filter/fcityledgerdetailedbalance.h \
    ../RGDoubleClick/rgdoubleclickdelegate.h \
    ../RGDoubleClick/rgcityledgerdetailedbalance.h \
    ../Filter/fcashreport.h \
    ../Cache/cachecardexgroup.h \
    ../Selector/dwselectorcardexgroup.h \
    ../RowEditor/recityledger.h \
    ../Base/vauchers.h \
    ../Filter/fvauchers.h \
    dlgtransferroom.h \
    dlguserrights.h \
    ../Selector/dwselectorvaucher.h \
    ../Selector/dwtemplateselector.h \
    ../Cache/cachevaucher.h \
    ../Print/pprintvaucher.h \
    ../Selector/dwselectorreservationcardex.h \
    dlgreservationremarks.h \
    ../Filter/fevents.h \
    ../Cache/cacheorderstate.h \
    ../Selector/dwselectororderstate.h \
    ../Selector/dwselectorresttable.h \
    ../Selector/dwselectordish.h \
    ../Cache/cachedish.h \
    ../Cache/cachedishstate.h \
    ../Selector/dwselectordishstate.h \
    dlgpretax.h \
    dlgprinttaxsideoption.h \
    ../Filter/fdailymovementcommon.h \
    ../Charts/chsample.h \
    dlgsearchinvoice.h \
    ../RowEditor/rebanquetcomment.h \
    ../Filter/fcityledgerbalanceextended.h \
    ../Filter/fcallhistory.h \
    dlgtaxback.h \
    ../Filter/fcardexsales.h \
    dlgadvance.h \
    ../Filter/fcommonfilterbydate.h \
    dlgprintrandomtax.h \
    dlgtaxreference.h \
    ../Widgets/wvauchereditor.h \
    ../Widgets/wsyncinvoices.h \
    ../RowEditor/recomplimentarycomment.h \
    ../Base/databaseresult.h \
    dlgexportsinglevaucher.h \
    ../Controls/etoolbarbutton.h \
    dlgwelcomebuttonconfig.h \
    ../Widgets/wreportbuilder.h \
    ../Widgets/wremarks.h \
    ../Controls/efilterlineedit.h \
    ../Print/pprintreceipt.h \
    ../Widgets/whotelstatus.h \
    dlgclinitialbalance.h \
    ../Filter/flistsourcereserve.h \
    ../Filter/fyearlyfinancialreport.h \
    ../Widgets/wcustomreports.h \
    dlgreportbuildergroups.h \
    ../Controls/ecomboboxcompleter.h \
    ../Widgets/wreportssetold.h \
    ../Controls/eradiobutton.h \
    ../Controls/ecombomonth.h \
    ../Base/baseuid.h \
    dlgviewinvoicecorrections.h \
    dlgperemovereason.h \
    dlgutils.h \
    dlgdishhistory.h \
    ../Cache/cachecheckoutinvoice.h \
    ../Selector/dwselectorcheckoutinvoice.h \
    dlgremotinvoices.h \
    dlggroupparams.h \
    dlgopengrouporsingle.h \
    dlgexportother.h \
    dlgreserveconfirmstatus.h \
    ../Filter/freservaionremarks.h \
    dlgchangeexportvaucher.h \
    dlggroupreservationfuck.h \
    dlgsimpleselectroom.h \
    fnousedadvance.h \
    ../RoomChart/dlgooroomprop.h \
    fonlinerest.h \
    ../Cache/cachereservegroup.h \
    ../Selector/dwselectorreservationgroup.h \
    ../Filter/freservegroups.h \
    ../RowEditor/recardexgroup.h \
    ../RowEditor/reguesttitle.h \
    ../RowEditor/renationalityfile.h \
    dlghelp.h \
    ../Filter/froomstate.h \
    ../Filter/fcanceledreservations.h \
    ../Widgets/wweb.h \
    ../Filter/freservebycreate.h \
    ../Widgets/storedoc.h \
    ../RowEditor/restorepartner.h \
    ../Cache/cachestorepartners.h \
    ../Selector/dwselectorstorepartners.h \
    ../Cache/cachestoredoc.h \
    ../Cache/cachestoredocstate.h \
    ../Selector/dwselectorstoredoctype.h \
    ../Selector/dwselectorstoredocstate.h \
    ../Base/defstore.h \
    ../Filter/fstoredocs.h \
    ../Filter/fmaterialsinstore.h \
    ../Filter/fstoremovement.h \
    ../RowEditor/recoupon.h \
    ../Cache/cachecoupontype.h \
    dlgcalculateoutputofrestaurant.h \
    wwelcomerest.h \
    ../Base/defrest.h \
    dlgback.h \
    fonlinebreakfast.h \
    ../Filter/fmonthlyoccperc.h \
    fnatbyperiod.h \
    dlgcl.h \
    dlgpaymentmode.h \
    dlgchangeclofvaucher.h \
    dlgnoshow.h \
    ../Filter/fnoshowcancelationfee.h \
    ../Filter/frestsalebystore.h \
    dlgcheckadvancebeforecancel.h \
    dlgrefundvaucher.h \
    dlgeditservtax.h \
    ../Cache/cachetaxmap.h \
    dlgreserveshortinfo.h \
    dlgnewvaucheroption.h \
    ../Filter/fallguestindaterange.h \
    ../RowEditor/remodelofcars.h \
    ../RowEditor/recarclient.h \
    ../Cache/cachecar.h \
    ../Selector/dwselectorcar.h \
    ../Cache/cacherestdebtholder.h \
    ../Filter/freportbypayment.h \
    ../Cache/cachedocpayment.h \
    fpartnersdebt.h \
    dlgcouponsales.h \
    fcouponsales.h \
    ../RowEditor/recouponseria.h \
    ../Cache/cachecouponseria.h \
    ../Selector/dwselectorcouponseria.h \
    ../Cache/cachecashdesk.h \
    ../Selector/dwselectorcashdesk.h \
    ../Widgets/wcashentry.h \
    ../Cache/cachecashdoc.h \
    ../Filter/fcash.h \
    ../RowEditor/recashdesk.h \
    ../Filter/fstoreentry.h \
    ../Widgets/wstoreentry.h \
    dlgpartnerpayment.h \
    dlgcashop.h \
    ../Selector/dwselectorcashdoctype.h \
    ../Filter/fcouponstatistics.h \
    ../Filter/fdebtofcostumers.h \
    ../Cache/cachedebtcostumer.h \
    ../Selector/dwselectordebtcostumer.h \
    ../Filter/fbreakfast.h \
    ../Filter/fsalary.h \
    ../Filter/fdiscountreport.h \
    ../Filter/fsalesbycar.h \
    ../Filter/fdiscounttotal.h \
    dlgtaxback2.h \
    ../Filter/fdebtholders.h \
    dlgpostbreakfast.h \
    ../Controls/eqdoubleedit.h \
    ../Cache/cacherestfullmenu.h \
    ../Selector/dwselectorrestfullmenu.h \
    dlghouseitem.h \
    ../Filter/fhouseitems.h \
    dlgdebtpay.h \
    dlgcityadvance.h \
    ../Vouchers/basevoucher.h \
    ../Vouchers/voucher_tc.h \
    ../Cache/cachecladvance.h \
    ../Selector/dwselectorcladvance.h \
    ../Filter/fcladvance.h \
    ../Print/pprintcheckin.h \
    ../Filter/fcitytrayledger.h \
    ../Filter/finhousedetailbalance.h \
    ../Filter/favailablerooms.h \
    ../Widgets/wdate2.h \
    ../Base/paymentmode.h \
    dlgconfiggrid.h \
    ../Base/appconfig.h \
    ../Cache/cacheroominventory.h \
    ../Selector/dwselectorroominventory.h \
    ../RowEditor/reroominventory.h \
    ../RowEditor/reroominventorystate.h \
    ../Cache/cacheroominventorystate.h \
    ../Cache/cacheroominventorystate.h \
    ../Print/pexportinvoicetoexcel.h \
    dlggroupreserveoption.h \
    ../Cache/cachegroupreservations.h \
    ../Selector/dwselectorgroupreservation.h \
    ../GridOptionWidgets/gowidget.h \
    ../GridOptionWidgets/godaily.h \
    ../Base/demo.h \
    ../Vouchers/voucher_av.h \
    dlgexport.h \
    goexpextedarrivals.h \
    smtp.h \
    dlgemail.h \
    ../Print/pprintstoredoc.h \
    ../Print/pprintstoreentry.h \
    ../Filter/guestcheckin.h \
    ../Filter/flengthofstay.h \
    ../Filter/fexportreservation.h \
    dlgreservationguests.h \
    ../Base/doubledatabase.h \
    ../Cache/cachediscounttype.h \
    ../Selector/dwselectordiscounttype.h \
    ../Filter/fdiscountfullstat.h \
    ../Cache/cacheunit.h \
    ../Selector/dwselectorunit.h \
    ftstorereport.h \
    ../Filter/fbalanceoncard.h

FORMS    += mainwindow.ui \
    ../Filter/wsalary.ui \
    login.ui \
    loginsettings.ui \
    message.ui \
    databasesconnections.ui \
    databaseconnection.ui \
    databasepasswordchange.ui \
    wmaindesk.ui \
    wreservation.ui \
    wguest.ui \
    winvoice.ui \
    wreportgrid.ui \
    wwelcome.ui \
    dlgcontact.ui \
    dlgnotes.ui \
    about.ui \
    dlgfiltervalues.ui \
    dlgroom.ui \
    wreservationroomtab.ui \
    dlgchartdaterange.ui \
    dlgusergroup.ui \
    dlgroomview.ui \
    dlgendofday.ui \
    dlgcardex.ui \
    dwmaindeskhint.ui \
    dwreservationitemsize.ui \
    dlgpostingcharges.ui \
    ../Selector/dwselector.ui \
    dlgpaymentsdetails.ui \
    ../RowEditor/reresthall.ui \
    ../RowEditor/reresttable.ui \
    ../RowEditor/rerestmenunames.ui \
    ../RowEditor/rerestmenupart.ui \
    ../RowEditor/rerestdishtype.ui \
    ../RowEditor/rerestdish.ui \
    ../RowEditor/rerestdishmod.ui \
    ../RowEditor/rereststore.ui \
    ../RowEditor/rerestprinter.ui \
    ../RoomChart/reservewidget.ui \
    ../RoomChart/dlgreservequickinfo.ui \
    ../Filter/ftrackchanges.ui \
    ../RowEditor/reusers.ui \
    ../Print/pprintpreview.ui \
    ../RowEditor/recreditcard.ui \
    ../Filter/fcityledgerbalance.ui \
    dlguserpasswords.ui \
    dlgadvanceentry.ui \
    ../RoomChart/roomstate.ui \
    dlginvoicecancelation.ui \
    dlgraiseuser.ui \
    wglobaldbconfig.ui \
    dlggposorderinfo.ui \
    ../RowEditor/rerestdishcomplex.ui \
    ../RowEditor/reroomcategory.ui \
    ../RowEditor/reroombed.ui \
    ../RowEditor/reroomcategoryrate.ui \
    reroomcategoryraterow.ui \
    dlgtransfer.ui \
    ../Filter/finvoices.ui \
    waccinvoice.ui \
    dlgtracking.ui \
    wsynchronize.ui \
    dlgcreategroupreservation.ui \
    ../Filter/frestauranttotal.ui \
    ../Filter/fdailymovement.ui \
    ../Filter/freservationscommon.ui \
    ../RoomChart/reservewidgetaction.ui \
    dlgdiscount.ui \
    ../Filter/fcategorytosell.ui \
    ../RowEditor/recurrency.ui \
    ../Filter/fforecastoccupancycategory.ui \
    ../Filter/faccmonthlyreport.ui \
    dlgtransferinvoiceamount.ui \
    ../Print/dlgprintreservation.ui \
    dlgtaxprintsetup.ui \
    dlginvoiceprintoption.ui \
    ../RowEditor/reinvoiceitem.ui \
    ../Filter/finhouseguest.ui \
    ../Filter/fexpectedarrivals.ui \
    ../Widgets/wrecheckin.ui \
    ../Filter/fexpectedarrivalsimple.ui \
    ../Filter/fcashreportsummary.ui \
    dlgreceiptvaucher.ui \
    ../Filter/fcityledgerdetailedbalance.ui \
    ../Filter/fcashreport.ui \
    ../RowEditor/recityledger.ui \
    ../Filter/fvauchers.ui \
    dlgtransferroom.ui \
    dlguserrights.ui \
    dlgreservationremarks.ui \
    ../Filter/fevents.ui \
    dlgpretax.ui \
    dlgprinttaxsideoption.ui \
    ../Filter/fdailymovementcommon.ui \
    ../Charts/chsample.ui \
    dlgsearchinvoice.ui \
    ../RowEditor/rebanquetcomment.ui \
    ../Filter/fcityledgerbalanceextended.ui \
    ../Filter/fcallhistory.ui \
    dlgtaxback.ui \
    ../Filter/fcardexsales.ui \
    dlgadvance.ui \
    ../Filter/fcommonfilterbydate.ui \
    dlgprintrandomtax.ui \
    dlgtaxreference.ui \
    ../Widgets/wvauchereditor.ui \
    ../Widgets/wsyncinvoices.ui \
    ../RowEditor/recomplimentarycomment.ui \
    dlgexportsinglevaucher.ui \
    dlgwelcomebuttonconfig.ui \
    ../Widgets/wreportbuilder.ui \
    ../Controls/efilterlineedit.ui \
    ../Widgets/whotelstatus.ui \
    dlgclinitialbalance.ui \
    ../Filter/flistsourcereserve.ui \
    ../Filter/fyearlyfinancialreport.ui \
    ../Widgets/wcustomreports.ui \
    dlgreportbuildergroups.ui \
    ../Widgets/wreportssetold.ui \
    dlgviewinvoicecorrections.ui \
    dlgposremoveoption.ui \
    dlgperemovereason.ui \
    dlgutils.ui \
    dlgdishhistory.ui \
    dlgremotinvoices.ui \
    dlggroupparams.ui \
    dlgopengrouporsingle.ui \
    dlgexportother.ui \
    dlgreserveconfirmstatus.ui \
    ../Filter/freservaionremarks.ui \
    dlgchangeexportvaucher.ui \
    dlggroupreservationfuck.ui \
    dlgsimpleselectroom.ui \
    fnousedadvance.ui \
    ../RoomChart/dlgooroomprop.ui \
    fonlinerest.ui \
    ../Filter/freservegroups.ui \
    ../RowEditor/recardexgroup.ui \
    ../RowEditor/reguesttitle.ui \
    ../RowEditor/renationalityfile.ui \
    dlghelp.ui \
    ../Filter/froomstate.ui \
    ../Filter/fcanceledreservations.ui \
    ../Widgets/wweb.ui \
    ../Filter/freservebycreate.ui \
    ../Widgets/storedoc.ui \
    ../RowEditor/restorepartner.ui \
    ../Filter/fstoredocs.ui \
    ../Filter/fmaterialsinstore.ui \
    ../Filter/fstoremovement.ui \
    ../RowEditor/recoupon.ui \
    dlgcalculateoutputofrestaurant.ui \
    wwelcomerest.ui \
    dlgback.ui \
    fonlinebreakfast.ui \
    ../Filter/fmonthlyoccperc.ui \
    fnatbyperiod.ui \
    dlgcl.ui \
    dlgpaymentmode.ui \
    dlgchangeclofvaucher.ui \
    dlgnoshow.ui \
    ../Filter/fnoshowcancelationfee.ui \
    ../Filter/frestsalebystore.ui \
    dlgcheckadvancebeforecancel.ui \
    dlgrefundvaucher.ui \
    dlgeditservtax.ui \
    dlgreserveshortinfo.ui \
    dlgnewvaucheroption.ui \
    ../Filter/fallguestindaterange.ui \
    ../RowEditor/remodelofcars.ui \
    ../RowEditor/recarclient.ui \
    ../Filter/freportbypayment.ui \
    fpartnersdebt.ui \
    dlgcouponsales.ui \
    fcouponsales.ui \
    ../RowEditor/recouponseria.ui \
    ../Widgets/wcashentry.ui \
    ../Filter/fcash.ui \
    ../RowEditor/recashdesk.ui \
    ../Filter/fstoreentry.ui \
    ../Widgets/wstoreentry.ui \
    dlgpartnerpayment.ui \
    dlgcashop.ui \
    ../Filter/fcouponstatistics.ui \
    ../Filter/fdebtofcostumers.ui \
    ../Filter/fbreakfast.ui \
    ../Filter/fsalary.ui \
    ../Filter/fdiscountreport.ui \
    ../Filter/fsalesbycar.ui \
    ../Filter/fdiscounttotal.ui \
    dlgtaxback2.ui \
    ../Filter/fdebtholders.ui \
    dlgpostbreakfast.ui \
    dlghouseitem.ui \
    ../Filter/fhouseitems.ui \
    dlgdebtpay.ui \
    dlgcityadvance.ui \
    ../Filter/fcladvance.ui \
    ../Filter/fcitytrayledger.ui \
    ../Filter/finhousedetailbalance.ui \
    ../Filter/favailablerooms.ui \
    ../Widgets/wdate2.ui \
    dlgconfiggrid.ui \
    ../RowEditor/reroominventory.ui \
    ../RowEditor/reroominventorystate.ui \
    dlggroupreserveoption.ui \
    ../GridOptionWidgets/godaily.ui \
    dlgexport.ui \
    goexpextedarrivals.ui \
    dlgemail.ui \
    ../Filter/guestcheckin.ui \
    ../Filter/flengthofstay.ui \
    ../Filter/fexportreservation.ui \
    dlgreservationguests.ui \
    ../Filter/fdiscountfullstat.ui \
    ftstorereport.ui \
    ../Filter/fbalanceoncard.ui

RESOURCES += \
    res.qrc

LIBS += -lVersion
LIBS += -LC:/OpenSSL-Win32/lib
LIBS += -lopenssl
LIBS += -llibcrypto
LIBS +=  -lwsock32

DISTFILES += \
    last_cache.txt

INCLUDEPATH += $$PWD/../Base
INCLUDEPATH += $$PWD/../Cache
INCLUDEPATH += $$PWD/../Selector
INCLUDEPATH += $$PWD/../Controls
INCLUDEPATH += $$PWD/../RowEditor
INCLUDEPATH += $$PWD/../RoomChart
INCLUDEPATH += $$PWD/../Filter
INCLUDEPATH += $$PWD/../Print
INCLUDEPATH += $$PWD/../Widgets
INCLUDEPATH += $$PWD/../RGDoubleClick
INCLUDEPATH += $$PWD/../GridOptionWidgets
INCLUDEPATH += $$PWD/../Vouchers
INCLUDEPATH += /OpenSSL-Win32/include
INCLUDEPATH += /OpenSSL-Win32/include/openssl
INCLUDEPATH += /Projects/NewTax

DEFINES += _ORGANIZATION_=\\\"SmartHotel\\\"
DEFINES += _APPLICATION_=\\\"SmartHotel\\\"
DEFINES += _DBDRIVER_=\\\"QMYSQL\\\"
