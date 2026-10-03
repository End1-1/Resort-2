#include "fresortlog.h"
#include "ui_fresortlog.h"
#include "wreportgrid.h"
#include "defines.h"

#include <QDialog>
#include <QDialogButtonBox>
#include <QTextEdit>
#include <QVBoxLayout>

FResortLog::FResortLog(QWidget *parent)
    : WFilterBase(parent)
    , ui(new Ui::FResortLog)
{
    ui->setupUi(this);
    ui->cbEntityType->clear();
    ui->cbEntityType->addItem(tr("All"), QString());
    ui->cbEntityType->addItem(tr("Dish"), QStringLiteral("dish"));
    ui->cbEntityType->addItem(tr("Store document"), QStringLiteral("store_doc"));
    ui->cbEntityType->addItem(tr("Complex dish"), QStringLiteral("dish_complex"));
    ui->cbEntityType->addItem(tr("Gift card"), QStringLiteral("gift_card"));
    ui->cbEntityType->addItem(tr("Store recalc"), QStringLiteral("store_recalc"));
    fReportGrid->setupTabTextAndIcon(tr("Resort audit log"), ":/images/settings.png");
    ui->deStart->setDate(QDate::currentDate().addDays(-30));
    ui->deEnd->setDate(QDate::currentDate());

    fDockUser = new DWSelectorUsers(this);
    fDockUser->configure();
    fDockUser->setSelector(ui->leUser);
    connect(fDockUser, SIGNAL(user(CI_User*)), this, SLOT(user(CI_User*)));

    connect(fReportGrid, SIGNAL(doubleClickOnRow(QList<QVariant>)), this, SLOT(doubleClickOnRow(QList<QVariant>)));
}

FResortLog::~FResortLog()
{
    delete ui;
}

QString FResortLog::reportTitle()
{
    return tr("Resort audit log %1 - %2")
            .arg(ui->deStart->date().toString(def_date_format))
            .arg(ui->deEnd->date().toString(def_date_format));
}

QWidget *FResortLog::firstElement()
{
    return ui->deStart;
}

void FResortLog::apply(WReportGrid *rg)
{
    rg->fModel->clearColumns();
    rg->fModel->setColumn(140, QString(), tr("Date/time"))
            .setColumn(120, QString(), tr("User"))
            .setColumn(90, QString(), tr("Type"))
            .setColumn(70, QString(), tr("Record"))
            .setColumn(120, QString(), tr("Action"))
            .setColumn(140, QString(), tr("Field"))
            .setColumn(200, QString(), tr("Old value"))
            .setColumn(200, QString(), tr("New value"));

    QString where = QStringLiteral("where date(l.f_datetime) between '%1' and '%2' ")
            .arg(ui->deStart->date().toString(def_mysql_date_format))
            .arg(ui->deEnd->date().toString(def_mysql_date_format));

    if(!ui->leUser->fHiddenText.isEmpty()) {
        where += QStringLiteral(" and l.f_user_id in (%1) ").arg(ui->leUser->fHiddenText);
    }

    const QString entityType = ui->cbEntityType->currentData().toString();
    if(!entityType.isEmpty()) {
        where += QStringLiteral(" and l.f_entity_type='%1' ").arg(entityType);
    }

    if(!ui->leEntityId->text().trimmed().isEmpty()) {
        where += QStringLiteral(" and l.f_entity_id='%1' ")
                .arg(ui->leEntityId->text().trimmed());
    }

    const QString query = QStringLiteral(
                "select l.f_id, l.f_datetime, l.f_user, l.f_entity_type, l.f_entity_id, "
                "l.f_action, l.f_field, l.f_value_old, l.f_value_new, l.f_snapshot "
                "from r_resort_log l ") + where
            + QStringLiteral(" order by l.f_datetime desc, l.f_id desc ");

    rg->fModel->setSqlQuery(query);
    rg->fModel->apply(rg);
    rg->fTableView->resizeColumnsToContents();
}

void FResortLog::user(CI_User *c)
{
    dockResponse<CI_User, CacheUsers>(ui->leUser, c);
}

void FResortLog::doubleClickOnRow(const QList<QVariant> &row)
{
    if(row.count() < 10) {
        return;
    }

    QString text;
    text += tr("Date/time") + QStringLiteral(": ") + row.at(1).toString() + QStringLiteral("\n\n");
    text += tr("User") + QStringLiteral(": ") + row.at(2).toString() + QStringLiteral("\n");
    text += tr("Type") + QStringLiteral(": ") + row.at(3).toString() + QStringLiteral("\n");
    text += tr("Record") + QStringLiteral(": ") + row.at(4).toString() + QStringLiteral("\n");
    text += tr("Action") + QStringLiteral(": ") + row.at(5).toString() + QStringLiteral("\n\n");

    const QString field = row.at(6).toString();
    if(!field.isEmpty()) {
        text += tr("Field") + QStringLiteral(": ") + field + QStringLiteral("\n\n");
    }

    const QString oldVal = row.at(7).toString();
    const QString newVal = row.at(8).toString();
    const QString snapshot = row.at(9).toString();

    if(!oldVal.isEmpty()) {
        text += tr("Old value") + QStringLiteral(":\n") + oldVal + QStringLiteral("\n\n");
    }
    if(!newVal.isEmpty()) {
        text += tr("New value") + QStringLiteral(":\n") + newVal + QStringLiteral("\n\n");
    }
    if(!snapshot.isEmpty() && snapshot != oldVal) {
        text += tr("Snapshot before change") + QStringLiteral(":\n") + snapshot;
    }

    QDialog dlg(fReportGrid);
    dlg.setWindowTitle(tr("Audit log entry"));
    dlg.resize(900, 600);
    auto *layout = new QVBoxLayout(&dlg);
    auto *editor = new QTextEdit(&dlg);
    editor->setReadOnly(true);
    editor->setPlainText(text);
    layout->addWidget(editor);
    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Close, &dlg);
    connect(buttons, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);
    layout->addWidget(buttons);
    dlg.exec();
}
