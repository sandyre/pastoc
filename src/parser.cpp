//
//  parser.cpp
//  pastoc
//
//  Created by Alexandr Borzykh on 10/11/2017.
//  Copyright © 2017 HATE|RED. All rights reserved.
//

#include "parser.hpp"

#include "parsing_rules.hpp"

#include <fstream>
#include <iostream>
#include <streambuf>
#include <vector>

namespace pastoc
{

	void Parser::Process(const std::string& inputPath)
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

		parsers::PascalGrammar<std::string::const_iterator>	pascalGrammar;
		std::string::const_iterator curIter = fileContent.begin(), endIter = fileContent.end();
		try
		{
			qi::phrase_parse(curIter, endIter, pascalGrammar, boost::spirit::ascii::space);
		}
		catch (const std::exception& ex)
		{
			std::cout << "Exception thrown at parsing phase: " << ex.what() << std::endl;
		}

		if (curIter != endIter)
		{
			const size_t symbolIdx = std::distance(fileContent.begin(), curIter);
			const auto lineIter = std::find_if(endlIndexes.begin(), endlIndexes.end(),
											   [&](size_t idx)
											   {
												   return idx > symbolIdx;
											   });
			const size_t lineIdx = lineIter != endlIndexes.end() ? std::distance(endlIndexes.begin(), lineIter) : 0;

			std::cout << "Syntax error at line: " << lineIdx << " symbol: " << symbolIdx << std::endl;
		}
		else
		{
			std::cout << "Parsing phase ended gracefully" << std::endl;
		}
	}

}
