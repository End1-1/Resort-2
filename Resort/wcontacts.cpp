#include "wcontacts.h"
#include "dlgcontact.h"

WContacts::WContacts(QWidget *parent) :
    WReportGrid(parent)
{
    fModel->setColumn(100, "f_id", tr("Code"))
            .setColumn(200, "f_name", tr("Name"))
            .setColumn(100, "f_tel1", tr("Tel.1"))
            .setColumn(100, "f_tel2", tr("Tel.2"))
            .setColumn(100, "f_fax", tr("Fax"))
            .setColumn(150, "f_email", tr("Email"))
            .setColumn(300, "f_notes", tr("Notes"));
    fModel->setSqlQuery("select f_id, f_name, f_tel1, f_tel2, f_fax, f_email, f_notes from f_contacts order by f_name");
    fModel->apply(this);

    setBtnNewVisible();
    fRowEditorDialog = new DlgContact(fRowValues, this);
    addToolBarButton(":/images/garbage.png", tr("Remove"), SLOT(removeContact()), this);
}

WContacts::~WContacts()
{

}

void WContacts::setupTab()
{
    setupTabTextAndIcon(tr("Contacts"), ":/images/phone-book.png");
}

void WContacts::removeContact()
{
    QList<QVariant> out;
    int row;
    if ((row = fillRowValuesOut(out)) < 0) {
        message_info_tr("Nothing is selected");
        return;
    }
    if (message_confirm_tr("Confirm to delete selected contact") != QDialog::Accepted) {
        return;
    }
    fDbBind[":f_id"] = out.at(0);
    fDb.select("delete from f_contacts where f_id=:f_id", fDbBind, fDbRows);
    fModel->removeRow(row);
}

