#ifdef _MSC_VER
#include "gamelet.hpp"
#else
#include "../engine_core/gamelet.hh"
#include "../engine_core/game_engine_api.hh"
#endif
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <cmath>
#include <ctime>
#include <queue>
#include <list>
//utils

namespace Sample_Gamelet_Utils
{
    /**
    @summary replaces all characters in a string
    @param context string to perform replacement on
    @param from old string
    @param new string
    */
    std::string& replaceAll(std::string& context, const std::string& from, const std::string& to)
    {
        size_t lookHere = 0;
        size_t foundHere;
        while((foundHere = context.find(from, lookHere)) != std::string::npos)
        {
              context.replace(foundHere, from.size(), to);
              lookHere = foundHere + to.size();
        }
        return context;
    }

}
//utils
#define PI          3.14159265
#define PI_DIV_2    1.57079633
#define PI_MUL_2    6.28318531
typedef double point[2];
using namespace std;

enum game_object_state_enum
{
    active=0,dead=1
} game_object_state;

/**
	private gamelet game object
*/


class gamelet_object
{
private:
    void init(double _ang,double _vel,double _cords[2]);
	bool _isActivated;
public:
    game_object_state_enum current_state;
    double ang;
    double vel;
    double cords[2];
    string entity_id;

    map <string,string> Dynamic_properies;
    gamelet_object();
    gamelet_object(double ang,double vel);
    gamelet_object(double ang,double vel,double *_cords);
	void activateObject()
	{
		_isActivated=true;
	}
	bool isActivated()
	{
		return _isActivated;
	}
	~gamelet_object();
    void move();
    void serialize_me(ostringstream &msgout);
};

/**
@summary return serialzed information of this object
*/
void gamelet_object::serialize_me(ostringstream &_temp_strs)
{

    _temp_strs<<"{";
    //serialze static properties
    _temp_strs<<"\"current_state\":";
    _temp_strs <<"\""<<current_state<<"\"";
    _temp_strs<<",";
    _temp_strs<<"\"player_id\":";
    _temp_strs <<"\""<<entity_id<<"\"";
    _temp_strs<<",";
    _temp_strs<<"\"x\":";
    _temp_strs <<"\""<<cords[0]<<"\"";
    _temp_strs<<",";
    _temp_strs<<"\"y\":";
    _temp_strs <<"\""<< cords[1]<<"\"";
    _temp_strs<<",";
    _temp_strs <<"\"vel\":"<<"\""<<vel<<"\""<<",\"ang\":"<<"\""<<ang<<"\"";
    //serliaze dynamic properities
    for (
         map<string,string>::iterator itr=Dynamic_properies.begin();
         itr!=Dynamic_properies.end();
         itr++)
         {
             _temp_strs<<",\""<<itr->first<<"\":\""<<itr->second<<"\"";
         }
    _temp_strs<<"}";
}
/**
@summary initazlies this gamelet object
*/
void gamelet_object::init(double _ang,double _vel,double _cords[2])
{
    ang=_ang;
    vel=_vel;
    cords[0]=_cords[0];
    cords[1]=_cords[1];
    this->current_state=active;
    cout<<"initializing sample gamelet object\n";
	_isActivated=false;
}
gamelet_object::gamelet_object()
{
    double _cords[2]= {0.0,0.0};
    init(0,0,_cords);
}
gamelet_object::gamelet_object(double _ang,double _vel)
{
    double _cords[2]= {0.0,0.0};
    init(_ang,_vel,_cords);
}
gamelet_object::gamelet_object(double _ang,double _vel,double *_cords)
{
    init(_ang,_vel,_cords);
}
gamelet_object::~gamelet_object()
{
    cout <<"destroying sample gamelet inner object ";
}

void gamelet_object::move()
{
    //object move start
    cords[0]+=(cos(ang)*vel);
    cords[1]+=(sin(ang)*vel);
    //object move end
    //bound checking start
    if (cords[0]>=240 || cords[0] <=0 || cords[1]>=240 || cords[1] <=0)
    {
        ang+=PI;
        if (ang>PI_MUL_2)
            ang-=PI_MUL_2;
        if (ang<0)
            ang+=PI_MUL_2;
    }
    if (cords[0]<=0)
        cords[0]++;
    if (cords[1]<=0)
        cords[1]++;
    if (cords[0]>=240)
        cords[0]--;
    if (cords[1]>=240)
        cords[1]--;
    //bounds checking end

    //cout <<"game object has moved to "<<cords[0]<<" , "<<cords[1]<<"\n";

}



class sample_gamelet : public gamelet
{
private:
    //game specific data

    gamelet_object entry_point;
    list <gamelet_object*> my_objects;
    map <string,gamelet_object*> players;
    map <string ,gamelet_object*> portals;
    point bounds[2];
    gamelet_object * add_object(point &cords,const string &obj_Type,const string &entity_id);
    pthread_mutex_t innerObjectLock;
	bool isInialized;
public:
    sample_gamelet();
    ~sample_gamelet();
    void dummy_func();
    void add_object_auto(const std::string & game_session_id);
    void add_object_auto_by_player(std::string & game_session_id,std::string & player_id);
    void add_player(std::string & player_id);
    void remove_player(std::string & player_id);
    void init_gamelet(gamelet_desc *my_gamelet_desc);
    void main_loop();
    //void get_model_string(char *buffer,unsigned int max_size);
    void get_model_string(std::string &msgout);
    void handle_client_request(const handle_client_request_Args & req_Ags,std::string & reply);
	bool is_initalized()
	{
		return isInialized;
	}
};

sample_gamelet::sample_gamelet()
{
	isInialized=false;
    pthread_mutex_init(&innerObjectLock,NULL);
    cout<<"SAMPLE GAMELET CTOR LOCK TEST\n";
    cout<<"SAMPLE GAMELET CTOR innerObjectLock LOCKING \n";
    pthread_mutex_lock(&innerObjectLock);
    pthread_mutex_unlock(&innerObjectLock);
    cout<<"SAMPLE GAMELET CTOR innerObjectLock UNLOCKED \n";
    cout<<"constructor sample gamelet called!\n";
}

sample_gamelet::~sample_gamelet()
{
    cout<<"destroing gamelet\n";
    /*for (int i=0; i<num_of_objects; i++)
        delete my_objects[i];
	*/
	cout<<"SAMPLEGAMELET :: dtor LOCKING innerObjectLock\n";
	pthread_mutex_lock(&innerObjectLock);
	for (
			list<gamelet_object*>::iterator  my_objects_itr=my_objects.begin();
			my_objects_itr!=my_objects.end();
			my_objects_itr++)
			{
				delete *my_objects_itr;
			}
	my_objects.clear();
	pthread_mutex_unlock(&innerObjectLock);
	cout<<"SAMPLEGAMELET :: dtor UNLOCKING innerObjectLock\n";
	pthread_mutex_destroy(&innerObjectLock);
}
void sample_gamelet::dummy_func()
{
    cout<<"dummy function , this will not be printed"<<endl;
}
/** @brief insers an object with description
  *
  * insers an object with description
  * LOCKES SHARED RESOUCES MY_OBJECTS
  */
gamelet_object* sample_gamelet::add_object(point &cords,const string &obj_Type,const string &entity_id)
{
    //cout<<"adding auto loaction portal to game_id "<<entity_id<<endl;
    cout<<"new object id is  "<<entity_id<<endl;
    cout<<"new object is "<<obj_Type<<endl;
    cout<<"setting object params\n";
    //create new gamelet object
    gamelet_object *newObject=new gamelet_object();
	newObject->activateObject();
    //put him on the entry point
    newObject->cords[0]=cords[0];
    newObject->cords[1]=cords[1];
    newObject->vel=0;
    //set player id
    newObject->entity_id=entity_id;
    // register player with gamelet object

    newObject->Dynamic_properies["objectType"]=obj_Type;

    //insert new object into active gamelet list
    cout<<"SAMPLE GAMELET locking innerObjectLock for "<<entity_id<<"\n";

	pthread_mutex_lock(&innerObjectLock);
	my_objects.push_back(newObject);
	pthread_mutex_unlock(&innerObjectLock);

    cout<<"SAMPLE GAMELET UNlocking innerObjectLock for "<<entity_id<<"\n";
    cout<<"testing add..."<<endl;
    ostringstream msg;
    //my_objects[num_of_objects-1]->serialize_me(msg);
    //cout<<"last players serialization is "<<msg.str()<<endl;
    cout<<"adding player with id "<<entity_id<<" to gamelet"<<endl;
    return newObject;
}

///add object in auto location
void sample_gamelet::add_object_auto(const std::string & game_session_id)
{
    point pt;
    pt[0]=rand()%(int)bounds[1][0]+bounds[0][0];
    pt[1]=rand()%(int)bounds[1][1]+bounds[0][1];

    gamelet_object* newObject= add_object(pt,"portal",game_session_id);

    //TODO add another lock for auxilary conatier for better granulaty
    cout<<"SAMPLE GAMELET :: add_object_auto LOCKING innerObjectLock\n";
    pthread_mutex_lock(&innerObjectLock);

    portals[game_session_id]=newObject;

    pthread_mutex_unlock(&innerObjectLock);

    cout<<"SAMPLE GAMELET :: add_object_auto UNLOCKING innerObjectLock\n";

}
///add object in smae location as specified player
///if such player does not exist in this session invoke auto placement
void sample_gamelet::add_object_auto_by_player(std::string & game_session_id,std::string & player_id)
{
    gamelet_object *requested_player=NULL;

    cout<<"SAMPLEGAMELET :: add_object_auto_by_player LOCKING innerObjectLock\n";

    pthread_mutex_lock(&innerObjectLock);
    //critical section start
    	map <string,gamelet_object*>::iterator itr=  players.find(player_id);
    if (itr!=players.end())
    {
        requested_player=itr->second;
    }
    if (requested_player!=NULL)
    {
        gamelet_object* newObject=add_object(requested_player->cords,"portal",game_session_id);
        portals[game_session_id]=newObject;
    }
    else
    {
        add_object_auto(game_session_id);
    }
    //critical section end
    pthread_mutex_unlock(&innerObjectLock);

    cout<<"SAMPLEGAMELET :: add_object_auto_by_player UNLOCKED innerObjectLock\n";
}
///add a player with specfied id to this session
void sample_gamelet::add_player(std::string & player_id)
{

    //critical section start
        gamelet_object* newObject=add_object(entry_point.cords,(rand()%10>4?"boy":"girl"),player_id);
    cout<<"SAMPLEGAMELET :: add_object_player LOCKING innerObjectLock\n";
    pthread_mutex_lock(&innerObjectLock);
        players[player_id]=newObject;
    //critical section end
    pthread_mutex_unlock(&innerObjectLock);
    cout<<"SAMPLEGAMELET :: add_object_player UNLOCKING innerObjectLock\n";
}
///remove player with specfied id to this session
void sample_gamelet::remove_player(std::string & player_id)
{
    cout<<"SAMPLEGAMELET :: remove_player LOCKING innerObjectLock\n";

	pthread_mutex_lock(&innerObjectLock);

    //critical section start
	cout<<"removing player with id "<<player_id <<" from gamelet"<<endl;
	cout<<"BEFORE remove : players: "<<players.size()<<" my_objects: "<<my_objects.size()<<"\n";
		gamelet_object *obj_to_remove=players[player_id];
		players.erase(player_id);
		my_objects.remove(obj_to_remove);
	cout<<"AFTER remove : players: "<<players.size()<<" my_objects: "<<my_objects.size()<<"\n";
	//critical section end
    pthread_mutex_unlock(&innerObjectLock);

    cout<<"SAMPLEGAMELET :: remove_player UNLOCKING innerObjectLock\n";
}
///initalized gamelet
///will be called first at gamelet invocation
///will initiate
void sample_gamelet::init_gamelet(gamelet_desc *my_gamelet_desc)
{
	strcpy(this->gamelet_name,"sample_gamelet_ver_1_0_0");
    cout<<"init bounds\n";
    bounds[0][0]=0;//x1
    bounds[0][1]=0;//y1
    bounds[1][0]=200;//x2
    bounds[1][1]=200;//y2

    this->my_gamelet_desc=my_gamelet_desc;
    cout<<"initalizing gamelet with session id "<<this->my_gamelet_desc->gamelet_session_id<<" and gamlete class id "<<this->my_gamelet_desc->gamelet_type_id<<endl;

    srand(time(NULL));
    //double cords[2],vel,ang;
  //  gamelet_object * new_object=NULL;
  //  for (int i=0; i<num_of_objects; i++)
  //  {
  //      cords[0]=rand()%(int)bounds[1][0]+bounds[0][0];
  //      cords[1]=rand()%(int)bounds[1][1]+bounds[0][1];
  //      vel=1.0/(rand()%3);
  //      ang=1.0/(rand()%30);

  //      new_object=new gamelet_object(ang,vel,cords);
		//new_object->activateObject();
  //      //my_objects[i]=new_object;
		//my_objects.push_back(new_object);
		////lst_my_objects.push_back(new_object);
  //  }


	isInialized=true;
}

struct keyValuePair{
    string key;
    string val;
};
/**
@summary the main gamelelt loop code (to be excecuted repeatedly be the engine)
*/
void sample_gamelet::main_loop()
{


    //cout<<"changing game model\n";
    //for (int i=0; i<num_of_objects; i++)
    //    my_objects[i]->move();




   // cout<<"resolving portal collision locking resources\n";
    //cout<<"SAMPLEGAMELET :: mainloop LOCKING innerObjectLock\n";
    pthread_mutex_lock(&innerObjectLock);
    //critical section start

    //move objects
    for (
		list<gamelet_object*>::iterator my_objects_itr = my_objects.begin();
		my_objects_itr!=my_objects.end();
		my_objects_itr++)
		{
			(*my_objects_itr)->move();
		}

    //resolve portal collision start
    	queue<keyValuePair> playersToMoveOutOfThisGamelet;
    	keyValuePair kvp;
    	for (
         map<string,gamelet_object*>::iterator itr_players=players.begin();
         itr_players!=players.end();
         itr_players++)
         {
            for (
                map<string,gamelet_object*>::iterator itr_portals=portals.begin();
                itr_portals!=portals.end();
                itr_portals++)
            {
                if ( abs(itr_players->second->cords[0]-itr_portals->second->cords[0]) <=30 &&
                       abs(itr_players->second->cords[1]-itr_portals->second->cords[1])<=30)
                    {
                        string playerIdToMove=itr_players->second->entity_id;
                        string targetGameletSessionId=itr_portals->second->entity_id;
                        //cout<<"collision detected , resolving collision of "<<playerIdToMove<<" with "<<targetGameletSessionId<<"\n";

                        //will deadlock if transfered to self so prevent that
                        cout<<"current session id is "<<this->my_gamelet_desc->gamelet_session_id<<"\n";
                        cout<<"need move to "<<targetGameletSessionId<<"\n";

                        if (targetGameletSessionId!= this->my_gamelet_desc->gamelet_session_id)
                        {
                            //playersToMoveOutOfThisGamelet[playerIdToMove]=targetGameletSessionId;
                            kvp.key=playerIdToMove;
                            kvp.val=targetGameletSessionId;
                            playersToMoveOutOfThisGamelet.push(kvp);

                        }
                            //_world_manager->move_player(playerIdToMove,targetGameletSessionId);
                        else
                        {
                            cout<<"self teleporting is not supported!\n";
                            exit(1);
                        }

                        //this->my_gamelet_desc->request["movePlayer"]["playerid"]=itr_players->second->entity_id;
                        //itr_players->second->Dynamic_properies["objectType"]="portal";

                    }
            }
         }
    //critical section end
    pthread_mutex_unlock(&innerObjectLock);
    //cout<<"SAMPLEGAMELET :: mainloop UNLOCKING innerObjectLock\n";

    //lock is released , so now can resolve object to be removed
    Iworld_manager *_world_manager= this->my_gamelet_desc->_world_manager;
    while(!playersToMoveOutOfThisGamelet.empty())
    {
        kvp=playersToMoveOutOfThisGamelet.front();
        cout<<"need to move player "<<kvp.key<<" to "<<kvp.val<<"\n";
        _world_manager->move_player(kvp.key,kvp.val);
        playersToMoveOutOfThisGamelet.pop();
    }
    //cout<<"portal collision resolved , resources unlocked\n";

    //resolve portal collision end


 }

///JSON object serialization
void sample_gamelet::get_model_string(std::string &msgout)
{
    cout<<"generating gamelt encoding\n";
    //collect data
    ostringstream _temp_strs;
    _temp_strs<<"{ ";

    _temp_strs<<"\"objects\":[ ";
    //for (int i=0; i<num_of_objects; i++)
    //{
    //    my_objects[i]->serialize_me(_temp_strs);
    //    if (i<(num_of_objects-1))
    //        _temp_strs<<",";// comma for all but last
    //}
	unsigned int i=0;
	for (
		list<gamelet_object*>::iterator my_objects_itr=my_objects.begin();
		my_objects_itr!=my_objects.end();
		my_objects_itr++)
		{
			(*my_objects_itr)->serialize_me(_temp_strs);
			if (i<(my_objects.size()-1))
				_temp_strs<<",";// comma for all but last
			i++;
		}
    _temp_strs<<" ]";//array of of objects
    _temp_strs<<" }";// response object
    msgout=_temp_strs.str();

}


/**

@deprecated
@summary will check for request and parameters,
	excetute request with given params
	and update in reply
	will expect the request to be of space seperated strings
	in key value pairs
*/
///plugin server side level protocol goes here

void sample_gamelet::handle_client_request(const handle_client_request_Args & req_Ags,std::string & reply)
{
    string req_const=req_Ags.messageQuery;
    string playerid=req_Ags.playerid;

    cout<<"handling client request\n";
    map<string,string> data_params;
    string val="";
    string key="";

    //unescape by replacing '>', ';' with ' ' (whitespace)
    string req=req_const;
    Sample_Gamelet_Utils::replaceAll(req,">"," ");
    Sample_Gamelet_Utils::replaceAll(req,";"," ");
	//unescape by replacing '>', ';' with ' ' (whitespace)
    //read values
    stringstream mystream;

    mystream<<req;
    while( !mystream.eof() )
    {
        mystream>>key>>val;
        data_params[key]=val;
    }
    map<string,string>::const_iterator iter;
    iter=data_params.find("userKey");
    if (iter!=data_params.end())
    {
        char key=iter->second[0];
        //handle key
        //hande key end
        cout<<"TDODO add handle for char \n"<<key<<" for player with playerid="<<playerid<<endl;
        //get player
        map<string,gamelet_object*>::iterator player_itr= players.find(playerid);
        //player obj
        if (player_itr==players.end())
        {
            cout<<"no player with this id in this gamelet !";
            return;
        }
        gamelet_object* player_Obj=player_itr->second;

        //gamelet_object*
        switch (key)
        {
            case 'a':
                player_Obj->ang=PI;
                break;
            case 'd':
                player_Obj->ang=0;
                break;
            case 's':
                player_Obj->ang=-PI_DIV_2;
                break;
            case 'w':
                player_Obj->ang=PI_DIV_2;
                break;
            case ',':
                player_Obj->vel=2;
                break;
            case '.':
                player_Obj->vel=0;
                break;
            default:
                cout<<"unrecognized key : "<<key<<endl;
                break;
        }
    }
    /*
    cout<<"sample gamelet recieved \n";
    for( iter=data_params.begin() ; iter != data_params.end() ; ++iter )
    {
        cout << iter->first << " = " << iter->second << endl;
    }
    */
    reply="OK";

}


extern "C" gamelet* make()
{
    cout<<"running factory function\n";
    return new sample_gamelet();
}


