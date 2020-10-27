#ifndef EQLINEEDIT_H
#define EQLINEEDIT_H

#include <QLineEdit>
#include <QToolButton>
#include <QResizeEvent>
#include <QMouseEvent>

class QTableWidget;
class DWSelector;

class EQLineEdit : public QLineEdit
{
    Q_OBJECT
    Q_PROPERTY (bool ShowButton READ getShowButton WRITE setShowButtonOnFocus)
    Q_PROPERTY (bool AlwaysUpper READ getAlwaysUpper WRITE setAlwaysUpper)
    Q_PROPERTY (QString Field READ getField WRITE setField)
    Q_PROPERTY (bool PrimaryKey READ getPrimaryKey WRITE setPrimaryKey)
    Q_PROPERTY (bool EnableHiddenText READ getHiddenTextEnabled WRITE setHiddenTextEnabled)
    Q_PROPERTY (QString HiddenTextValidator READ getHiddenTextValidator WRITE setHiddenTextValidator)
public:
    EQLineEdit(QWidget *parent = 0);
    ~EQLineEdit();
    void setText(const QString &text);
    QString text() const;
    void setInt(int val);
    void setDouble(double val);
    void setBgColor(const QColor &color);
    void setMultipleOfDouble(const QString &val1, const QString &val2, int prec = 0);
    void setMultipleOfDouble(double val1, double val2, int prec = 0);
    void setDevideOfDouble(double val1, double val2, int prec = 0);
    void addDouble(double value);
    void setSumOfDouble(double val1, double val2, int prec = 0);
    bool getShowButton();
    void setShowButtonOnFocus(bool value);
    QString getField();
    void setField(const QString &name);
    bool getPrimaryKey();
    void setPrimaryKey(bool value);
    bool getHiddenTextEnabled();
    void setHiddenTextEnabled(bool v);
    bool getAlwaysUpper();
    void setAlwaysUpper(bool v);
    QString getHiddenTextValidator();
    void setHiddenTextValidator(const QString &v);
    int asInt();
    double asDouble();
    QString fHiddenText;
    QString fShowText;
    bool fClickBool;
    int fRow;
    int fColumn;
    double fMax;
    int fTag;
    QTableWidget *fTable;
    void setSelector(DWSelector *selector);
    inline int len() {return text().length();}
    inline bool isEmpty() {return text().isEmpty();}
    inline bool notEmpty() {return !text().isEmpty();}
signals:
    void customButtonClicked(bool value);
    void focusIn();
    void focusOut();
protected:
    virtual void resizeEvent(QResizeEvent *event);
    virtual void focusInEvent(QFocusEvent *event) override;
    virtual void focusOutEvent(QFocusEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void leaveEvent(QEvent *event);
    virtual bool eventFilter(QObject *watched, QEvent *event);

private:
    QToolButton *fButton;
    bool fShowButtonOnFocus;
    bool fAlwaysUpper;
    QString fField;
    bool fPrimaryKey;
    bool fEnableHiddenText;
    QString fHiddenTextValidator;
    DWSelector *fSelector;
private slots:
    void buttonClicked(bool value);
    void correctButtonPosition();
    void hiddenTextChanged(const QString &text);
};

#endif // EQLINEEDIT_H
