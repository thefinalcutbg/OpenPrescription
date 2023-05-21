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
};

void DbUpdater::updateDb()
{
	DbMigrations::migrateTo1();
}
