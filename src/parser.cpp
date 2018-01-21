//
//  parser.cpp
//  pastoc
//
//  Created by Alexandr Borzykh on 10/11/2017.
//  Copyright © 2017 HATE|RED. All rights reserved.
//

#include "parser.hpp"

#include "AST.hpp"
#include "parsing_rules.hpp"

#include <fstream>
#include <iostream>
#include <streambuf>
#include <vector>

namespace pastoc
{

	boost::optional<ast::PascalProgram> Parser::Process(const std::string& inputPath)
	{
		std::ifstream fileStream(inputPath);
		if (!fileStream.is_open())
			throw std::runtime_error("Input file does not exist");

		const std::string fileContent { std::istreambuf_iterator<char>(fileStream), std::istreambuf_iterator<char>() };
		std::vector<size_t> endlIndexes;
		{
			for (size_t idx = 0; idx < fileContent.size(); ++idx)
				if (fileContent[idx] == '\n')
					endlIndexes.push_back(idx);
		}

		bool success = false;
		ast::PascalProgram program;
		parsers::PascalGrammar<std::string::const_iterator> pascalGrammar;
		std::string::const_iterator curIter = fileContent.begin(), endIter = fileContent.end();
		try
		{ success = qi::phrase_parse(curIter, endIter, pascalGrammar, boost::spirit::qi::space, program); }
		catch (const boost::spirit::qi::expectation_failure<std::string::const_iterator>& ex)
		{
			std::cout << "Expected: " << ex.what_ << std::endl;
			std::cout << "Got: \"" << std::string(ex.first, ex.last) << "\"" << std::endl;
			return boost::optional<ast::PascalProgram>();
		}

		if (!success && curIter != endIter)
		{
			const size_t symbolIdx = std::distance(fileContent.begin(), curIter);
			const auto lineIter = std::find_if(endlIndexes.begin(), endlIndexes.end(),
											[&](size_t idx)
											{
												return idx > symbolIdx;
											});
			const size_t lineIdx = lineIter != endlIndexes.end() ? std::distance(endlIndexes.begin(), lineIter) : 0;

			std::cout << "Syntax error at line: " << lineIdx << " symbol: " << symbolIdx << std::endl;
			return boost::optional<ast::PascalProgram>();
		}
		else
		{
			std::cout << "Parsing phase ended gracefully" << std::endl;

			std::cout << "AST constructed, printing out" << std::endl << std::endl;
			std::cout << "Program name: " << program.Name << std::endl;
			std::cout << "Block:" << std::endl;
			std::cout << "Variables:" << std::endl;
			for (const auto& decl : program.Block.Decls.Decls)
				std::cout << decl << std::endl;

			return program;
		}
	}

}
