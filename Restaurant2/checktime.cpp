#include "checktime.h"
#include "database2.h"
#include "message.h"

CheckTime::CheckTime(QObject *parent)
    : QObject{parent}
{}

bool CheckTime::check()
{
    Db b = Preferences().getDatabase(Base::fDbName);
    Database2 db2;
    if (!db2.open(b.dc_main_host, b.dc_main_path, b.dc_main_user, b.dc_main_pass)) {
        // Не забудь обработать ошибку подключения, если это не сделано раньше
        return false;
    }

    // Используем UTC на стороне сервера, чтобы не зависеть от его настроек timezone
    db2.exec("SELECT UTC_TIMESTAMP() AS f_datetime");

    if (db2.next()) {
        QDateTime dtServer = db2.dateTimeValue("f_datetime");

        // Принудительно устанавливаем спецификацию UTC для времени с сервера,
        // так как UTC_TIMESTAMP() возвращает "голые" данные без инфо о зоне
        dtServer.setTimeSpec(Qt::UTC);

        // Получаем текущее время системы сразу в UTC
        QDateTime dtLocal = QDateTime::currentDateTimeUtc();

        // Считаем разницу в секундах
        qint64 diff = qAbs(dtServer.secsTo(dtLocal));

        // 200 секунд — это около 3.5 минут.
        // Для фискальных операций (налоговой) разница обычно не должна превышать 5 минут (300 сек).
        if (diff > 200) {
            QString errorMsg = tr("Time on server and on machine is different.") + "<br>" + tr("Server (UTC): ")
                               + dtServer.toString("yyyy-MM-dd HH:mm:ss") + "<br>" + tr("Local (UTC): ")
                               + dtLocal.toString("yyyy-MM-dd HH:mm:ss") + "<br>" + tr("Difference: %1 sec").arg(diff);

            message_error(errorMsg);
            return false;
        }
    }
    return true;
}
