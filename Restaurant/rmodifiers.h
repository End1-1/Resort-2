#ifndef RMODIFIERS_H
#define RMODIFIERS_H

#include "baseextendeddialog.h"

namespace Ui {
class RModifiers;
}

class RModifiers : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit RModifiers(QWidget *parent = 0);
    ~RModifiers();
    void setModifiers(const QStringList &modifiers);
    QString mod();
private slots:
    void on_btnCancel_clicked();
    void on_btnOk_clicked();
private:
    Ui::RModifiers *ui;
    QString fMod;
};

#endif // RMODIFIERS_H
