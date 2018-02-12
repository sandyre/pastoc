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

#include <boost/spirit/include/karma.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/phoenix/bind/bind_member_variable.hpp>
#include <boost/bind.hpp>

namespace karma = boost::spirit::karma;
namespace phoenix = boost::phoenix;

namespace pastoc {
namespace generators
{

	template <typename OutputIterator>
	class cpp_grammar : public karma::grammar<OutputIterator, ast::pascal_program()>
	{
	public:
		cpp_grammar()
			:	cpp_grammar::base_type(_program)
		{
			_identifier.name("identifier");
			_identifier %= karma::string;

			_program.name("program");
			_program %=
					"// Program name: " << _identifier << karma::eol <<
					"int main(int argc, char* argv[])" << karma::eol <<
					_block << karma::eol;

			_block.name("block");
			_block %=
					"{" << karma::eol <<
					_declarations << karma::eol <<
					karma::skip[_compound_statement] <<
					"}" << karma::eol;

			_integer_type %= "int" << karma::omit[karma::eol];
			_real_type %= "double" << karma::omit[karma::eol];
			_string_type %= "std::string" << karma::omit[karma::eol];
			_boolean_type %= "bool" << karma::omit[karma::eol];

			_type_specifier.name("type_specifier");
			_type_specifier %=
					_integer_type |
					_real_type |
					_string_type |
					_boolean_type;

			_declarations.name("declarations");
			_declarations %= (_type_specifier[karma::_1 = phoenix::bind(&ast::declarations::type, karma::_val)] << karma::omit[karma::eol]) |
					_empty;

			_empty %= karma::omit[karma::eol];
		}

	private:
		karma::rule<OutputIterator, ast::pascal_program()>			_program;
		karma::rule<OutputIterator, ast::identifier()>				_identifier;
		karma::rule<OutputIterator, ast::block()>					_block;
		karma::rule<OutputIterator, ast::declarations()>			_declarations;
		karma::rule<OutputIterator, ast::compound_statement()>		_compound_statement;

		karma::rule<OutputIterator, ast::empty()>					_empty;

		karma::rule<OutputIterator, ast::type_spec()>				_type_specifier;
		karma::rule<OutputIterator, ast::integer()>					_integer_type;
		karma::rule<OutputIterator, ast::real()>					_real_type;
		karma::rule<OutputIterator, ast::string()>					_string_type;
		karma::rule<OutputIterator, ast::boolean()>					_boolean_type;
	};

}}

#endif /* generating_rules_h */
