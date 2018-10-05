#include "rerestdishtype.h"
#include "ui_rerestdishtype.h"
#include "cacherestdishtype.h"
#include <QColorDialog>

RERestDishType::RERestDishType(QList<QVariant> &values, QWidget *parent) :
    RowEditorDialog(values, TRACK_DISH_TYPE, parent),
    ui(new Ui::RERestDishType)
{
    ui->setupUi(this);
    ui->leTextColor->setText("-16777216");
    addWidget(ui->leCode, "Code");
    addWidget(ui->lePartCode, "");
    addWidget(ui->lePartName, "Part");
    addWidget(ui->leNameAm, "Name, am");
    addWidget(ui->leNameEn, "Name, en");
    addWidget(ui->leNameRu, "Name, ru");
    addWidget(ui->leBgColor, "Background color");
    addWidget(ui->leTextColor, "Text color");
    addWidget(ui->leQueue, "Queue");
    addWidget(ui->chActive, "Enabled");
    fTable = "r_dish_type";
    fCacheId = cid_rest_dish_type;
    fDockPart = new DWSelectorDishPart(this);
    fDockPart->configure();
    fDockPart->setSelector(ui->lePartCode);
    connect(fDockPart, SIGNAL(dishPart(CI_RestDishPart*)), this, SLOT(dishPart(CI_RestDishPart*)));
}

RERestDishType::~RERestDishType()
{
    delete ui;
}

bool RERestDishType::isDataCorrect()
{
    if (ui->lePartCode->asInt() == 0) {
        message_error(tr("Part code must be defined"));
        return false;
    }
    return true;
}

void RERestDishType::dishPart(CI_RestDishPart *d)
{
    if (!d) {
        if (ui->lePartCode->asInt() == 0) {
            ui->lePartCode->clear();
            ui->lePartName->clear();
            return;
        }
        d = CacheRestDishPart::instance()->get(ui->lePartCode->text());
        if (!d) {
            ui->lePartCode->clear();
            ui->lePartName->clear();
            return;
        }
    }
    ui->lePartCode->setText(d->fCode);
    ui->lePartName->setText(d->fName);
}

void RERestDishType::on_btnCancel_clicked()
{
    reject();
}

void RERestDishType::on_btnOk_clicked()
{
    save();
}

void RERestDishType::on_btnBgColor_clicked()
{
    QColor color = QColorDialog::getColor(ui->leBgColor->color(), this, tr("Background color"));
    ui->leBgColor->setText(QString::number((int)color.rgb()));
}

void RERestDishType::on_btnTextColor_clicked()
{
    QColor color = QColorDialog::getColor(ui->leTextColor->color(), this, tr("Text color"));
    ui->leTextColor->setText(QString::number((int)color.rgb()));
}
