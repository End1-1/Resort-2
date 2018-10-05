#ifndef DLGSMILE_H
#define DLGSMILE_H

#include "baseextendeddialog.h"
#include <QTimer>

namespace Ui {
class DlgSmile;
}

class DlgSmile : public BaseExtendedDialog {
    Q_OBJECT
public:
    explicit DlgSmile(QWidget *parent = 0);
    ~DlgSmile();
private:
    Ui::DlgSmile *ui;
    QTimer fTimer;
    bool fUp;
    int fCount;
private slots:
    void timeout();
};

#endif // DLGSMILE_H
