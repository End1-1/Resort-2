#ifndef EQCHECKBOX_H
#define EQCHECKBOX_H

#include <QCheckBox>

class EQCheckBox : public QCheckBox
{
    Q_OBJECT
    Q_PROPERTY(QString Field READ getField WRITE setField)
    Q_PROPERTY(int Tag READ getTag WRITE setTag)
    Q_PROPERTY(bool RequireLang READ getRequireLang WRITE setRequireLang)
public:
    EQCheckBox(QWidget *parent = 0);
    QString getField();
    void setField(const QString &name);
    int getTag();
    void setTag(int tag);
    void setStyle(const QString &style);
    bool getRequireLang();
    void setRequireLang(bool value);
    int fRow;
    int fColumn;
private:
    QString fField;
    int fTag;
    bool fRequireLang;
};

#endif // EQCHECKBOX_H
