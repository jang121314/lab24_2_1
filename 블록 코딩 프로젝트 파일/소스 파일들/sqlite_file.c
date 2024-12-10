
#include "head.h"
#include "sqlite3.h"

extern Container mainContainer;

const char* DB_path = "_data.db";
const char* create_table = "CREATE TABLE IF NOT EXISTS Containers ( id INTEGER PRIMARY KEY AUTOINCREMENT, data BLOB );";

const char* clear_table = "DELETE FROM Containers;";
const char* table_input = "INSERT INTO Containers (data) VALUES (?);";
const char* table_output = "SELECT data FROM Containers WHERE id = ?;";
const char* get_count = "SELECT COUNT(*) FROM Containers;";
const char* count_query = "SELECT COUNT(*) FROM Containers;";
sqlite3* db;

void Container_input(Container* data);
void Container_output(Container* data, int index);
void _SQL_save(Container* data);

void error_data_code(char* str, int line)
{
	sqlite3_close(db);
	__error(str, line);
}
#undef _error
#define _error(TEXT) error_data_code(TEXT, __LINE__ )

bool db_check()
{
	char* errmsg = NULL;
	if (sqlite3_exec(db, create_table, 0, 0, &errmsg) != SQLITE_OK) _error(errmsg);

	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(db, count_query, -1, &stmt, 0);
	sqlite3_step(stmt);

	int count = sqlite3_column_int(stmt, 0);
	sqlite3_finalize(stmt);

	if (count == 0) return 0;
	else return 1;
}

size_t SQL_get_count()
{
	size_t count = 0;
	sqlite3_stmt* stmt = NULL;
	if (sqlite3_prepare_v2(db, get_count, -1, &stmt, 0) == SQLITE_OK)
	{
		if (sqlite3_step(stmt) == SQLITE_ROW) 
		{
			count = sqlite3_column_int(stmt, 0);
		}
		sqlite3_finalize(stmt);
	}
	return count;
}

void SQL_load()
{
	if (sqlite3_open(DB_path, &db) != SQLITE_OK) _error(sqlite3_errmsg(db)); // _error("데타베이스가 우리를 반기질 않소");
	if (db_check() == 1)
	{
		size_t count = SQL_get_count();
		if (1 <= count) Container_output(&mainContainer, 1);
		Container* head = &mainContainer;
		if (2 <= count) for (int i = 2; i < count; i++)
		{
			Container* container = malloc(sizeof(Container));
			Container_output(container, i);
			if (head != NULL)
			{
				head->nextContainer = container;
				head = container;
			}
		}
	}
	sqlite3_close(db);
}

void SQL_save()
{
	char* errmsg = NULL;
	if (sqlite3_open(DB_path, &db) != SQLITE_OK) _error(sqlite3_errmsg(db));
	if (sqlite3_exec(db, clear_table, 0, 0, &errmsg) != SQLITE_OK) _error(errmsg);
	Container_input(&mainContainer);
	if (mainContainer.nextContainer == NULL); else _SQL_save(mainContainer.nextContainer);
	sqlite3_close(db);
}

void _SQL_save(Container* data)
{
	if (data == NULL) return; else
	{
		Container_input(data);
		_SQL_save(data->nextContainer);
		free(data);
	}
}

void Container_input(Container* data)
{
	sqlite3_stmt* stmt = NULL;
	if(sqlite3_prepare_v2(db, table_input, -1, &stmt, 0) != SQLITE_OK) _error("데타베이스 코드 컴파일 실패");
	sqlite3_bind_blob(stmt, 1, data, sizeof(Container), SQLITE_STATIC);
	if (sqlite3_step(stmt) != SQLITE_DONE) _error("데타베이스 코드 작동 실패");
	sqlite3_finalize(stmt);
}

void Container_output(Container* data, int index)
{
	sqlite3_stmt* stmt = NULL;
	if (sqlite3_prepare_v2(db, table_output, -1, &stmt, 0) != SQLITE_OK) _error("데타베이스 코드 컴파일 실패");
	sqlite3_bind_int(stmt, 1, index);
	int res = sqlite3_step(stmt);
	if (res == SQLITE_ROW)
	{
		const void* p = sqlite3_column_blob(stmt, 0);
		size_t size = sqlite3_column_bytes(stmt, 0);
		SDL_memcpy(data, p, size);
	}
	else if (res == SQLITE_DONE); else _error("데타베이스 코드 작동 실패");
	sqlite3_finalize(stmt);
}

