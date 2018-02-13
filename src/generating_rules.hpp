//
//  generating_rules.hpp
//  pastoc
//
//  Created by Alexandr Borzykh on 22/11/2017.
//  Copyright © 2017 HATE|RED. All rights reserved.
//

#ifndef generating_rules_h
#define generating_rules_h

#include "AST.hpp"

#include <iostream>

namespace pastoc {
namespace generators
{

	namespace Detail
	{

		struct type_spec_visitor : boost::static_visitor<std::string>
		{
			std::string operator()(ast::integer) const	{ return "int"; }
			std::string operator()(ast::real) const 	{ return "double"; }
			std::string operator()(ast::string) const	{ return "std::string"; }
			std::string operator()(ast::boolean) const	{ return "bool"; }
		};


		struct declarations_visitor : boost::static_visitor<std::string>
		{
			std::string operator()(ast::empty) const	{ return ""; }

			std::string operator()(const std::vector<ast::variable_declaration>& decls) const
			{
				std::ostringstream ss;
				for (const ast::variable_declaration& decl : decls)
				{
					ss << boost::apply_visitor(type_spec_visitor(), decl.type) << " ";
					for (std::vector<ast::identifier>::const_iterator iter = decl.ids.begin(); iter != decl.ids.end(); ++iter)
					{
						if (iter == decl.ids.end() - 1)
							ss << *iter;
						else
							ss << *iter << ", ";
					}

					ss << ";\n";
				}

				return ss.str();
			}
		};


		struct expr_visitor : boost::static_visitor<std::string>
		{
			struct boolean_expr_visitor : boost::static_visitor<std::string>
			{
				std::string operator()(ast::bool_true) const	{ return "true"; }
				std::string operator()(ast::bool_false) const	{ return "false"; }
			};

			std::string operator()(const ast::boolean_expr& expr) const { return boost::apply_visitor(boolean_expr_visitor(), expr); }
			std::string operator()(const ast::string_expr& expr) const	{ return "\"" + expr + "\""; }
		};


		struct statement_visitor : boost::static_visitor<std::string>
		{
			std::string operator()(ast::empty) const	{ return ""; }

			std::string operator()(const ast::assignment_statement& statement) const
			{
				std::ostringstream ss;
				ss << statement.var << " = " << boost::apply_visitor(expr_visitor(), statement.expr) << ";";
				return ss.str();
			}
		};

	}


	std::ostream& operator<<(std::ostream& os, ast::pascal_program const& program);
	std::ostream& operator<<(std::ostream& os, ast::block const& block);
	std::ostream& operator<<(std::ostream& os, ast::declarations const& decls);
	std::ostream& operator<<(std::ostream& os, ast::compound_statement const& comp_statement);


	std::ostream& operator<<(std::ostream& os, ast::pascal_program const& program)
	{
		os << "// This file is generated from pascal program named " << program.name << "\n";
		os << "// Default include directives\n";
		os << "#include <iostream>\n";
		os << "#include <string>\n\n";
		os << "int main(int argc, char* argv[])\n";
		os << program.main_block;
		return os;
	}


	std::ostream& operator<<(std::ostream& os, ast::block const& block)
	{
		os << "{\n";
		os << block.decls;
		os << "\n";
		os << block.statement;
		os << "}\n";
		return os;
	}


	std::ostream& operator<<(std::ostream& os, ast::declarations const& decls)
	{
		os << boost::apply_visitor(Detail::declarations_visitor(), decls);
		return os;
	}


	std::ostream& operator<<(std::ostream& os, ast::compound_statement const& comp_statement)
	{
		for (const ast::statement& statement : comp_statement.statement_list)
			os << boost::apply_visitor(Detail::statement_visitor(), statement) << "\n";
		return os;
	}

}}

#endif /* generating_rules_h */
