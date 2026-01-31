#ifndef EDATEEDIT_H
#define EDATEEDIT_H

#include "defines.h"
#include <QLineEdit>
#include <QToolButton>
#include <QCalendarWidget>

class EDateEdit : public QLineEdit
{
    Q_OBJECT
    Q_PROPERTY(QString Field READ getField WRITE setField)
public:
    EDateEdit(QWidget *parent = 0);
    inline QDate date() { return QDate::fromString(text(), def_date_format);}
    inline void setDate(const QDate &date) { setText(date.toString(def_date_format)); }
    inline QString dateMySql(bool appostroph = true) {return (appostroph ? QString("'%1'").arg(date().toString(def_mysql_date_format)) : date().toString(def_mysql_date_format));}
    QString getField();
    void setField(const QString &field);
    int fRow;
    int fColumn;
protected:
    virtual void focusInEvent(QFocusEvent *e);
private:
    bool fIsValid;
    QString fField;
    QAction* calAction = nullptr;
    QCalendarWidget* calendar;
    void setBgColor(const QColor &color);
private slots:
    void showCalendar();
    void dateSelected(const QDate &d);
    void newText(const QString &arg1);
signals:
    void dateChanged(const QDate &date);
};

extern QDate EDateEditFirstDate;

#endif // EDATEEDIT_H
