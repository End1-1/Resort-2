#ifndef GOODSMOVEMENT_H
#define GOODSMOVEMENT_H

#include "wfilterbase.h"

namespace Ui {
class goodsmovement;
}

class goodsmovement : public BaseWidget
{
    Q_OBJECT

public:
    explicit goodsmovement(QWidget *parent = nullptr);
    ~goodsmovement();

private:
    Ui::goodsmovement *ui;
    virtual void setupTab() override;

private slots:
    void editStoreClicked();
    void on_btnLoad_clicked();
};

#endif // GOODSMOVEMENT_H
