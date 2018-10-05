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

    QRegExp rw("[\\s(,][a-zA-Z0-9]*\\.");
    int pos = 0;
    while ((pos = rw.indexIn(where, pos)) != -1) {
        QString table = where.mid(pos, rw.matchedLength());
        table.remove(table.length() - 1, 1).remove(0, 1);
        checkTableName(table, from, rg);
        pos += rw.matchedLength();
    }
    bool firstSelect = true;
    rg->fModel->clearColumns();
    for (int i = 0, count = rg->fFields.count(); i < count; i++) {
        if (!rg->fIncludes[rg->fFields.at(i)]) {
            continue;
        }
        rg->fModel->setColumn(rg->fFieldsWidths.at(i),
                              rg->fFields.at(i),
                              rg->fFieldTitles.at(i));
        if (firstSelect) {
            firstSelect = false;
        } else {
            select += ",";
        }
        if (rg->fFields.at(i).contains("concat")
                || rg->fFields.at(i).contains("count(")
                || rg->fFields.at(i).contains("sum(")) {
            QRegExp re("([(,].+\\b)");
            re.setMinimal(true);
            QStringList tableNames = extractTableName(re, rg->fFields.at(i));
            for (QStringList::iterator it = tableNames.begin(); it != tableNames.end(); it++) {
                checkTableName((*it).remove(0, 1), from, rg);
            }
        } else if (rg->fFields.at(i).contains("coalesce")) {
            QRegExp re("[(\\+\\-][a-z]+\\b");
            re.setMinimal(true);
            QStringList tableNames = extractTableName(re, rg->fFields.at(i));
            for (QStringList::iterator it = tableNames.begin(); it != tableNames.end(); it++) {
                checkTableName((*it).remove(0, 1), from, rg);
            }
        } else {
            QRegExp re(".+\\b");
            re.setMinimal(true);
            QStringList tableNames = extractTableName(re, rg->fFields.at(i));
            for (QStringList::const_iterator it = tableNames.begin(); it != tableNames.end(); it++) {
                checkTableName(*it, from, rg);
            }
        }
        select += rg->fFields.at(i);
    }
    rg->fModel->setSqlQuery(select + " " + from + " " + where);
    rg->fModel->apply(rg);
}

QStringList WFilterBase::extractTableName(const QRegExp &re, const QString &str)
{
    re.indexIn(str);
    QStringList l = re.capturedTexts();
    return l;
}

void WFilterBase::checkTableName(const QString &alias, QString &from, WReportGrid *rg)
{
    for (int i = 0; i < rg->fTables.count(); i++) {
        QRegExp rt("\\s[a-zA-Z]+$");
        rt.indexIn(rg->fTables.at(i));
        QStringList lt = rt.capturedTexts();
        if (lt.count() > 0) {
            QString compareTableName(lt.at(0));
            compareTableName.remove(0, 1);
            if (alias == compareTableName) {
                if (!from.contains(rg->fTables.at(i))) {
                    if (i == 0) {
                        from = from.insert(5, rg->fTables.at(i)) + " ";
                    } else {
                        QString joinCond = rg->fJoinConds.at(i);
                        QRegExp rjt("=[a-z]+\\b");
                        QStringList joinsTables = extractTableName(rjt, joinCond);
                        foreach (QString s, joinsTables) {
                            checkTableName(s.remove(0,1), from, rg);
                        }
                        from += rg->fJoins.at(i) + QString(" join ") +
                                rg->fTables.at(i) + QString(" on ") + joinCond + " ";
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
    QStringList groupFields = check->getField().split(";", QString::SkipEmptyParts);
    foreach (QString s, groupFields) {
        if (check->getRequireLang()) {
            s += def_lang;
        }
        fReportGrid->fIncludes[s] = value;
    }
}
