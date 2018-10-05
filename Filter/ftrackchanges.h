#ifndef FTRACKCHANGES_H
#define FTRACKCHANGES_H

#include "wfilterbase.h"
#include "dwselectortrackingtables.h"
#include "dwselectorusers.h"

namespace Ui {
class FTrackChanges;
}

class FTrackChanges : public WFilterBase
{
    Q_OBJECT
public:
    explicit FTrackChanges(QWidget *parent = 0);
    ~FTrackChanges();
    virtual void apply(WReportGrid *rg);
    virtual QWidget *firstElement();
private:
    Ui::FTrackChanges *ui;
    DWSelectorTrackingTables *fDockTable;
    DWSelectorUsers *fDockUser;
private slots:
    void tracking(CI_TrackingTables *c);
    void user(CI_User *c);
};

#endif // FTRACKCHANGES_H
