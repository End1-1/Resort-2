#include "dialog.h"
#include <QKeyEvent>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent)
{

}

void Dialog::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) {
        enter();
        return;
    }
    QDialog::keyPressEvent(e);
}

void Dialog::enter()
{

}
