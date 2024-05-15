#include <iostream>

#include "Cluster.hpp"
#include "Config.hpp"

int	main(int argc, char **argv)
{
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
