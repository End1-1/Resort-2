#include "dlgsessionopen.h"
#include "ui_dlgsessionopen.h"
#include "base.h"
#include "cacheusers.h"
#include "database2.h"
#include "defrest.h"
#include "preferences.h"
#include "rmessage.h"
#include "session.h"
#include "reportprint.h"
#include <QDateTime>
#include <QPushButton>

DlgSessionOpen::DlgSessionOpen(int branch, QWidget *parent,
                               const QString &printedBy, const QString &printer) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgSessionOpen),
    fBranch(branch),
    fPrintedBy(printedBy),
    fPrinter(printer)
{
    ui->setupUi(this);

    for(QPushButton *btn : {ui->btnOpen, ui->btnPrintTotal, ui->btnCancel}) {
        const int w = qBound(280, btn->sizeHint().width() + 24, 420);
        btn->setFixedSize(w, 90);
    }

    if(fPrintedBy.isEmpty()) {
        fPrintedBy = WORKING_USERNAME;

        if(fPrintedBy.isEmpty()) {
            CI_User *u = CacheUsers::instance()->get(WORKING_USERID);

            if(u) {
                fPrintedBy = u->fFull;
            }
        }
    }

    if(fPrinter.isEmpty()) {
        fPrinter = defrest(dr_first_receipt_printer);
    }

    ui->leBranch->setText(QString::number(branch));
    ui->leStart->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));

    Database2 db2;
    Db b = Preferences().getDatabase(Base::fDbName);

    if(db2.open(b.dc_main_host, b.dc_main_path, b.dc_main_user, b.dc_main_pass)) {
        db2[":f_id"] = branch;
        db2.exec("select f_name from r_branch where f_id=:f_id");

        if(db2.next()) {
            ui->leBranch->setText(db2.string("f_name"));
        }
    }
}

DlgSessionOpen::~DlgSessionOpen()
{
    delete ui;
}

void DlgSessionOpen::prepareToShow()
{
#ifdef QT_DEBUG
    showMaximized();
#else
    showFullScreen();
#endif
    qApp->processEvents();
}

bool DlgSessionOpen::ensureOpen(int branch, QWidget *parent,
                              const QString &printedBy, const QString &printer)
{
    DlgSessionOpen *d = new DlgSessionOpen(branch, parent, printedBy, printer);
    d->prepareToShow();
    const bool ok = d->exec() == QDialog::Accepted;
    delete d;
    return ok;
}

void DlgSessionOpen::on_btnOpen_clicked()
{
    const int existing = Session::findOpen(fBranch);

    if(existing > 0) {
        Session::setCurrentId(existing);
        accept();
        return;
    }

    const int id = Session::open(fBranch);

    if(id <= 0) {
        message_error(tr("Cannot open session"));
        return;
    }

    Session::setCurrentId(id);
    accept();
}

void DlgSessionOpen::on_btnPrintTotal_clicked()
{
    ReportPrint::printSessionCloseTotal(fPrintedBy, fPrinter);
}

void DlgSessionOpen::on_btnCancel_clicked()
{
    reject();
}
