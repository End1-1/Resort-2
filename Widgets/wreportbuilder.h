#ifndef WREPORTBUILDER_H
#define WREPORTBUILDER_H

#include "basewidget.h"

namespace Ui {
class WReportBuilder;
}

class WReportBuilder : public BaseWidget
{
    Q_OBJECT

public:
    explicit WReportBuilder(QWidget *parent = 0);
    ~WReportBuilder();
    virtual void setup();
private slots:
    void on_lwNames_clicked(const QModelIndex &index);

    void on_lwCols_clicked(const QModelIndex &index);

    void on_btnNew_clicked();

    void on_btnSave_clicked();

    void on_btnExtractCols_clicked();

    void on_btnRemove_clicked();

    void on_leColTitle_textChanged(const QString &arg1);

    void on_leColWidth_textChanged(const QString &arg1);

    void on_lwFilters_clicked(const QModelIndex &index);

    void on_leFilterField_textChanged(const QString &arg1);

    void on_leFilterTitle_textChanged(const QString &arg1);

    void on_leFilterMark_textChanged(const QString &arg1);

    void on_cbFilterType_currentIndexChanged(const QString &arg1);

    void on_btnAddFilter_clicked();

    void on_btnRemoveFilter_clicked();

    void on_btnEditGrup_clicked();

    void on_leFilterBuild_textChanged(const QString &arg1);

    void on_btnAddFilter_2_clicked();

    void on_btnRemoveFilter_2_clicked();

    void on_leColField_textChanged(const QString &arg1);

    void on_leFilterExprEmpty_textChanged(const QString &arg1);

private:
    Ui::WReportBuilder *ui;
    void clearFilterFields();
};

#endif // WREPORTBUILDER_H
