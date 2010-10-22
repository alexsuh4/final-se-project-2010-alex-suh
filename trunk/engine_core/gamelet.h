#ifndef _GAMELET_HH
#define _GAMELET_HH
/**
gamelet api file .
version 0.0.0.1
*/
/*
//not really needed here as acts as only interface class
#ifdef _MSC_VER
//for pthread functionality under win32
#include "posix_for_windows.hpp"
#endif
*/
#include <string>
#include <map>
/**
provide an interface for world manager functionality
*/
class Iworld_manager
{
    public:
    //virtual ~Iworld_manager()=0;
    virtual void move_player(std::string & player_id,std::string & gamelet_session_id)=0;
	virtual void SaveToDB(std::string &gameletSessionID)=0;
	virtual void SaveToDB(std::string &gameletSessionID,std::map<std::string,std::string> & params)=0;
	virtual void LoadFromDB(std::string &gameletSessionID,std::map<std::string,std::string> & params)=0;

};

//holds information about current gamelet

typedef std::map<std::string,std::map<std::string,std::string> > api_request;
typedef struct _gamelet_desc {
	std::string gamelet_session_id;
	std::string gamelet_type_id;
    //api_request request;
    Iworld_manager *_world_manager;
}gamelet_desc;

typedef struct _handle_client_request_Args {
    std::string playerid;
    std::map<std::string,std::string> aadditional_Args;
    std::string messageQuery;
}handle_client_request_Args;


class gamelet{
protected:
		//a gamelet descriptor
		gamelet_desc *my_gamelet_desc;
public:
	char gamelet_name[100];
    gamelet() {};
	virtual ~gamelet() {};
	//dummy function
	virtual void dummy_func() =0;
	//add object in auto location
	virtual void add_object_auto(const std::string &game_session_id) =0;
	//add object in smae location as specified player
	//if such player does not exist in this session invoke auto placement
	virtual void add_object_auto_by_player(std::string &game_session_id,std::string & player_id) =0;
	//add a player with specfied id to this session
	virtual void add_player(std::string &player_id) =0;
	//remove player with specfied id to this session
	virtual void remove_player(std::string &player_id) =0;
	//initalized gamelet
	//will be called first at gamelet invocation
	//will initiate
	//engine will give gamelet it's gamelet descriptor object
	virtual void init_gamelet(gamelet_desc *game_desc) =0;
	//the main servers side loop of this gamelet
	virtual void main_loop() =0;
	//will encode gamelet model into a std::string
	//it is essesnsialy a method to pass
	//free data between plugin client and server
	//expects a null terminated std::string
	//result is passed to the parameter supplied [ into buffer ]
	// max size will contain maximum size for the buffer
	//virtual void get_model_string(char *buffer,unsigned int max_size) =0;
	virtual void get_model_string(std::string & msgout) =0;
	///handle client specific requests
	///will accapt request in req std::string and replys in reply parameters (out param)
	///params are of type std::string
	virtual void handle_client_request(const handle_client_request_Args & req_Ags,std::string & reply) =0;
	virtual bool is_initalized()=0;
	virtual void get_name(char *name)=0;
	virtual void get_path(char *path)=0;
	virtual void DeSerializeFromParams(std::map<std::string,std::string> & addidionalValues)=0;
	virtual void SerializeToParams(std::map<std::string,std::string> & params)=0;
};

extern "C" gamelet* make();


#endif
