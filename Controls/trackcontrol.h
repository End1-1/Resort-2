#ifndef TRACKCONTROL_H
#define TRACKCONTROL_H

#include <QString>
#include <QList>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QMap>

#define TRACK_RESERVATION 1
#define TRACK_GUEST 3
#define TRACK_CONTACT 4
#define TRACK_NOTE 5
#define TRACK_ROOM 6
#define TRACK_USER_GROUP 7
#define TRACK_USER 8
#define TRACK_ROOM_VIEW 9
#define TRACK_PARTNERS 10
#define TRACK_REST_HALL 11
#define TRACK_MENU_NAME 12
#define TRACK_REST_TABLE 13
#define TRACK_MENU_PART 14
#define TRACK_DISH_TYPE 15
#define TRACK_DISH 16
#define TRACK_DISH_MOD 17
#define TRACK_REST_STORE 18
#define TRACK_REST_PRINTERS 19
#define TRACK_USER_RIGHTS 20
#define TRACK_CREDIT_CARD 21
#define TRACK_DISH_COMPLEX 22
#define TRACK_REST_ORDER 23
#define TRACK_ROOM_CATEGORY 24
#define TRACK_ROOM_BED 25
#define TRACK_ROOM_CATEGORY_RATE 26
#define TRACK_CURRENCY 27
#define TRACK_INVOICE_ITEM 28
#define TRACK_GLOBAL_CONFIG 29
#define TRACK_CITY_LEDGER 30
#define TRACK_ROOM_STATE 31
#define TRACK_RESERVATION_REMARKS 32
#define TRACK_BANQUET_COMMENT 33
#define TRACK_VAUCHER 34
#define TRACK_COMPLIMENTARY_COMMENT 35
#define TRACK_REPORT 36
#define TRACK_MARKET_SIGMENT 37
#define TRACK_NATIONALITY_FILE 38
#define TRACK_GUEST_TITLE 39
#define TRACK_STORE_PARTNERS 40
#define TRACK_COUPONS 41
#define TRACK_MODELOF_CARS 42
#define TRACK_CAR_CLIENT 43
#define TRACK_COUPON_SERIA 44
#define TRACK_CASH_DESK 45
#define TRACK_ROOM_INVENTORY 46
#define TRACK_ROOM_INVENTORY_STATE 47
#define TRACK_RESERVATION_GROUP 48

class QWidget;

bool cmpWidgetName(QWidget *w, const QString &with);
bool isLineEdit(QWidget *w);
bool isEDateEdit(QWidget *w);
bool isDoubleEdit(QWidget *w);
bool isColorLineEdit(QWidget *w);
bool isTimeEdit(QWidget *w);
bool isDateEdit(QWidget *w);
bool isCheckBox(QWidget *w);
bool isTextEdit(QWidget *w);
bool isComboBox(QWidget *w);
bool isComboCompleter(QWidget *w);
bool isComboMonth(QWidget *w);
bool isSpinBox(QWidget *w);

class TrackWidget {
public:
    QWidget *fWidget;
    QString fKey;
    QString fOldValue;
    TrackWidget ();
} ;

class TrackControl : public QObject
{
    Q_OBJECT
public:
    static QString fDbHost;
    static QString fDbDb;
    static QString fDbUser;
    static QString fDbPass;
    int fTable;
    TrackControl(int table);
    QString fRecord;
    QString fInvoice;
    QString fReservation;
    ~TrackControl();
    TrackControl &addWidget(QWidget *w, const QString &key);
    QListIterator<TrackWidget *> widgetIterator();
    bool hasChanges();
    void saveChanges();
    void resetChanges();
    bool isValueChanged(QWidget *w);
    QString oldValue(QWidget *w);
    QString newValue(const QString &trackName);
    void insertMessage(const QString &msg, const QString &v1, const QString &v2);
    void insert(const QString &action, const QVariant &value1, const QVariant &value2);
    static void insert (int table, const QString &action, const QVariant &value1, const QVariant &value2, const QString &record = "-", const QString &invoice = "-", const QString &reservation = "-");
private:
    QList<TrackWidget*> fTrackWidgets;
    QMap<QString, TrackWidget*> fTrackWidgetsMap;
    bool isTrackChanged(TrackWidget &t);
    void setOldValue(TrackWidget &t);
    QString getNewValue(TrackWidget &t);
    QStringList fMsgMsg;
    QStringList fMsgOld;
    QStringList fMsgNew;
    static QSqlDatabase *fDb;
private slots:
    void lineEditTextChanged(const QString &text);
    void edateTextChanged(const QString &text);
    void dateEditChanged(const QDate &date);
    void textEditTextChanged();
    void checkBoxClicked();
    void comboIndexChanged(int index);
    void spinValueChanged(int value);
    void timeEditChanged(const QTime &time);
};

Q_DECLARE_METATYPE(TrackControl*)

#endif // TRACKCONTROL_H
