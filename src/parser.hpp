//
//  parser.hpp
//  pastoc
//
//  Created by Alexandr Borzykh on 10/11/2017.
//  Copyright © 2017 HATE|RED. All rights reserved.
//

#ifndef parser_hpp
#define parser_hpp

#include "AST.hpp"

#include <boost/optional.hpp>

#include <string>

namespace pastoc
{

	class Parser
	{
	public:
		static boost::optional<ast::pascal_program> Process(const std::string& inputPath);
	};

}

#endif /* parser_hpp */
