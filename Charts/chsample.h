#ifndef CHSAMPLE_H
#define CHSAMPLE_H

#include "basewidget.h"

namespace Ui {
class ChSample;
}

class ChSample : public BaseWidget
{
    Q_OBJECT

public:
    explicit ChSample(QWidget *parent = 0);
    ~ChSample();

private:
    Ui::ChSample *ui;
};

#endif // CHSAMPLE_H
