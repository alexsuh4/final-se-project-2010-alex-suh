#include "engine_core/libengine_core.h"
int main()
{
	//this is now you call shell commands
	//system("ls");
	std::cout<<"hi world(started)\n";
	char dll_path[500];
	//set default path to app
	strcpy(dll_path,"/opt/game_engine");
	if (argc>1)
	{
		std::cout<<"statring world from path "<<dll_path;
		strcpy(dll_path,argv[1]);
	}
	else
	{
		std::cout<<"using default path ("<<dll_path<<")\n,use "<<argv[0]<<" <new_path> to set another path\n";
	}

	world_manager w;
	w.init(dll_path);
	return 0;
}