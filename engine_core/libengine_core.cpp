#include "libengine_core.h"

using namespace std;


/////////////////////////////////////////////////////////////////////////////////////
///	player	
/////////////////////////////////////////////////////////////////////////////////////
///initialize running player id's to 0
unsigned long player::running_player_id=0;
player::player(string& _player_id)
{
	pthread_mutex_init(&additionalData_lock,NULL);
	pthread_mutex_init(&messages_lock,NULL);
	
    player_id=_player_id;
    gamelet_session_id="";
}
player::~player()
{
	pthread_mutex_destroy(&additionalData_lock);
	pthread_mutex_lock(&messages_lock);
	ChatMessage *msg;
	while(!messages.empty())
    {
		msg=messages.front();
        delete msg;
        messages.pop();
    }
	pthread_mutex_unlock(&messages_lock);
    std::cout<<"destructor for player with id "<<player_id<<" been called\n";
}
/**
	returns the next availble player id to use
*/
void player::generate_next_id(string & newplayerid)
{
    Utils::generate_uuid(newplayerid);
}
string player::getValue(std::string &key)
{
	string result;
	pthread_mutex_lock(&additionalData_lock);
		result= additionalData[key];
	pthread_mutex_unlock(&additionalData_lock);
	return result;
}
void player::setValue(std::string &key,std::string &val)
{
	pthread_mutex_lock(&additionalData_lock);
		additionalData[key]=val;
	pthread_mutex_unlock(&additionalData_lock);
}

void player::addMessage(std::string &from,std::string msgBody)
{
	pthread_mutex_lock(&messages_lock);
		messages.push(new ChatMessage(this->player_id,from,msgBody));
	pthread_mutex_unlock(&messages_lock);
}
void player::serializeMessages(std::string &msgout)
{
	pthread_mutex_lock(&messages_lock);
	ChatMessage *msg;
	ostringstream oss;
	/*oss<<"\"ChatMessages\":";*/
	oss<<"[";
		while(!messages.empty())
		{
			/*oss<<"\"message\":";*/
			oss<<"{";
			msg=messages.front();
			oss<<"\"from\":\""<<msg->from<<"\"";
			oss<<",";
			oss<<"\"body\":\""<<msg->messageBody<<"\"";
			oss<<"}";
			if (messages.size()>1)
				oss<<",";
			messages.pop();
		}
	oss<<"]";
	pthread_mutex_unlock(&messages_lock);
	msgout+=oss.str();
}
/**
	game container -
		maintains a list of game objects in the game world
*/

////////////////////////////////////////////////////////////////////////////////////
///	game(world) container
////////////////////////////////////////////////////////////////////////////////////

void game_container::insert_player(std::string & player_id,player* new_player)
{
    pthread_mutex_lock(&players_mutex);

    players[player_id]=new_player;

    pthread_mutex_unlock(&players_mutex);
}
/**
	insert new gamelet session
*/
void game_container::insert_session(string & session_id,gamelet_session *new_gamelet_session)
{
    pthread_mutex_lock(&gamelet_sessions_mutex);
    gamelet_sessions[session_id]=new_gamelet_session;
    pthread_mutex_unlock(&gamelet_sessions_mutex);
}
/**
	game session removal
*/
void game_container::remove_session(string & session_id)
{
    pthread_mutex_lock(&gamelet_sessions_mutex);
    std::cout<<"removal of session with id "<<session_id<<" requested, to be implemented\n ";
    pthread_mutex_unlock(&gamelet_sessions_mutex);
}
/**
	added mapping between gamelet class name and factory (maker) function
*/
void game_container::add_gamelet_class(std::string & gamelet_class_name,Function_Ptr gamelet_class_factory_func)
{
    pthread_mutex_lock (&gamelet_classes_mutex);

    gamelet_classes[gamelet_class_name]= gamelet_class_factory_func;

    pthread_mutex_unlock(&gamelet_classes_mutex);
}

/** @brief returns last gamelet session inserted
  *
  * returns last gamelet session inserted or null if empty
  */
gamelet_session* game_container::get_last_gamelet_session()
{
    gamelet_session *ssn=NULL;

    pthread_mutex_lock(&gamelet_sessions_mutex);
    if (!gamelet_sessions.empty()&& gamelet_sessions.size()>1)
    {
        map<std::string,gamelet_session*>::iterator itr=gamelet_sessions.end();
        itr--;
        itr--;
        ssn=itr->second;
    }
    pthread_mutex_unlock(&gamelet_sessions_mutex);
    return ssn;
}

void game_container::get_All_gamelet_sessions(map<string,gamelet_session*> &all_gamelets)
{
    pthread_mutex_lock(&gamelet_sessions_mutex);
    for (
        map<string,gamelet_session*>::iterator gamelet_session_itr=gamelet_sessions.begin();
        gamelet_session_itr!=gamelet_sessions.end();
        gamelet_session_itr++
    )
    {
        all_gamelets[gamelet_session_itr->first]=gamelet_session_itr->second;
    }
    pthread_mutex_unlock(&gamelet_sessions_mutex);
}
/**
	removes player with specified player id
*/
void game_container::remove_player(std::string & player_id)
{
    pthread_mutex_lock(&players_mutex);
    //not implemented yet
    std::cout<<"player with id "<<player_id<<" removal requested. to be implemented\n";
    pthread_mutex_unlock(&players_mutex);
}
/**
	@return retuerns player object with specified player id
            or NULL if non exists
*/
player* game_container::get_player(std::string & player_id)
{
    pthread_mutex_lock(&players_mutex);

    map<string,player*>::iterator it;
    player* player_result=NULL;
    it=players.find(player_id);
    if (!(it==players.end()))
        player_result=it->second;
    else
        player_result=NULL;

    pthread_mutex_unlock(&players_mutex);

    return player_result;
}
/**
	returns a factory method for gamelet class
	@param gamelet_class_name name of gamelet class to load
	@return potiner to gamelet factory method or null
            if gamelet class with such name does not exists
*/
Function_Ptr game_container::get_factory_for_class(const std::string & gamelet_class_name)
{
    cout<<"atempting to lock shared resource gamelet_classes...\n";
    pthread_mutex_lock (&gamelet_classes_mutex);
    cout<<"shared resource gamelet_classes succesfully locked\n";

    //lock start

    //map<std::string,Function_Ptr>::iterator result;
    Function_Ptr fcn_result;
    cout<<"attempting to find gamelet class named "<<gamelet_class_name<<endl;
    //result=gamelet_classes.find(gamelet_class_name);
	fcn_result=gamelet_classes[gamelet_class_name];
    /*if (result==gamelet_classes.end() )
    {
        cout<<"no gamelet factory with class name "<<gamelet_class_name<<" in repository\n";
        fcn_result=NULL;
    }
    else
    {
        cout<<"gamelet factory with class name "<<gamelet_class_name<<" exists in repository\n";
        fcn_result=result->second;
    }*/
	if (fcn_result==NULL )
    {
        cout<<"no gamelet factory with class name "<<gamelet_class_name<<" in repository\n";
    }
    else
    {
        cout<<"gamelet factory with class name "<<gamelet_class_name<<" exists in repository\n";
    }
    //lock end
    pthread_mutex_unlock(&gamelet_classes_mutex);
    cout<<"shared resource gamelet_classes released.\n";
    return fcn_result;
}
/**
	returns a gaelet session object by it's id
	@param gamelet_session_id gamelet session id
	@return gamelet session if exists or NULL if not exists gamelet session with specified  gamelet_session id
*/
gamelet_session* game_container::get_gamelet_session(const std::string & gamelet_session_id)
{
    cout<<"setting lock on gamelet_sessions_mutex ... \n";

    pthread_mutex_lock(&gamelet_sessions_mutex);

    cout<<"inside critical section\n";
    map<std::string,gamelet_session*>::iterator result= gamelet_sessions.find(gamelet_session_id);
    gamelet_session* game_session_result=NULL;
    if (result==gamelet_sessions.end())
        game_session_result=NULL;
    else
        game_session_result=result->second;

    pthread_mutex_unlock(&gamelet_sessions_mutex);

    cout<<"gamelet_sessions_mutex unlocked, critical section exited...\n";
    //return gamelet_sessions[gamelet_session_id];
    return game_session_result;
}

/**
	destroy game container
	TODO actual game container destroy code
*/
game_container::~game_container()
{
    std::cout<<"game container destructor called\n";
    std::cout<<"releasing mutexs...\n";
    pthread_mutex_destroy(&players_mutex);
    pthread_mutex_destroy(&gamelet_classes_mutex);
    pthread_mutex_destroy(&gamelet_sessions_mutex);
    std::cout<<"done.\n";
}
/**
	default constructor
*/
game_container::game_container()
{
    std::cout<<"game container constructor called\n";
    std::cout<<"initalizing mutex locks...\n";
    pthread_mutex_init(&players_mutex,NULL);
    pthread_mutex_init(&gamelet_classes_mutex,NULL);
    pthread_mutex_init(&gamelet_sessions_mutex,NULL);
    cout<<"tesing mutex player_mutex ...\n";
    pthread_mutex_lock(&players_mutex);
    pthread_mutex_unlock(&players_mutex);
    cout<<"player mutex test succsfull.\n";

    std::cout<<"done.\n";
}

game_container *game_container::currentInstance;
game_container* game_container::Instance()
{
	if (!currentInstance)
		currentInstance=new game_container();
	return currentInstance;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///	world manager
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
	world manager	-
		manages the game world
*/
world_manager::world_manager():the_world_container(*game_container::Instance())
{
    //init database
    cout <<"initalizng database component\n";
    DataManager=Alexsuh::Data::dataManager::GetInstance();
    if (DataManager)
    {
        cout<<"dataManager succesfully initalized \n";
    }
    else
    {
        cout<<"coutl not inti dataManager , Press any key to continue\n";
        cin.get();
    }
    ///init("/opt/game_engine/");
    ///init(app_path);
}
world_manager::~world_manager()
{
    std::cout<<"wolrd_manager ended";
}
/**
	will handle player loggin into the system
	player object return is owned by world manager
	@param username user name
	@param password user password
	@return player obejct pointer
*/
player* world_manager::login(std::string username,std::string password)
{
	if (username.length()>20 || password.length()>20)
		return NULL;
    std::cout<<"login requested for user name "<<username<<" and password "<<password<<endl;
    
	//todo- actual validation code
    //dataManager->Instance()->Login(username,password);
	
	char qry[255];
	std::vector<Alexsuh::Data::DBObject*> *result=NULL;
	sprintf(qry,"select * from sampleprojectdb.players where player_name='%s' and player_password='%s'",username.c_str(),password.c_str());
	std::string player_name;
	std::string player_email;
	try
	{
		result= DataManager->toDBObject(string(qry));
		if (result->empty())
			return NULL;
		player_name=(*result)[0]->data["player_name"];
		player_email=(*result)[0]->data["player_email"];
		
	}
	catch(sql::SQLException sql_exp)
	{
		Alexsuh::ErrorHandling::ErrorManager::Instance()->ExceptionHandler(sql_exp);
		return NULL;
	}
	std::string new_user_id;
    player::generate_next_id(new_user_id);
    player *p=new player(new_user_id);
	p->setValue(string("player_name"),player_name);
	p->setValue(string("player_email"),player_email);
    the_world_container.insert_player(new_user_id,p);
    return p;
}
/***
	long world_manager::logout(unsigned long player_id)
	will handfle player logging out of the system

	@param player_id player id to remove from logged in players
*/
void world_manager::logout(std::string & player_id)
{
    std::cout<<"logout requested for id "<<player_id;
    /// TODO cleanup
    ///retirive player object
    ///do cleanup
    ///close empty gamelets

    ///remove from player list
    the_world_container.remove_player(player_id);
    ///remove from memory
    std::cout<<"player has been logged out";
}


player* world_manager::register_player(const std::string user_name, const char password[], const std::string &email, std::map<std::string,std::string> *additionaldata)
{
	std::cout<<"registering player with following data : \n";
	std::cout<<"user name :"<<user_name<<endl;
	std::cout<<"password :"<<password<<endl;
	std::cout<<"email :"<<email<<endl;
	if (additionaldata)
	{
		cout<<"additional Data supplied "<<endl;
	}
	else
	{
		cout<<"additional Data Not supplied "<<endl;
	}
	//allocate new gamelet for player
	std::string current_gamelet_id;
	std::string default_gamelet;
	gamelet_session::get_next_gamelet_session_id(default_gamelet);
	current_gamelet_id=default_gamelet;

	//data access starts
	
	ostringstream oss;
	oss<<"call sampleprojectdb.registerPlayer(";
	oss<<"'"<<user_name<<"'";
	oss<<",'"<<password<<"'";
	oss<<",'"<<email<<"'";
	oss<<",'"<<current_gamelet_id<<"'";
	oss<<",'"<<default_gamelet<<"'";
	oss<<")";
	std::string qry=oss.str();
	try
	{
		std::vector<Alexsuh::Data::DBObject*> *result= DataManager->toDBObject(qry);
		delete result;
	}
	catch(sql::SQLException sql_exp)
	{
		Alexsuh::ErrorHandling::ErrorManager::Instance()->ExceptionHandler(sql_exp);
	}
	/*string qry=+string("
	sprintf("call registerPlayer(@user
	std::string sql=*/
	
	
	//this->DataManager->toDBObject(
	//data access ends

	return NULL;
}

gamelet_session *rootGamelet=NULL;
gamelet_session * world_manager::start_gamelet(const std::string& gamelet_class,const std::string& gamelet_session_id)
{
    ///retrieve gamelet factory method
    Function_Ptr gamelet_maker=the_world_container.get_factory_for_class(gamelet_class);
    if (gamelet_maker==NULL)
    {
        cout<<"warning , failed tio retrieve factory for gamelet class '"<<gamelet_class<<"'";
        return NULL;
    }
    cout<<"function pointer is "<<gamelet_maker<<endl;
    cout<<"creating gamelet object\n";

    ///call factory
    gamelet *g=(gamelet*)gamelet_maker();

    cout<<"gamelet object "<<(g==NULL?"NOT CREATED":"CREATED")<<endl;
    if (g==NULL)
    {
        cout<<"could not create gamelet object new_gamelelt_objet==NULL\n";
        return NULL;
    }
    ///register new gamelet object
    //string new_session_id;

    //cout<<"genereting session id ... \n";

    //gamelet_session::get_next_gamelet_session_id(new_session_id);


    cout<<"creating gamelet session ...\n";
    gamelet_session *g_session=new gamelet_session(g,this);
    if (gamelet_session_id!="")
    {
        g_session->session_id=gamelet_session_id;
    }
    cout<<"gamelet session created ,new session id is "<<g_session->session_id<<" ";

    if (g_session!=NULL)
    {
        cout<<"succesfully";
    }
    else
    {
        cout<<"unsuccesfully, aborting\n";
        return NULL;
    }

    cout<<endl;

    cout<<"inserting new session into game session repository\n";
    the_world_container.insert_session(g_session->session_id,g_session);

    cout<<"starting new gamelet session\n";

    ///start new gamelet session
    g_session->init();
    ///start session thread
    g_session->Start(NULL);
    std::cout<<"gamelet session started\n";

    ///test portals start
    cout<<"testing portals start\n";
    if ( rootGamelet==NULL)
    {
        map < string , gamelet_session*> all_gamelets;
        the_world_container.get_All_gamelet_sessions(all_gamelets);
        rootGamelet=all_gamelets.begin()->second;
    }

    if (rootGamelet!=NULL)
        g_session->get_gamelet()->add_object_auto(rootGamelet->session_id);
    cout<<"testing portals end\n";
    ///test portals end

    return g_session;
}
/**
	starts a gamelet of a given
	@param gamelet_class Name of class of gamelet whose session should be spawned
	@return gamelet_session* newly started session or NULL if gameemlt class with
            such name does not exists
*/
gamelet_session * world_manager::start_gamelet(const std::string& gamelet_class)
{
    return start_gamelet(gamelet_class,"");
}
///thread luach wrapper
void *luanch_session_thread(void* target_obj)
{
    return NULL;
}
void world_manager::stop_gamelet(unsigned long gamelet_session_id)
{
    std::cout<<"stop_gamelet no."<<gamelet_session_id;
}
/***
	move player with specified id into another gamelet
*/
void world_manager::move_player(std::string & player_id,gamelet_session *_gamelet_session)
{
    std::cout<<"moving player with id "<<player_id<<" to new place \n";

    if (_gamelet_session==NULL)
    {
        cerr<<"no session supplied\n";
        return;
    }
    player *p=the_world_container.get_player(player_id);
    if (p==NULL)
    {
        cerr<<"no player with id "<<player_id<<endl;
        return;
    }
    //remove player from old gamelet
    if (!p->gamelet_session_id.empty())
    {
        gamelet_session *old_session=the_world_container.get_gamelet_session(p->gamelet_session_id);
        if (old_session!=NULL)
        {
            gamelet *g=old_session->get_gamelet();
            if (g!=NULL)
            {
                g->remove_player(player_id);
                old_session->AddMessageToClientByPlayerId(player_id,"movePlayerTo",_gamelet_session->session_id);

            }
        }
    }
    //insert player into new gamelet
    gamelet *new_gamelet=_gamelet_session->get_gamelet();
    if (new_gamelet==NULL)
    {
        cerr<<"no gamelet present for session \n";
        return;
    }
    cout<<"player id is "<<player_id<<endl;
    //tell gamelet to add player with palyer id
    new_gamelet->add_player(player_id);
    //tell player which gamelet he belongs to
    p->gamelet_session_id=_gamelet_session->session_id;
}

void world_manager::move_player(std::string & player_id,std::string & gamelet_session_id)
{
    cout<<"request to move player been recieved , player id = "<<player_id<<" to gamelet session "<<gamelet_session_id<<endl;

    //cout<<"request to move player been recieved"<<endl;
    gamelet_session * _new_session= the_world_container.get_gamelet_session(gamelet_session_id);
    cout<<"gamelet session with id "<<gamelet_session_id<<" retrieved \n";
    if (_new_session==NULL)
    {
        cout<<"_new session == NULL !\n";
        return;
    }
    if (_new_session!=NULL)
        move_player(player_id,_new_session);
    cout<<"moved player id = "<<player_id<<" to gamelet session "<<gamelet_session_id<<endl;


}
/****
initalize world manager from a given path
*/
void world_manager::init(const char *path)
{

    std::cout<<"initalizeing world manager from path "<<path<<endl;


    ///handle for shared lib
    void *handle;

    ///error std::string
    char *err_string;
    ///using static strings
    char dll_path[500];
    strcpy(dll_path,path);
    char dll_name[500];
#ifndef _MSC_VER
	//linux
	sprintf(dll_name,"%s/sample_gamelet.so",dll_path);
#else
	//windows
	char shared_library_name[]="sample_gamelet.dll";
	char delimeter='\\';
	if (strlen(dll_path)>0)
	{
		//other direcory
		sprintf(dll_name,"%s%c%s",dll_path,delimeter,shared_library_name);
	}
	else
	{
		//same dir as executable
		sprintf(dll_name,"%s",shared_library_name);
	}
#endif

    std::cout<<"opening library at : "<<dll_name<<"\n";
    handle=dlopen(dll_name,RTLD_LAZY);
    err_string = dlerror();
	if( handle==NULL || err_string )
    {
        printf("error opening library!,aborting\n%s\n",err_string);
        return;
    }
    std::cout<<"opened succesfully\n";
    std::cout<<"seeking function named 'test'\n";

    ///function pointer

    gamelet* (*test_fctn)();
    *(void **)(&test_fctn) =dlsym(handle,"make");
    err_string = dlerror();
    if( err_string )
    {
        printf("error seeking function!,aborting\n%s\n",err_string);
        return;
    }
    std::cout<<"function loaded!\n";
    std::string gamelet_class_name="sample_gamelet";

    the_world_container.add_gamelet_class(gamelet_class_name,test_fctn);
    std::cout<<"gamelet factory loaded to game container\n";


    // TODO crate a repository of all hadles opened
    cout<<"checking that gamelelt registered succesfully ...\n";
    Function_Ptr test_factory= the_world_container.get_factory_for_class(gamelet_class_name);
    if (test_factory==NULL)
        cout<<"gamelet registration FAILED\n";
    else
        cout<<"gamemelt registered succesully\n";


    ///init config start
	#ifndef	_MSC_VER
    string config_path=string(path)+string("/engineConfig.cfg");
	#else
	path="conf";
	string config_path=string(path)+string("\\")+string("engineConfig.cfg");
	#endif
	if (config.LoadNestedConfig(config_path))
    {
        cout<<"config loaded from "<<config_path<<"\n";
        cout<<"saved gamelets found :\n";
        map<string,dataNode*>::iterator gamelets_itr;
        cout<<"there are "<<config.nestedConfig.size()<<" gamelet_sessions \n";
        string gamelet_type;
        string gamelet_session_id;
        string gamelet_data;
        gamelet_session *new_gamelet_session;
        for (
            gamelets_itr = config.nestedConfig.begin();
            gamelets_itr !=config.nestedConfig.end();
            gamelets_itr ++
        )
        {
            cout<<"gamelet_type : "<<(gamelets_itr->second->find("gamelet_type")->second)<<"\n";
            cout<<"gamelet_id : "<<(gamelets_itr->second->find("id")->second)<<"\n";
            cout<<"data-1 : "<<(gamelets_itr->second->find("data-1")->second)<<"\n";
            cout<<"====\n";
            gamelet_type=           gamelets_itr->second->find("gamelet_type")->second;
            gamelet_session_id=     gamelets_itr->second->find("id")->second;
            gamelet_data=           gamelets_itr->second->find("data-1")->second;
            new_gamelet_session=start_gamelet(gamelet_type,gamelet_session_id);
            //cout<<"old session "<<new_gamelet_session->session_id<<endl;
            //new_gamelet_session->session_id=gamelet_session_id;
            //cout<<"new session "<<new_gamelet_session->session_id<<endl;

            if (the_world_container.get_gamelet_session(gamelet_session_id)==NULL)
                cout<<"changing session id failed \n";
            else
                cout<<"changinr session id succesfull \n";
        }

    }
    else
        cout<<"could not load config from"<<config_path<<"\n";

    ///init config end
    std::cout<<"strating NET server "<<endl;
    ///start serevr
    ///staring server
    game_server server(12345,(*this));
    //entering blocking tcp server
    server.start_server();
    std::cout<<"world manager ended\n";
}
/****
return world container
*/
game_container & world_manager::get_game_container()
{
    cout<<"returning gamelet container\n";
    return the_world_container;
}

///////////////////////////////////////////////////////////////
///	gamelet session
/////////////////////////////////////////////////////////////
/***initialize gamew_session_id counter*/
unsigned long gamelet_session::gamelet_session_running_id=0;
/****
creates a new gamelet session
@param g* a gamelet object araound which session to be spawned
*/
gamelet_session::gamelet_session(gamelet *g,Iworld_manager *_world_manager_param)
{
    pthread_mutex_init(&SysMessagesToClient_lock,NULL);
    pthread_mutex_init(&SysMessagesToClientByPlayerId_lock,NULL);

    this->_world_manager=_world_manager_param;
    //lock inner gamelet object until it's initalized
    pthread_mutex_init(&inner_gamelet_mutex,NULL);
    pthread_mutex_lock(&inner_gamelet_mutex);

    exec_time_main_loop=-1;

    cout<<"initializing base constructor\n";
    ///initialize base constructor
    Thread();

    cout<<"setting session gamelet\n";
    ///set session parameters
    this->session_gamelet=g;
    ///generate id
    gamelet_session::get_next_gamelet_session_id(this->session_id);
    ///print some debug info
    std::cout<<"gamelet session constructor called\n";
    std::cout<<"new gamelet session id is "<<this->session_id<<"\n";

    gamelet_desc *gamelet_descriptor=new gamelet_desc();
    gamelet_descriptor->gamelet_session_id=this->session_id;
    gamelet_descriptor->gamelet_type_id=-1;
    gamelet_descriptor->_world_manager=this->_world_manager;

    session_gamelet->init_gamelet(gamelet_descriptor);

    pthread_mutex_unlock(&inner_gamelet_mutex);

}
/****
end gamelet session
*/
gamelet_session::~gamelet_session()
{
    std::cout<<"gamelet session desctructor";
    pthread_mutex_destroy(&inner_gamelet_mutex);

}
/****
init  gamelet
*/
void gamelet_session::init()
{
    std::cout<<"gamelet session init called\n";
    is_stop=0;
}
/****
gamelet run function
*/
void gamelet_session::run()
{
    std::cout<<"running gamelet main loop\n";


    while(is_stop==0)
    {
        timeval tim;
        gettimeofday (&tim, NULL);
        double t1=tim.tv_sec+(tim.tv_usec/1000000.0);


        session_gamelet->main_loop();
        //std::cout<<"gamelet session run called\n";
        Utils::msleep(100);

        gettimeofday (&tim, NULL);
        double t2=tim.tv_sec+(tim.tv_usec/1000000.0);


        exec_time_main_loop=(t2-t1)*1000;
        //cout<<"exec time was "<<exec_time_main_loop<<" msecs"<<endl;
    }

}
/****
stop gamelet run
*/
void gamelet_session::stop()
{
    std::cout<<"gamelet session stop called\n";
    is_stop=1;
}
/****
return session gamelet
*/
gamelet* gamelet_session::get_gamelet()
{
    std::cout<<"gamelet session get gamelet called";
    pthread_mutex_lock(&inner_gamelet_mutex);
    gamelet * inner_gamelet=session_gamelet;
    pthread_mutex_unlock(&inner_gamelet_mutex);
    return inner_gamelet;
}
/****
	return next availble gamelet session id
*/
void gamelet_session::get_next_gamelet_session_id(string &newgameletsessionid)
{
    Utils::generate_uuid(newgameletsessionid);
}

///game session threading
/****
	gamelet session thread initialization
*/
void gamelet_session::Setup()
{
    /// Do any setup here
    std::cout<<"game session thread been called\n";
}
/****
	gamelet session running code
*/
void gamelet_session::Execute(void* arg)
{
    /// Your code goes here
    std::cout<<"session SessionThread exceute been called\n";
    ///run gamelet session main method
    run();
}

void gamelet_session::serialize(string& ser_out)
{
    ostringstream msgoutStream;
    pthread_mutex_lock(&SysMessagesToClient_lock);
    int num_of_keys=SysMessagesToClient.size();
    int key_index=0;
    for (
        map<string,string>::iterator SysMessagesToClient_itr=SysMessagesToClient.begin();
        SysMessagesToClient_itr!=SysMessagesToClient.end();
        SysMessagesToClient_itr++
    )
    {
        msgoutStream<<"\""<<SysMessagesToClient_itr->first<<"\":\""<<SysMessagesToClient_itr->second<<"\"";
        if (key_index<num_of_keys)
            msgoutStream<<",";
        key_index++;
    }
    SysMessagesToClient.clear();
    pthread_mutex_unlock(&SysMessagesToClient_lock);
    ser_out=msgoutStream.str();

}
void gamelet_session::serialize(dataNode &ser_out)
{
    /* code goes here*/
}
void gamelet_session::deSerialize(const string& ser_in)
{

}
void gamelet_session::deSerialize(const dataNode& ser_in)
{
    /* code goes here*/
}
void gamelet_session::AddMessageToClient(string & key, string &val)
{
    pthread_mutex_lock(&SysMessagesToClient_lock);
    SysMessagesToClient[key]=val;
    pthread_mutex_unlock(&SysMessagesToClient_lock);
}

void gamelet_session::AddMessageToClientByPlayerId(const string &player_id,const string & key,const string &val)
{
    pthread_mutex_lock(&SysMessagesToClientByPlayerId_lock);

    map<string,string> messagesForPlayr=SysMessagesToClientByPlayerId[player_id];
    messagesForPlayr[key]=val;

    pthread_mutex_unlock(&SysMessagesToClientByPlayerId_lock);
}

void gamelet_session::GetMessagesForClientByPlayerId(string &player_id,string &msgout)
{
    ostringstream msgoutStream;
    pthread_mutex_lock(&SysMessagesToClientByPlayerId_lock);
    //critical section start
        map<string,string>messagesForPlayr=SysMessagesToClientByPlayerId[player_id];
        if (messagesForPlayr.size()>0)
        {
            cout<<"message cout in messagesForPlayr "<<messagesForPlayr.size()<<"\n";
            exit(0);
        }
        /*
        if (messagesForPlayr.size()>0)
        {
            cout<<"messagesForPlayr for player "<<player_id<<" "<<messagesForPlayr.size()<<"\n";
            exit(0);
        }

    	int num_of_keys=messagesForPlayr.size();
        int key_index=0;
        for (
            map<string,string>::iterator SysMessagesToClient_itr=messagesForPlayr.begin();
            SysMessagesToClient_itr!=messagesForPlayr.end();
            SysMessagesToClient_itr++
        )
        {

            msgoutStream<<"\""<<SysMessagesToClient_itr->first<<"\":\""<<SysMessagesToClient_itr->second<<"\"";
            if (key_index<num_of_keys)
                msgoutStream<<",";
            key_index++;
        }
        messagesForPlayr.clear();
        */
    //critical section end
    pthread_mutex_unlock(&SysMessagesToClientByPlayerId_lock);
    msgout=msgoutStream.str();
}
/////////////////////////////////////////////////////////////////////////
///
///	Network Server
///
///
/// 		game network TCP server
///
////////////////////////////////////////////////////////////////////////
/****
initalize new game server
*/
game_server::game_server(int _portno, world_manager & _world_manager) : network(_portno),_world_manager(_world_manager)
{

    //this->_world_manager=_world_manager;
}
/****
	handle incoming connection request in a new request handler thread
*/
void game_server::handleReq(int socketfd)
{
    ///start new handler thread with reference to
    ///world manager object and
    ///socket descriptor
	ReqHandler::CreateHandler(socketfd,_world_manager);

}
/****
destroy game server
*/
game_server::~game_server()
{

}
/////////////////////////////////////////////////////////
////
////
////	request handler
////
////
////			starts here
////
///////////////////////////////////////////////////////
///consts
//header types
//request type
const int req_type_keepAlive=0;
const int req_type_none=1;
const int req_type_header_end=3;
const int req_type_login=2;
const int req_type_logout=4;
const int req_type_switch_gamelet=5;

const std::string field_name_messageType="0";
const std::string field_name_playerId="1";
const std::string field_name_header_end="3";
const std::string field_name_username="4";
const std::string field_name_password="5";
const std::string field_name_gamelet_type_id="6";
const std::string field_name_gamelet_session_id="7";

//header types
//return messages
std::string const NO_PLAYER_ID="No player Id was found!";
std::string const NO_SUCH_PLAYER="player is not logged in ";
std::string const PLAYER_FOUND="player is logged in !";
std::string const MSG_PARSE_MALFORMED_INPUT="malformed message , discarding ";
//return messages
///consts
///header parameters


/****
	handles incoming request to server
*/
void ReqHandler::handleRequest()
{
    char
    buffer_in[1000];		///buffered red
    int n=0;				///bytes written/red


    ///////////////////////////////////////////////////////////////////////////
    ///	read from network
    ///
    ///////////////////////////////////////////////////////////////////////////

    ///init buffer
    memset(buffer_in,'\0',sizeof(buffer_in));

    ///read from client socket into buffer
    ///	TODO change buffer_in to dynamic buffer (maybe read into std::string Vector
    n = read(socketfd,buffer_in,sizeof(buffer_in)-1);
    if (n < 0)
    {
        cerr<<"ERROR reading from socket\n";
        return ;
    }

    std::cout<<"recvieved message: "<<buffer_in<<endl;

    ////////////////////////////////////////////////////////////////////////////////////////
    ///	parse ,and calls for for game engine api (lib_game_core)
    ///////////////////////////////////////////////////////////////////////////////////////

    ///parse and prepere reponse buffer
    std::string msg_in,msg_out;
    msg_in=buffer_in;
    parse(msg_in,msg_out);

    /////////////////////////////////////////////////////////////////////////////////////
    ///	neccesery parsing been finished and reply from
    ///	gamelet serevr side been recieved
    ///	write to socked and close	(control back to handler)
    /////////////////////////////////////////////////////////////////////////////////////

    write(socketfd,msg_out.c_str(),msg_out.length());
    
	///free memory
    ///free (buffer_out);
    if (n < 0)
        throw "ERROR writing to socket";

    ///close the connection
    std::cout<<"closing connection and waiting new connections\n";
    close(socketfd);
    ////////////////////////////////////////////////////////////////////////////////////
    ///	end of handler thread
    ////////////////////////////////////////////////////////////////////////////////////
}



/****
	parses incoming request for header and gamelet specific data information
*/
void ReqHandler::parse(std::string const &msg_in,std::string &msg_out)
{

    stringstream mystream(stringstream::in | stringstream::out);
    mystream<<msg_in;
    ///dummy data

    std::string val="";
    std::string key="";


    ///read header parameters
    while(!mystream.eof())// && key.compare(field_name_header_end))
    {
        mystream>>key>>val;
        header_params[key]=val;
    }
    cout<<"red "<<header_params.size()<<" header params\n";

//dispatch message
    map<std::string,std::string>::iterator it;
    std::string message_type;
    it=header_params.find(field_name_messageType);
    if (it==header_params.end())
    {
        msg_out="no message type was found!\n";
        return;
    }
    else
    {
        std::cout<<"found message type of "<<it->second<<endl;
    }

    message_type=it->second;
    int message_code=atoi(message_type.c_str());
    switch (message_code)
    {
    case req_type_keepAlive:
        do_update(msg_out);
        break;
    case req_type_login:
        do_login(msg_out);
        break;
    case req_type_logout:
        do_logout(msg_out);
        break;
    case req_type_switch_gamelet:
        do_switch_gamelet(msg_out);
        break;
	case req_type_register:
		do_register(msg_out);
		break;
    default:
        msg_out="request not understood";
    }


}
void ReqHandler::do_register(std::string &msgout)
{
	msgout="REGISTING!!!";
	std::string user_name=header_params[field_name_username];
	std::string password=header_params[field_name_password];
	std::string email=header_params[field_name_email];
	
	_world_manager->register_player(user_name,password.c_str(),email,NULL);
}

/**
	pre thread setup stub
*/
void ReqHandler::Setup()
{
    ///pre thread setup stub
}
/**
	request handler thread body
*/
void ReqHandler::Execute(void* arg)
{
    std::cout<<"ReqHandler executing !\n";
    handleRequest();
    //GC disposes of this object start
#ifndef _MSC_VER
	//this should work, but it doesent , strange ...(linux)
	Dispose();
#else
	//ms hack
	delete this;
#endif
    //GC disposes of this object end
}
void ReqHandler::Dispose()
{
    Alexsuh::GarbageCollection::Disposble::Dispose();
}
void ReqHandler::RegisterSelf(Alexsuh::GarbageCollection::IObjectManager *_objManager)
{
    Alexsuh::GarbageCollection::Disposble::RegisterSelf(_objManager);
}
Alexsuh::GarbageCollection::IObjectManager *ReqHandler::objManager;

/**
	initializing request handler thread
*/
ReqHandler::ReqHandler(int _socketfd,world_manager & _world_manager) : Thread()
{
    std::cout<<"initializing request handler thread!\n";

    //init simple GC start


        //this->objManager=Alexsuh::GarbageCollection::ObjectsManager::GetInstance();


        //this->objManager->RegisterObject(this);

    //init simple GC end

    //get application Object Manager
    std::cout<<"ReqHandler::ReqHandler getting GC manager\n";
    objManager=Alexsuh::GarbageCollection::ObjectsManager::GetInstance();
    std::cout<<"ReqHandler::ReqHandler GC manager Aquired\n";
    std::cout<<"ReqHandler::ReqHandler registering object for GC Manager\n";
    //RegisterSelf(objManager);
    std::cout<<"ReqHandler::ReqHandler object for GC Manager registered\n";

    socketfd=_socketfd;
    this->_world_manager=&_world_manager;
    cout<<"ReqHandler::ReqHandler starting ReqHandler Thread\n";
    int ThreadCreateCode=Start(NULL);
#ifndef _MSC_VER
//unix thread creation debug code 
	switch (ThreadCreateCode)
    {
        case 0:
            cout<<"Thread completed Succesfully!\n";
            break;
        case EAGAIN:
            cout<<"Thread The system lacked the necessary resources to create another thread, or the system-imposed limit on the total number of threads in a process \n";
            break;
        case EINVAL:
            cout<<"The value specified by attr is invalid. \n";
            break;
        case EPERM:
            cout<<"The caller does not have appropriate permission to set the required scheduling parameters or scheduling policy. \n";
            break;
        default:
            cout<<"coult not create thread , error code was "<<ThreadCreateCode<<"\n";
            break;
    }
#endif


}
ReqHandler * ReqHandler::CreateHandler(int socketfd,world_manager& _world_manager)
{
	return new ReqHandler(socketfd,_world_manager);
}
///consts
//header types
//request type
const int ReqHandler::req_type_keepAlive;
const int ReqHandler::req_type_none;
const int ReqHandler::req_type_header_end;
const int ReqHandler::req_type_login;
const int ReqHandler::req_type_logout;
const int ReqHandler::req_type_switch_gamelet;
const int ReqHandler::req_type_register;

//request types
//field names
std::string ReqHandler::field_name_messageType="0";
std::string ReqHandler::field_name_playerId="1";
std::string ReqHandler::field_name_header_end="3";
std::string ReqHandler::field_name_username="4";
std::string ReqHandler::field_name_password="5";
std::string ReqHandler::field_name_gamelet_type_id="6";
std::string ReqHandler::field_name_gamelet_session_id="2";
std::string ReqHandler::field_name_email="7";
//field names
//header types
//return messages
std::string  ReqHandler::NO_PLAYER_ID="no player with this id";
std::string  ReqHandler::NO_SUCH_PLAYER="no such player";
std::string  ReqHandler::PLAYER_FOUND="player found!";
std::string  ReqHandler::MSG_PARSE_MALFORMED_INPUT="malformed message input";
//return messages
///consts


/**
user login
@param msgout a return message
*/
void ReqHandler::do_login(std::string &msgout)
{
    std::string user_name=header_params[field_name_username];
    std::string password=header_params[field_name_password];
    /// TODO here actual user authorization and authentication takes place
    //register user
    player *newplayer=_world_manager->login(user_name,password);
    string player_id = newplayer->player_id;
    //get user default gamelet
    /// TODO actual default gamelet lookup will go here
    //std::string user_default_startup_gamelet="mydefaultgamelet123";
    header_params[field_name_gamelet_type_id]="sample_gamelet";
    //gamelet_session *ssn=_world_manager->get_game_container().get_gamelet_session(user_default_startup_gamelet);
    //get and start user gamelet
    header_params[field_name_playerId]=player_id;
    cout<<"accessing gamelet...\n";
    do_switch_gamelet(msgout);

    cout<<"accessing gamelet done\n";
    msgout+=";"+player_id;
    cout<<msgout+"\n";
}
/**
gamelet status update and keep alive implementation
@param msgout a return message
*/
void ReqHandler::do_update(std::string &msgout)
{
    //lookup gamelet
    cout<<"seeking player id \n";
    std::string userid=header_params[field_name_playerId];
    cout<<"user id is "<<userid;
    cout<<"seeking player \n";
    player *pl=_world_manager->get_game_container().get_player(userid);
	//messages start

	std::string messageTo=header_params["messageTo"];
	std::string messageBody=header_params["messageBody"];
	//if has messages for player
	if (!messageTo.empty() && !messageBody.empty())
	{
		//add messages to relevant player
		player* plTo=_world_manager->get_game_container().get_player(messageTo);
		if (plTo!=NULL)
		{
			plTo->addMessage(pl->getValue(string("player_name")),messageBody);
		}
	}

	//messages end
    cout<<"player is player name="<<pl->name<<" ; gamelet session id "<<pl->gamelet_session_id<<"\n";
    cout<<"seeking gamelet session \n";
    gamelet_session* _game_session=_world_manager->get_game_container().get_gamelet_session(pl->gamelet_session_id);
    if (_game_session==NULL)
    {
        cerr<<"no Gamelet session was found!\n";
        return;
    }
    cout<<"gamelet session found \n";
    

    cout<<"retieve session gamelet\n";
    gamelet *gmlt=_game_session->get_gamelet();
    if (gmlt==NULL)
    {
        cerr<<"gamelet not fount ! \n";
        return;
    }
    else
    {
        cout<<"gmlt is "<<gmlt->gamelet_name<<endl;
    }
    cout<<"gamelet retrieved. \n";
    cout<<"getting gamelet model string\n";
    gmlt->get_model_string(msgout);

    //handle gamelelt specific data start

    map<std::string,std::string>::iterator itr=header_params.find("gamelet_data");
    if (itr!=header_params.end())
    {
        cout<<"found message for gamelet :"<<(itr->second)<<endl; //["gamelet_data"];
        string msg=itr->second;
        string replay;
        //prepere request
        handle_client_request_Args req_Args;
        req_Args.messageQuery=msg;
        req_Args.playerid=userid;
        gmlt->handle_client_request(req_Args,replay);
    }
    else
    {
        cout<<"no gamelet spesific data in this batch\n";
    }

    //handle gamelet specific data end

 
    ostringstream msgoutStream;

	//system data start
	string sessionSerializationString;
    _game_session->serialize(sessionSerializationString);
	
	string MessagesForClientByPlayerId;
	_game_session->GetMessagesForClientByPlayerId(pl->player_id,MessagesForClientByPlayerId);

	map <string ,string > upadteParams;
	char exec_time_main_loop_cstr[10];
	sprintf(exec_time_main_loop_cstr,"%g",_game_session->exec_time_main_loop);
	std::string chatMessages;
	pl->serializeMessages(chatMessages);
	
	upadteParams ["updateTime"]						=	exec_time_main_loop_cstr;
	upadteParams ["sessionSerializationString"]		=	sessionSerializationString;
	upadteParams ["MessagesForClientByPlayerId"]	=	MessagesForClientByPlayerId;	
	upadteParams ["ChatMessages"]					=	chatMessages;

	ostringstream system_data_Stream;
	
	std::string key;
	std::string value;
	int isFirst=1;
	for (map<string,string>::iterator itr=upadteParams.begin();
		itr!=upadteParams.end();
		itr++)
	{
		if (!itr->second.empty())
		{
			if (isFirst)
				isFirst=0;
			else
			{
				//add ',' if not the first
				system_data_Stream<<",";
			}
			key=itr->first;
			value=itr->second;
			if (value.find(string("["))==string::npos)
			{
				system_data_Stream<<"\""<<itr->first<<"\":\""<<value<<"\"";
			}
			else
			{
				system_data_Stream<<"\""<<itr->first<<"\":"<<value<<"";
			}
		}
	}

	msgoutStream<<"{\"system_data\":{"<<system_data_Stream.str()<<"}";

    msgoutStream<<",\"GameletModel\":"<<msgout<<"}";
    msgout=msgoutStream.str();
    cout<<" ReqHandler::do_update returning result "<<msgout;
}
/**
user logout
@param msgout a return message
*/
void ReqHandler::do_logout(std::string &msg_out)
{
    std::string userid=header_params[field_name_playerId];
    _world_manager->logout(userid)	;
    msg_out="logged out succefully";
}

/**
1.5 gamelet switching
@param msgout a return message
*/
void ReqHandler::do_switch_gamelet(std::string &msgout)
{
    ///determine if gamelet session id or gamelet type is given

    std::string gamelet_session_id;
    map<std::string,std::string>::iterator itr_gamelet_session_id=header_params.find(field_name_gamelet_session_id);

    if (itr_gamelet_session_id!=header_params.end())
        gamelet_session_id=itr_gamelet_session_id->second;
    else
        gamelet_session_id=std::string();

    std::string gamelet_type_id;
    map<std::string,std::string>::iterator itr_gamelet_type_id=header_params.find(field_name_gamelet_type_id);
    if (itr_gamelet_type_id!=header_params.end())
        gamelet_type_id=itr_gamelet_type_id->second;
    else
        gamelet_type_id=std::string();

    //std::string gamelet_type_id=header_params[field_name_gamelet_type_id];

    ///1.5.4	New Gamelet is retrieved
    gamelet_session *newsession;
    if (!gamelet_session_id.empty() )
    {
        cout<<"accessing gamelet_session_id of "<<gamelet_session_id<<"\n";
        ///gamelet session id is given , so try retrieving gamelet session by id
        newsession=_world_manager->get_game_container().get_gamelet_session(gamelet_session_id);
    }
    else if (!gamelet_type_id.empty())
    {
        cout<<"accessing gamelet_type_id of "<<gamelet_type_id<<"\n";
        ///gamelet type id is given without spesific session , so spawn a new session
        newsession=_world_manager->start_gamelet(gamelet_type_id);
    }
    else
    {
        cout<<"nor gamelet_session_id , gamelet_type_id are present , bad input \n";
        ///do not know what to do , fail
        msgout=MSG_PARSE_MALFORMED_INPUT;
        return;
    }
    ///no gamemlet with this name or session id is missing
    if (newsession==NULL)
    {
        //no such gamelet class exists
        msgout+="ERROR! no such gamelet or session !\n";
        return;
    }
    ///player is retrieved
    std::string playerid=header_params[field_name_playerId];
    cout<<"for player "<<playerid<<"\n";
    ///player is pulled out of the old gamelet session
    ///player is inserted into the new game session
    cout<<"moving player ... \n";
    _world_manager->move_player(playerid,newsession);
    //TODO actual client code path retrievel
    msgout="gamelets/sampleGamelet/js/samplegamelet.js";
}



////////////////////////////
//  Req Handler End
//////////////////////////////


//utils
namespace Utils
{
//int UUID_INITIATED=0;
void generate_uuid(string & newuuid)
{
	time_t t=time(NULL);
	srand(t);
    if (!UUID_INITIATED)
    {
        
        UUID_INITIATED=1;
    }
    char strUuid[200];
	memset(strUuid,0,sizeof(char)*200);
    sprintf(strUuid, "%x%x-%x-%x-%x-%x%x%x",
            rand(), rand(),                 // Generates a 64-bit Hex number
            rand(),                         // Generates a 32-bit Hex number
            ((rand() & 0x0fff) | 0x4000),   // Generates a 32-bit Hex number of the form 4xxx (4 indicates the UUID version)
            rand() % 0x3fff + 0x8000,       // Generates a 32-bit Hex number in the range [0x8000, 0xbfff]
            rand(), rand(), rand());        // Generates a 96-bit Hex number

    newuuid=string(strUuid);
}
/**
courtsey of http://cc.byexamples.com/2007/05/25/nanosleep-is-better-than-sleep-and-usleep/
*/
#ifndef _MSC_VER
//unix/linux
int msleep(unsigned long milisec)
{
    struct timespec req= {0};
    time_t sec=(int)(milisec/1000);
    milisec=milisec-(sec*1000);
    req.tv_sec=sec;
    req.tv_nsec=milisec*1000000L;
    while(nanosleep(&req,&req)==-1)
        continue;
    return 1;
}
#else
//win32
int msleep(unsigned long milisec)
{
	Sleep(milisec);
	return 1;
}
#endif
}
////////////////////////////////////////
//implementation of ConfigContainer START
//
//
//
//
////////////////////////////////////////
ConfigContainer::ConfigContainer()
{
    //ctor
}

ConfigContainer::~ConfigContainer()
{
    //dtor
}

/** @brief insertKey
  *
  * @todo: document this function
  */
void ConfigContainer::insertKey(const std::string &key,const std::string& val)
{
    config[key]=val;
}

/** @brief getKey
  *
  * @todo: document this function
  */
std::string* ConfigContainer::getKey(const std::string &key)
{
    map<string,string>::iterator itr=config.find(key);
    if (itr==config.end())
        return NULL;
    else
        return &(itr->second);
}

/** @brief LoadConfig
  *
  * @todo: document this function
  */
bool ConfigContainer::LoadConfig(const std::string& path)
{
    ifstream myfile (path.c_str());
    if (!myfile.is_open())
    {
        return false;
    }
    string key , val;
    while (!myfile.eof())
    {
        myfile>>key>>val;
        if (!myfile.eof())
            insertKey(key,val);
    }
    myfile.close();
    return true;
}

bool ConfigContainer::LoadNestedConfig(const std::string & path)
{
	system("dir");
    ifstream myfile (path.c_str());
    bool result=true;
    if (!myfile.is_open())
    {
        return false;
    }
    string key , val;
    char c;
    dataNode *_dataNode;
    ///continue until end of file
    while (!myfile.eof())
    {
        ///search next starting brackets
        c=myfile.get();
        while (c!='{' && !myfile.eof())
            c=myfile.get();
        if (myfile.eof())
        {
            cout<<"could not find starting tag '{' , aborting \n";
            result=false;
            break;
        }
        _dataNode=new dataNode();
        ///map keys to values until next closing bracket
        key="";
        val="";
        while (!myfile.eof() && key!="}")
        {
            myfile>>key;
            if (key=="}" || myfile.eof())
                break;
            myfile>>val;

            _dataNode->insert(pair<string,string>(key,val));
        }//read inner element
        if (key!="}")
        {
            delete _dataNode;
            cout<<"malform data node , must end with }, near "<<val;
            result=false;
            break;
        }//confirm closing tag exists
        ///map the whole thing to id, if id was not found drop and alert
        string id = _dataNode->find("id")==_dataNode->end()?string():_dataNode->find("id")->second;
        if (id=="")
        {
            delete _dataNode;
            cout<<"missing id variable , skipping node near "<<val;
            continue;
        }//confirm id exists end
        nestedConfig[id]=_dataNode;
    }//main parsing loop end
    myfile.close();//cleanup
    return result;
}

////////////////////////////////////////
//implementation of Confi4gContainer END
//
//
//
//
////////////////////////////////////////


////////////////////////////////////////
//implementation of GC START
//
//
//
//
////////////////////////////////////////

namespace Alexsuh
{

    namespace GarbageCollection
    {
        ObjectsManager::ObjectsManager()
        {
            Thread();
            Thread::Start(NULL);
        }
        ObjectsManager::~ObjectsManager()
        {
            pthread_mutex_destroy(&objects_lock);
            pthread_mutex_destroy(&objectsManagerInstanceLock);
        }
        ///blocking call to register obejct obejct
        void ObjectsManager::RegisterObject(IDisposble *disposbleObject)
        {
            pthread_mutex_lock(&objects_lock);
            //critical section start
            	this->objects[disposbleObject]=true;
            //critical section end
            pthread_mutex_unlock(&objects_lock);
        }
        ///blockig call to dispose object
        void ObjectsManager::DisposeObject(IDisposble *disposbleObject)
        {
            pthread_mutex_lock(&objects_lock);
            //critical section start
            	this->objects[disposbleObject]=false;
            //critical section end
            pthread_mutex_unlock(&objects_lock);
        }
        ///blocking get only one instance of this manager
        IObjectManager* ObjectsManager::GetInstance()
        {
            pthread_mutex_lock(&objectsManagerInstanceLock);
            if (currentObjectManagerInstance==NULL)
            {
                currentObjectManagerInstance=new ObjectsManager();
            }
            pthread_mutex_unlock(&objectsManagerInstanceLock);
            return currentObjectManagerInstance;
        }
        ///loop and cleand disposed object every period of time
        ///blocking
        void ObjectsManager::Execute(void * Args)
        {
            queue <IDisposble*> objects_to_dispose;

            while (true)
            {
                pthread_mutex_lock(&objects_lock);


#ifdef _DEBUG_GC
                cout<<"ObjectsManager::Execute Running GC operetion\n";
#endif
                for(
                    map<IDisposble*,bool>::iterator objects_itr=objects.begin();
                    objects_itr!=objects.end();
                    objects_itr++
                    )
                    {
                        //if object not alive it is aligble for removle
                        if (!objects_itr->second)
                        {
                            //add to disposble queues
                            objects_to_dispose.push(objects_itr->first);
                        }
                    }
#ifdef _DEBUG_GC
                    cout<<"ObjectsManager::Execute found"<<objects_to_dispose.size()<<" objects to be disposed \n";
#endif
					int objectsBeforeDispose=objects.size();

                    //dispose all from disposble queue
                    IDisposble *current_disposble;
                    while(!objects_to_dispose.empty())
                    {
                         current_disposble=objects_to_dispose.front();
                         //remove from containers
                         objects_to_dispose.pop();
                         objects.erase(current_disposble);
                         //free memory
                         delete current_disposble;
						 current_disposble=NULL;
                    }

                    int objectsAfterDispose=objects.size();
#ifdef _DEBUG_GC
                    cout<<"objects disposed : "<<(objectsBeforeDispose-objectsAfterDispose)<<"\n";
#endif
                pthread_mutex_unlock(&objects_lock);

                //sleep for next cleanup cycle
				
				Utils::msleep(5000);
            }

        }

        pthread_mutex_t ObjectsManager::objects_lock;
        pthread_mutex_t ObjectsManager::objectsManagerInstanceLock;

        int ObjectsManager::init_class()
        {

            pthread_mutex_init(&(ObjectsManager::objects_lock),NULL);
            pthread_mutex_init(&(ObjectsManager::objectsManagerInstanceLock),NULL);
            return 0;
        }
        int ObjectsManager::init_param=ObjectsManager::init_class();
        ObjectsManager* ObjectsManager::currentObjectManagerInstance;
    }

}


////////////////////////////////////////
//implementation of GC END
//
//
//
//
////////////////////////////////////////


////////
//implemetation DB start
//
//
//
////////
Alexsuh::Data::dataManager *Alexsuh::Data::dataManager::currentInstance;
Alexsuh::Data::dataManager* Alexsuh::Data::dataManager::GetInstance()
{
	if (!currentInstance)
		currentInstance=new dataManager();
	return currentInstance;
}
////////
//implemetation DB end
//
//
//
////////

////////
//implemetation Error start
//
//
//
////////
Alexsuh::ErrorHandling::ErrorManager *Alexsuh::ErrorHandling::ErrorManager::currentInstance;
Alexsuh::ErrorHandling::ErrorManager* Alexsuh::ErrorHandling::ErrorManager::Instance()
{
	if (!currentInstance)
		currentInstance=new ErrorManager();
	return currentInstance;
}
////////
//implemetation Error end
//
//
//
////////



/**
unit testing entry point
*/
int main(int argc,char** argv)
{
	world_manager *w=new world_manager();
	w->init("");
    std::cout<<"hello world from game engine core\n";
    return 0;
}



