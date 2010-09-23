#ifndef _LIBENGINE_CORE_HH_INCLUDED
#define _LIBENGINE_CORE_HH_INCLUDED
#ifndef _MSC_VER
//posix
#include <dlfcn.h>
#include <sys/time.h>
#else
//win32
#include "../posix_to_win32_port.h"
#include <sys/timeb.h>
#include <time.h>
#endif


#include <map>
#include <iostream>
#include <string>
#include <cstdio>
#include <sstream>
#include <cstdlib>
#include <fstream>
#include "gamelet.h"
#include "threading/Thread.h"
#include "networking/libnetwork_module.h"
#include <cassert>
#include <queue>
#include "../engine_core/data/mySqlClass.h"
using namespace std;

namespace Alexsuh
{

    namespace GarbageCollection
    {
        class IDisposble
        {
        };
        class IObjectManager
        {
            public :
            virtual void RegisterObject(IDisposble *)=0;
            virtual void DisposeObject(IDisposble *)=0;

        };
        class Disposble : public IDisposble
        {
            protected:
                IObjectManager *objManager;
            public:
                virtual void Dispose()
                {
                    assert(objManager!=NULL);
                    if (objManager!=NULL)
                        objManager->DisposeObject(this);
                }
                virtual void RegisterSelf(IObjectManager *_objManager)
                {
                    objManager=_objManager;
                    objManager->RegisterObject(this);
                }
        };
        class ObjectsManager: public Thread , public IObjectManager
        {
            private :
                ObjectsManager();
                map <IDisposble *,bool> objects;
                static pthread_mutex_t objects_lock;
                static pthread_mutex_t objectsManagerInstanceLock;
                static int init_param;
                static int init_class();
                static ObjectsManager* currentObjectManagerInstance;
            public:
                void RegisterObject(IDisposble *);
                void DisposeObject(IDisposble *);
                static IObjectManager* GetInstance();
                ~ObjectsManager();

            //THREAD members START

            protected:
                void Execute(void*);


            //THREAD members END

        };

    }

}

typedef gamelet* (*Function_Ptr)(void );
/**
*	@file
*	@author Alex Suchman
*	@version 1.0
*
*	@section LICENSE
*
*	This program is free software; you can redistribute it and/or
* 	modify it under the terms of the GNU General Public License as
* 	published by the Free Software Foundation; either version 2 of
*	the License, or (at your option) any later version.
*
* 	This program is distributed in the hope that it will be useful, but
* 	WITHOUT ANY WARRANTY; without even the implied warranty of
* 	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* 	General Public License for more details at
* 	http://www.gnu.org/copyleft/gpl.html
*
*	@section DESCRIPTION
*
*	this file contain the player class , the game container class and the world manager class
*
*	player class
		represent a player in the game
	game container
		represents a holder of all game objects
	world manager
		handles the interaction between varius game objects ' handles api calls from gamelets , and
		overall manages the game world
	game_server
		network connection server
	requestHandler
		request handler for request recieved from server
*
*/




typedef std::map<string,string> dataNode;
/**
    Class to load and storesimple and complex configuration
*/
class ConfigContainer
{
    public:
        /** Default constructor */
        ConfigContainer();
        /** Default destructor */
        virtual ~ConfigContainer();


        bool LoadConfig(const std::string &path);
        bool LoadNestedConfig(const std::string &path);
        std::string* getKey(const std::string &key);
        void insertKey(const std::string& key,const std::string& val);
        std::map<std::string,dataNode*> nestedConfig;
        std::map<std::string,std::string> config; //!< Member variable "config"
    protected:
    private:

};
/**
classes implementing this interface are expected to
support serialization and descialization from and to
string and dataNode
*/
class ISeriazible
{
    public :
    virtual void serialize(string& ser_out)=0;
    virtual void serialize(dataNode &ser_out)=0;
    virtual void deSerialize(const string& ser_in)=0;
    virtual void deSerialize(const dataNode& ser_in)=0;
};
/**
	gamelet_session

	this class repressent a a gamelet running engine
	each session is wrapper a round a single gamelet object
	a control interface is exposed
*/
class gamelet_session : public Thread , ISeriazible
{
private:
    /*gamelet object*/
    gamelet *session_gamelet;

    /*stop flag*/
    char is_stop;
    //holds the next free gamelet session id number
    //for newly registewred gamelets
    static unsigned long gamelet_session_running_id;
    pthread_mutex_t inner_gamelet_mutex;
    //used to count time it took to precess main loop, to ease extrapolation in client side

    map <string,string> SysMessagesToClient;
    map <string,map<string,string> > SysMessagesToClientByPlayerId;
    pthread_mutex_t SysMessagesToClient_lock;
    pthread_mutex_t SysMessagesToClientByPlayerId_lock;
public:
    double exec_time_main_loop;

    string session_id;
    gamelet_session(gamelet *g,Iworld_manager *_world_manager);
    ~gamelet_session();
    /*init and run gamelet*/
    void init();
    /*gamelet run function*/
    void run();
    /*stop gamelet run*/
    void stop();
    /*return session gamelet*/
    gamelet *get_gamelet();
    /*returnsd next free availble session id*/
    static void get_next_gamelet_session_id(string&);
    /*point back to manager*/
    Iworld_manager *_world_manager;

    //Iseriazble start
    void serialize(string& ser_out);
    void serialize(dataNode &ser_out);
    void deSerialize(const string& ser_in);
    void deSerialize(const dataNode& ser_in);
    //Iseriazble end
    void AddMessageToClient(string & key, string &val);
    void AddMessageToClientByPlayerId(const string &player_id,const string & key,const string &val);
    void GetMessagesForClientByPlayerId(string &player_id,string &msgout);
    /*thread specific methods*/
protected:
    ///init before thread run
    void Setup();
    ///thread run
    void Execute(void*);


};

/**
represents a connected or registed user
*/
class player
{
private:
    /**
    	holds a running number of next free player id
    */
    static unsigned long running_player_id;
public:
    /**
    	return next free player id for new player creation
    */
    static void generate_next_id(string& newplayerid);
    /**
    player PKID
    */
    string player_id;
    ///a gamelet session ehere this player is currently is
    string gamelet_session_id;
    /// player name
    std::string name;
//public:
    /**
    player constructor
    @param _player_id id of this player
    */
    player(string &_player_id);
    ~player();

};

///contains instances of all running
///gamelets and logged in players
class game_container
{
private:
    /**
    //unsync map of player id's and players logged in
    //will not remove players but only mark them as not logged in
    //will periodicly remove those ghosts from map
    //possible solution for performace loss may be using a shadow copy of this list which be
    //be kept clean of ghost  nodes and periodicly swapped with working map (one poiner change)
    */
    map<std::string,player*> players;

    pthread_mutex_t players_mutex;
    /**
    //will hold map of sessions id's and gamelet object running those sessions
    //not synced
    //will mark dead gamelet sessions as dead (not moved)
    //will periodicly lock and remove unused gamelet sessions from map
    //performance - same as in 'palyers' list
    */
    map<std::string,gamelet_session*> gamelet_sessions;

    pthread_mutex_t gamelet_sessions_mutex;
    /**
    //an unsynced list of factory function of loaded gamelets
    //only insert is allowed as removal is not expected unsless application ends
    //in which case memory will be freed
    */
    map<std::string,Function_Ptr> gamelet_classes;

    pthread_mutex_t gamelet_classes_mutex;
public:
    /**
    	default consturct , initializer the containers
    */
    game_container();
    /**
    	destroys this game container
    */
    ~game_container();
    /**
    	insert a new player into the players container

    	@param player_id identification number of this player , assumed thats it unique.can be
    					 generated(for new players) by player statuc utility function

    	@param new_player a player object representing the player to store
    */
    void insert_player(std::string & player_id,player* new_player);
    /**
    	remove player from the container

    	@player_id id of player who needs to be removed

    */
    void remove_player(std::string & player_id);
    /**
    	returns a player by player id

    	returns a player by player id , based on underlying implementation
    	should take anything between @f$ O{log{n}} @$f and constant time

    	@param player_id id of player to get

    	@return A player obejct with corresponding player id , or NULL if non found in
    			repository

    */
    player *get_player(std::string & player_id);
    /**
    	insert a gamlet session

    	@param session_id id of session that is being inserted

    	@param the session object being inserted
    */
    void insert_session(string& session_id,gamelet_session *new_gamelet_session);
    /**
    		remove gamelet session

    		@param session_id id of session to remove
    */
    void remove_session(string& session_id);
    /**
    	retuns factory mwthod of a gmelet deriving class with given name

    	retuns factory mwthod of a gmelet deriving class with given name.
    	method hould not be case sensitive

    	@param gamelet_class_name name of gamelet deriving class whus factory we need

    	@return	a factory method or null if name des not match
    */
    Function_Ptr get_factory_for_class(const std::string & gamelet_class_name);

    /**
    	get gamelet session with specified id

    	@param gamelet_session_id id of gamelet to return

    	@return a gamelet session with matching session id or NULL if none exists
    */
    gamelet_session *get_gamelet_session(const std::string & gamelet_session_id);
    /**
    	addes a new gamelet class factory to repository

    	@param gamelet_class_name class name of new gaelet deriving class

    	@param gamelet_class_factory_func a factory function that produces object of said
    			gamelet deriving class.
    */
    void add_gamelet_class(std::string &gamelet_class_name,Function_Ptr gamelet_class_factory_func);
    gamelet_session* game_container::get_last_gamelet_session();
    void game_container::get_All_gamelet_sessions(map<string,gamelet_session*> &all_gamelets);
};

//end of game container class


///////////////////////////////
// world manager
//////////////////////////////
class world_manager : Iworld_manager
{
private:
    game_container the_world_container;
    std::string database_connection;
    ConfigContainer config;
public:
    Alexsuh::Data::dataManager *DataManager;
    world_manager();
    virtual ~world_manager();
    /**
    	load gamelet class from definition (config)
    	file given as parameter path
    */
    void init(const char *path);
    /**
    	check user name and password for matching and existance in
    	playewr database and and register new player if exsists
    	@return reference to newly created and registered player
    */
    player& login(std::string user_name,std::string password);
    /**
    	unregister player with specified id from list of ative players
    */
    void logout(std::string & player_id);
    /**
    	starts a gamelet with non registered gamelet id
    */
    gamelet_session* start_gamelet(const std::string& gamelet_class);
    gamelet_session* start_gamelet(const std::string& gamelet_class,const std::string& gamelet_session_id);

    /**
    	stop gamelet with specified ID
    */
    void stop_gamelet(unsigned long gamelet_session_id);
    /**
    	move player with specified id into another gamelet
    */
    void move_player(std::string & player_id,gamelet_session *_gamelet_session);
    /**
    	return game_container
    */
    game_container & get_game_container();

    ///IWorld Manager
    /** @brief moves player to gameelt session
      *
      * moves spesified player by playerid to specified gamelet session
      * by gamelet session id
      */
    void world_manager::move_player(std::string & player_id,std::string & gamelet_session_id);

    ///IWorld Manager
};


/////////////////////////////////////
//game server
////////////////////////////////////
/**
	tcp server for game
	will spawn handler threads for each incoming connection
*/
class game_server :  public network
{
private:
    world_manager _world_manager;

public:
    ///pornumber and reefrence to world manager object constructor
    game_server(int _portno,const world_manager & _world_manager);

    ///	handle recieved connection
    void handleReq(int sockedfd);

    ~game_server();
};

//////////////////////////////////////
//remote request from client handler
/////////////////////////////////////
/**
	reqest handler thread
*/
class ReqHandler : public Thread ,public Alexsuh::GarbageCollection::Disposble
{
private:
    ///consts
    ///header parameters
    map<std::string,std::string> header_params;
    ///data parameters
    map<std::string,std::string> data_params;

    ///socket descriptor
    int socketfd;
    /// reference to world manager
    world_manager *_world_manager;
    ///handles request
    void handleRequest();
    ///requst handler garbage collector
    //static void removeThread(ReqHandler*);
    /**
    	parses a message recieved
    */
    void parse(std::string const &msg_in,std::string &msg_out);
    void do_login(std::string &msgout);
    void do_update(std::string &msgout);
    void do_logout(std::string &msgout);
    void do_switch_gamelet(std::string &msgout);


    //typedef void (*ReqHandler::handle_Ptr)(string&);

    ///maps request header to handler fucntion
    //static map<string,handle_Ptr> reqHandlers;
    ///initalizes handler fucntion map
    //static map<string,handle_Ptr> init_handler_list(map<string,handle_Ptr>);
public:
    ///IDisposble START
    static Alexsuh::GarbageCollection::IObjectManager *objManager;
    void Dispose();
    void RegisterSelf(Alexsuh::GarbageCollection::IObjectManager *_objManager);
    ///IDisposble END

    ///consts
    //header types
    //request type
    static const int req_type_keepAlive=0;
    static const int req_type_none=1;
    static const int req_type_header_end=3;
    static const int req_type_login=2;
    static const int req_type_logout=4;
    static const int req_type_switch_gamelet=5;

    //request type
    //field name
    static std::string field_name_messageType;
    static std::string field_name_playerId;
    static std::string field_name_header_end;
    static std::string field_name_username;
    static std::string field_name_password;
    static std::string field_name_gamelet_type_id;
    static std::string field_name_gamelet_session_id;
    //field name
    //header types
    //return messages
    static std::string  NO_PLAYER_ID;
    static std::string  NO_SUCH_PLAYER;
    static std::string  PLAYER_FOUND;
    static std::string  MSG_PARSE_MALFORMED_INPUT;
    //return messages


    /**
    	request handler construcotr
    	@param socketfd socket descriptor to which IO accures
    	@param _world_manager worldmanager on which contex this request operates
    */
    ReqHandler(int socketfd,world_manager & _world_manager);
protected:
    /// Thread setup routine
    void Setup();
    /// Thread runtime routine
    void Execute(void*);
};


//utils
namespace Utils
{
int UUID_INITIATED=0;
void generate_uuid(string& newuuid);
int msleep(unsigned long milisec);
}
#endif

