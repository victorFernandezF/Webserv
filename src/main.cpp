#include <iostream>

#include "Cluster.hpp"
#include "Config.hpp"


int	main(int argc, char **argv)
{
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
		cluster.runServers();
		cluster._closeFDs();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
	return (0);
}
