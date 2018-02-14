//
//  app.cpp
//  pastoc
//
//  Created by Alexandr Borzykh on 10/11/2017.
//  Copyright © 2017 HATE|RED. All rights reserved.
//

#include "app.hpp"

#include "parser.hpp"
#include "generator.hpp"

App::App(const std::string& inputPath)
{
	pastoc::Generator::Generate(inputPath, pastoc::Parser::Process(inputPath));
}
