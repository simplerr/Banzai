#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>
#include "mysql++.h"
using namespace std;

struct Server;

//! Contains the player and server tables.
class Database
{
public:
	Database();
	~Database();

	bool validUser(string name, string pass);
	void addWin(string name);
	void addLoss(string name);
	void addUser(string name, string pass, string mail);

	int getWins(string name);
	int getLosses(string name);
	vector<Server> getServers();
	void addServer(string host, string publicIp, string localIp);
	void removeServer(string host);
private:
	mysqlpp::Connection mConnection;
};

extern Database* gDatabase;

#endif