#ifndef RECARCLIENT_H
#define RECARCLIENT_H

#include "roweditordialog.h"

namespace Ui {
class RECarClient;
}

class RECarClient : public RowEditorDialog
{
    Q_OBJECT

public:
    explicit RECarClient(QList<QVariant> &values, QWidget *parent = 0);
    ~RECarClient();
    virtual void setValues() override;
    static void openReport();
    static void openReport1();
    virtual void valuesToWidgets();
private slots:
    void on_btnSave_clicked();

    void on_btnReject_clicked();
    void doubleClickOnRow(const QList<QVariant> &v);

private:
    Ui::RECarClient *ui;
};

#endif // RECARCLIENT_H
