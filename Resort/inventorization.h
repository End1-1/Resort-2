#ifndef INVENTORIZATION_H
#define INVENTORIZATION_H

#include "basewidget.h"

namespace Ui {
class Inventorization;
}

class Inventorization : public BaseWidget
{
    Q_OBJECT

public:
    explicit Inventorization(QWidget *parent = nullptr);
    ~Inventorization();

private:
    Ui::Inventorization *ui;
    int fDocId;
    virtual void setupTab() override;

private slots:
    void editStoreClicked();
    void on_btnLoad_clicked();
    void on_tbl_cellDoubleClicked(int row, int column);
    void on_btnWriteOutput_clicked();
    void on_btnRemoveOutput_clicked();
};

#endif // INVENTORIZATION_H
