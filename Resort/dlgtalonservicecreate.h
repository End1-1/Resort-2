#ifndef DLGTALONSERVICECREATE_H
#define DLGTALONSERVICECREATE_H

#include <QDialog>

namespace Ui {
class DlgTalonServiceCreate;
}

class DlgTalonServiceCreate : public QDialog
{
    Q_OBJECT

public:
    explicit DlgTalonServiceCreate(QWidget *parent = nullptr);
    ~DlgTalonServiceCreate();

private slots:
    void on_cbGroup_currentIndexChanged(int index);
    void on_btnCancel_clicked();
    void on_btnOk_clicked();

private:
    Ui::DlgTalonServiceCreate *ui;
    bool loadGroups();
    void refreshLastNumber();
    int lastNumber() const;
    QString makeCode(int groupId, int number) const;
    void showCreatedCodes(const QStringList &codes);
};

#endif // DLGTALONSERVICECREATE_H
