#include "DbDoctor.h"
#include "Database/Database.h"

Doctor DbDoctor::getDoctor()
{
    Db db;

    db.newStatement(
        "SELECT rzi, lpk, fname, lname, egn, specialty, phone FROM doctor"
    );

    while (db.hasRows())
    {
        return Doctor{
            .RZI = db.asString(0),
            .LPK = db.asString(1),
            .fname = db.asString(2),
            .lname = db.asString(3),
            .egn = db.asString(4),
            .specialty = db.asInt(5),
            .phone = db.asString(6)
        };
    }

    return {};
}


bool DbDoctor::hasDoctor()
{
    Db db;

    db.newStatement(
        "SELECT COUNT(*) FROM doctor"
    );

    while (db.hasRows())
    {
        return db.asBool(0);
    }

    return false;
}

void DbDoctor::update(const Doctor& d)
{
    Db db;

    db.newStatement(
        "UPDATE doctor SET "
        "lpk = ?,"
        "rzi = ?,"
        "fname = ?,"
        "lname = ?,"
        "phone = ?,"
        "egn = ?,"
        "specialty = ?"

    );

    db.bind(1, d.LPK);
    db.bind(2, d.RZI);
    db.bind(3, d.fname);
    db.bind(4, d.lname);
    db.bind(5, d.phone);
    db.bind(6, d.egn);
    db.bind(7, d.specialty.getIdx());

    db.execute();
}

void DbDoctor::insertDoctor(const Doctor& d)
{
    Db db;

    db.newStatement(
        "INSERT INTO doctor ("
        "lpk,rzi,fname,lname,phone,egn,specialty) "
        "VALUES (?,?,?,?,?,?,?)"

    );

    db.bind(1, d.LPK);
    db.bind(2, d.RZI);
    db.bind(3, d.fname);
    db.bind(4, d.lname);
    db.bind(5, d.phone);
    db.bind(6, d.egn);
    db.bind(7, d.specialty.getIdx());

    db.execute();
}


