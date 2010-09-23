/**
this file is depreceted
*/
/**
	will launch a new minigame with type app_id and id of gameid, . If game id
	is negative then will launch ageneric game app_id.
	App_id - type of application
	Game_id - an id of application instance
*/
void goto_world_by_session_id(unsigned long player_id,unsigned long gamelet_session_id ,unsigned int gamelet_id);
/**
      returns a connection to a sql database in order to store and retrieve data
      for persistent minigame.
      game_id - the game for which connection to be retrieved.
      If we decide to change password or user name of the database it can be done
      by only changing this function.
*/
void * get_sql_connection(unsigned long gamelet_session_id);
