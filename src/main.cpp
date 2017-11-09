//
//  main.cpp
//  pastoc
//
//  Created by Alexandr Borzykh on 06/11/2017.
//  Copyright © 2017 HATE|RED. All rights reserved.
//

#include "parsing_rules.hpp"
#include "app.hpp"

#include <fstream>
#include <iostream>
#include <streambuf>
#include <string>
#include <vector>

void print_help()
{
	std::cout << "PasToC transpiler. Synapse: PATH" << std::endl;
}

int main(int argc, const char * argv[])
{
	if (argc != 2)
	{
		print_help();
		return 1;
	}

	App app(argv[1]);
	return 0;
}
