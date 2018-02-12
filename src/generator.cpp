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

		std::cout << "Generation step ... ";

		const std::string outputFileName = inputPath.substr(0, inputPath.size() - 5) + ".c";
		std::string generatedContent;

		using sink_type = std::back_insert_iterator<std::string>;
		sink_type sink(generatedContent);

		const generators::cpp_grammar<sink_type> grammar;
		const bool success = karma::generate(sink, grammar, *ast);

		if (!success)
		{
			std::cout << "failed at some point, abort\n";
			return;
		}

		std::cout << "completed\n";

		std::ofstream outputFile(outputFileName);
		outputFile << generatedContent;

		std::cout << "Transpiled code is saved into: " << outputFileName << std::endl;
	}

}
