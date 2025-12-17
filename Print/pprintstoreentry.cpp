#include "pprintstoreentry.h"
#include "pprintpreview.h"
#include "pprintscene.h"
#include "ptextrect.h"
#include "pimage.h"
#include "cachedish.h"
#include "cachestoredoc.h"
#include "cachereststore.h"
#include <QApplication>
#include <QMessageBox>

PPrintStoreEntry::PPrintStoreEntry(QWidget *parent) :
    BaseWidget(parent)
{
}

void PPrintStoreEntry::print(const QString &id)
{
    PPrintStoreEntry p;
    p.fDockId = id;
    p.printDoc();
}

void PPrintStoreEntry::printDoc()
{
    DatabaseResult dr1;
    fDbBind[":f_id"] = fDockId;
    dr1.select(fDb, "select * from st_header where f_id=:f_id", fDbBind);

    if(dr1.rowCount() == 0) {
        QMessageBox::critical(fPreferences.getDefaultParentForMessage(), tr("Error"), tr("Document number is not exists"));
        return;
    }

    int rowHeight = 80;
    int top = 5;
    PPrintPreview *pp = new PPrintPreview();
    PPrintScene *ps = pp->addScene(0, Portrait);
    QString invHeader = tr("Store document") + " #" + fDockId;
    PTextRect *trHeader = new PTextRect(20, 20, 2100, 200, invHeader, 0, QFont(qApp->font().family(), 50));
    trHeader->setBorders(false, false, false, false);
    trHeader->setTextAlignment(Qt::AlignHCenter);
    top += ps->addTextRect(trHeader)->textHeight();
    PTextRect trAll;
    trAll.setBorders(false, false, false, false);
    trAll.setFont(QFont(qApp->font().family(), 30));
    ps->addTextRect(10, top, 600, rowHeight, tr("Date"), &trAll);
    top += ps->addTextRect(610, top, 400, rowHeight, dr1.value("f_date").toDate().toString(def_date_format), &trAll)->textHeight();
    ps->addTextRect(10, top, 600, rowHeight, tr("Storage"), &trAll);
    top += ps->addTextRect(610, top, 400, rowHeight, CacheRestStore::instance()->get(dr1.value("f_store").toString())->fName, &trAll)->textHeight();
    DatabaseResult dr2;
    fDbBind[":f_doc"] = fDockId;
    dr2.select(fDb, "select * from st_body where f_doc=:f_doc", fDbBind);
    trAll.setBorders(true, true, true, true);

    for(int i = 0; i < dr2.rowCount(); i++) {
        if(top > sizePortrait.height()) {
            top = 5;
            ps = pp->addScene(0, Portrait);
        }

        QList<int> col;
        col << 10 << 800 << 250 << 250 << 250;
        QStringList val;
        val << CacheDish::instance()->get(dr2.value(i, "f_goods").toString())->fName
            << float_str(dr2.value(i, "f_qty").toDouble(), 2)
            << float_str(dr2.value(i, "f_amount").toDouble(), 2);
        ps->addTableRow(top, rowHeight, col, val, &trAll);
    }

    top += 10;
    ps->addTextRect(10, top, 800, rowHeight, tr("Total"), &trAll);
    ps->addTextRect(810, top, 300, rowHeight, float_str(dr1.value("f_amount").toDouble(), 2), &trAll);
    pp->exec();
    delete pp;
}
