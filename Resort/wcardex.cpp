#include "wcardex.h"
#include "dlgcardex.h"

WCardex::WCardex(QWidget *parent) :
    WReportGrid(parent)
{
    fModel->setColumn(0, "f_id", tr("Code"))
            .setColumn(50, "F_group", tr("Group"))
            .setColumn(0, "", tr("Group code"))
            .setColumn(100,"f_cardex",tr("Cardex"))
            .setColumn(100,"f_cityLedger",tr("City Ledger"))
            .setColumn(300,"f_name",tr("Name"))
            .setColumn(150,"f_addr1",tr("Address 1"))
            .setColumn(150,"f_addr2",tr("Address 2"))
            .setColumn(150,"f_contact",tr("Contact"))
            .setColumn(150,"f_contactPos",tr("Contact pos"))
            .setColumn(100,"f_email",tr("Email"))
            .setColumn(100,"f_tel",tr("Tel."))
            .setColumn(100,"f_fax",tr("Fax"))
            .setColumn(100,"f_dateFrom",tr("Valid from"))
            .setColumn(100,"f_dateTo",tr("Valid to"))
            .setColumn(100,"f_commission",tr("Commission"))
            .setColumn(0, "f_vatMode", tr("VAT code"))
            .setColumn(80, "f_vatModeName", tr("VAT"))
            .setColumn(0, "f_bookingRequired", tr("Booking required"))
            .setColumn(100, "f_extra1", tr("Extra1"))
            .setColumn(100, "f_extra2", tr("Extra2"))
            ;

    fModel->setSqlQuery("select c.f_id, c.f_group, g.f_id, c.f_cardex,f_cityLedger,c.f_name,f_addr1,f_addr2, "
                        "f_contact,f_contactPos,"
                        "f_email,f_tel,f_fax,f_dateFrom,f_dateTo,f_commission,f_vatMode, v.f_" + def_lang + ", "
                        "f_bookingRequired, f_extra1, f_extra2 "
                        "from f_cardex c "
                        "left join f_vat_mode v on v.f_id=c.f_vatMode "
                        "left join f_cardex_group g on c.f_group=g.f_code "
                        "order by c.f_cardex");
    fModel->apply(this);

    fRowEditorDialog = new DlgCardex(fRowValues, this);
    setBtnNewVisible();
}

void WCardex::setupTab()
{
    setupTabTextAndIcon(tr("Partners"), ":/images/partner.png");
}
