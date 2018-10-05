#ifndef DWSELECTOR_H
#define DWSELECTOR_H

#include "utils.h"
#include "eqlineedit.h"
#include "cachebase.h"
#include "cacherights.h"
#include "preferences.h"
#include <QDockWidget>
#include <QTableWidget>
#include <QDialog>

namespace Ui {
class DWSelector;
}

class DWSelector : public QDockWidget
{
    Q_OBJECT
public:
    explicit DWSelector(int cacheId, QWidget *parent = 0);
    ~DWSelector();
    virtual void configure() = 0;
    void setSelector(EQLineEdit *selector);
    EQLineEdit *selector();
    void setFilterValue(const QString &text);
    void setFilterList(const QStringList &filters);
    int fCurrentRow;
protected:
    QTableWidget *fTable;
    int fCacheId;
    Preferences fPreferences;
    virtual void onShow();
    virtual void onHide();
    virtual void select(const QVariant &value);
    virtual void refresh();
    void applyTextFilter();
    void adjustWidth();
    void setRefreshButtonVisible(bool v);

    inline virtual bool otherFilter(CI_Base *b) {Q_UNUSED(b); return true;}

    template<typename T, typename C>
    void simpleTable() {
        C *c = C::instance();
        fTable->setColumnCount(2);
        fTable->setRowCount(c->elementsCount());
        Utils::tableSetColumnWidths(fTable, fTable->columnCount(), 80, 300);
        Utils::tableSetHeaderCaptions(fTable, fTable->columnCount(),
            tr("Code"), tr("Name"));
        adjustWidth();
        QMapIterator<QString, T*> i = c->it();
        int row = 0;
        while (i.hasNext()) {
            i.next();
            if (!i.value()->fValid) {
                continue;
            }
            if (!fFilterList.isEmpty()) {
                if (!fFilterList.contains(i.value()->fCode)) {
                    continue;
                }
            }
            if (!otherFilter(i.value())) {
                continue;
            }
            fTable->setItem(row, 0, new QTableWidgetItem(i.value()->fCode));
            fTable->setItem(row, 1, new QTableWidgetItem(i.value()->fName));
            T *ct = i.value();
            fTable->item(row, 0)->setData(Qt::UserRole, qVariantFromValue(ct));
            row++;
        }
        fTable->setRowCount(row);
    }
    QStringList fFilterList;
public slots:
    void thisShow(bool v);
protected:
    EQLineEdit *fLineSelector;
protected slots:
   virtual void selectorFocusOut();
    virtual void cacheUpdated(const QString &id);
private:
    Ui::DWSelector *ui;
private slots:
    void thisChangeVisibility(bool v);
    void on_lineEdit_textEdited(const QString &arg1);
    void on_tblData_doubleClicked(const QModelIndex &index);
    void on_btnRefresh_clicked();

signals:
    void selectItem(CI_Base *c);
};

#endif // DWSELECTOR_H
