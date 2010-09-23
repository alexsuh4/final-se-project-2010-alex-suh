#ifdef SAMPLE_GAMELET_UNIT_TEST
#include "gamelets/gamelet.hpp"
#include "cassert"
#include <iostream>
#include <string>
#include <fstream>
#include <ctime>

using namespace std;
void my_assert(bool condition,string &message)
{
	if (!condition)
	{
		std::cerr<<message;
		assert(condition);
	}
}

//namespace Alexsuh
// {
//	 namespace std::coutging
//	 {
//	enum std::coutgingTypes 
//		{
//		std::cout_TO_COUT,
//		std::cout_TO_CERR,
//		std::cout_TO_FILE,
//		DO_NOT_std::cout
//		};
//
//	enum std::coutgineLevels {
//		std::cout_FINE,
//		std::cout_WARNING,
//		std::cout_ERROR,
//		std::cout_CRITICAL
//		};
//
//	class std::coutger
//	{
//		
//		public:
//		std::coutger()
//		{
//			std::coutLevel=std::cout_WARNING;
//			std::coutTo=std::cout_TO_COUT;
//		}
//		std::coutgineLevels std::coutLevel;
//		std::coutgingTypes std::coutTo;
//		ofstream *std::coutfile;
//	    void operator << (const string & _msg)
//	    {
//			dostd::cout(_msg,std::cout_WARNING);
//	    }
//		
//		/*void operator << (const string & _msg , const std::coutgineLevels &lvl)
//		{
//			dostd::cout(_msg,lvl);
//		}*/
//		void dostd::cout(const string & _msg , const std::coutgineLevels &lvl)
//		{
//			if (lvl<std::coutLevel)
//				return;
//			switch(std::coutTo)
//			{
//				case std::cout_TO_CERR:
//					std::cerr<<_msg;
//					break;
//				case std::cout_TO_COUT:
//					std::cout<<_msg;
//					break;
//				case std::cout_TO_FILE:
//					if (std::coutfile)
//						(*std::coutfile)<<_msg;
//					break;
//				default:
//					break;
//			}
//		}
//	};
//	};//NS std::cout
//};// NS alexsuh

struct player
{
	string name;
	string player_id;
	string current_gamelet_id;
};
class testWM: public Iworld_manager
{
public :
	map<string,player*> players;
	map<string,gamelet*> gamelets;
	void move_player(std::string & player_id,std::string & gamelet_session_id)
	{
		player* p=players[player_id];
		my_assert( (p!=NULL&& p->player_id==player_id) , ("no player with given id !"+player_id));
		string old_gamelet_id=p->current_gamelet_id;
		
		gamelet *old_gamelet= gamelets[old_gamelet_id];
		
		gamelet * new_gamelet=gamelets[gamelet_session_id];
		my_assert(new_gamelet!=NULL, ("no gamelet with id "+gamelet_session_id));
		
		if (old_gamelet)
			old_gamelet->remove_player(player_id);
		new_gamelet->add_player(player_id);

		p->current_gamelet_id=gamelet_session_id;
	}
};


int main(int argc,char **argv)
{

	cout <<"initalizing ...\n";
	srand( time (NULL) );

	std::cout<<"starting unit test\n";
	
	testWM *test_wm=new testWM();
	gamelet_desc *desc=new gamelet_desc();
	desc->gamelet_type_id="sample_gamelet";
	desc->_world_manager=test_wm;
	
	std::cout<<"gamlet constructor \n";
	gamelet *gamelet1=make();
	gamelet *gamelet2=make();
	string gamelet_session1="gameletSession1";
	string gamelet_session2="gameletSession2";

	desc->gamelet_session_id=gamelet_session1;
	gamelet1->init_gamelet(desc);
	desc->gamelet_session_id=gamelet_session2;
	gamelet2->init_gamelet(desc);

	test_wm->gamelets[gamelet_session1]=gamelet1;
	test_wm->gamelets[gamelet_session2]=gamelet2;
	
	string player_id="player1Id";
	player *p1=new player();
	p1->name="player 1 name";
	p1->player_id=player_id;

	test_wm->players[player_id]=p1;
	
	test_wm->move_player(player_id,gamelet_session1);
	my_assert(p1->current_gamelet_id==gamelet_session1,("player not moved to"+gamelet_session1));

	test_wm->move_player(player_id,gamelet_session2);
	my_assert(p1->current_gamelet_id==gamelet_session2,("player not moved to"+gamelet_session2));
	
	bool test_move_player=1;
	bool test_get_model_string=1;
	bool test_add_object_by_player=1;
	bool test_add_object_auto=1;
	bool test_main_loop=1;
	
	/**
	*/
	unsigned int test_freq_max=10;
	int test_move_player_freq=-1;
	int test_get_model_string_freq=-1;
	int test_add_object_by_player_freq=-1;
	int test_add_object_auto_freq=-1;
	int test_main_loop_freq=-1;

	unsigned long loop_rounds=1000;
	std::cout<<"running test of "<<loop_rounds<<" rounds \n";
	string msgout;
	string object_target_id;
	object_target_id=gamelet_session1;
	
	for (unsigned long i=0;i<loop_rounds;i++)
	{
		if (test_main_loop && ((rand()%test_freq_max+1)>  test_main_loop_freq) )
		{
			gamelet1->main_loop();
			gamelet2->main_loop();
		}
		if(test_get_model_string && ((rand()%test_freq_max+1)> test_get_model_string_freq) )
		{
			
				gamelet1->get_model_string(msgout);
				std::cout<<"MODEL STRING GAMELET 1 "<<msgout<<"\n";
				gamelet2->get_model_string(msgout);
				std::cout<<"MODEL STRING GAMELET 2 "<<msgout<<"\n";	
		}
		if (test_move_player && ((rand()%test_freq_max+1)> test_move_player_freq) )
		{
			test_wm->move_player(player_id,gamelet_session1);
			test_wm->move_player(player_id,gamelet_session2);
		}
		if (test_add_object_auto && ((rand()%test_freq_max+1)> test_add_object_auto_freq) )
		{
			gamelet1->add_object_auto(string(object_target_id));
			gamelet2->add_object_auto(string(object_target_id));
		}
	}
	
	delete p1;
	delete gamelet1;
	delete gamelet2;
	delete desc;
	delete test_wm;
	std::cout<<"completed succesfully\n";
	
	
}
#endif