#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

class Dialog : public QDialog
{
public:
    Dialog(QWidget *parent = nullptr);
    virtual void keyPressEvent(QKeyEvent *e) override;

protected:
    virtual void enter();
};

#endif // DIALOG_H
