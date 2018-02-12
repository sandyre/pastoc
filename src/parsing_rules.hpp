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
			_string_expr %= +(qi::char_("a-zA-Z"));

			_program.name("program");
			_program %= qi::lit("Program") > qi::no_skip[qi::omit[+qi::space]] > _identifier > ';' > _block > '.';

			_block.name("block");
			_block %= _declarations > _compound_statement;

			_declarations.name("declarations");
			_declarations %=
					(qi::no_case[qi::lit("Var")] > qi::no_skip[qi::omit[+qi::space]] > +(_variable_declaration > qi::lit(';'))) |
					_empty;

			_compound_statement.name("compound_statement");
			_compound_statement %= qi::no_case[qi::lit("Begin")] > qi::no_case[qi::lit("End")];

			_variable_declaration.name("variable_declaration");
			_variable_declaration %= (qi::skip(qi::space)[_identifier > *(qi::lit(',') > _identifier) > qi::lit(':')] > _type_specifier);

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
		}

	private:
		qi::symbols<char, std::string>									_reserved_keywords;

		qi::rule<IteratorT, ast::pascal_program(), SkipperT>			_program;
		qi::rule<IteratorT, ast::block(), SkipperT>						_block;
		qi::rule<IteratorT, ast::declarations(), SkipperT>				_declarations;
		qi::rule<IteratorT, ast::variable_declaration(), SkipperT>		_variable_declaration;
		qi::rule<IteratorT, ast::compound_statement(), SkipperT>		_compound_statement;

		qi::rule<IteratorT, ast::empty(), SkipperT>						_empty;
		qi::rule<IteratorT, ast::identifier(), SkipperT>				_identifier;
		qi::rule<IteratorT, ast::variable(), SkipperT>					_variable;
		qi::rule<IteratorT, ast::type_spec(), SkipperT>					_type_specifier;
		qi::rule<IteratorT, ast::rel_operator(), SkipperT>				_rel_operator;
		qi::rule<IteratorT, ast::string_expr(), SkipperT>				_string_expr;
		qi::rule<IteratorT, ast::boolean_expr(), SkipperT>				_boolean_expr;
	};

}}

#endif /* parsing_rules_hpp */
