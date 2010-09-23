
#ifndef _MYSQLCLASS
#define _MYSQLCLASS
#include <iostream>

#include "mysql_connection.h"
#include "mysql_driver.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cassert>
#include <string>
#include <map>
#include <list>
#include <vector>
#include <sstream>
namespace Alexsuh
{
	namespace Data
	{
		class DBObject
		{
			public:
				std::map<std::string,std::string> data;
				DBObject()
				{
				}
				void printData()
				{
					for (std::map<std::string,std::string>::iterator itr=data.begin();
						itr!=data.end();
						itr++)
					{
						std::cout<<	(itr->first)	<<	"="	<<	(itr->second)	<<std::endl;
					}
				}
				void addData(sql::ResultSet *rs,const std::string &_dataType,const int _columnIndex,std::string & columnName)
				{
					std::string dataType=_dataType;
					std::ostringstream oss;
					std::string result;//text
					if (
						dataType.find("CHAR")!=std::string::npos
						|| dataType.find("TEXT")!=std::string::npos
						|| dataType.find("STRING")!=std::string::npos)
					{

						oss<<rs->getString(_columnIndex);
					}
					else if (
						dataType.find("INT")//int
						)
					{
						oss<<(rs->getInt(_columnIndex));
					}
					else if(
						dataType.find("FLOAT")
						|| dataType.find("DOUBLE")
						)
					{
						oss<<(rs->getInt(_columnIndex));
					}
					else
					{
						//unsupported type, ignore
						return;
					}
					result=oss.str();
					data[columnName]=result;
				}

				void ReadFromResultSet(sql::ResultSet *rs)
				{
					sql::ResultSetMetaData *rsMetaData=rs->getMetaData();
					int colNum;
					int colCount=rsMetaData->getColumnCount();
					std::string colName;
					std::string colType;

					for (colNum=1;colNum<=colCount;colNum++)
					{
						colName=rsMetaData->getColumnName(colNum);
						colType=rsMetaData->getColumnTypeName(colNum);
						this->addData(rs,colType,colNum,colName);
					}

				}
		};
		class dataManager
		{
			private:
			static dataManager *currentInstance;
			dataManager()
			{
				dbHost="tcp://localhost:3306";
				user="root";
				password="root";
				mySqlDriver=sql::mysql::get_mysql_driver_instance();
			}
			public:
			static dataManager* GetInstance();

			std::string dbHost;
			std::string user;
			std::string password;
			sql::mysql::MySQL_Driver *mySqlDriver;

			sql::Connection* GetSQLConnection()
			{
				return mySqlDriver->connect(dbHost,user,password);
			}
			sql::ResultSet *ExecuteQuery(const std::string &sqlQry,sql::Connection *con)
			{
				sql::Statement *sttmnt= con->createStatement();
				sql::ResultSet *rs= sttmnt->executeQuery(sqlQry);
				return rs;
			}
			std::vector<Alexsuh::Data::DBObject*> *toDBObject(const std::string &sqlQry)
			{
				sql::Connection* con=this->GetSQLConnection();
				sql::Statement *sttmnt= con->createStatement();
				sql::ResultSet *rs= sttmnt->executeQuery(sqlQry);

				Alexsuh::Data::DBObject *dObj;

				std::vector<Alexsuh::Data::DBObject*> *result=new std::vector<Alexsuh::Data::DBObject*>();
				while(rs->next())
				{
					dObj=new Alexsuh::Data::DBObject();
					dObj->ReadFromResultSet(rs);
					result->push_back(dObj);
				}
				//cleanup
				delete rs;
				delete sttmnt;
				delete con;

				return result;
			}
		};

	}

}
#endif
