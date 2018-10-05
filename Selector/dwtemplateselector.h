#ifndef DWTEMPLATESELECTOR_H
#define DWTEMPLATESELECTOR_H

#include "dwselector.h"
#include "base.h"

template<typename CI, typename CA>
class DWTemplateSelector : public DWSelector
{
public:
    DWTemplateSelector(QWidget *parent) : DWSelector(CA::instance()->fCacheId, parent) {setRefreshButtonVisible(true);}
    virtual void configure() {simpleTable<CI, CA>();}
    void setDialog(Base *b, int selectorNumber) {fDialog = b; fSelectorNumber = selectorNumber;}
    virtual void select(const QVariant &value) {fDialog->selector(fSelectorNumber, value);}
    virtual void selectorFocusOut() {
        CI *c = 0;
        if (fLineSelector) {
            c = CA::instance()->get(fLineSelector->text());
        }
        fDialog->selector(fSelectorNumber, qVariantFromValue(c));
    }

protected:
    virtual void refresh() {
        CA::instance()->clear();
        CA::instance()->load();
        configure();
        applyTextFilter();
    }
private:
    //static DWTemplateSelector *fInstance;
    Base *fDialog;
    int fSelectorNumber;
};



#endif // DWTEMPLATESELECTOR_H
