#ifndef FRESERVATIONSCOMMON_H
#define FRESERVATIONSCOMMON_H

#include "wfilterbase.h"
#include "dwselectorreservestatus.h"
#include "dwselectorreservestate.h"
#include "dwselectorroom.h"
#include "dwselectorroomarrangment.h"
#include "dwselectorcardex.h"
#include "dwselectorreservationgroup.h"

namespace Ui {
class FReservationsCommon;
}

class WReportGrid;

class FReservationsCommon : public WFilterBase
{
    Q_OBJECT

public:
    explicit FReservationsCommon(QWidget *parent = 0);
    ~FReservationsCommon();
    virtual void apply(WReportGrid *rg);
    virtual QWidget *firstElement();
    virtual QString reportTitle();
private:
    Ui::FReservationsCommon *ui;
    DWSelectorReserveStatus *fDockStatus;
    DWSelectorReserveState *fDockState;
    DWSelectorRoom *fDockRoom;
    DWSelectorRoomArrangment *fDockArr;
    DWSelectorCardex *fDockCardex;
    DWSelectorReservationGroup *fDockGroup;
private slots:
    void state(CI_ReserveState *c);
    void status(CI_ReserveStatus *c);
    void room(CI_Room *c);
    void arrangement(CI_RoomArrangment *c);
    void reserveGroup(CI_ReserveGroup *c);
    void cardex(CI_Cardex *c);
    void openInvoice();
};

#endif // FRESERVATIONSCOMMON_H
