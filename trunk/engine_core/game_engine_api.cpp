#include <iostream>
#include "game_engine_api.hh"
using namespace std;
/**
	will launch a new minigame with type app_id and id of gameid, . If game id
	is negative then will launch ageneric game app_id.
	App_id - type of application
	Game_id - an id of application instance
*/
void goto_world_by_session_id(std::string &player_id,std::string & gamelet_session_id ,std::string & gamelet_id)
{
	cout<<"sending player with id "<< player_id <<"to gamelet session of id "<<gamelet_session_id<<" of type "<<gamelet_id<<endl;
}
/**
      returns a connection to a sql database in order to store and retrieve data
      for persistent minigame.
      game_id - the game for which connection to be retrieved.
      If we decide to change password or user name of the database it can be done
      by only changing this function.
*/
void * get_sql_connection(std::string & gamelet_session_id)
{
	cout<<"returning sql connection ..."<<endl;
	return 0;
}
