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


		template <typename IteratorT>
		class TypeSpecifierGrammar : public qi::grammar<IteratorT, std::string()>
		{
		public:
			TypeSpecifierGrammar()
				:	TypeSpecifierGrammar::base_type(_rule, "TypeSpecifier")
			{
				_rule %= qi::lit("Boolean") | qi::lit("Integer") | qi::lit("Real") | qi::lit("String");
			}

		private:
			qi::rule<IteratorT, std::string()>	_rule;
		};


		template <typename IteratorT>
		class VariableDeclarationGrammar : public qi::grammar<IteratorT>
		{
		public:
			VariableDeclarationGrammar()
				:	VariableDeclarationGrammar::base_type(_rule, "VariableDeclaration")
			{
				_rule %= _identifierGrammar >> *(',' > _identifierGrammar) >> ':'
						>> _typeSpecifierGrammar;
			}

		private:
			qi::rule<IteratorT>					_rule;
			IdentifierGrammar<IteratorT>		_identifierGrammar;
			TypeSpecifierGrammar<IteratorT>		_typeSpecifierGrammar;
		};


		template <typename IteratorT>
		class DeclarationsGrammar : public qi::grammar<IteratorT>
		{
		public:
			DeclarationsGrammar()
				:	DeclarationsGrammar::base_type(_rule, "Declarations")
			{
				_rule %= qi::lit("Var ") >> +(_variableDeclarationGrammar >> ';');
			}

		private:
			qi::rule<IteratorT>						_rule;
			VariableDeclarationGrammar<IteratorT>	_variableDeclarationGrammar;
		};


		template <typename IteratorT>
		class BlockGrammar : public qi::grammar<IteratorT>
		{
		public:
			BlockGrammar()
				:	BlockGrammar::base_type(_rule, "Block")
			{
				_rule %= _declarationsGrammar;
			}

		private:
			qi::rule<IteratorT>						_rule;
			DeclarationsGrammar<IteratorT>			_declarationsGrammar;
		};

	}


	template <typename IteratorT>
	class PascalGrammar : public qi::grammar<IteratorT>
	{
	public:
		PascalGrammar()
			:	PascalGrammar::base_type(_rule, "Pascal")
		{
			_rule %= _programNameGrammar >> _blockGrammar >> '.';
		}

	private:
		qi::rule<IteratorT>							_rule;
		detail::ProgramNameGrammar<IteratorT>		_programNameGrammar;
		detail::BlockGrammar<IteratorT>				_blockGrammar;
	};

}}

#endif /* parsing_rules_hpp */
