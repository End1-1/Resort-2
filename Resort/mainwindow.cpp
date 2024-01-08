#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "preferences.h"
#include "login.h"
#include "fdebtholders.h"
#include "appconfig.h"
#include "loginsettings.h"
#include "databasesconnections.h"
#include "message.h"
#include "wreportgrid.h"
#include "dlgonetwoall.h"
#include "recoupon.h"
#include "freportbypayment.h"
#include "fdiscountreport.h"
#include "recouponseria.h"
#include "inventorization.h"
#include "ftstorereport.h"
#include "fdiscounttotal.h"
#include "fattendance.h"
#include "fearningswash.h"
#include "fsalesbycar.h"
#include "fbalanceoncard.h"
#include "fcouponsservice.h"
#include "fcash.h"
#include "goodsmovement.h"
#include "fstoreentry.h"
#include "fpartnersdebt.h"
#include "fcouponsales.h"
#include "fdebtholders.h"
#include "wstoreentry.h"
#include "fdebtofcostumers.h"
#include "about.h"
#include "baseuid.h"
#include "remodelofcars.h"
#include "frestsalebystore.h"
#include "recashdesk.h"
#include "fcouponstatistics.h"
#include "fasexportsale.h"
#include "ecomboboxcompleter.h"
#include "fonlinebreakfast.h"
#include "wfilterbase.h"
#include "recarclient.h"
#include "fstoredocs.h"
#include "wcustomreports.h"
#include "wcontacts.h"
#include "wusers.h"
#include "fmaterialsinstore.h"
#include "fstoremovement.h"
#include "wusersgroups.h"
#include "storedoc.h"
#include "fcoupondocuments.h"
#include "cachebase.h"
#include "fsalarybyemployes.h"
#include "fonlinerest.h"
#include "reresthall.h"
#include "reresttable.h"
#include "rerestmenunames.h"
#include "wreportssetold.h"
#include "rerestmenupart.h"
#include "rerestdishtype.h"
#include "rerestdish.h"
#include "rerestdishmod.h"
#include "rereststore.h"
#include "rerestprinter.h"
#include "recreditcard.h"
#include "ftrackchanges.h"
#include "restorepartner.h"
#include "cacherights.h"
#include "dlguserpasswords.h"
#include "wglobaldbconfig.h"
#include "rerestdishcomplex.h"
#include "frestauranttotal.h"
#include "fcashreportsummary.h"
#include "fcommonfilterbydate.h"
#include "fcashreport.h"
#include "recomplimentarycomment.h"
#include "databaseresult.h"
#include "wreportbuilder.h"
#include "dlgchangepassword.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QShortcut>
#include <QNetworkProxy>

static QMap<int, QList<QAction*> > fMenu;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    fCommand(0)
{
    ui->setupUi(this);


    QFont f(qApp->font());
    f.setBold(true);
    f.setPointSize(f.pointSize() + 1);
    ui->actionChange_password->setVisible(false);
    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(tabCloseRequested(int)));
    fMainWindow = this;
    fTab = ui->tabWidget;
    logout();
    fSocket.setProxy(QNetworkProxy::NoProxy);
    fCommand.setSocket(&fSocket);
    connect(&fSocket, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));
    connect(&fSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
    connect(&fSocket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));
    connect(&fCommand, SIGNAL(parseCommand(QString)), this, SLOT(parseSocketCommand(QString)));

    QShortcut *shortcut = new QShortcut(QKeySequence("F2"), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(shortcutSlot()));
    QShortcut *shortcut12 = new QShortcut(QKeySequence("F12"), this);
    connect(shortcut12, SIGNAL(activated()), this, SLOT(on_actionLogout_triggered()));
    QShortcut *shortcat11 = new QShortcut(QKeySequence("F11"), this);
    connect(shortcat11, SIGNAL(activated()), this, SLOT(shortcutFullScreen()));

    QString bgpath = qApp->applicationDirPath() + "/bg.png";
    qDebug() << bgpath;
    QPixmap bkgnd(bgpath);
    bkgnd = bkgnd.scaled(ui->wbg->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    ui->wbg->setPalette(palette);

    ui->wbg->setStyleSheet(QString(".QWidget{border-image:url(%1)0 0 0 0 stretch stretch;}").arg(qApp->applicationDirPath()
                                                                                        + "/bg.jpg"));

}

MainWindow::~MainWindow()
{
    disconnect(&fSocket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));
    disconnect(&fSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_actionLogin_triggered()
{
    login();
}

void MainWindow::on_actionLogout_triggered()
{
    logout();
}

void MainWindow::on_actionLock_triggered()
{
    lock();
}

void MainWindow::on_actionDatabases_triggered()
{
    if (fPreferences.getString(def_preferences_password).length() > 0) {
        LoginSettings ls(this);
        if (ls.exec() != QDialog::Accepted) {
            return;
        }
    }
    DatabasesConnections dc(this);
    dc.exec();
}

void MainWindow::login()
{
    Login l(this);
    if (l.exec() == QDialog::Rejected) {
        return;
    }
    logout();
    Db db = fPreferences.getDatabase(fDbName);    
    fDb.setConnectionParams(db.dc_main_host, db.dc_main_path, db.dc_main_user, db.dc_main_pass);
    fDb.open();
    fSocket.disconnectFromHost();

    buildMenuOfRole();
    CacheBase<CI_Base>::setDatabase(fDb.db());
    enableMainMenu(true);
    fTimer.start(60000);
    ui->actionChange_password->setVisible(true);
    if (fPreferences.getDb(def_receip_vaucher_id).toInt() == 0) {
        message_error(tr("Receipt voucher id not defined"));
        return;
    }
    setWindowTitle(fPreferences.getLocal("dbname").toString());
}

void MainWindow::addTabWidget(BaseWidget *widget)
{
    int index = ui->tabWidget->addTab(widget, "");
    widget->setTab(ui->tabWidget, index);
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
}

void MainWindow::setCurrentWidget(QWidget *w)
{
    ui->tabWidget->setCurrentWidget(w);
}

void MainWindow::expandTab()
{
    ui->tabWidget->setMaximumWidth(16777215);
    ui->wbg->setMaximumWidth(0);
}

void MainWindow::tabCloseRequested(int index)
{
    BaseWidget *w = static_cast<BaseWidget*>(ui->tabWidget->widget(index));
    if (!w->canClose()) {
        return;
    }
    ui->tabWidget->removeTab(index);
    delete w;
    if (ui->tabWidget->count() == 0) {
        ui->tabWidget->setMaximumWidth(0);
        ui->wbg->setMaximumWidth(167772150);
    }
}

void MainWindow::socketReadyRead()
{
    QTcpSocket *s = static_cast<QTcpSocket*>(sender());
    fCommand.readBytes(s->readAll());
}

void MainWindow::parseSocketCommand(const QString &command)
{
    QJsonDocument jDoc = QJsonDocument::fromJson(command.toUtf8());
    QJsonObject jObj = jDoc.object();
    QString cmd = jObj.value("command").toString();
    if (cmd == "refresh_reservations") {

    } else if (cmd == "update_cache") {
        int cacheId = jObj.value("cache").toInt();
        QString item = jObj.value("item").toString();
        CacheOne::updateCache(cacheId, item);
    } else {
        QVariantMap m = jObj.toVariantMap();
        for (int i = 0, count = ui->tabWidget->count(); i < count; i++) {
            BaseWidget *b = static_cast<BaseWidget*>(ui->tabWidget->widget(i));
            if (!b) {
                return;
            }
            b->handleBroadcast(m);
        }
    }
}

void MainWindow::socketError(QAbstractSocket::SocketError f_cityLedger)
{
    Q_UNUSED(f_cityLedger)
    if (fTimer.isActive()) {
        message_error(tr("Lost connection to broadcast server, force logout") + "<br>" + fSocket.errorString());
        logout();
    }
}

void MainWindow::socketDisconnected()
{
    if (fTimer.isActive()) {
        message_error_tr("Lost connection to broadcast server, force logout");
        logout();
    }
}

void MainWindow::logout()
{
    fTimer.stop();
    disableMainMenu();
    while (ui->tabWidget->count() > 0) {
        tabCloseRequested(0);
    }
    CacheOne::clearAll();
    fSocket.close();
    ui->actionChange_password->setVisible(false);
    fCustomReports.clear();
}

void MainWindow::lock()
{
    Login l(this);
    l.setLockUser(WORKING_USERID);
    do {
    } while (l.exec() != QDialog::Accepted);
}

void MainWindow::enableMainMenu(bool value)
{
    ui->wmenu->setVisible(value);
    ui->lstMenu->setMinimumHeight(0);
    ui->lstMenu->setMaximumHeight(0);
    if (!value) {
        return;
    }
}

void MainWindow::disableMainMenu()
{
    enableMainMenu(false);
}

void MainWindow::buildMenuOfRole()
{
    UserPermssions::init(WORKING_USERGROUP);
            QList<QAction*> a1;
            if (check_permission(pr_view_revenue)) {
                a1.append(ui->actionRestaurant);
                a1.append(ui->actionEarnings_carwash);
                a1.append(ui->actionEarnings_waitroom);
                a1.append(ui->actionEarnings_shop);
                a1.append(ui->actionArmSoftExport);
                a1.append(ui->actionSales_by_storages);
                a1.append(ui->actionReport_by_payment);
                a1.append(ui->actionSales_report_by_cars);
            }
            if (check_permission(pr_cashbox)) {
                a1.append(ui->actionAccounts);
            }
            fMenu.insert(0, a1);

            QList<QAction*> a2;
            if (check_permission(pr_edit_store_doc)) {
                a2.append(ui->actionNew_store_document);
                a2.append(ui->actionNew_store_checkpoint);
                a2.append(ui->action_inventorization);
            }
            if (check_permission(pr_store_doc_journal)) {
                a2.append(ui->actionDocuments_list);
                a2.append(ui->actionStore_movement);
                a2.append(ui->actionT_Report);
                a2.append(ui->actionStore_entries);
                a2.append(ui->actionMaterials_in_the_store);
                a2.append(ui->actionPartners_debts);
                a2.append(ui->action_goods_movement);
            }

            fMenu.insert(1, a2);

            QList<QAction*> a3;
            if (check_permission(pr_goods_list)) {
                a3.append(ui->actionType_of_dishes);
                a3.append(ui->actionCarwash_index);
                a3.append(ui->actionWaiting_room_index);
                a3.append(ui->actionShop_index);
                a3.append(ui->actionStorage_goods_index);
                a3.append(ui->actionComplex_dish);
                a3.append(ui->actionAllDishes);
            }
            fMenu.insert(2, a3);

            QList<QAction*> a4;
            if (check_permission(pr_coupon_view)) {
                a4.append(ui->actionCoupons);
                a4.append(ui->actionCoupons_seria);
                a4.append(ui->actionCoupons_sales);
                a4.append(ui->actionCoupons_statistics);
                a4.append(ui->actionModels_of_cars);
                a4.append(ui->actionCostumers);
                a4.append(ui->actionCostumers_cars);
                a4.append(ui->actionCoupons_one_off);
                a4.append(ui->actionCoupons_present);
                a4.append(ui->actionDiscount_report);
                a4.append(ui->actionDiscount_total);
                a4.append(ui->actionBalance_of_the_cards);
                a4.append(ui->actionCoupon_of_service);
                a4.append(ui->actionCoupon_of_service_documents);
                a4.append(ui->actionCoupon_of_service_actions);
            }
            fMenu.insert(3, a4);

            QList<QAction*> a5;
            if (check_permission(pr_indexes)) {
                a5.append(ui->actionHakk);
                a5.append(ui->actionTables);
                a5.append(ui->actionStorages);
                a5.append(ui->actionAccounts_2);
                a5.append(ui->actionCash_operation);
                a5.append(ui->actionPartners_2);
                a5.append(ui->actionUsers_groups);
                a5.append(ui->actionUsers);
            }
            if (check_permission(pr_global_config)) {
                a5.append(ui->actionGlobal_config);
            }
            fMenu.insert(4, a5);

            QList<QAction*> a6;
            if (check_permission(pr_attendance)){
                a6.append(ui->actionAttendance);
                a6.append(ui->actionSalary_by_employes);
            }
            fMenu.insert(5, a6);

}

void MainWindow::buildMenu(QToolButton *btn, const QList<QAction *> &l)
{
    ui->lstMenu->clear();
    for (QAction *a: l)  {
        QToolButton *b = new QToolButton();
        b->setText(a->text());
        b->setIcon(a->icon());
        b->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        b->setStyleSheet("border: 1px solid gray; background-color: rgb(240, 240, 170); margin: 1px; font-size: 10pt;");
        connect(b, SIGNAL(clicked()), a, SLOT(trigger()));
        QListWidgetItem *item = new QListWidgetItem(ui->lstMenu);
        item->setSizeHint(QSize(100, 40));
        ui->lstMenu->setItemWidget(item, b);
    }
    ui->lstMenu->setMinimumHeight(ui->lstMenu->count() * 40);
    ui->lstMenu->setMaximumHeight(ui->lstMenu->count() * 40);
    ui->verticalLayout->insertWidget(btn->property("index").toInt(), ui->lstMenu);
}

void MainWindow::dish(int t)
{
    int ot = 0;
    if (t > 0) {
        ot = DlgOneTwoAll::getOption(tr("Visible"), tr("All"), tr("Active"), tr("Not active"), this);
    }

    QString cond;
    switch (ot) {
    case 2:
        cond = " and d.f_id in (select f_dish from r_menu where f_state=1) ";
        break;
    case 3:
        cond = " and d.f_id in (select f_dish from r_menu where f_state=0) ";
        break;
    }
    if (t > 0) {
        cond += " and p.f_id=" + QString("%1 ").arg(t);
    }

    QList<int> widths;
    widths << 100
           << 120
           << 0
           << 120
           << 0
           << 120
           << 150
           << 150
           << 0
           << 0
           << 0
           << 80
           << 80
           << 80
           << 80
           << 0
           << 80
           << 0
           << 80;
    QStringList fields;
    fields << "f_id"
           << "f_partname"
           << "f_defstore"
           << "f_defstorename"
           << "f_type"
           << "f_type_name"
           << "f_en"
<< "f_armsoftname"
           << "f_den"
           << "f_bgColor"
           << "f_textColor"
           << "f_queue"
           << "f_adgt"
           << "f_as"
           << "f_lastPrice"
           << "f_unit"
           << "f_unitName"
           << "f_minreminder"
           << "f_taxdebt"
              ;
    QStringList titles;
    titles << tr("Code")
               << tr("Տեսակ")
           << tr("Part")
           << tr("Store")
           << tr("Type code")
           << tr("Type")
           << tr("Name")
              << "ՀԾ անվանում"
           << tr("Description")
           << tr("Background color")
           << tr("Text color")
           << tr("Queue")
           << tr("ADGT")
           << tr("ArmSoft")
           << tr("Input price")
           << tr("Unit code")
           << tr("Unit name")
           << tr("Min. reminder")
           << tr("Tax debt")
              ;
    QString title = tr("Dishes");
    QString icon = ":/images/cutlery.png";
    QString query = "select d.f_id, p.f_en as f_partname, d.f_defstore, st.f_name as f_defstorename, d.f_type, t.f_en, "
            "d.f_en, d.f_armsoftname, d.f_text_en,  "
            "d.f_bgColor, d.f_textColor, d.f_queue, d.f_adgt, d.f_as, f_lastPrice, d.f_unit, u.f_name as f_unitName, "
            "d.f_minreminder, d.f_taxdebt "
            "from r_dish d "
            "inner join r_dish_type t on t.f_id=d.f_type "
            "inner join r_dish_part p on p.f_id=t.f_part "
            "inner join r_unit u on u.f_id=d.f_unit "
            "left join r_store st on st.f_id=d.f_defstore "
            "where 1=1 " + cond +
            "order by p.f_en, t.f_en, d.f_queue ";
    WReportGrid *r = addTab<WReportGrid>();
    r->fullSetup<RERestDish>(widths, fields, titles, title, icon, query);
}

void MainWindow::on_actionAbout_triggered()
{
    About *a = new About(this);
    a->exec();
    delete a;
}

void MainWindow::on_actionContacts_triggered()
{
    addTab<WContacts>();
}

void MainWindow::on_actionUsers_triggered()
{
    addTab<WUsers>();
}

void MainWindow::on_actionUsers_groups_triggered()
{
    addTab<WUsersGroups>();
}

void MainWindow::on_actionHakk_triggered()
{
    QList<int> widths;
    widths << 100
           << 200
           << 0
           << 150
           << 80
           << 80
           << 100
           << 80
           << 80
           << 30
           << 30
           << 30
           << 30
              ;
    QStringList fields;
    fields << "f_id"
           << "f_name"
           << "f_defaultMenu"
           << "f_menuName"
           << "f_defaultSvc"
           << "f_itemForInvoice"
           << "f_receiptPrinter"
           << "f_vatDept"
           << "f_noVatDept"
           << "f_showBanket"
           << "f_showHall"
            << "f_serviceitem"
            << "f_servicevalue"
              ;
    QStringList titles;
    titles << tr("Code")
             << tr("Name")
             << tr("Menu code")
             << tr("Menu")
             << tr("Service")
             << tr("Invoice id")
             << tr("Printer")
             << tr("VAT dept")
             << tr("No VAT dept")
             << tr("Banket")
             << tr("Hall")
               << tr("Service item")
               << tr("Serivce value")
                ;
    QString title = tr("Hall");
    QString icon = ":/images/hall.png";
    QString query = "select h.f_id, h.f_name, h.f_defaultMenu, m.f_" + def_lang + ", h.f_defaultSvc, "
            "h.f_itemForInvoice, h.f_receiptPrinter, f_vatDept, f_noVatDept, f_showBanket, f_showHall, "
            "f_serviceitem, f_servicevalue "
            "from r_hall h "
            "inner join r_menu_names m on m.f_id=h.f_defaultMenu ";
    WReportGrid *r = addTab<WReportGrid>();
    r->fullSetup<RERestHall>(widths, fields, titles, title, icon, query);
}

void MainWindow::on_actionTables_triggered()
{
    QList<int> widths;
    widths << 100
           << 100
           << 0
           << 100
           << 100;
    QStringList fields;
    fields << "f_id"
           << "f_name"
           << "f_hall"
           << "f_hallName"
           << "f_queue";
    QStringList titles;
    titles << tr("Code")
           << tr("Name")
           << tr("Hall code")
           << tr("Hall")
           << tr("Queue");
    QString title = tr("Tables");
    QString icon = ":/images/table.png";
    QString query = "select t.f_id, t.f_name, t.f_hall, h.f_name, t.f_queue "
            "from r_table t "
            "inner join r_hall h on h.f_id = t.f_hall "
            "order by f_hall, f_queue ";
    WReportGrid *r = addTab<WReportGrid>();
    r->fullSetup<RERestTable>(widths, fields, titles, title, icon, query);
}

void MainWindow::on_actionNames_of_menu_triggered()
{
    QList<int> widths;
    widths << 100
           << 230
           << 230
           << 230;
    QStringList fields;
    fields << "f_id"
           << "f_am"
           << "f_en"
           << "f_ru";
    QStringList titles;
    titles << tr("Code")
           << tr("Name, am")
           << tr("Name, en")
           << tr("Name, ru");
    QString title = tr("Names of menu");
    QString icon = ":/images/cutlery.png";
    QString query = "select f_id, f_am, f_en, f_ru from r_menu_names ";
    WReportGrid *r = addTab<WReportGrid>();
    r->fullSetup<RERestMenuNames>(widths, fields, titles, title, icon, query);
}

void MainWindow::on_actionMain_partitions_of_menu_triggered()
{
    QList<int> widths;
    widths << 100
           << 230
           << 230
           << 230;
    QStringList fields;
    fields << "f_id"
           << "f_am"
           << "f_en"
           << "f_ru";
    QStringList titles;
    titles << tr("Code")
           << tr("Name, am")
           << tr("Name, en")
           << tr("Name, ru");
    QString title = tr("Main partitions of menu");
    QString icon = ":/images/cutlery.png";
    QString query = "select f_id, f_am, f_en, f_ru from r_dish_part";
    WReportGrid *r = addTab<WReportGrid>();
    r->fullSetup<RERestMenuPart>(widths, fields, titles, title, icon, query);
}

void MainWindow::on_actionType_of_dishes_triggered()
{
    QList<int> widths;
    widths << 100
           << 0
           << 120
           << 120
           << 120
           << 120
           << 0
           << 0
           << 80
           << 0;
    QStringList fields;
    fields << "f_id"
           << "f_part"
           << "f_part_name"
           << "f_am"
           << "f_en"
           << "f_ru"
           << "f_bgColor"
           << "f_textColor"
           << "f_queue"
           << "f_active";
    QStringList titles;
    titles << tr("Code")
           << tr("Part code")
           << tr("Part")
           << tr("Name, am")
           << tr("Name, en")
           << tr("Name, ru")
           << tr("Backgroun color")
           << tr("Text color")
           << tr("Queue")
           << tr("Active");
    QString title = tr("Type of dish");
    QString icon = ":/images/cutlery.png";
    QString query = "select t.f_id, t.f_part, p.f_" + def_lang + ", t.f_am, t.f_en, t.f_ru, "
            "f_bgColor, f_textColor, t.f_queue, f_active "
            "from r_dish_type t "
            "inner join r_dish_part p on p.f_id=t.f_part "
            "order by p.f_" + def_lang + ", t.f_" + def_lang;
    WReportGrid *r = addTab<WReportGrid>();
    r->fullSetup<RERestDishType>(widths, fields, titles, title, icon, query);
}

void MainWindow::on_actionDishes_triggered()
{
    dish(2);
}

void MainWindow::on_actionModifiers_triggered()
{
    QList<int> widths;
    widths << 80
           << 250
           << 250
           << 250;
    QStringList fields;
    fields << "f_id"
           << "f_am"
           << "f_en"
           << "f_ru";
    QStringList titles;
    titles << tr("Code")
           << tr("Name, am")
           << tr("Name, en")
           << tr("Name, ru");
    QString title = tr("Dish modifiers");
    QString icon = ":/images/cutlery.png";
    QString query = "select f_id, f_am, f_en, f_ru from r_dish_mod order by f_" + def_lang;
    WReportGrid *r = addTab<WReportGrid>();
    r->fullSetup<RERestDishMod>(widths, fields, titles, title, icon, query);
}

void MainWindow::on_actionStorages_triggered()
{
    QList<int> widths;
    widths << 100
           << 300;
    QStringList fields;
    fields << "f_id"
           << "f_name";
    QStringList titles;
    titles << tr("Code")
           << tr("Name");
    QString title = tr("Storages");
    QString icon = ":/images/cutlery.png";
    QString query = "select f_id, f_name from r_store";
    WReportGrid *r = addTab<WReportGrid>();
    r->fullSetup<RERestStore>(widths, fields, titles, title, icon, query);
}

void MainWindow::on_actionMenu_review_triggered()
{
    QList<int> widths;
    widths << 0
           << 0
           << 100
           << 0
           << 100
           << 0
           << 100
           << 250
           << 250
           << 80
           << 0
           << 100
           << 100
           << 100
           << 80
              ;
    QStringList fields;
    fields << "f_id"
           << "f_menu_id"
           << "f_menu_name"
           << "f_part_id"
           << "f_part_name"
           << "f_type_id"
           << "f_type_name"
           << "f_dish_name"
            << "f_armsoftname"
           << "f_price"
           << "f_store_id"
           << "f_store_name"
           << "f_print1"
           << "f_print2"
           << "f_as"
              ;
    QStringList titles;
    titles << tr("Code")
           << tr("Menu code")
           << tr("Menu")
           << tr("Part code")
           << tr("Part")
           << tr("Type code")
           << tr("Type")
           << tr("Dish")
           << "ՀԾ անվանւոմ"
           << tr("Price")
           << tr("Store code")
           << tr("Store")
           << tr("Printer 1")
           << tr("Printer 2")
           << tr("ArmSoft")
              ;
    QString query = "select  d.f_id, m.f_id, mn.f_" + def_lang + ", t.f_part, p.f_" + def_lang + ", "
            "d.f_type, t.f_" + def_lang + ", d.f_" + def_lang + ", d.f_armsoftname, m.f_price, "
            "m.f_store, s.f_name, m.f_print1, m.f_print2, d.f_as "
            "from r_menu m "
            "inner join r_menu_names mn on mn.f_id=m.f_menu "
            "inner join r_store s on s.f_id=m.f_store "
            "inner join r_dish d on d.f_id=m.f_dish "
            "inner join r_dish_type t on t.f_id=d.f_type "
            "inner join r_dish_part p on p.f_id=t.f_part "
            "order by 2, 4, 6, 7";
    QString title = tr("Review of menu");
    QString icon = ":/images/cutlery.png";
    WReportGrid *r = addTab<WReportGrid>();
    r->fullSetup<RERestDish>(widths, fields, titles, title, icon, query, false);
}

void MainWindow::on_actionPrinters_triggered()
{
    QList<int> widths;
    widths << 100
           << 300;
    QStringList fields;
    fields << "f_id"
           << "f_name";
    QStringList titles;
    titles << tr("Code")
           << tr("Name");
    QString title = tr("Printers");
    QString icon = ":/images/cutlery.png";
    QString query = "select f_id, f_name from r_printers";
    WReportGrid *r = addTab<WReportGrid>();
    r->fullSetup<RERestPrinter>(widths, fields, titles, title, icon, query);
}

void MainWindow::on_actionCredit_card_triggered()
{
    QList<int> widths;
    widths << 100
           << 300;
    QStringList fields;
    fields << "f_id"
           << "f_name";
    QStringList titles;
    titles << tr("Code")
           << tr("Name");
    QString title = tr("Credit cards");
    QString icon = ":/images/credit-card.png";
    QString query = "select f_id, f_name from f_credit_card";
    WReportGrid *r = addTab<WReportGrid>();
    r->fullSetup<RECreditCard>(widths, fields, titles, title, icon, query);
}

void MainWindow::on_actionChange_password_triggered()
{
    DlgUserPasswords *d = new DlgUserPasswords(this);
    d->setSelfMode();
    d->exec();
    delete d;
}

void MainWindow::on_actionGlobal_config_triggered()
{
    addTab<WGlobalDbConfig>();
}

void MainWindow::on_actionComplex_dish_triggered()
{
    dish(4);
}

QString MainWindow::actionTitle(QObject *a)
{
    return static_cast<QAction*>(a)->text();
}

void MainWindow::on_actionRestaurant_triggered()
{
    FRestaurantTotal::open();
}

void MainWindow::on_actionCash_report_total_triggered()
{
    addTab<WReportGrid>()->setQueryModel<FCashReportSummary>();
}

void MainWindow::on_actionCash_repoort_detailed_triggered()
{
    FCashReport::open();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    bool canClose = true;
    for (int i = 1; i < ui->tabWidget->count(); i++) {
        BaseWidget *w = static_cast<BaseWidget*>(ui->tabWidget->widget(i));
        if (!w->canClose()) {
            canClose = false;
            break;
        }
    }
    if (canClose) {
        if (message_confirm(tr("Confirm to close application")) != QDialog::Accepted) {
            canClose = false;
        }
    }
    if (!canClose) {
        event->ignore();
        return;
    }
    QMainWindow::closeEvent(event);
}

void MainWindow::shortcutFullScreen()
{
    if (isFullScreen()) {
        showMaximized();
    } else {
        showFullScreen();
    }
}

void MainWindow::shortcutSlot()
{
    if (ui->tabWidget->count() > 0) {
        ui->tabWidget->setCurrentIndex(0);
    }
}

void MainWindow::customReport()
{
    QAction *a = static_cast<QAction*>(sender());
    if (!fCustomReports.contains(a)) {
        message_error_tr("Application error. Contact with developer. Message custom action == 0");
        return;
    }
    int reportId = fCustomReports[a];
    FCommonFilterByDate::open(reportId);
}

void MainWindow::on_actionComplimentary_comment_triggered()
{
    REComplimentaryComment::openComplimentaryComment();
}

void MainWindow::on_actionReport_buillder_triggered()
{
    addTab<WReportBuilder>();
}

void MainWindow::on_actionReports_set_triggered()
{
    addTab<WCustomReports>();
}

void MainWindow::on_actionStatistics_triggered()
{
    addTab<WReportsSetOld>();
}

void MainWindow::on_actionNew_store_document_triggered()
{
    StoreDoc::openStoreDocument(0);
}

void MainWindow::on_actionPartners_2_triggered()
{
    REStorePartner::openStorePartners();
}

void MainWindow::on_actionDocuments_list_triggered()
{
    FStoreDocs::openReport();
}

void MainWindow::on_actionMaterials_in_the_store_triggered()
{
    FMaterialsInStore::openFilterReport<FMaterialsInStore, WReportGrid>();
}

void MainWindow::on_actionStore_movement_triggered()
{
    FStoreMovement::openFilterReport<FStoreMovement, WReportGrid>();
}

void MainWindow::on_actionCoupons_triggered()
{
    RECoupon::openReport();
}

void MainWindow::on_actionSales_by_storages_triggered()
{
    FRestSaleByStore::openFilterReport<FRestSaleByStore, WReportGrid>();
}

void MainWindow::on_actionModels_of_cars_triggered()
{
    REModelOfCars::openReport();
}

void MainWindow::on_actionCostumers_cars_triggered()
{
    RECarClient::openReport1();
}

void MainWindow::on_actionReport_by_payment_triggered()
{
    FReportByPayment::openFilterReport<FReportByPayment, WReportGrid>();
}

void MainWindow::on_actionPartners_debts_triggered()
{
    FPartnersDebt::openFilterReport<FPartnersDebt, WReportGrid>();
}

void MainWindow::on_actionCoupons_sales_triggered()
{
    FCouponSales::openFilterReport<FCouponSales, WReportGrid>();
}

void MainWindow::on_actionCoupons_seria_triggered()
{
    RECouponSeria::openReport();
}

void MainWindow::on_actionAccounts_triggered()
{
    FCash::openFilterReport<FCash, WReportGrid>();
}

void MainWindow::on_actionAccounts_2_triggered()
{
    RECashDesk::openReport();
}

void MainWindow::on_actionStore_entries_triggered()
{
    FStoreEntry::openFilterReport<FStoreEntry, WReportGrid>();
}

void MainWindow::on_actionNew_store_checkpoint_triggered()
{
    WStoreEntry::openDoc(0);
}

void MainWindow::on_actionPartner_payments_triggered()
{
    //FPartnerPayments::openFilterReport<FPartnerPayments, WReportGrid>();
}

void MainWindow::on_actionCoupons_statistics_triggered()
{
    FCouponStatistics::openFilterReport<FCouponStatistics, WReportGrid>();
}

void MainWindow::on_actionDebts_triggered()
{
    FDebtOfCostumers::openFilterReport<FDebtOfCostumers, WReportGrid>();
}

void MainWindow::on_actionDiscount_report_triggered()
{
    FDiscountReport::openFilterReport<FDiscountReport, WReportGrid>();
}

void MainWindow::on_actionSales_report_by_cars_triggered()
{
    FSalesByCar::openFilterReport<FSalesByCar, WReportGrid>();
}

void MainWindow::on_actionDiscount_total_triggered()
{
    FDiscountTotal::openFilterReport<FDiscountTotal, WReportGrid>();
}

void MainWindow::on_actionT_Report_triggered()
{
    FTStoreReport::openFilterReport<FTStoreReport, WReportGrid>();
}

void MainWindow::on_tbEarning_clicked()
{
    buildMenu(static_cast<QToolButton*>(sender()), fMenu[0]);
}

void MainWindow::on_tbStore_clicked()
{
    buildMenu(static_cast<QToolButton*>(sender()), fMenu[1]);
}

void MainWindow::on_tbStore_2_clicked()
{
    buildMenu(static_cast<QToolButton*>(sender()), fMenu[2]);
}

void MainWindow::on_tbStore_3_clicked()
{
    buildMenu(static_cast<QToolButton*>(sender()), fMenu[3]);
}

void MainWindow::on_tbStore_4_clicked()
{
    buildMenu(static_cast<QToolButton*>(sender()), fMenu[4]);
}

void MainWindow::on_actionCostumers_triggered()
{
    FDebtHolders::openFilterReport<FDebtHolders, WReportGrid>();
}

void MainWindow::on_actionCoupons_one_off_triggered()
{
    RECarClient::openReport1();
}

void MainWindow::on_actionCoupons_present_triggered()
{
    RECarClient::openReport1();
}

void MainWindow::on_actionBalance_of_the_cards_triggered()
{
    FBalanceOnCard::openFilterReport<FBalanceOnCard, WReportGrid>();
}

void MainWindow::on_tbExit_clicked()
{
    close();
}

void MainWindow::on_actionEarnings_carwash_triggered()
{
    FEarningsWash *w = FEarningsWash::openFilterReport<FEarningsWash, WReportGrid>();
    w->setHall(1);
}

void MainWindow::on_actionEarnings_waitroom_triggered()
{
    FEarningsWash *w = FEarningsWash::openFilterReport<FEarningsWash, WReportGrid>();
    w->setHall(2);
}

void MainWindow::on_actionCarwash_index_triggered()
{
    dish(1);
}

void MainWindow::on_actionWaiting_room_index_triggered()
{
    dish(2);
}

void MainWindow::on_actionStorage_goods_index_triggered()
{
    dish(3);
}

void MainWindow::on_actionEarnings_shop_triggered()
{
    FEarningsWash *w = FEarningsWash::openFilterReport<FEarningsWash, WReportGrid>();
    w->setHall(4);
}

void MainWindow::on_actionShop_index_triggered()
{
    dish(5);
}

void MainWindow::on_actionSalary_by_employes_triggered()
{
    FSalaryByEmployes::openFilterReport<FSalaryByEmployes, WReportGrid>();
}

void MainWindow::on_actionCoupon_of_service_triggered()
{
    FCouponsService::openFilterReport<FCouponsService, WReportGrid>();
}

void MainWindow::on_actionCoupon_of_service_documents_triggered()
{
    FCouponDocuments::openFilterReport<FCouponDocuments, WReportGrid>();
}

void MainWindow::on_tbEarning_2_clicked()
{
    buildMenu(static_cast<QToolButton*>(sender()), fMenu[5]);
}

void MainWindow::on_actionAttendance_triggered()
{
    FAttendance::openFilterReport<FAttendance, WReportGrid>();
}

void MainWindow::on_actionArmSoftExport_triggered()
{
    FAsExportSale::openFilterReport<FAsExportSale, WReportGrid>();
}

void MainWindow::on_btnChangeMyPassword_clicked()
{
    DlgChangePassword(this).exec();
}

void MainWindow::on_action_inventorization_triggered()
{
    addTab<Inventorization>();
}

void MainWindow::on_action_goods_movement_triggered()
{
    addTab<goodsmovement>();
}

void MainWindow::on_actionAllDishes_triggered()
{
    dish(0);
}
