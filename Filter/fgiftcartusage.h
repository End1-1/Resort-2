#ifndef FGIFTCARTUSAGE_H
#define FGIFTCARTUSAGE_H

#include "wfilterbase.h"

namespace Ui {
class FGiftCartUsage;
}

class FGiftCartUsage : public WFilterBase
{
    Q_OBJECT

public:
    explicit FGiftCartUsage(QWidget *parent = nullptr);
    ~FGiftCartUsage() override;
    QString reportTitle() override;
    QWidget *firstElement() override;
    void apply(WReportGrid *rg) override;

    static void openReport(const QString &cardCode, const QString &cardLabel);

public slots:
    void onRowDoubleClick(const QList<QVariant> &row);

private:
    Ui::FGiftCartUsage *ui;
    QString fCardCode;
};

#endif // FGIFTCARTUSAGE_H
