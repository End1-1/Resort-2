#ifndef DWSELECTORVATMODE_H
#define DWSELECTORVATMODE_H

#include "dwselector.h"
#include "cachevatmode.h"

class DWSelectorVATMode : public DWSelector
{
    Q_OBJECT
public:
    DWSelectorVATMode(QWidget *parent = 0);
    virtual void configure();
private:
    EQLineEdit *fId;
    EQLineEdit *fName;
protected:
    virtual void select(const QVariant &value);
private slots:
    void focusOut();
signals:
    void vatMode(CI_VATMode *ci);
};

#endif // DWSELECTORVATMODE_H
