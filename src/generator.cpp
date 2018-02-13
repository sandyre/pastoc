//
//  generator.cpp
//  pastoc
//
//  Created by Alexandr Borzykh on 22/11/2017.
//  Copyright © 2017 HATE|RED. All rights reserved.
//

#include "generator.hpp"

#include "generating_rules.hpp"

#include <fstream>
#include <iostream>

namespace pastoc
{
	void Generator::Generate(const std::string& inputPath, boost::optional<pastoc::ast::pascal_program> ast)
	{
		if (!ast)
		{
			std::cout << "Generation step failed to start (parser returned null-AST)\n";
			return;
		}

		std::cout << "Generation step is in process ... ";

		const std::string outputFileName = inputPath.substr(0, inputPath.size() - 5) + ".c";
		std::string generatedContent;

		try
		{
			using namespace generators;
			std::ofstream outputFile(outputFileName);
			outputFile << ast.get();
		}
		catch (const std::exception& ex)
		{
			std::cout << " failed, exception thrown: " << ex.what() << std::endl;
			return;
		}

		std::cout << "done\nTranspiled code is saved into: " << outputFileName << std::endl;
	}

}
