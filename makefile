#
#	makefile 
#
#
CORE_DIR=engine_core
GAMELET_DIR=gamelets
WEB_DIR=web
WEB_PUBLISH_DIR=/opt/monkey/htdocs
MYSQL_DIR=$(CORE_DIR)/data
# after compilation finished , delete all intermidiate object files (*.o) and 
# copy all web content to web publish directory specified by WEB_PUBLISH_DIR 
all : libengine_core.so sample_gamelet libgame_engine_api.so main network thread mySqlClass
	( rm *.o ;echo "publishing web files..." ;rm -fvr $(WEB_PUBLISH_DIR)/* ;cp -ru $(WEB_DIR)/* $(WEB_PUBLISH_DIR))




############################################################
#
#	ENRY POINT COMPILATION
#
#
#
#############################################################

main : main.cpp  sample_gamelet.so  network  libengine_core.so
	g++ -g -Wall -o  main main.cpp -L./ -lengine_core 
	

#############################################################

############################################################
#
#	THREAD LIBRARY COMPILATION
#
#
#					
#############################################################
thread : Thread.o
	(echo "genereting thread")

Thread.o : $(CORE_DIR)/threading/Thread.hh $(CORE_DIR)/threading/Thread.cpp
	g++ -g -Wall -c  -fPIC $(CORE_DIR)/threading/Thread.cpp



################################################################


############################################################
#
#	LIBRARY ENGINE CORE COMPILATION
#
#
#
#############################################################


libengine_core.so : libengine_core.o Thread.o libnetwork_module.o 
	g++ -g -Wall --no-undefined -shared -lc -o libengine_core.so libengine_core.o Thread.o libnetwork_module.o -ldl -lpthread -lmysqlcppconn 

libengine_core.o : $(CORE_DIR)/libengine_core.cpp $(CORE_DIR)/libengine_core.hh  $(CORE_DIR)/threading/Thread.hh $(CORE_DIR)/threading/Thread.cpp 
	g++ -g -Wall -c  -fPIC $(CORE_DIR)/libengine_core.cpp

	
###############################################################


############################################################
#
#	NETWORK MODULE COMPILATION
#
#
#
############################################################
#libnetwork_module.so : libnetwork_module.o Thread.o
#	g++ -g -Wall -shared -lc -o libnetwork_module.so libnetwork_module.o Thread.o -lpthread 
network : libnetwork_module.o
	(echo "network compiled";)

libnetwork_module.o : $(CORE_DIR)/networking/libnetwork_module.cpp $(CORE_DIR)/networking/libnetwork_module.hh $(CORE_DIR)/threading/Thread.hh $(CORE_DIR)/threading/Thread.cpp
	g++ -g -Wall  -c -fPIC $(CORE_DIR)/networking/libnetwork_module.cpp 

#############################################################




############################################################
#
#	SAMPLE GAMELET COMPILATION
#
#
#
############################################################
# copy game engine api to sample gamelet root to make less complex compilation

sample_gamelet: sample_gamelet.so
	(cp sample_gamelet.so $(GAMELET_DIR))
sample_gamelet.so : sample_gamelet.o libgame_engine_api.so
	(cp libgame_engine_api.so $(GAMELET_DIR)  ;g++ -g -Wall -shared -lc -o sample_gamelet.so sample_gamelet.o -L./ -lgame_engine_api)

sample_gamelet.o : $(GAMELET_DIR)/sample_gamelet.cpp $(CORE_DIR)/gamelet.hh $(CORE_DIR)/game_engine_api.hh 
	g++ -g -Wall -fPIC -c $(GAMELET_DIR)/sample_gamelet.cpp 

############################################################




############################################################
#
#	ENGINE API COMPILATION
#
#
#
############################################################
libgame_engine_api.o : $(CORE_DIR)/libgame_engine_api.cpp $(CORE_DIR)/libgame_engine_api.hh
	g++ -g -Wall -c -fPIC $(CORE_DIR)/libgame_engine_api.cpp

libgame_engine_api.so : libgame_engine_api.o
	g++ -g -Wall -shared -lc -o libgame_engine_api.so libgame_engine_api.o

############################################################
#
#	my sql data layer compile
#
#
#
############################################################

#mySqlClass :  mySqlClass.o 
#	(echo "my sql data layer compiled";cp *.o ..)

#mySqlClass.o : $(MYSQL_DIR)/mySqlClass.cpp $(MYSQL_DIR)/mySqlClass.h
#	g++ -g -Wall -c -fPIC $(MYSQL_DIR)/mySqlClass.cpp -l/usr/local/lib -I/usr/local/include -I/usr/local/include/mysql  