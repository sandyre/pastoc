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

	boost::optional<ast::pascal_program> Parser::Process(const std::string& inputPath)
	{
		std::ifstream fileStream(inputPath);
		if (!fileStream.is_open())
			throw std::runtime_error("Input file does not exist");

		const std::string fileContent { std::istreambuf_iterator<char>(fileStream), std::istreambuf_iterator<char>() };

		ast::pascal_program program;
		parsers::pascal_grammar<std::string::const_iterator, qi::space_type> pascal_grammar;
		std::string::const_iterator curIter = fileContent.begin(), endIter = fileContent.end();
		try
		{ qi::phrase_parse(curIter, endIter, pascal_grammar, boost::spirit::qi::space, program); }
		catch (const boost::spirit::qi::expectation_failure<std::string::const_iterator>& ex)
		{
			std::string::const_iterator scanIter = fileContent.begin();
			size_t lineIdx = 1;
			size_t symbolIdx = 1;
			while (scanIter != ex.first)
			{
				if (*scanIter == '\n')
				{
					++lineIdx;
					symbolIdx = 1;
				}
				else
					++symbolIdx;

				++scanIter;
			}

			std::cout << "Syntax error at line: " << lineIdx << " symbol: " << symbolIdx << std::endl;
			std::cout << "Expected: " << ex.what_ << std::endl;
			std::cout << "Got: \"" << std::string(ex.first, ex.last) << "\"" << std::endl;
			return boost::optional<ast::pascal_program>();
		}

		std::cout << "Parsing phase ended gracefully" << std::endl;
		std::cout << "Program name: " << program.name << std::endl;
		return program;
	}

}
