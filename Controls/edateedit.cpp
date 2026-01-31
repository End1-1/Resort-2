#include "edateedit.h"
#include <QHBoxLayout>
#include <QStyle>
#include <QDate>
#include <QAction>

QDate EDateEditFirstDate = QDate::currentDate();

EDateEdit::EDateEdit(QWidget *parent) : QLineEdit(parent)
{
    setInputMask("00/00/0000");
    setText(QDate::currentDate().toString("dd/MM/yyyy"));
    setFixedWidth(100);
    // Иконка справа внутри lineedit (ровно и красиво)
    QIcon ico = QIcon(":/images/calendar.png");
    calAction = addAction(ico, QLineEdit::TrailingPosition);
    calAction->setToolTip("Open calendar");
    // Чуть-чуть сдвинем текст, чтобы не залезал под иконку
    setTextMargins(2, 0, 2, 0);
    connect(calAction, &QAction::triggered, this, &EDateEdit::showCalendar);
    connect(this, &QLineEdit::textChanged, this, &EDateEdit::newText);
    calendar = new QCalendarWidget();
    calendar->setWindowFlags(Qt::Popup);
    calendar->setGridVisible(true);
    connect(calendar, &QCalendarWidget::clicked, this, &EDateEdit::dateSelected);
}

QString EDateEdit::getField()
{
    return fField;
}

void EDateEdit::setField(const QString &field)
{
    fField = field;
}

void EDateEdit::focusInEvent(QFocusEvent *e)
{
    QLineEdit::focusInEvent(e);
    setCursorPosition(0);
}

void EDateEdit::showCalendar()
{
    QDate d = QDate::fromString(text(), "dd/MM/yyyy");

    if(d.isValid())
        calendar->setSelectedDate(d);

    QPoint p = mapToGlobal(QPoint(0, height()));
    calendar->move(p);
    calendar->show();
}

void EDateEdit::dateSelected(const QDate &d)
{
    setText(d.toString("dd/MM/yyyy"));
    calendar->hide();
    emit dateChanged(d);
}

void EDateEdit::newText(const QString &s)
{
    QDate d = QDate::fromString(s, "dd/MM/yyyy");
    bool ok = d.isValid();
    setStyleSheet(ok ? "" : "background:#ffaaaa;");
}
