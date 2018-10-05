#ifndef WUSERS_H
#define WUSERS_H

#include "wreportgrid.h"

class WUsers : public WReportGrid
{
    Q_OBJECT
public:
    WUsers(QWidget *parent = 0);
protected:
    virtual void setupTab();
    virtual void processValues(int row, bool isNew);
private slots:
    void changePassword();
};

#endif // WUSERS_H
