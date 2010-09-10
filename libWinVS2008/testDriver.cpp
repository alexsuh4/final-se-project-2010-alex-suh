#include "sharedLib.h"
#include "gamelet.h"
using namespace std;
void test_gamelet()
{
	//handle for shared lib
	void *handle;

	//error std::string
	char *err_string;
	//using static strings
	char dll_path[]="/home/dsl/workspace/game_engine";
	char dll_name[500];
	int rc;
	sprintf(dll_name,"%s/sample_gamelet.so",dll_path);
	//sprintf(dll_name,"%s/libtest.so",dll_path);

	printf("opening library at : %s\n",dll_name);
	handle=dlopen(dll_name,RTLD_LAZY);
	err_string = dlerror();
	if( err_string )
	{
		printf("error opening library!,aborting\n%s\n",err_string);
		return;
	}
	printf("opened succesfully\n");
	printf("seeking function named 'test'\n");

	//function pointer

	gamelet* (*test_fctn)();
	*(void **)(&test_fctn) =dlsym(handle,"make");
	err_string = dlerror();
	if( err_string )
	{
		printf("error seeking function!,aborting\n%s\n",err_string);
		return;
	}
	printf("function loaded!\n");
	printf("initializing gamelet...\n");
	gamelet *g= (gamelet*)(test_fctn)();

	printf("closing handle...\n");
	rc = dlclose(handle);
  	err_string = dlerror();
  	if( rc )
	{
		printf("error closing library!,aborting\n%s\n",err_string);
		exit(1);
	}
	printf("handle closed!\n)");

	printf("testing interface...\n");
	gamelet_desc my_gamelet_desc;

	my_gamelet_desc.gamelet_session_id="1";
	my_gamelet_desc.gamelet_type_id="1";
	string player_id("12345"),new_session_id("98");
	std::cout<<"\ninit...";g->init_gamelet(&my_gamelet_desc);
	std::cout<<"\nremove player...";g->remove_player(player_id);
	std::cout<<"\ndummy function...";g->dummy_func();
	std::cout<<"\nadd object by player...";g->add_object_auto_by_player(player_id,new_session_id);
	std::cout<<"\nauto placing portal...";g->add_object_auto(new_session_id);
	std::cout<<"\nadd player...";g->add_player(player_id);
	std::cout<<"\nremove player...";g->remove_player(player_id);
	std::cout<<"\nmain loop...";//g->main_loop();
	//test_gamelet_2(g);
	std::cout<<"\ntesting destructor..."; delete g;


}


void main()
{
	test_gamelet();
}