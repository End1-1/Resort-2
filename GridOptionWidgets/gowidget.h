#ifndef GOWIDGET_H
#define GOWIDGET_H

#include <QWidget>
#include <QSettings>

class GOWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GOWidget(const QString &name, QWidget *parent = nullptr);
    ~GOWidget();
    virtual void save() = 0;
    QVariant value(const QString &name);
    static QVariant value(const QString &name, const QString &path);
protected:
    QString fName;
    QSettings *fSettings;
signals:

public slots:
};

#endif // GOWIDGET_H
