#include "wfilterbase.h"
#include "wreportgrid.h"
#include "eqcheckbox.h"

WFilterBase::WFilterBase(QWidget *parent) :
    BaseWidget(parent)
{
    fReportGrid = static_cast<WReportGrid*>(parent);
    fTrackControl = new TrackControl(TRACK_REPORT);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}

WFilterBase::~WFilterBase()
{
}

QString WFilterBase::reportTitle()
{
    return tr("Report title");
}

bool WFilterBase::officialTitle()
{
    return false;
}

bool WFilterBase::handlePrint()
{
    return false;
}

void WFilterBase::finalPrint(PPrintScene *ps, int top)
{
    Q_UNUSED(ps);
    Q_UNUSED(top);
}

void WFilterBase::buildQuery(WReportGrid *rg, const QString &where)
{
    rg->fModel->clearColumns();

    QString select = "select ";
    QString from = "from  ";

    QRegularExpression rw("[\\s(,][a-zA-Z0-9]*\\.");

    auto itMatch = rw.globalMatch(where);
    while (itMatch.hasNext()) {
        auto match = itMatch.next();
        QString table = match.captured(0);
        table.remove(table.length() - 1, 1).remove(0, 1);
        checkTableName(table, from, rg);
    }

    bool firstSelect = true;

    for (int i = 0, count = rg->fFields.count(); i < count; i++) {
        const QString &field = rg->fFields.at(i);

        if (!rg->fIncludes[field])
            continue;

        rg->fModel->setColumn(rg->fFieldsWidths[rg->fFieldTitles[field]],
                              field,
                              rg->fFieldTitles[field]);

        if (!firstSelect)
            select += ",";
        firstSelect = false;

        if (field.contains("concat") || field.contains("count(") || field.contains("sum(")) {
            QRegularExpression re("([(,].+\\b)");
            re.setPatternOptions(QRegularExpression::InvertedGreedinessOption);

            QStringList tableNames = extractTableName(re, field);
            for (QString &t : tableNames)
                checkTableName(t.remove(0, 1), from, rg);

        } else if (field.contains("coalesce")) {
            QRegularExpression re("[(\\+\\-][a-z]+\\b");
            re.setPatternOptions(QRegularExpression::InvertedGreedinessOption);

            QStringList tableNames = extractTableName(re, field);
            for (QString &t : tableNames)
                checkTableName(t.remove(0, 1), from, rg);

        } else {
            QRegularExpression re(".+\\b");
            re.setPatternOptions(QRegularExpression::InvertedGreedinessOption);

            QStringList tableNames = extractTableName(re, field);
            for (const QString &t : tableNames)
                checkTableName(t, from, rg);
        }

        select += field;
    }

    rg->fModel->setSqlQuery(select + " " + from + " " + where);
    rg->fModel->apply(rg);
}

QStringList WFilterBase::extractTableName(const QRegularExpression &re, const QString &str)
{
    auto match = re.match(str);
    return match.capturedTexts();
}

void WFilterBase::checkTableName(const QString &alias, QString &from, WReportGrid *rg)
{
    for (int i = 0; i < rg->fTables.count(); i++) {
        QRegularExpression rt("\\s[a-zA-Z]+$");
        auto match = rt.match(rg->fTables.at(i));

        if (match.hasMatch()) {
            QString compareTableName = match.captured(0);
            compareTableName.remove(0, 1);

            if (alias == compareTableName) {
                if (!from.contains(rg->fTables.at(i))) {
                    if (i == 0) {
                        from.insert(5, rg->fTables.at(i));
                        from += " ";
                    } else {
                        QString joinCond = rg->fJoinConds.at(i);

                        QRegularExpression rjt("=[a-z]+\\b");
                        QStringList joinsTables = extractTableName(rjt, joinCond);

                        for (QString s : joinsTables)
                            checkTableName(s.remove(0, 1), from, rg);

                        from += rg->fJoins.at(i) + " join " + rg->fTables.at(i) + " on " + joinCond
                                + " ";
                    }
                }
                return;
            }
        }
    }
}

QWidget *WFilterBase::gridOptionWidget()
{
    return 0;
}

void WFilterBase::groupCheckClicked(bool value)
{
    EQCheckBox *check = static_cast<EQCheckBox*>(sender());
    QStringList groupFields = check->getField().split(";", Qt::SkipEmptyParts);
    foreach (QString s, groupFields) {
        if (check->getRequireLang()) {
            s += def_lang;
        }
        fReportGrid->fIncludes[s] = value;
    }
}
