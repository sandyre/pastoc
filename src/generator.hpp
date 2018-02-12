//
//  generator.hpp
//  pastoc
//
//  Created by Alexandr Borzykh on 22/11/2017.
//  Copyright © 2017 HATE|RED. All rights reserved.
//

#ifndef generator_hpp
#define generator_hpp

#include "AST.hpp"

#include <boost/optional.hpp>

#include <string>

namespace pastoc
{

	class Generator
	{
	public:
		static void Generate(const std::string& inputPath, boost::optional<pastoc::ast::pascal_program> ast);
	};

}

#endif /* generator_hpp */
