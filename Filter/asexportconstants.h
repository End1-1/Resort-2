#ifndef ASEXPORTCONSTANTS_H
#define ASEXPORTCONSTANTS_H

#include <QString>

struct AsExportConstants
{
    qint64 docNumberStart = 1;
    QString buyer;
    QString buyerAccount;
    QString prepayAccount;
    QString cashlessAccount;
    QString cashlessAmount = QStringLiteral("0");
    QString prepayUsage = QStringLiteral("*");
    QString comment;
    QString vatCalcMethod = QStringLiteral("2");
    QString vatAccount;
    QString issueMethod = QStringLiteral("*");
    QString docStatus = QStringLiteral("1");
    QString vatLine = QStringLiteral("1");
    QString transType = QStringLiteral("1");
    QString ecoTax = QStringLiteral("0");
    QString expenseAccount;
    QString revenueAccount;

    void load();
    void save() const;
    bool isValid(QString *errorMessage = nullptr) const;
};

#endif // ASEXPORTCONSTANTS_H
