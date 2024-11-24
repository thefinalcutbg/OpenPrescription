#include "DbUpdater.h"
#include "Database/Database.h"
#include "Resources.h"

#include <unordered_map>
#include <vector>
#include <JsonCpp/json.h>

namespace DbMigrations {	

	void migrateTo1() {

		if (Db::version() != 0) return;

		Db db;

		db.execute("ALTER TABLE medication ADD from_date TEXT");
		db.execute("ALTER TABLE medication ADD to_date TEXT");
		db.execute("PRAGMA user_version = 1");
	}

	void migrateTo2() {

		if (Db::version() != 1) return;

		Db db;

		db.execute("PRAGMA foreign_keys=OFF");
		db.execute("ALTER TABLE doctor DROP COLUMN egn");
		db.execute("PRAGMA user_version = 2");
	}

	void migrateTo3() {

		if (Db::version() != 2) return;

		Db db;

		db.execute("ALTER TABLE prescription ADD COLUMN is_pregnant INTEGER");
		db.execute("ALTER TABLE prescription ADD COLUMN is_breastfeeding INTEGER");
		db.execute("PRAGMA user_version = 3");
	}

	void migrateTo4()
	{
		if (Db::version() != 3) return;

		Json::Value ekatteJson;

		Json::Reader().parse(Resources::citiesJson(), ekatteJson);

		std::unordered_map<int, int> ekatte_map;

		for (auto& val : ekatteJson)
		{
			if (!val.isMember("wrong_ekatte")) continue;

			ekatte_map[val["wrong_ekatte"].asInt()] = val["ekatte"].asInt();
		}

		std::vector<std::pair<long long, int>> rowid_ekatte;

		Db db("SELECT rowid, ekatte FROM patient");

		while (db.hasRows()) {

			long long rowid = db.asRowId(0);
			int ekatte = db.asInt(1);

			if (!ekatte_map.count(ekatte)) continue;

			rowid_ekatte.push_back(std::make_pair(rowid, ekatte_map[ekatte]));
		}

		for (auto& pair : rowid_ekatte)
		{
			db.newStatement("UPDATE patient SET ekatte=? WHERE rowid=?");
			db.bind(1, pair.second);
			db.bind(2, pair.first);
			db.execute();
		}

		db.execute("PRAGMA user_version = 4");
	}

	void migrateTo5()
	{
		if (Db::version() != 4) return;

		Db db;

		db.execute("CREATE TABLE medication_template (rowid INTEGER PRIMARY KEY NOT NULL, numMed_rowid INTEGER NOT NULL, is_form BOOLEAN NOT NULL, quantity INT NOT NULL, priority INT, substitution BOOLEAN, notes VARCHAR (2000), dosage TEXT);");

		db.execute("PRAGMA user_version = 5");
	}
};

void DbUpdater::updateDb()
{
	DbMigrations::migrateTo1();
	DbMigrations::migrateTo2();
	DbMigrations::migrateTo3();
	DbMigrations::migrateTo4();
	DbMigrations::migrateTo5();
}
