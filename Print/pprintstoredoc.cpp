#include "pprintstoredoc.h"
#include "pprintpreview.h"
#include "pprintscene.h"
#include "ptextrect.h"
#include "cacheinvoiceitem.h"
#include "pimage.h"
#include "cachestoredoc.h"
#include "cachedish.h"
#include "cachereststore.h"
#include <QApplication>
#include <QMessageBox>

PPrintStoreDoc::PPrintStoreDoc(QWidget *parent) :
    BaseWidget(parent)
{

}

void PPrintStoreDoc::print(const QString &id)
{
    PPrintStoreDoc p;
    p.fDocId = id;
    p.printDoc();
}

void PPrintStoreDoc::printDoc()
{
    DatabaseResult dr1;
    fDbBind[":f_id"] = fDocId;
    dr1.select(fDb, "select * from r_docs where f_id=:f_id", fDbBind);
    if (dr1.rowCount() == 0) {
        QMessageBox::critical(fPreferences.getDefaultParentForMessage(), tr("Error"), tr("Document number is not exists"));
        return;
    }

    int rowHeight = 80;
    int top = 5;
    PPrintPreview *pp = new PPrintPreview();
    PPrintScene *ps = pp->addScene(0, Portrait);
    QString invHeader = tr("Store document #") + fDocId;
    PTextRect *trHeader = new PTextRect(20, 20, 2100, 200, invHeader, 0, QFont(qApp->font().family(), 50));
    trHeader->setBorders(false, false, false, false);
    trHeader->setTextAlignment(Qt::AlignHCenter);
    top += ps->addTextRect(trHeader)->textHeight();
    PTextRect trAll;
    trAll.setBorders(false, false, false, false);
    trAll.setFont(QFont(qApp->font().family(), 30));
    ps->addTextRect(10, top, 600, rowHeight, tr("Date"), &trAll);
    top += ps->addTextRect(610, top, 400, rowHeight, dr1.value("f_date").toDate().toString(def_date_format), &trAll)->textHeight();
    top += ps->addTextRect(10, top, 600, rowHeight, CacheStoreDocType::instance()->get(dr1.value("f_type").toString())->fName, &trAll)->textHeight();

    DatabaseResult dr2;
    fDbBind[":f_doc"] = fDocId;
    dr2.select(fDb, "select * from r_body where f_doc=:f_doc and f_sign=1", fDbBind);
    if (dr1.value("f_type").toInt() == 3) {
        fDbBind[":f_doc"] = fDocId;
        dr2.select(fDb, "select * from r_body where f_doc=:f_doc", fDbBind);
    }

    if (dr2.rowCount() > 0) {
        QString storage = tr("Storage, input");
        if (dr1.value("f_type").toInt() == 3) {
            storage = tr("Storage, output");
        }
        ps->addTextRect(10, top, 460, rowHeight, storage, &trAll);
        top += ps->addTextRect(610, top, 400, rowHeight, CacheRestStore::instance()->get(dr2.value("f_store").toString())->fName, &trAll)->textHeight();
        if (dr1.value("f_type").toInt() == 2) {
            DatabaseResult dr3;
            dr3.select(fDb, "select * from r_body where f_doc=:f_doc and f_sign=-1", fDbBind);
            ps->addTextRect(10, top, 600, rowHeight, tr("Storage, output"), &trAll);
            top += ps->addTextRect(610, top, 400, rowHeight, CacheRestStore::instance()->get(dr2.value("f_store").toString())->fName, &trAll)->textHeight();
        }
    }

    trAll.setBorders(true, true, true, true);
    for (int i = 0; i < dr2.rowCount(); i++) {
        if (top > sizePortrait.height()) {
            top = 5;
            ps = pp->addScene(0, Portrait);
        }
        QList<int> col;
        col << 10 << 800 << 250 << 250 << 250;
        QStringList val;
        val << CacheDish::instance()->get(dr2.value(i, "f_material").toString())->fName
            << float_str(dr2.value(i, "f_qty").toDouble(), 2)
            << float_str(dr2.value(i, "f_price").toDouble(), 2)
            << float_str(dr2.value(i, "f_total").toDouble(), 2);
        ps->addTableRow(top, rowHeight, col, val, &trAll);
    }
    top += 10;
    ps->addTextRect(10, top, 800, rowHeight, tr("Total"), &trAll);
    ps->addTextRect(810, top, 300, rowHeight, float_str(dr1.value("f_amount").toDouble(), 2), &trAll);
    pp->exec();
    delete pp;
}
