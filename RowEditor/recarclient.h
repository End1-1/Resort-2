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
    virtual void selector(int number, const QVariant &value);
    static void openReport(const QString &num);
    static void openReport1();
    static void openReport2();
    static void openReport3();
    virtual void valuesToWidgets();
private slots:
    void on_btnSave_clicked();

    void on_btnReject_clicked();
    void doubleClickOnRow(const QList<QVariant> &v);

private:
    Ui::RECarClient *ui;
};

#endif // RECARCLIENT_H
