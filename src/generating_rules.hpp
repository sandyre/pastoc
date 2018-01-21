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

namespace karma = boost::spirit::karma;

namespace pastoc {
namespace generators
{

	namespace detail
	{

		template <typename IteratorT>
		class ProgramNameGrammar : public karma::grammar<IteratorT, std::string()>
		{
		public:
			ProgramNameGrammar()
				:	ProgramNameGrammar::base_type(_rule)
			{
				_rule = karma::lit("// Program name: ") << karma::eol;
				 //_rule = karma::lit("// Program name: ") << karma::string << karma::eol;
			}

		private:
			karma::rule<IteratorT, std::string()>		_rule;
		};


		template <typename IteratorT>
		class BlockGrammar : public karma::grammar<IteratorT, ast::Block()>
		{
		public:
			BlockGrammar()
				:	BlockGrammar::base_type(_rule)
			{
				_rule = karma::lit("BLOCK");
			}

		private:
			karma::rule<IteratorT, ast::Block()>		_rule;
		};

	}


	template <typename IteratorT>
	class PascalGrammar : public karma::grammar<IteratorT, ast::PascalProgram()>
	{
	public:
		PascalGrammar()
			:	PascalGrammar::base_type(_rule)
		{
			_rule = _programNameGrammar << karma::omit;
		}

	private:
		karma::rule<IteratorT, ast::PascalProgram()>	_rule;
		detail::ProgramNameGrammar<IteratorT>			_programNameGrammar;
	};

}}

#endif /* generating_rules_h */
