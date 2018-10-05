#ifndef CACHEFORCOMBOBOX_H
#define CACHEFORCOMBOBOX_H

#include "eqcombobox.h"
#include <QMapIterator>

class CacheForComboBox
{
public:
    template<typename T, typename C>
    static void configure(EQComboBox *cb) {
        T *t = T::instance();
        QMapIterator<QString, C*>it(t->it());
        while (it.hasNext()) {
            it.next();
            cb->addItem(it.value()->fName, it.value()->fCode);
        }
    }
private:
    CacheForComboBox();
};

#endif // CACHEFORCOMBOBOX_H
