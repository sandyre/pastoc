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

	void Generator::Generate(const std::string& inputPath, boost::optional<pastoc::ast::PascalProgram> ast)
	{
		if (!ast)
		{
			std::cout << "Generation step failed to start (parser returned null-AST)\n";
			return;
		}

		std::cout << "Generation step ...";

		const std::string outputFileName = inputPath.substr(0, inputPath.size() - 5) + ".c";
		std::string generatedContent;

		using sink_type = std::back_insert_iterator<std::string>;
		sink_type sink(generatedContent);

		const generators::PascalGrammar<sink_type> grammar;
		try { karma::generate(sink, grammar, *ast); }
		catch (const std::exception& ex)
		{
			std::cout << "Failed\n\t";
			std::cout << "Reason: " << ex.what();
			return;
		}

		std::cout << " Completed\n";

		std::ofstream outputFile(outputFileName);
		outputFile << generatedContent;

		std::cout << "Transpiled code is saved into file named " << outputFileName << std::endl;
	}

}
