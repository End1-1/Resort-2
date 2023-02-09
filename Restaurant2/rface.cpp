#include "rface.h"
#include "ui_rface.h"
#include "rlogin.h"
#include "rmessage.h"
#include "rdesk.h"
#include "splash.h"
#include "cacherights.h"
#include "rchangehall.h"
#include <QPainter>
#include <QScrollBar>
#include <QItemDelegate>
#include <QThread>
#include <QHostInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkProxy>

#define hall_refresh_timeout 10

class TableItemDelegate : public QItemDelegate {
protected:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
        QRect rectFill =  option.rect;
        rectFill.adjust(2, 2, -2, -2);
        painter->fillRect(rectFill, Qt::white);
        if (!index.isValid()) {
            return;
        }
        TableStruct *t = index.data(Qt::UserRole).value<TableStruct*>();
        if (!t) {
            return;
        }

        if (!t->fOrder == 0) {
            painter->fillRect(rectFill, QColor::fromRgb(200, 245, 195));
        }

        if (t->fPrint != 0) {
            painter->fillRect(rectFill, Qt::yellow);
        }

        if (t->fLockStation.length() > 0) {
            painter->fillRect(rectFill, QColor::fromRgb(245, 166, 166));
        }

        QRect rectTable = rectFill;
        rectTable.adjust(3, 3, -3, -3);
        QFont f(qApp->font());
        f.setPointSize(18);
        f.setBold(true);
        painter->setPen(Qt::black);
        painter->setFont(f);
        painter->drawText(rectTable, t->fName);
        painter->setPen(Qt::darkRed);
        rectTable.adjust(-1, -1, 0, 0);
        painter->drawText(rectTable, t->fName);

        if (t->fOrder > 0) {
            int y = rectTable.top() + 10;
            int x = rectTable.left() + QFontMetrics(f).width(t->fName) + 15;
            int tableNameHeight = rectTable.top() + QFontMetrics(f).height() + 5;
            f.setPointSize(8);
            painter->setFont(f);
            int deltaY = QFontMetrics(f).height() - 1;
            painter->drawText(x, y, t->fStaff);
            y += deltaY;
            painter->drawText(x, y, t->fAmount);
            x = rectTable.left();
            y = tableNameHeight + 2;
            painter->drawText(x, y, t->fDateOpen);
            y += deltaY;
            painter->drawText(x, y, t->fComment);
        }
    }
};

RFace::RFace(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::RFace),
    fCommand(0)
{
    ui->setupUi(this);

    fCurrenTableState = 0;
    fTimerCounter = 0;
    fSocket.setProxy(QNetworkProxy::NoProxy);
    fIsConfigured = true;
    // setup tax parameters
    fDbBind[":f_comp"] = QHostInfo::localHostName();
    Base::fDb.select("select f_address, f_port, f_password, f_adg, f_hall, f_menu from s_tax_print where upper(f_comp)=upper(:f_comp)", fDbBind, fDbRows);
    if (fDbRows.count() > 0) {
        if (fDbRows.count() > 0) {
            fPreferences.setDb(def_tax_address, fDbRows.at(0).at(0).toString());
            fPreferences.setDb(def_tax_port, fDbRows.at(0).at(1).toString());
            fPreferences.setDb(def_tax_password, fDbRows.at(0).at(2).toString());
            fPreferences.setDb(def_tax_adg, fDbRows.at(0).at(3).toString());
            qDebug() << fDbRows.at(0).at(4).toString();
            fPreferences.setDb(def_default_hall, fDbRows.at(0).at(4).toString());
            fPreferences.setDb(def_default_menu, fDbRows.at(0).at(5).toString());
            fCurrentHall = fDbRows.at(0).at(4).toInt();
        }
        qDebug() << fPreferences.getDb(def_default_hall);
        if (fPreferences.getDb(def_default_hall).toInt() == 0) {
            message_error("Please, set default hall for this station.");
            fIsConfigured = false;
            qApp->quit();
        }

    }
    fParent = this;

    fDbBind[":f_key"] = def_working_day;
    fDb.select("delete from f_global_settings where f_key=:f_key", fDbBind, fDbRows);

    fDbBind[":f_settings"] = 1;
    fDbBind[":f_key"] = def_working_day;
    fDbBind[":f_value"] = WORKING_DATE.toString("dd/MM/yyyy");
    fDb.insert("f_global_settings", fDbBind);
}

RFace::~RFace()
{
    delete ui;
}

bool RFace::setup()
{
    fCommand.setSocket(&fSocket);
    connect(&fSocket, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));
    connect(&fSocket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));
    connect(&fCommand, SIGNAL(parseCommand(QString)), this, SLOT(parseCommand(QString)));
    fSocket.connectToHost(fDb.fDb.hostName(), 1250);
    if (fSocket.waitForConnected()) {
        QString data = QString("{\"db\" : {\"database\" : \"%1\", \"user\" : \"%2\", \"password\" : \"password\"}, "
                               "\"command\" : {\"command\": \"identify\"}}")
                .arg(fDb.fDb.databaseName())
                .arg(WORKING_USERNAME);
        int size = data.toUtf8().length();
        QByteArray dataToSend;
        dataToSend.append(reinterpret_cast<const char*>(&size), sizeof(size));
        dataToSend.append(data.toUtf8(), data.toUtf8().length());
        fSocket.write(dataToSend, dataToSend.length());
        fSocket.flush();
    } else {
        message_error(tr("Cannot connect to broadcast server, force logout"));
        message_error(fDb.fDb.hostName());
        return false;
    }
    Splash s(this);
    s.show();
    s.setText(tr("Load hall..."));
    fHall.init();
    s.setText(tr("Load menu..."));
    DishesTable dt;
    dt.init(&s);
    s.setText(tr("Setup current menu..."));

    s.hide();
    return true;
}

void RFace::socketReadyRead()
{
    QTcpSocket *s = static_cast<QTcpSocket*>(sender());
    fCommand.readBytes(s->readAll());
}

void RFace::socketDisconnected()
{
    message_error(tr("Connection to the server lost. Application will quit"));
    qApp->quit();
}

void RFace::on_tableWidget_clicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }
}

void RFace::on_btnExit_clicked()
{
    fSocket.disconnect();
    accept();
}

User *RFace::login()
{
    QString login;
    if (RLogin::getLogin(login, tr("Login"), this)) {
        User *u = new User(login, this);
        if (!u->isValid()) {
            delete u;
            u = 0;
            message_error(tr("Access denied"));
            return 0;
        }
        return u;
    } else {
        return 0;
    }
}

