#include <Windows.h>
#include "Database.h"
#include "Enums.h"

#define IDC_MAINEDIT  101
#define USERNAME "p0_10076749"
#define PASSWORD "b311m4ns1"
#define DATABASE "p0_10076749_Banzai"
#define SERVER "sql201.php0h.com"

#define USERNAME "root"
#define PASSWORD "test"
#define DATABASE "banzai"
#define SERVER "localhost"

//! Constructor.
Database::Database() : mConnection(false)	// Important to pass false!!!!!
{
	mConnection.connect(DATABASE, SERVER, USERNAME, PASSWORD);

	// Check if connected TODO:
	if(mConnection.connected())
		int a = 1;
}

//! Destructor.
Database::~Database()
{

}

//! Checks if valid user.
bool Database::validUser(string name, string pass)
{
	string q = "SELECT * FROM Users WHERE Name='"+name+"' AND Password='"+pass+"'";
	//MessageBox(0, q.c_str(), 0, 0);
	mysqlpp::Query query = mConnection.query(q);
	mysqlpp::StoreQueryResult res = query.store();

	if(res.num_rows() != 0)	{
		return true;
	}
	else 
		return false;
}

//! Adds a win to a player.
void Database::addWin(string name)
{
	// Get current wins.
	int wins = getWins(name);
	char buffer[256];
	string q = "UPDATE Users SET Wins="+string(itoa(wins+1, buffer, 10))+" WHERE Name='"+name+"'";
	mysqlpp::Query query(&mConnection, true, q.c_str());
	query.exec();
}

//! Adds a loss to a player.
void Database::addLoss(string name)
{
	// Get current losses.
	int losses = getLosses(name);
	char buffer[256];
	string q = "UPDATE Users SET Losses="+string(itoa(losses+1, buffer, 10))+" WHERE Name='"+name+"'";
	mysqlpp::Query query(&mConnection, true, q.c_str());
	query.exec();
}

//! Returns number of wins for a player.
int Database::getWins(string name)
{
	string q = "SELECT * FROM Users WHERE Name='"+name+"'";
	mysqlpp::Query query = mConnection.query(q);
	mysqlpp::StoreQueryResult res = query.store();

	if(res.num_rows() != 0)
		return res[0]["Wins"];
	else
		return 0;
}

//! Returns number of losses for a player.
int Database::getLosses(string name)
{
	string q = "SELECT * FROM Users WHERE Name='"+name+"'";
	mysqlpp::Query query = mConnection.query(q);
	mysqlpp::StoreQueryResult res = query.store();

	if(res.num_rows() != 0)
		return res[0]["Losses"];
	else
		return 0;
}

//! Returns all online servers.
vector<Server> Database::getServers()
{	
	vector<Server> servers;
	string q = "SELECT * FROM Games";
	mysqlpp::Query query = mConnection.query(q);
	mysqlpp::StoreQueryResult res = query.store();
	
	for(int i = 0; i < res.size(); i++)
	{
		string host = res[i]["Host"];
		string ip = res[i]["Ip"];

		Server server;
		server.host = host;
		server.ip = ip;
		servers.push_back(server);
	}

	return servers;
}

//! Adds a server to the online server table.
void Database::addServer(string host, string ip)
{
	string q = "INSERT INTO Games VALUES ('"+host+"', '"+ip+"')";
	mysqlpp::Query query = mConnection.query(q);
	mysqlpp::StoreQueryResult res = query.store();
}

//! Removes a server from the online server table.
void Database::removeServer(string host)
{
	string q = "DELETE FROM Games WHERE Host='"+host+"'";
	mysqlpp::Query query = mConnection.query(q);
	mysqlpp::StoreQueryResult res = query.store();
}