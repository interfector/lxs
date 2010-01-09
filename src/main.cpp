#include <iostream>
#include <cstring>
#include <lxs.h>

int main(int argc,char *argv[])
{
	int err = 0;
	SymClass test;

	if(!argv[1])
	{
		test.stdin_read();
		
		std::cout << "\n";

	} else if (!strcmp(argv[1],"-s")) {
		if (!argv[2])
		{
			std::cout << "Error bad usage, type -h for help." << std::endl;

			return 1;
		}

		err = test.readfile(argv[2]);

		if( err )
		{
			std::cout << "Error reading file." << std::endl;

			return 1;
		}

	} else if (!strcmp(argv[1],"-x")) {
		if (!argv[2])
		{
			std::cout << "Error bad usage, type -h for help." << std::endl;

			return 1;
		}

		err = test.read_bin(argv[2]);

		if ( err == 1 )
		{
			std::cout << "Error reading file." << std::endl;

			return 1;
		} else if ( err == 2 ) {
			std::cout << "Not a valid LXS file." << std::endl;

			return 1;
		}

	} else if (!strcmp(argv[1],"-c")) {
		if (!argv[2] || !argv[3]) 
		{
			std::cout << "Error bad usage, type -h for help." << std::endl;

			return 1;
		}

		if(!test.pseudo_compile(argv[2],argv[3]))
		{
			std::cout << "Compilation complete!" << std::endl;
		} else {
			std::cout << "Error reading file." << std::endl;

			return 1;
		}

		return 0;
	} else if (!strcmp(argv[1],"-h")) {
		std::cout << "Usage: " << argv[0] << " [-s] [-x] [-c]\n\t-s\tSelect the source file.\n";
		std::cout << "\t-x\tSelect the binary source file.\n\t-c\tCompile from plain text source to binary source file.\n";
		
		return 0;
	} else {
		std::cout << "Error bad usage, type -h for help." << std::endl;

		return 1;
	}

	std::cout << "====== EXECUTING CODE ======\n\n";

	test.execute();

	std::cout << "\n====== END EXECUTING ======\n";
	
	test.dump();

	return 0;
}
