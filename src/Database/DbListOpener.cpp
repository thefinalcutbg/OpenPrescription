#include "DbListOpener.h"
#include "Model/User.h"
#include "Database.h"

std::vector<PatientRow> DbListOpener::getPatientRows()
{
    std::vector<PatientRow> rows;
    rows.reserve(50);

    std::string query =
        "SELECT rowid, id, fname, mname, lname , phone FROM patient ORDER BY id ASC";

    for (Db db(query);db.hasRows();)
    {
       rows.emplace_back(PatientRow{});
       
       rows.back().rowID = db.asRowId(0);
       rows.back().patientRowId = db.asRowId(0);

       rows.back().patientId = db.asString(1);

       rows.back().name = db.asString(2) + " " +
                          db.asString(3) + " " +
                          db.asString(4);

       rows.back().phone = db.asString(5);
    }

    return rows;
}

std::vector<PrescriptionRow> DbListOpener::getPrescriptionRows(const Date& from, const Date& to)
{
    std::vector<PrescriptionRow> rows;
    rows.reserve(50);

    std::string query =
        "SELECT " 
        "prescription.rowid, "
        "prescription.date, "
        "prescription.nrn, "
        "patient.rowid, "
        "patient.id, "
        "patient.fname, "
        "patient.mname, "
        "patient.lname, "
        "patient.phone "
        "FROM prescription INNER JOIN patient ON prescription.patient_rowid = patient.rowid "
        "WHERE "
        "prescription.date BETWEEN '" + from.to8601() + "' AND '" + to.to8601() + "' "
        "AND prescription.lpk = '" + User::doctor().LPK + "' "
        "AND prescription.rzi = '" + User::doctor().RZI + "' "
        "ORDER BY prescription.date ASC ";

    Db db(query);

    while (db.hasRows())
    {
        rows.emplace_back();

        auto& p = rows.back();

       p.rowID = db.asRowId(0);
       p.date = db.asString(1);
       p.nrn = db.asString(2);
       p.patientRowId = db.asRowId(3);
       p.patientId = db.asString(4);
       p.patientName = db.asString(5) + " " + db.asString(6) + " " + db.asString(7);
       p.patientPhone = db.asString(8);
    }

    return rows;
}


void DbListOpener::deleteRecord(TabType type, long long rowid)
{
    static constexpr const char* tableNames[2]{ "patient","prescription" };

    std::string tableName{ tableNames[static_cast<int>(type)] };

    Db::crudQuery("DELETE FROM " + tableName + " WHERE rowid = " + std::to_string(rowid));
}
