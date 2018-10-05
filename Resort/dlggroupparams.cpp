#include "dlggroupparams.h"
#include "ui_dlggroupparams.h"
#include "wguest.h"
#include "dlgreservationremarks.h"
#include "cacheroomcategory.h"

DlgGroupParams::DlgGroupParams(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgGroupParams)
{
    ui->setupUi(this);
    Utils::tableSetColumnWidths(ui->tblRoom, ui->tblRoom->columnCount(), 30, 100);
    fDockCardex = new DWSelectorCardex(this);
    fDockCardex->configure();
    fDockCardex->setSelector(ui->leCardexCode);
    connect(fDockCardex, &DWSelectorCardex::cardex, [this](CI_Cardex *c){
        dockResponse<CI_Cardex, CacheCardex>(ui->leCardexCode, ui->leCardexName, c);
    });
    CI_Cardex *car = CacheCardex::instance()->get(fPreferences.getDb(def_default_cardex).toString());
    if (car) {
        ui->leCardexCode->setText(car->fCode);
        ui->leCardexName->setText(car->fName);
    }
    fDockGuest = new DWSelectorGuest(this);
    fDockGuest->configure();
    fDockGuest->setSelector(ui->leGuest);
    connect(fDockGuest, &DWSelectorGuest::guest, [this](CI_Guest *g){
        if (!g) {
            if (!ui->leGuest->fHiddenText.isEmpty()) {
                g = CacheGuest::instance()->get(ui->leGuest->fHiddenText);
            }
        }
        if (g) {
            ui->leGuest->setText(g->fName);
            ui->leGuest->fHiddenText = g->fCode;
        } else {
            ui->leGuest->clear();
            ui->leGuest->fHiddenText.clear();
        }
    });

    connect(ui->teRemarks, &EQTextEdit::doubleClicked, [this](){
        QString remark = ui->teRemarks->toPlainText();
        if (DlgReservationRemarks::Remarks(remark)) {
            ui->teRemarks->setPlainText(remark);
        }
    });
    ui->chCardex->setChecked(false);
    CacheRoomCategory *crc = CacheRoomCategory::instance();
    ui->tblCat->setRowCount(crc->elementsCount());
    QMapIterator<QString, CI_RoomCategory*> it(crc->it());
    int row = 0;
    while (it.hasNext()) {
        it.next();
        ui->tblCat->setItemWithValue(row, 0, it.value()->fName);
        row++;
    }
}

DlgGroupParams::~DlgGroupParams()
{
    delete ui;
}

void DlgGroupParams::addRoom(const QString &room)
{
    int row = ui->tblRoom->rowCount();
    ui->tblRoom->setRowCount(row + 1);
    ui->tblRoom->addCheckBox(row, 0)->setChecked(true);
    ui->tblRoom->setItemWithValue(row, 1, room);
}

void DlgGroupParams::setGroupId(int group)
{
    fDbBind[":f_group"] = group;
    fDb.select("select f_room, f_cardex, f_guest from f_reservation where f_group=:f_group and f_state in (1,2,6)", fDbBind, fDbRows);
    foreach_rows {
        addRoom(it->at(0).toString());
    }
    ui->leGroupId->setInt(group);
    CI_Cardex *ca = CacheCardex::instance()->get(fDbRows.at(0).at(1).toString());
    dockResponse<CI_Cardex, CacheCardex>(ui->leCardexCode, ui->leCardexName, ca);
    CI_Guest *g = CacheGuest::instance()->get(fDbRows.at(0).at(2).toString());
    if (!g) {
        if (!ui->leGuest->fHiddenText.isEmpty()) {
            g = CacheGuest::instance()->get(ui->leGuest->fHiddenText);
        }
    }
    if (g) {
        ui->leGuest->setText(g->fName);
        ui->leGuest->fHiddenText = g->fCode;
    } else {
        ui->leGuest->clear();
        ui->leGuest->fHiddenText.clear();
    }
}

void DlgGroupParams::setDate(const QDate &d1, const QDate &d2)
{
    ui->deStart->setDate(d1);
    ui->deEnd->setDate(d2);
}

QStringList DlgGroupParams::checkedRooms()
{
    QStringList l;
    for (int i = 0; i < ui->tblRoom->rowCount(); i++) {
        if (ui->tblRoom->checkBox(i, 0)->isChecked()) {
            l << ui->tblRoom->toString(i, 1);
        }
    }
    return l;
}

bool DlgGroupParams::roomChecked(const QString &room)
{
    for (int i = 0; i < ui->tblRoom->rowCount(); i++) {
        if (ui->tblRoom->toString(i, 1) == room) {
            return ui->tblRoom->checkBox(i, 0)->isChecked();
        }
    }
    return false;
}

bool DlgGroupParams::date(QDate &d1, QDate &d2)
{
    if (ui->chDate->isChecked()) {
        d1 = ui->deStart->date();
        d2 = ui->deEnd->date();
        return true;
    }
    return false;
}

bool DlgGroupParams::cardex(QVariant &car)
{
    if (ui->chCardex->isChecked()) {
        car = ui->leCardexCode->text();
        return true;
    }
    return false;
}

bool DlgGroupParams::arrangement(QVariant &arr)
{
    if (ui->chArrangement->isChecked()) {
        arr = ui->cbArrangement->currentData();
        return true;
    }
    return false;
}

bool DlgGroupParams::guest(QVariant &guest)
{
    if (ui->chGuest->isChecked()) {
        guest = ui->leGuest->fHiddenText;
        return true;
    }
    return false;
}

bool DlgGroupParams::remarks(QVariant &rem)
{
    if (ui->chRemarks->isChecked()) {
        rem = ui->teRemarks->toPlainText();
        return true;
    }
    return false;
}

void DlgGroupParams::prepareForCreate()
{
    DatabaseResult dr;
    dr.select(fDb, "select rc.f_short, f_room, f_rate "
              "from f_room rm "
              "left join f_room_classes rc on rc.f_id=rm.f_class "
              "order by 1, 2 ", fDbBind);
    ui->tblRoom->setRowCount(dr.rowCount());
    for (int i = 0; i < dr.rowCount(); i++) {
        ui->tblRoom->addCheckBox(i, 0);
        ui->tblRoom->setItemWithValue(i, 1, dr.value(i, 1));
        ui->tblRoom->setItemWithValue(i, 2, dr.value(i, 0));
        ui->tblRoom->setItemWithValue(i, 3, dr.value(i, 2));
    }
}

void DlgGroupParams::on_btnCancel_clicked()
{
    reject();
}

void DlgGroupParams::on_chAllRoom_clicked(bool checked)
{
    for (int i = 0; i < ui->tblRoom->rowCount(); i++) {
        ui->tblRoom->checkBox(i, 0)->setChecked(checked);
    }
}

void DlgGroupParams::on_btnSave_clicked()
{
    accept();
}

void DlgGroupParams::on_btnAddGuest_clicked()
{
    QList<QVariant> values;
    WGuest *g = WGuest::guest(values);
    if (g->exec() == QDialog::Accepted) {
        CI_Guest *guest = CacheGuest::instance()->get(values.at(0).toString());
        int wait = 10000;
        while (guest == 0 && wait > 0) {
            qApp->processEvents();
            guest = CacheGuest::instance()->get(values.at(0).toString());
            wait--;
        }
        if (!guest) {
            message_error_tr("Cannot register new guest, contact to application developer");
            return;
        }
        ui->leGuest->setText(guest->fName);
        ui->leGuest->fHiddenText = guest->fCode;
    }
    delete g;
}

void DlgGroupParams::on_leGuest_returnPressed()
{
    if (ui->leGuest->text().trimmed().isEmpty()) {
        return;
    }
    QStringList fn = ui->leGuest->text().split(" ");
    if (fn.count() > 0) {
        QString searchName = fn.at(0).toLower();
        if (fn.count() > 1) {
            searchName += " " + fn.at(1).toLower();
        }
        fDb.select("select f_id from f_guests where concat(lower(f_firstName), ' ', lower(f_lastName)) like '" + searchName + "%'", fDbBind, fDbRows);
        if (fDbRows.count()  > 0) {
            fDockGuest->setFilterValue(ui->leGuest->text());
            fDockGuest->show();
            fDockGuest->activateWindow();
            fDockGuest->raise();
        } else {
            fDbBind[":f_title"] = "";
            fDbBind[":f_firstName"] = fn.at(0);
            if (fn.count() > 1) {
                fDbBind[":f_lastName"] = fn.at(1);
            } else {
                fDbBind[":f_lastName"] = "";
            }
            int gid = fDb.insert("f_guests", fDbBind);
            CI_Guest g;
            g.fCode = QString::number(gid);
            g.fName = fn.at(0) + " ";
            if (fn.count() > 1) {
                g.fName += fn.at(1);
            }
            BroadcastThread::cmdRefreshCache(cid_guest, g.fCode);
            ui->leGuest->setText(g.fName);
            ui->leGuest->fHiddenText = g.fCode;
        }
    }
}

void DlgGroupParams::on_chAllCheckBox_clicked(bool checked)
{
    ui->chCardex->setChecked(checked);
    ui->chGuest->setChecked(checked);
    ui->chDate->setChecked(checked);
    ui->chRemarks->setChecked(checked);
    ui->chArrangement->setChecked(checked);
}

void DlgGroupParams::on_leGuest_textChanged(const QString &arg1)
{
    ui->chGuest->setChecked(!arg1.isEmpty());
}

void DlgGroupParams::on_leCardexCode_textChanged(const QString &arg1)
{
    ui->chCardex->setChecked(!arg1.isEmpty());
}

void DlgGroupParams::on_cbArrangement_currentIndexChanged(int index)
{
    Q_UNUSED(index)
    ui->chArrangement->setChecked(true);
}

void DlgGroupParams::on_teRemarks_textChanged()
{
    ui->chRemarks->setChecked(!ui->teRemarks->toPlainText().isEmpty());
}
