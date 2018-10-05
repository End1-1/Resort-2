#ifndef EQTIMEEDIT_H
#define EQTIMEEDIT_H

#include <QTimeEdit>

class EQTimeEdit : public QTimeEdit
{
    Q_OBJECT
    Q_PROPERTY (QString Field READ getField WRITE setField)
public:
    EQTimeEdit(QWidget *parent = 0);
    QString getField();
    void setField(const QString &field);
    void setBgColor(const QColor &color);
    QString toString();
private:
    QString fField;
};

#endif // EQTIMEEDIT_H
