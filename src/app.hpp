//
//  app.hpp
//  pastoc
//
//  Created by Alexandr Borzykh on 10/11/2017.
//  Copyright © 2017 HATE|RED. All rights reserved.
//

#ifndef app_hpp
#define app_hpp

#include <string>

#include "parser.hpp"
#include "generator.hpp"

class App
{
public:
	App(const std::string& inputPath)
	{
		pastoc::Generator::Generate(inputPath, pastoc::Parser::Process(inputPath));
	}
};

#endif /* app_hpp */
