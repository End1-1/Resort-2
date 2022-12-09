#include "roweditordialog.h"
#include "eqcheckbox.h"
#include "ecolorlineedit.h"
#include "eqcombobox.h"
#include "eqtextedit.h"
#include "eqtimeedit.h"
#include "edateedit.h"
#include <QDateEdit>
#include <QDateTimeEdit>
#include <QTextEdit>

RowEditorDialog::RowEditorDialog(QList<QVariant> &values, int trackId, QWidget *parent) :
    BaseExtendedDialog(parent),
    fValues(values)
{
    isNew = true;
    fCacheId = 0;
    fTrackId = trackId;
    fTrackControl = new TrackControl(fTrackId);
}

RowEditorDialog::~RowEditorDialog()
{

}

void RowEditorDialog::setValues()
{
    clearWidgets();
    isNew = true;
    if (fValues.count() > 0) {
        isNew = false;
        valuesToWidgets();
    } else {
        fTrackControl->resetChanges();
    }
}

RowEditorDialog &RowEditorDialog::addWidget(QWidget *w, const QString &trackName)
{
    fWidgets << w;
    if (!trackName.isEmpty()) {
        fTrackControl->addWidget(w, trackName);
    }
    return *this;
}


void RowEditorDialog::valuesToWidgets()
{
    int j = 0;
    EQLineEdit *id = 0;
    for (int i = 0; i < fWidgets.count(); i++) {
        QWidget *w = fWidgets[i];
        if (isLineEdit(w)) {
            EQLineEdit *e = static_cast<EQLineEdit*>(w);
            e->setText(fValues.at(j).toString());
            if (e->getField() == "f_id") {
                id = e;
            }
        } else if(isColorLineEdit(w)) {
            static_cast<EColorLineEdit*>(w)->setText(fValues.at(j).toString());
        } else if (!strcmp(w->metaObject()->className(), "QDateEdit") ||
                   !strcmp(w->metaObject()->className(), "EQDateEdit")) {
            static_cast<QDateEdit*>(w)->setDate(fValues.at(j).toDate());
        } else if (!strcmp(w->metaObject()->className(), "QDateTimeEdit")) {
            static_cast<QDateTimeEdit*>(w)->setDateTime(fValues.at(j).toDateTime());
        } else if(isTimeEdit(w)) {
            static_cast<QTimeEdit*>(w)->setTime(QTime::fromString(fValues.at(j).toString(), "HH:mm:ss"));
        } else if (isComboBox(w)) {
            static_cast<EQComboBox*>(w)->setCurrentIndex(static_cast<EQComboBox*>(w)->findData(fValues.at(j)));
            j++;
        } else if (!strcmp(w->metaObject()->className(), "QTextEdit") ||
                   !strcmp(w->metaObject()->className(), "EQTextEdit")) {
            static_cast<QTextEdit*>(w)->setPlainText(fValues.at(j).toString());
        } else if (!strcmp(w->metaObject()->className(), "QCheckBox") ||
                   !strcmp(w->metaObject()->className(), "EQCheckBox")) {
            static_cast<QCheckBox*>(w)->setChecked(fValues.at(j).toInt());
        } else if (isEDateEdit(w)) {
            static_cast<EDateEdit*>(w)->setDate(fValues.at(j).toDate());
        } else {
            message_error("Application error. Please contact with developer and tell him next information:<br>"
                          "valuesToWidgets. Unknown classname: " + QString(w->metaObject()->className()));
        }
        j++;
    }
    if (id) {
        fTrackControl->resetChanges();
    }
}

void RowEditorDialog::widgetsToValues()
{
    fValues.clear();
    for (int i = 0; i < fWidgets.count(); i++) {
        QWidget *w = fWidgets[i];
        if (isLineEdit(w)) {
            fValues << static_cast<QLineEdit*>(w)->text();
        } else if (isEDateEdit(w)) {
            fValues << static_cast<EDateEdit*>(w)->text();
        } else if(isColorLineEdit(w)) {
            fValues << static_cast<EColorLineEdit*>(w)->text();
        } else if (!strcmp(w->metaObject()->className(), "QDateEdit") ||
                   !strcmp(w->metaObject()->className(), "EQDateEdit")) {
            fValues << static_cast<QDateEdit*>(w)->date().toString(def_date_format);
        } else if (!strcmp(w->metaObject()->className(), "QDateTimeEdit")) {
            fValues << static_cast<QDateTimeEdit*>(w)->dateTime().toString(def_date_time_format);
        } else if(isTimeEdit(w)) {
            fValues << static_cast<QTimeEdit*>(w)->time().toString(def_time_format);
        } else if (isComboBox(w)) {
            fValues << static_cast<QComboBox*>(w)->itemData(static_cast<QComboBox*>(w)->currentIndex());
            fValues << static_cast<QComboBox*>(w)->currentText();
        } else if (!strcmp(w->metaObject()->className(), "QTextEdit") ||
                   !strcmp(w->metaObject()->className(), "EQTextEdit")) {
            fValues << static_cast<QTextEdit*>(w)->toPlainText();
        } else if (!strcmp(w->metaObject()->className(), "QCheckBox") ||
                   !strcmp(w->metaObject()->className(), "EQCheckBox")) {
            fValues << (int) static_cast<QCheckBox*>(w)->isChecked();
        } else {
            message_error("Application error. Please contact with developer and tell him next information:<br>"
                          "widgetToValues. Unknown classname: " + QString(w->metaObject()->className()));
        }
    }
}

void RowEditorDialog::clearWidgets()
{
    for (QList<QWidget*>::const_iterator it = fWidgets.begin(); it != fWidgets.end(); it++) {
        QWidget *w = *it;
        if (!strcmp(w->metaObject()->className(), "EQLineEdit")) {
            EQLineEdit *e = static_cast<EQLineEdit*>(w);
            e->clear();
        } else if (!strcmp(w->metaObject()->className(), "QLineEdit")) {
            QLineEdit *e = static_cast<QLineEdit*>(w);
            e->clear();
        } else if (isColorLineEdit(w)) {
            //EColorLineEdit *c = static_cast<EColorLineEdit*>(w);
            //c->setText("-1");
        } else if (!strcmp(w->metaObject()->className(), "EQCheckBox") ||
                   !strcmp(w->metaObject()->className(), "QCheckBox")) {
            QCheckBox *c = static_cast<QCheckBox*>(w);
            c->setCheckState(Qt::Unchecked);
        } else if (!strcmp(w->metaObject()->className(), "QTextEdit") ||
                   !strcmp(w->metaObject()->className(), "EQTextEdit")) {
            QTextEdit *t = static_cast<QTextEdit*>(w);
            t->clear();
        } else if (isTimeEdit(w)) {
            static_cast<QTimeEdit*>(w)->setTime(QTime::fromString("00:00", def_time_format));
        } else if (isDateEdit(w)) {
            static_cast<QDateEdit*>(w)->setDate(WORKING_DATE);
        }
    }
}

bool RowEditorDialog::saveOnly()
{
    fDataErrors.clear();
    if (!isDataCorrect()) {
        QString error;
        for (QStringList::const_iterator it = fDataErrors.begin(); it != fDataErrors.end(); it++) {
            error += *it + "<br>";
        }
        message_error(error);
        return false;
    }
    EQLineEdit *id = 0;
    for (QList<QWidget*>::const_iterator it = fWidgets.begin(); it != fWidgets.end(); it++) {
        QWidget *w = *it;
        if (isLineEdit(w)) {
            EQLineEdit *l = static_cast<EQLineEdit*>(w);
            if (l->getPrimaryKey()) {
                id = l;
            } else {
                if (!l->getField().isEmpty()) {
                    fDbBind[":" + l->getField()] = l->text();
                }
            }
        } else if (isColorLineEdit(w)) {
            EColorLineEdit *l = static_cast<EColorLineEdit*>(w);
            if (!l->getField().isEmpty()) {
                fDbBind[":" + l->getField()] = l->text();
            }
        } else if (isComboBox(w)) {
            EQComboBox *c = static_cast<EQComboBox*>(w);
            if (!c->getField().isEmpty()) {
                fDbBind[":" + c->getField()] = c->asInt();
            }
        } else if (isDateEdit(w)) {
            EQDateEdit *e = static_cast<EQDateEdit*>(w);
            if (!e->getField().isEmpty()) {
                fDbBind[":" + e->getField()] = e->date();
            }
        } else if (isTimeEdit(w)) {
            EQTimeEdit *t = static_cast<EQTimeEdit*>(w);
            if (!t->getField().isEmpty()) {
                fDbBind[":" + t->getField()] = t->time();
            }
        } else if (isCheckBox(w)) {
            EQCheckBox *c = static_cast<EQCheckBox*>(w);
            if (!c->getField().isEmpty()) {
                fDbBind[":" + c->getField()] = (int) c->isChecked();
            }
        } else if (isTextEdit(w)) {
            EQTextEdit *t = static_cast<EQTextEdit*>(w);
            if (!t->getField().isEmpty()) {
                fDbBind[":" + t->getField()] = t->toPlainText();
            }
        } else if (isEDateEdit(w)) {
            EDateEdit *e = static_cast<EDateEdit*>(w);
            if (!e->getField().isEmpty()) {
                fDbBind[":" + e->getField()] = e->date();
            }
        }
    }
    if (!id) {
        QString error = QString("Please, contact to application developer. Error message: \
        RowEditor. id==0. Classname: %1").arg(metaObject()->className());
        message_error(error);
        return false;
    }
    if (id->asInt() == 0) {
        id->setInt(fDb.insert(fTable, fDbBind));
    } else {
        fDb.update(fTable, fDbBind, where_id(id->text()));
    }
    widgetsToValues();
    if (fTrackControl->hasChanges()) {
        fTrackControl->fRecord = id->text();
        fTrackControl->saveChanges();
    }

    return true;
}

void RowEditorDialog::save()
{
    if (saveOnly()) {
        accept();
    }
}

bool RowEditorDialog::isDataCorrect()
{
    return true;
}

void RowEditorDialog::returnPressed()
{
    save();
}

void RowEditorDialog::escapePressed()
{
    reject();
}
