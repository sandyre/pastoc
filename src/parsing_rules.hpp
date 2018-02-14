//
//  parsing_rules.hpp
//  pastoc
//
//  Created by Alexandr Borzykh on 06/11/2017.
//  Copyright © 2017 HATE|RED. All rights reserved.
//

#ifndef parsing_rules_h
#define parsing_rules_h

#include "AST.hpp"

#include <boost/spirit/include/qi.hpp>

namespace qi = boost::spirit::qi;

namespace pastoc {
namespace parsers
{

	template <typename IteratorT, typename SkipperT>
	class pascal_grammar : public qi::grammar<IteratorT, ast::pascal_program(), SkipperT>
	{
	public:
		pascal_grammar()
			:	pascal_grammar::base_type(_program, "Pascal")
		{
			_reserved_keywords.add
					("absolute", "")
					("and", "")
					("array", "")
					("asm", "")
					("begin", "")
					("break", "")
					("case", "")
					("const", "")
					("constructor", "")
					("continue", "")
					("destructor", "")
					("dispose", "")
					("div", "")
					("do", "")
					("downto", "")
					("else", "")
					("end", "")
					("exit", "")
					("false", "")
					("file", "")
					("for", "")
					("function", "")
					("goto", "")
					("if", "")
					("implementation", "")
					("in", "")
					("inherited", "")
					("inline", "")
					("interface", "")
					("label", "")
					("mod", "")
					("new", "")
					("nil", "")
					("not", "")
					("object", "")
					("of", "")
					("on", "")
					("operator", "")
					("or", "")
					("packed", "")
					("procedure", "")
					("program", "")
					("record", "")
					("repeat", "")
					("self", "")
					("set", "")
					("shl", "")
					("shr", "")
					("string", "")
					("then", "")
					("to", "")
					("true", "")
					("try", "")
					("type", "")
					("unit", "")
					("until", "")
					("uses", "")
					("var", "")
					("while", "")
					("with", "")
					("xor", "");

			_empty.name("empty");
			_empty %= qi::attr(ast::empty());

			_identifier.name("identifier");
			_identifier %= +(qi::char_("a-zA-Z")) - qi::no_case[_reserved_keywords];

			_variable.name("variable");
			_variable %= _identifier;

			_string_expr.name("string_expr");
			_string_expr %= qi::lit("'") > qi::no_skip[*(qi::char_ - qi::lit("'"))] > qi::lit("'");

			_program.name("program");
			_program %= qi::lit("Program") > qi::no_skip[qi::omit[+qi::space]] > _identifier > qi::lit(";") > _block > qi::lit(".");

			_block.name("block");
			_block %= _declarations > _compound_statement;

			_declarations.name("declarations");
			_declarations %=
					(qi::no_case[qi::lit("Var")] > qi::no_skip[qi::omit[+qi::space]] > +(_variable_declaration > qi::lit(';'))) |
					_empty;

			_compound_statement.name("compound_statement");
			_compound_statement %=
					qi::no_case[qi::lit("Begin")] >
					qi::skip(qi::space)[_statement > *(qi::lit(";") > _statement)] >
					qi::no_case[qi::lit("End")];

			_statement.name("statement");
			_statement %=
					_writeln_statement |
					_assignment_statement |
					_empty;

			_assignment_statement.name("assignment_statement");
			_assignment_statement %= _identifier > qi::lit(":=") > _expr;

			_writeln_statement.name("writeln_statement");
			_writeln_statement %=
					qi::no_case[qi::lit("writeln")] > qi::lit("(") >
					(_expr > *(qi::lit(",") > _expr)) > qi::lit(")");

			_expr.name("expr");
			_expr %=
					_simple_expr |
					_string_expr |
					_boolean_expr;

			_variable_declaration.name("variable_declaration");
			_variable_declaration %= (qi::skip(qi::space)[_identifier > *(qi::lit(",") > _identifier) > qi::lit(":")] > _type_specifier);

			_type_specifier.name("type_spec");
			_type_specifier %=
					(qi::no_case[qi::lit("Integer")] > qi::attr(ast::integer())) |
					(qi::no_case[qi::lit("Real")] > qi::attr(ast::real())) |
					(qi::no_case[qi::lit("String")] > qi::attr(ast::string())) |
					(qi::no_case[qi::lit("Boolean")] > qi::attr(ast::boolean()));

			_rel_operator.name("rel_operator");
			_rel_operator %=
					(qi::lit("=") > qi::attr(ast::rel_op_eq())) |
					(qi::lit("<>") > qi::attr(ast::rel_op_ne())) |
					(qi::lit("<") > qi::attr(ast::rel_op_lt())) |
					(qi::lit("<=") > qi::attr(ast::rel_op_lte())) |
					(qi::lit(">") > qi::attr(ast::rel_op_gt())) |
					(qi::lit(">=") > qi::attr(ast::rel_op_gte()));

			_boolean_expr.name("boolean_expr");
			_boolean_expr %=
					(qi::no_case[qi::lit("true")] > qi::attr(ast::bool_true())) |
					(qi::no_case[qi::lit("false")] > qi::attr(ast::bool_false()));

			_simple_expr.name("simple_expr");
			_simple_expr %= _term > *_signed_term;

			_factor.name("factor");
			_factor %=
					qi::int_ |
					qi::double_ |
					_variable |
					_plus_factor |
					_minus_factor;

			_plus_factor.name("plus_factor");
			_plus_factor %= qi::lit("+") > _factor;

			_minus_factor.name("minus_factor");
			_minus_factor %= qi::lit("-") > _factor;

			_sign.name("sign");
			_sign %=
					(qi::lit("+") > qi::attr(ast::plus())) |
					(qi::lit("-") > qi::attr(ast::minus()));

			_factor_oper.name("sign");
			_factor_oper %=
					(qi::lit("*") > qi::attr(ast::mul())) |
					(qi::lit("/") > qi::attr(ast::div()));

			_oper_factor.name("oper_factor");
			_oper_factor %= _factor_oper > _factor;

			_term.name("term");
			_term %= _factor > *_oper_factor;

			_signed_term.name("signed_term");
			_signed_term %= _sign > _term;
		}

	private:
		qi::symbols<char, std::string>									_reserved_keywords;

		qi::rule<IteratorT, ast::pascal_program(), SkipperT>			_program;
		qi::rule<IteratorT, ast::block(), SkipperT>						_block;
		qi::rule<IteratorT, ast::declarations(), SkipperT>				_declarations;
		qi::rule<IteratorT, ast::variable_declaration(), SkipperT>		_variable_declaration;
		qi::rule<IteratorT, ast::statement(), SkipperT>					_statement;
		qi::rule<IteratorT, ast::assignment_statement(), SkipperT>		_assignment_statement;
		qi::rule<IteratorT, ast::compound_statement(), SkipperT>		_compound_statement;
		qi::rule<IteratorT, ast::writeln_statement(), SkipperT>			_writeln_statement;

		qi::rule<IteratorT, ast::empty(), SkipperT>						_empty;
		qi::rule<IteratorT, ast::identifier(), SkipperT>				_identifier;
		qi::rule<IteratorT, ast::variable(), SkipperT>					_variable;
		qi::rule<IteratorT, ast::type_spec(), SkipperT>					_type_specifier;
		qi::rule<IteratorT, ast::rel_operator(), SkipperT>				_rel_operator;
		qi::rule<IteratorT, ast::string_expr(), SkipperT>				_string_expr;
		qi::rule<IteratorT, ast::boolean_expr(), SkipperT>				_boolean_expr;
		qi::rule<IteratorT, ast::simple_expr(), SkipperT>				_simple_expr;
		qi::rule<IteratorT, ast::expr(), SkipperT>						_expr;

		qi::rule<IteratorT, ast::factor(), SkipperT>					_factor;
		qi::rule<IteratorT, ast::plus_factor(), SkipperT>				_plus_factor;
		qi::rule<IteratorT, ast::minus_factor(), SkipperT>				_minus_factor;
		qi::rule<IteratorT, ast::oper_factor(), SkipperT>				_oper_factor;

		qi::rule<IteratorT, ast::sign(), SkipperT>						_sign;
		qi::rule<IteratorT, ast::factor_oper(), SkipperT>				_factor_oper;
		qi::rule<IteratorT, ast::term(), SkipperT>						_term;
		qi::rule<IteratorT, ast::signed_term(), SkipperT>				_signed_term;
	};

}}

#endif /* parsing_rules_hpp */
