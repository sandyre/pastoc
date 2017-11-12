//
//  AST.hpp
//  pastoc
//
//  Created by Alexandr Borzykh on 09/11/2017.
//  Copyright © 2017 HATE|RED. All rights reserved.
//

#ifndef AST_hpp
#define AST_hpp

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/include/qi.hpp>

#include <string>
#include <vector>

namespace pastoc {
namespace ast
{

	struct VariableDeclaration
	{
		std::vector<std::string>	Vars;
		std::string					Type;
	};

	struct Declarations
	{
		std::vector<VariableDeclaration>	Decls;
	};

	struct Block
	{
		Declarations	Decls;
	};

	struct PascalProgram
	{
		std::string		Name;
		Block			Block;
	};

}}


BOOST_FUSION_ADAPT_STRUCT(
	pastoc::ast::VariableDeclaration,
	(std::vector<std::string>, Vars),
	(std::string, Type)
)

BOOST_FUSION_ADAPT_STRUCT(
	pastoc::ast::Declarations,
	(std::vector<pastoc::ast::VariableDeclaration>, Decls)
)

BOOST_FUSION_ADAPT_STRUCT(
	pastoc::ast::Block,
	(pastoc::ast::Declarations, Decls)
)

BOOST_FUSION_ADAPT_STRUCT(
	pastoc::ast::PascalProgram,
	(std::string, Name),
	(pastoc::ast::Block, Block)
)

inline std::ostream& operator<<(std::ostream& stream, const pastoc::ast::VariableDeclaration& decl)
{
	for (const auto& varName : decl.Vars)
		stream << varName << " -> " << decl.Type << std::endl;
	return stream;
}

#endif /* AST_hpp */
