#ifndef BASE_H
#define BASE_H

#include "defines.h"
#include "preferences.h"
#include "utils.h"
#include "database.h"
#include "databaseresult.h"
#include "eqlineedit.h"
#include "eqdateedit.h"
#include "baseuid.h"
#include "doubledatabase.h"
#include <QDate>
#include <QDebug>
#include <QDateTime>

class Base
{
public:
    Base();
    ~Base();
    static QString fDbName;
    Database fDb;
    Database fAirDb;
    static QString fAirHost;
    static QString fAirDbName;
    static QString fAirUser;
    static QString fAirPass;
    static Preferences fPreferences;
    QList<QList<QVariant> > fDbRows;

    //T cache record , C - cache instance
    template<typename T, typename C>
    void dockResponse(EQLineEdit *code, EQLineEdit *name, T *t) {
        if (t) {
            if (code) {
                code->setText(t->fCode);
            }
            name->setText(t->fName);
        } else {
            if (code) {
                name->clear();
                if (code->text().isEmpty()) {
                    code->clear();
                } else {
                    t = C::instance()->get(code->text());
                    if (t) {
                        code->setText(t->fCode);
                        name->setText(t->fName);
                    } else {
                        code->clear();
                        name->clear();
                    }
                }
            } else {
                if (code) {
                    t = C::instance()->get(code->text());
                    if (t) {
                        if (code) {
                            code->setText(t->fCode);
                        name->setText(t->fName);
                        }
                    } else {
                        if (code) {
                            code->clear();
                        }
                        name->clear();
                    }
                }
            }
        }
    }

    template<typename T, typename C>
    void dockResponse(EQLineEdit *l, T *t) {
        if (l) {
            if (t) {
                l->setText(t->fName);
                l->fHiddenText = t->fCode;
            } else {
                if (!l->isEmpty()) {
                    C *c = C::instance();
                    t = c->get(l->fHiddenText);
                    if (t) {
                        l->setText(t->fName);
                        l->fHiddenText = t->fCode;
                    }
                }
            }
        }
    }
    QMap<QString, QVariant> fDbBind;
    virtual void selector(int selectorNumber, const QVariant &value);
protected:

};

#endif // BASE_H
