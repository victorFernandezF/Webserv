#include <iostream>
#include <signal.h>

#include "Cluster.hpp"
#include "Config.hpp"

/*void handler(int s)
{

}*/


int	main(int argc, char **argv)
{
	/*struct sigaction sigIntHandler;

   sigIntHandler.sa_handler = handler;
   sigemptyset(&sigIntHandler.sa_mask);
   sigIntHandler.sa_flags = 0;

   sigaction(SIGINT, &sigIntHandler, NULL);*/

	std::string file;

	if (argc > 2)
		return (0);
	if (argc == 1)
		file = "./inc/default.conf";
	else
		file = argv[1];
	try
	{
		Cluster cluster;
		Config test(file, cluster);
		std::cout << cluster << std::endl;
		cluster.runServers();
		cluster._closeFDs();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
	return (0);
}
