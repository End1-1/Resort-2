#include "dlgcreategroupreservation.h"
#include "ui_dlgcreategroupreservation.h"
#include "wreservation.h"
#include "wreservationroomtab.h"
#include "cacheroomcategory.h"
#include "cachebed.h"
#include "dlgreservationremarks.h"
#include "wguest.h"

DlgCreateGroupReservation::DlgCreateGroupReservation(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgCreateGroupReservation)
{
    ui->setupUi(this);
    Utils::tableSetColumnWidths(ui->tblCount, ui->tblCount->columnCount(), 50, 30, 30, 30, 80, 80);
    CI_Cardex *cdx= CacheCardex::instance()->get(fPreferences.getDb(def_default_cardex).toString());
    cardex(cdx);
    fDockGuest = new DWSelectorGuest(this);
    fDockGuest->configure();
    fDockGuest->setSelector(ui->leGuest);
    connect(fDockGuest, SIGNAL(guest(CI_Guest*)), this, SLOT(guest(CI_Guest*)));
    fDockCardex = new DWSelectorCardex(this);
    fDockCardex->configure();
    fDockCardex->setSelector(ui->leCardexCode);
    connect(fDockCardex, SIGNAL(cardex(CI_Cardex*)), this, SLOT(cardex(CI_Cardex*)));
    loadRooms();

    QTableWidgetItem *i1 = new QTableWidgetItem(tr("Any"));
    i1->setData(Qt::UserRole, 0);
    QTableWidgetItem *i2 = new QTableWidgetItem(tr("Yes"));
    i1->setData(Qt::UserRole, 1);
    QTableWidgetItem *i3 = new QTableWidgetItem(tr("No"));
    i2->setData(Qt::UserRole, 2);
    ui->tblSmoke->setItem(0, 0, i1);
    ui->tblSmoke->setItem(0, 1, i2);
    ui->tblSmoke->setItem(0, 2, i3);

    CacheRoomCategory *c = CacheRoomCategory::instance();
    ui->tblCategory->setColumnCount(c->elementsCount());
    ui->tblCategory->setRowCount(1);
    QMapIterator<QString, CI_RoomCategory*> it = c->it();
    int col = 0;
    while (it.hasNext()) {
        it.next();
        ui->tblCategory->setItem(0, col++, new QTableWidgetItem(it.value()->fName));
    }

    CacheBed *cb = CacheBed::instance();
    ui->tblBed->setColumnCount(cb->elementsCount());
    ui->tblBed->setRowCount(1);
    col = 0;
    QMapIterator<QString, CI_Bed*> ib = cb->it();
    while (ib.hasNext()) {
        ib.next();
        ui->tblBed->setItem(0, col++, new QTableWidgetItem(ib.value()->fCode));
    }

    col = 0;
    ui->tblFloor->setColumnCount(9);
    ui->tblFloor->setRowCount(1);
    for (int i = 0; i < ui->tblFloor->columnCount(); i++) {
        ui->tblFloor->setItem(0, i, new QTableWidgetItem(QString::number(i + 1)));
    }

    fSingleMode = false;
}

DlgCreateGroupReservation::~DlgCreateGroupReservation()
{
    delete ui;
}

void DlgCreateGroupReservation::loadRooms()
{
   }

void DlgCreateGroupReservation::setSingleMode(bool mode)
{
    fSingleMode = mode;
    if (fSingleMode) {
        ui->wdtGroup->setVisible(false);
        ui->btnCreate->setText(tr("Select"));
        setWindowTitle(tr("Select room"));
    }
}

void DlgCreateGroupReservation::setReservationTab(WReservationRoomTab *t)
{
    fTab = t;
    ui->deArrival->setDate(t->date1());
    ui->deDeparture->setDate(t->date2());
    makeRooms();
}

void DlgCreateGroupReservation::groupPriceChanged(const QString &arg1)
{
    EQLineEdit *e = static_cast<EQLineEdit*>(sender());
    ui->tblCount->item(e->fRow, 4)->setText(arg1);
    ui->tblCount->setItem(e->fRow, 5, Utils::tableItem(ui->tblCount->toDouble(e->fRow, 3) * ui->tblCount->toDouble(e->fRow, 4)));
    fPrice[ui->tblCount->item(e->fRow, 0)->text()] = arg1;
}

void DlgCreateGroupReservation::singleHandle(bool v)
{
    countSelected();
    if (!fSingleMode) {
        return;
    }
    if (!v) {
        return;
    }

}

void DlgCreateGroupReservation::guest(CI_Guest *c)
{
    if (c) {
        ui->leGuest->setText(c->fName);
        ui->leGuest->fHiddenText = c->fCode;
    }
}

void DlgCreateGroupReservation::cardex(CI_Cardex *c)
{
    dockResponse<CI_Cardex, CacheCardex>(ui->leCardexCode, ui->leCardexName, c);
}
void DlgCreateGroupReservation::makeRooms()
{
    QModelIndexList selCat = ui->tblCategory->selectionModel()->selectedIndexes();
    QStringList lstCat;
    for (int i = 0; i < selCat.count(); i++) {
        lstCat << selCat.at(i).data(Qt::DisplayRole).toString();
    }
    QModelIndexList selFloor = ui->tblFloor->selectionModel()->selectedIndexes();
    QStringList lstFloor;
    for (int i = 0; i < selFloor.count(); i++) {
        lstFloor << selFloor.at(i).data(Qt::DisplayRole).toString();
    }
    QModelIndexList selBed = ui->tblBed->selectionModel()->selectedIndexes();
    QStringList lstBed;
    for (int i = 0;i < selBed.count(); i++) {
        lstBed << selBed.at(i).data(Qt::DisplayRole).toString();
    }
    QModelIndexList selSmoke = ui->tblSmoke->selectionModel()->selectedIndexes();
    int smoke = 0;
    if (selSmoke.count() > 0) {
        smoke = selSmoke.at(0).column();
    }

    countSelected();
}

void DlgCreateGroupReservation::countSelected()
{

}

void DlgCreateGroupReservation::on_btnCreate_clicked()
{

}

void DlgCreateGroupReservation::on_btnAddGuest_clicked()
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

void DlgCreateGroupReservation::on_btnClearRoomFilter_clicked()
{
    ui->tblBed->clearSelection();
    ui->tblCategory->clearSelection();
    ui->tblFloor->clearSelection();
    ui->tblSmoke->clearSelection();
    makeRooms();
}

void DlgCreateGroupReservation::on_tblCategory_clicked(const QModelIndex &index)
{
    Q_UNUSED(index)
    makeRooms();
}

void DlgCreateGroupReservation::on_tblBed_clicked(const QModelIndex &index)
{
    Q_UNUSED(index)
    makeRooms();
}

void DlgCreateGroupReservation::on_tblFloor_clicked(const QModelIndex &index)
{
    Q_UNUSED(index)
    makeRooms();
}

void DlgCreateGroupReservation::on_tblSmoke_clicked(const QModelIndex &index)
{
    Q_UNUSED(index)
    makeRooms();
}

void DlgCreateGroupReservation::on_deArrival_textEdited(const QString &arg1)
{
    Q_UNUSED(arg1)
    if (ui->deArrival->date() < WORKING_DATE) {
        ui->deArrival->setStyleSheet("background:red");
        return;
    } else {
        ui->deArrival->setStyleSheet("background:white");
    }
    ui->sbNights->setValue(ui->deArrival->date().daysTo(ui->deDeparture->date()));
    makeRooms();
}

void DlgCreateGroupReservation::on_deDeparture_textEdited(const QString &arg1)
{
    Q_UNUSED(arg1)
    if (ui->deDeparture->date() < WORKING_DATE) {
        ui->deDeparture->setStyleSheet("background:red");
        return;
    } else {
        ui->deDeparture->setStyleSheet("background:white");
    }
    ui->sbNights->setValue(ui->deArrival->date().daysTo(ui->deDeparture->date()));
    makeRooms();
}

void DlgCreateGroupReservation::on_leGuest_returnPressed()
{

}
