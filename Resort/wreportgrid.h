#ifndef WREPORTGRID_H
#define WREPORTGRID_H

#include "basewidget.h"
#include "tablemodel.h"
#include "roweditordialog.h"
#include "wfilterbase.h"
#include "pprintpreview.h"
#include "cacherights.h"

#define tr_col(lang, width) (def_lang == lang ? width : 0)

namespace Ui {
class WReportGrid;
}

class WReportGrid : public BaseWidget
{
    Q_OBJECT
public:
    friend class WFilterBase;
    friend class FTrackChanges;
    friend class FCityLedgerBalance;
    friend class FCategoryToSell;
    explicit WReportGrid(QWidget *parent = 0);
    ~WReportGrid();
    QToolButton *addToolBarButton(const QString &image, const QString &text, const char *slot, QObject *receiver = 0);
    int fillRowValuesOut(QList<QVariant> &out);
    void setTblTotalData(const QList<int> &columns, const QList<double> &values);
    void setTblNoTotalData();
    void countToTotal(int col);
    virtual void setupTabTextAndIcon(const QString &name, const QString &image);
    void addFilterWidget(WFilterBase *f);

    template<class T>
    void setStaticModel(const QString &sqlQuery, const QList<int> &widths, const QStringList &titles,
                        const QString &title, const QString &iconName) {
        fStaticQuery = sqlQuery;
        for (int i = 0; i < widths.count(); i++) {
            fModel->setColumn(widths.at(i), "", titles.at(i));
        }
        setupTabTextAndIcon(title, iconName);
        fFilter = new T(this);
        addFilterWidget(fFilter);
        fFilter->apply(this);
    }

    template<class T>
    void setQueryModel(const QMap<QString,int> &widhts, const QStringList &fields, const QMap<QString, QString> &titles,
                       const QMap<QString,bool> &includes, const QStringList &tables, const QStringList &joins,
                       const QStringList &joinConds, const QString &title, const QString &iconName) {
        fFieldsWidths = widhts;
        fFields = fields;
        fFieldTitles = titles;
        fIncludes = includes;
        fTables = tables;
        fJoins = joins;
        fJoinConds = joinConds;
        setupTabTextAndIcon(title, iconName);
        fFilter = new T(this);
        addFilterWidget(fFilter);
        fFilter->apply(this);
    }

    template<class T>
    void setQueryModel() {
        fFilter = new T(this);
        fFilter->setup();
        addFilterWidget(fFilter);
        fFilter->apply(this);
    }

    virtual void getModelData(QList<QList<QVariant> > &out);

    template<class T>
    T *fullSetup(const QList<int> widths, const QStringList &fields, const QStringList &titles,
                   const QString &title, const QString &icon, const QString &query, bool showNewBtn = true) {
        setupTabTextAndIcon(title, icon);
        for (int i = 0, count = widths.count(); i < count; i++) {
            fModel->setColumn(widths.at(i), fields.at(i), titles.at(i));
        }
        fStaticQuery = query;
        fModel->setSqlQuery(query);
        fModel->apply(this);
        T *t = 0;
        if (showNewBtn) {
            t = new T(fRowValues, this);
            fRowEditorDialog = t;
            setBtnNewVisible(showNewBtn);
        }
        return t;
    }
    void setHelp(const QString &helpFile);
    TableModel *fModel;
    QTableView *fTableView;
    EQTableWidget *fTableTotal;
    QString fStaticQuery;
    WFilterBase *fFilter;
    QMap<QString, int> fFieldsWidths;
    QStringList fFields;
    QMap<QString, QString> fFieldTitles;
    QStringList fTables;
    QStringList fJoins;
    QStringList fJoinConds;
    BaseWidget *fTitleWidget;
    QMenu *fGridMenu;
    QMap<QString,bool> fIncludes;
    void dontResizeSave(bool v);
    void setBtnNewVisible(bool value = true);
    virtual QWidget *gridOptionWidget();

public slots:
    virtual void on_btnRefresh_clicked();

protected:
    QString fGridClassName;
    QString fHelp;
    RowEditorDialog *fRowEditorDialog;
    QList<QVariant> fRowValues;
    virtual void processValues(int row, bool isNew);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual bool event(QEvent *event);

private slots:
    void newSum(const QList<int> &columns, const QList<double> &values);
    void tblMainHeaderClicked(int index);
    void tblMainHeaderResized(int index, int oldSize, int newSize);
    void tblMainCustomeMenu(const QPoint &point);
    void tblMaingridCustomMenu(const QPoint &point);
    void actionCopyGrid(bool v);
    void actionFilterColumn(bool v);
    void rowCount(int count);
    void totalGridHScroll(int value);
    void tblMainGridHScroll(int value);
    void on_leQuickSearch_textChanged(const QString &arg1);
    void on_btnExcel_clicked();
    void on_tblMain_doubleClicked(const QModelIndex &index);
    void on_btnNew_clicked();
    void on_btnFilter_clicked();
    void on_tblMain_clicked(const QModelIndex &index);
    void on_btnHideFilter_clicked();
    void on_btnApplyFilter_clicked();
    void on_btnClearQuickSearch_clicked();
    void on_btnPrint_clicked();
    void on_btnHelp_clicked();
    void on_btnConfigGrid_clicked();
    void endApply();

private:
    Ui::WReportGrid *ui;
    QMenu *fTableMenu;
    bool fSaveResizedColumn;
    void fillRowValues(int row);

signals:
    void doubleClickOnRow(const QList<QVariant> &values);
    void doubleClickOnRow2(int row, int column, const QList<QVariant> &values);
    void clickOnRow(int row);
    void newBtn();
};

#endif // WREPORTGRID_H
