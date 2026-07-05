#include "session.h"
#include "base.h"
#include "database2.h"
#include "defines.h"
#include "preferences.h"
#include <QDateTime>

int Session::fCurrentId = 0;

static bool openDb(Database2 &db2)
{
    Db b = Preferences().getDatabase(Base::fDbName);
    return db2.open(b.dc_main_host, b.dc_main_path, b.dc_main_user, b.dc_main_pass);
}

int Session::findOpen(int branch)
{
    Database2 db2;

    if(!openDb(db2)) {
        return 0;
    }

    db2[":f_branch"] = branch;
    db2[":f_status"] = STATUS_OPEN;
    db2.exec("select f_id from sessions where f_branch=:f_branch and f_status=:f_status limit 1");

    if(db2.next()) {
        return db2.integer("f_id");
    }

    return 0;
}

int Session::open(int branch)
{
    Database2 db2;

    if(!openDb(db2)) {
        return 0;
    }

    db2[":f_status"] = STATUS_OPEN;
    db2[":f_branch"] = branch;
    db2[":f_start"] = QDateTime::currentDateTime();
    int id = 0;

    if(!db2.insert("sessions", id)) {
        return 0;
    }

    return id;
}

bool Session::close(int sessionId)
{
    if(sessionId <= 0) {
        return false;
    }

    Database2 db2;

    if(!openDb(db2)) {
        return false;
    }

    db2[":f_id"] = sessionId;
    db2[":f_status"] = STATUS_CLOSED;
    db2[":f_end"] = QDateTime::currentDateTime();
    return db2.exec("update sessions set f_status=:f_status, f_end=:f_end where f_id=:f_id");
}

int Session::openTableCount(int branch)
{
    Database2 db2;

    if(!openDb(db2)) {
        return -1;
    }

    db2[":f_branch"] = branch;
    db2[":f_state"] = ORDER_STATE_OPENED;
    db2.exec("select count(*) as cnt "
             "from r_table t "
             "inner join r_hall h on h.f_id=t.f_hall "
             "inner join o_header o on o.f_id=t.f_order "
             "where h.f_branch=:f_branch and o.f_state=:f_state");

    if(db2.next()) {
        return db2.integer("cnt");
    }

    return 0;
}

int Session::currentId()
{
    return fCurrentId;
}

void Session::setCurrentId(int id)
{
    fCurrentId = id;
}
