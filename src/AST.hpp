//
//  AST.hpp
//  pastoc
//
//  Created by Alexandr Borzykh on 09/11/2017.
//  Copyright © 2017 HATE|RED. All rights reserved.
//

#ifndef AST_hpp
#define AST_hpp

#include <boost/variant.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/include/qi.hpp>

#include <string>
#include <list>
#include <tuple>
#include <vector>

namespace pastoc {
namespace ast
{

	struct empty { };


	struct rel_op_eq { };
	struct rel_op_ne { };
	struct rel_op_lt { };
	struct rel_op_lte { };
	struct rel_op_gt { };
	struct rel_op_gte { };

	using rel_operator = boost::variant<
		rel_op_eq,
		rel_op_ne,
		rel_op_lt,
		rel_op_lte,
		rel_op_gt,
		rel_op_gte
	>;


	struct integer { };
	struct real { };
	struct string { };
	struct boolean { };

	using type_spec = boost::variant<
		integer,
		real,
		string,
		boolean
	>;


	struct bool_true { };
	struct bool_false { };

	using boolean_expr = boost::variant<
		bool_true,
		bool_false
	>;


	using identifier = std::string;


	using variable = identifier;


	using string_expr = std::string;


	struct plus_factor_;
	struct minus_factor_;

	using factor = boost::variant<
		int,
		double,
		variable,
		boost::recursive_wrapper<plus_factor_>,
		boost::recursive_wrapper<minus_factor_>
	>;

	struct plus_factor_
	{
		factor		fact;
	};

	struct minus_factor_
	{
		factor		fact;
	};


	struct mul { };
	struct div { };

	using oper_factor = std::pair<boost::variant<mul, div>, factor>;

	struct term
	{
		factor					head_factor;
		std::list<oper_factor>	tail_factors;
	};


	struct plus { };
	struct minus { };

	using signed_term = std::pair<boost::variant<plus, minus>, term>;

	struct simple_expr
	{
		term						head_term;
		std::list<signed_term>		tail_terms;
	};


	struct simple_rel_operator_simple_expr
	{
		simple_expr		first;
		rel_operator	rel_op;
		simple_expr		second;
	};

	using expr = boost::variant<
		simple_expr,
		simple_rel_operator_simple_expr,
		boolean_expr,
		string_expr
	>;


	struct assignment_statement
	{
		variable	var;
		expr		expr;
	};

	using writeln_statement = std::list<expr>;

	struct conditional_statement;
	struct compound_statement;
	struct for_loop_statement;

	using statement = boost::variant<
		boost::recursive_wrapper<conditional_statement>,
		boost::recursive_wrapper<compound_statement>,
		boost::recursive_wrapper<for_loop_statement>,
		assignment_statement,
		writeln_statement,
		empty
	>;

	using statement_list = std::list<statement>;

	struct conditional_statement
	{
		expr								if_expr;
		statement							then_statement;
		boost::variant<statement, empty>	else_statement;
	};

	struct compound_statement
	{
		statement_list			statement_list;
	};

	struct for_loop_statement
	{
		assignment_statement	from_expr;
		expr					to_expr;
		statement				do_statement;
	};


	struct variable_declaration
	{
		std::list<identifier>	ids;
		type_spec				type;
	};


	using declarations = boost::variant<
		std::list<variable_declaration>,
		empty
	>;


	struct block
	{
		declarations		decls;
		compound_statement	statement;
	};


	struct pascal_program
	{
		variable	name;
		block		main_block;
	};

}}

BOOST_FUSION_ADAPT_STRUCT(
						  pastoc::ast::variable_declaration,
						  (std::list<pastoc::ast::identifier>, ids),
						  (pastoc::ast::type_spec, type)
						  )

BOOST_FUSION_ADAPT_STRUCT(
						  pastoc::ast::block,
						  (pastoc::ast::declarations, decls),
						  (pastoc::ast::compound_statement, statement)
						  )

BOOST_FUSION_ADAPT_STRUCT(
						  pastoc::ast::pascal_program,
						  (pastoc::ast::variable, name),
						  (pastoc::ast::block, main_block)
						  )

#endif /* AST_hpp */
