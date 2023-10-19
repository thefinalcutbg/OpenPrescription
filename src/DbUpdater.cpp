#include "DbUpdater.h"
#include "Database/Database.h"

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
};

void DbUpdater::updateDb()
{
	DbMigrations::migrateTo1();
	DbMigrations::migrateTo2();
	DbMigrations::migrateTo3();
}
