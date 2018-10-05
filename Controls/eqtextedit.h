#ifndef EQTEXTEDIT_H
#define EQTEXTEDIT_H

#include <QTextEdit>

class EQTextEdit : public QTextEdit
{
    Q_OBJECT
    Q_PROPERTY(QString Field READ getField() WRITE setField)
public:
    EQTextEdit(QWidget *parent = 0);
    QString getField();
    void setField(const QString &field);
    void setBgColor(const QColor &color);
protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *e);
private:
    QString fField;
signals:
    void doubleClicked();
};

#endif // EQTEXTEDIT_H
