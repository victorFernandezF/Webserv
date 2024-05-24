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


	if (argc != 2)
		return (0);
	try
	{
		Cluster cluster;
		Config test(argv[1], cluster);
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
