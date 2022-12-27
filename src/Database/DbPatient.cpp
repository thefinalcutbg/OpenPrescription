#include "DbPatient.h"
#include "Database.h"
#include "qdebug.h"

long long DbPatient::insert(const Patient& patient)
{
    Db db(
        "INSERT INTO patient "
        "(type, id, birth, sex, fname, mname, lname, "
        "ekatte, phone) "
        "VALUES (?,?,?,?,?,?,?,?,?)"
    );

    db.bind(1, patient.type);
    db.bind(2, patient.id);
    db.bind(3, patient.birth.to8601());
    db.bind(4, patient.sex);
    db.bind(5, patient.FirstName);
    db.bind(6, patient.MiddleName);
    db.bind(7, patient.LastName);
    db.bind(8, patient.city.getIdxAsInt());
    db.bind(9, patient.phone);


    if (db.execute()) return db.lastInsertedRowID();

    return 0;

}

bool DbPatient::update(const Patient& patient)
{
    Db db(

        "UPDATE patient SET "
        "type=?,"
        "birth=?,"
        "sex=?,"
        "fname=?,"
        "mname=?,"
        "lname=?,"
        "ekatte=?,"
        "phone=? "
        "WHERE rowid=?"
    );

    db.bind(1, patient.type);
    db.bind(2, patient.birth.to8601());
    db.bind(3, patient.sex);
    db.bind(4, patient.FirstName);
    db.bind(5, patient.MiddleName);
    db.bind(6, patient.LastName);
    db.bind(7, patient.city.getIdxAsInt());
    db.bind(8, patient.phone);
    db.bind(9, patient.rowid);

    return db.execute();

}

Patient DbPatient::get(std::string patientID, int type)
{
    std::string query = "SELECT rowid, type, id, birth, sex, fname, mname, lname, ekatte, phone "
        "FROM patient WHERE id = '" + patientID + "' "
        "AND type = " + std::to_string(type);

    Patient patient;

    for (Db db(query); db.hasRows();)
    {
        patient.rowid = db.asRowId(0),
        patient.type = db.asInt(1);
        patient.id = db.asString(2);
        patient.birth = Date(db.asString(3));
        patient.sex = db.asInt(4);
        patient.FirstName = db.asString(5);
        patient.MiddleName = db.asString(6);
        patient.LastName = db.asString(7);
        patient.city = Ekatte(db.asInt(8));
        patient.phone = db.asString(9);
    }

    return patient;
}

Patient DbPatient::get(long long rowid)
{
    std::string query = "SELECT rowid, type, id, birth, sex, fname, mname, lname, ekatte, phone "
        "FROM patient WHERE rowid = " + std::to_string(rowid);

    Patient patient;

    for (Db db(query); db.hasRows();)
    {
        patient.rowid = db.asRowId(0),
        patient.type = db.asInt(1);
        patient.id = db.asString(2);
        patient.birth = Date(db.asString(3));
        patient.sex = db.asInt(4);
        patient.FirstName = db.asString(5);
        patient.MiddleName = db.asString(6);
        patient.LastName = db.asString(7);
        patient.city = db.asInt(8);
        patient.phone = db.asString(9);

    }

    return patient;
}
 