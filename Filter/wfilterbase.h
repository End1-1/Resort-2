#ifndef WFILTERBASE_H
#define WFILTERBASE_H

#include "basewidget.h"

class WReportGrid;
class GOWidget;

class WFilterBase : public BaseWidget
{
    Q_OBJECT
public:
    explicit WFilterBase(QWidget *parent = 0);
    ~WFilterBase();
    virtual void apply(WReportGrid *rg) = 0;
    virtual QWidget *firstElement() = 0;
    virtual QString reportTitle();
    virtual bool officialTitle();
    virtual bool handlePrint();

    template<typename T, typename R>
    static T *openFilterReport() {
        R *rg = addTab<R>();
        T *v = new T(rg);
        rg->addFilterWidget(v);
        v->apply(rg);
        return v;
    }
    virtual void finalPrint(PPrintScene *ps, int top);
    virtual QWidget *gridOptionWidget();
protected:
    WReportGrid *fReportGrid;
    void buildQuery(WReportGrid *rg, const QString &where);
    QStringList extractTableName(const QRegExp &re, const QString &str);
    void checkTableName(const QString &alias, QString &from, WReportGrid *rg);

    //T cache users , C - cache instance
    template<typename T, typename C>
    void dockResponse(EQLineEdit *l, T *t) {
        if (t) {
            l->fHiddenText = t->fCode;
            l->setText(t->fName);
            l->fShowText = t->fName;
        } else {
            if (l->fHiddenText.isEmpty()) {
                l->clear();
                l->fShowText.clear();
            } else {
                C *ct = C::instance();
                QStringList list = l->fHiddenText.split(",", QString::SkipEmptyParts);
                QString text;
                bool first = true;
                for (QStringList::const_iterator it = list.begin(); it != list.end(); it++) {
                    t = ct->get(*it);
                    if (t) {
                        if (first) {
                            first = false;
                        } else {
                            text += ",";
                        }
                        text += t->fName;
                    }
                }
                l->fShowText = text;
                l->setText(text);
            }
        }
    }

    template<class T, class C>
    void dock(EQLineEdit *code, EQLineEdit *name, T *t) {
        if (!t) {
            if (code->text().length() > 0) {
                t = C::instance()->get(code->text());
            }
            if (!t) {
                code->clear();
                name->clear();
                return;
            }
        }
        code->setText(t->fCode);
        name->setText(t->fName);
    }
protected slots:
    void groupCheckClicked(bool value);
};

#endif // WFILTERBASE_H
