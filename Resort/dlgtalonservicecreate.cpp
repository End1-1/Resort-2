#include "dlgtalonservicecreate.h"
#include "ui_dlgtalonservicecreate.h"
#include "database2.h"
#include "doubledatabase.h"
#include "message.h"
#include <QApplication>
#include <QClipboard>
#include <QDate>
#include <QDialog>
#include <QDialogButtonBox>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QVBoxLayout>

DlgTalonServiceCreate::DlgTalonServiceCreate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgTalonServiceCreate)
{
    ui->setupUi(this);
    if(!loadGroups()) {
        ui->btnOk->setEnabled(false);
    }
}

DlgTalonServiceCreate::~DlgTalonServiceCreate()
{
    delete ui;
}

bool DlgTalonServiceCreate::loadGroups()
{
    Database2 db;
    if(!db.open(__dd1Host, __dd1Database, __dd1Username, __dd1Password)) {
        message_error(db.lastDbError());
        return false;
    }

    if(!db.exec("select f_id, f_name, f_price from talon_service_items_group order by f_id")) {
        message_error(db.lastDbError());
        return false;
    }

    while(db.next()) {
        const int id = db.integer("f_id");
        ui->cbGroup->addItem(QString("%1 %2").arg(id, 2, 10, QChar('0')).arg(db.string("f_name")), id);
        ui->cbGroup->setItemData(ui->cbGroup->count() - 1, db.doubleValue("f_price"), Qt::UserRole + 1);
    }

    if(ui->cbGroup->count() == 0) {
        message_error(tr("No talon groups"));
        return false;
    }

    refreshLastNumber();
    return true;
}

void DlgTalonServiceCreate::refreshLastNumber()
{
    ui->lbLastNumber->setText(QString("%1").arg(lastNumber(), 5, 10, QChar('0')));
    ui->lbPrice->setText(QString::number(ui->cbGroup->currentData(Qt::UserRole + 1).toDouble(), 'f', 2));
}

int DlgTalonServiceCreate::lastNumber() const
{
    const int groupId = ui->cbGroup->currentData().toInt();
    if(groupId <= 0) {
        return 0;
    }

    Database2 db;
    if(!db.open(__dd1Host, __dd1Database, __dd1Username, __dd1Password)) {
        return 0;
    }

    db[":f_group"] = groupId;
    if(!db.exec("select max(cast(right(f_code, 5) as unsigned)) as f_last "
                "from talon_service where f_group=:f_group") || !db.next()) {
        return 0;
    }

    return db.integer("f_last");
}

QString DlgTalonServiceCreate::makeCode(int groupId, int number) const
{
    return QString("%1%2%3")
           .arg(groupId, 2, 10, QChar('0'))
           .arg(QDate::currentDate().toString("ddMMyy"))
           .arg(number, 5, 10, QChar('0'));
}

void DlgTalonServiceCreate::showCreatedCodes(const QStringList &codes)
{
    QDialog dlg(this);
    dlg.setWindowTitle(tr("Created talons"));
    dlg.resize(360, 480);

    auto *text = new QPlainTextEdit(&dlg);
    text->setReadOnly(true);
    text->setPlainText(codes.join("\n"));
    text->selectAll();

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Close, &dlg);
    auto *copy = buttons->addButton(tr("Copy"), QDialogButtonBox::ActionRole);
    connect(copy, &QPushButton::clicked, text, [text]() {
        QApplication::clipboard()->setText(text->toPlainText());
        text->selectAll();
    });
    connect(buttons, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    auto *layout = new QVBoxLayout(&dlg);
    layout->addWidget(text);
    layout->addWidget(buttons);
    dlg.exec();
}

void DlgTalonServiceCreate::on_cbGroup_currentIndexChanged(int index)
{
    if(index < 0) {
        return;
    }
    refreshLastNumber();
}

void DlgTalonServiceCreate::on_btnCancel_clicked()
{
    reject();
}

void DlgTalonServiceCreate::on_btnOk_clicked()
{
    const int groupId = ui->cbGroup->currentData().toInt();
    const int qty = ui->sbQty->value();
    const double price = ui->cbGroup->currentData(Qt::UserRole + 1).toDouble();
    const int first = lastNumber() + 1;
    const int last = first + qty - 1;

    if(groupId < 1 || groupId > 99) {
        message_error(tr("Group code must be two digits"));
        return;
    }

    if(last > 99999) {
        message_error(tr("Talon number exceeds 5 digits"));
        return;
    }

    const QString firstCode = makeCode(groupId, first);
    const QString lastCode = makeCode(groupId, last);
    if(message_confirm(tr("Create %1 talons?\n%2 — %3").arg(qty).arg(firstCode, lastCode)) != RESULT_YES) {
        return;
    }

    Database2 db;
    if(!db.open(__dd1Host, __dd1Database, __dd1Username, __dd1Password)) {
        message_error(db.lastDbError());
        return;
    }

    if(!db.startTransaction()) {
        message_error(db.lastDbError());
        return;
    }

    QStringList codes;
    for(int number = first; number <= last; ++number) {
        const QString code = makeCode(groupId, number);
        codes.append(code);
        db[":f_code"] = code;
        db[":f_group"] = groupId;
        db[":f_price"] = price;
        db[":f_discount"] = 0;
        db[":f_trregister"] = -1;
        db[":f_special"] = 1;
        if(!db.insert("talon_service")) {
            const QString err = db.lastDbError();
            db.rollback();
            message_error(err);
            return;
        }
    }

    if(!db.commit()) {
        db.rollback();
        message_error(db.lastDbError());
        return;
    }

    showCreatedCodes(codes);
    accept();
}
