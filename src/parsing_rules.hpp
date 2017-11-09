//
//  parsing_rules.hpp
//  pastoc
//
//  Created by Alexandr Borzykh on 06/11/2017.
//  Copyright © 2017 HATE|RED. All rights reserved.
//

#ifndef parsing_rules_h
#define parsing_rules_h

#include <boost/spirit/include/qi.hpp>

#include <string>

namespace qi = boost::spirit::qi;

namespace pastoc {
namespace parsers
{

	namespace detail
	{

		template <typename IteratorT>
		class IdentifierGrammar : public qi::grammar<IteratorT, std::string()>
		{
		public:
			IdentifierGrammar()
				:	IdentifierGrammar::base_type(_rule, "Identifier")
			{
				_rule %= +(qi::char_("a-zA-Z"));
			}

		private:
			qi::rule<IteratorT, std::string()>		_rule;
		};


		template <typename IteratorT>
		class VariableGrammar : public qi::grammar<IteratorT, std::string()>
		{
		public:
			VariableGrammar()
				:	VariableGrammar::base_type(_rule, "Variable")
			{
				_rule %= _identifierGrammar;
			}

		private:
			qi::rule<IteratorT, std::string()>		_rule;
			IdentifierGrammar<IteratorT>			_identifierGrammar;
		};


		template <typename IteratorT>
		class ProgramNameGrammar : public qi::grammar<IteratorT>
		{
		public:
			ProgramNameGrammar()
				:	ProgramNameGrammar::base_type(_rule, "ProgramName")
			{
				_rule %= qi::lit("Program ") > _variableGrammar > ';';
			}

		private:
			qi::rule<IteratorT>			_rule;
			VariableGrammar<IteratorT>	_variableGrammar;
		};

	}

}}

#endif /* parsing_rules_hpp */
