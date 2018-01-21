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
				_keywords.add
						("Begin", "")
						("End", "");

				_rule.name("Identifier");
				_rule %= +(qi::char_("a-zA-Z") - _keywords);
			}

		private:
			qi::symbols<char, std::string>			_keywords;
			qi::rule<IteratorT, std::string()>		_rule;
		};


		template <typename IteratorT>
		class VariableGrammar : public qi::grammar<IteratorT, std::string()>
		{
		public:
			VariableGrammar()
				:	VariableGrammar::base_type(_rule, "Variable")
			{
				_rule.name("Variable");
				_rule %= _identifierGrammar;
			}

		private:
			qi::rule<IteratorT, std::string()>		_rule;
			IdentifierGrammar<IteratorT>			_identifierGrammar;
		};


		template <typename IteratorT>
		class ProgramNameGrammar : public qi::grammar<IteratorT, std::string()>
		{
		public:
			ProgramNameGrammar()
				:	ProgramNameGrammar::base_type(_rule, "ProgramName")
			{
				_rule.name("ProgramName");
				_rule %= qi::skip(qi::space)[qi::lit("Program") > qi::no_skip[qi::omit[+qi::space]] > _variableGrammar > ';'];
			}

		private:
			qi::rule<IteratorT, std::string()>			_rule;
			VariableGrammar<IteratorT>					_variableGrammar;
		};


		template <typename IteratorT>
		class TypeSpecifierGrammar : public qi::grammar<IteratorT, std::string()>
		{
		public:
			TypeSpecifierGrammar()
				:	TypeSpecifierGrammar::base_type(_rule, "TypeSpecifier")
			{
				_types.add
						("Boolean", "Boolean")
						("Integer", "Integer")
						("Real", "Real")
						("String", "String");

				_rule.name("TypeSpecifier");
				_rule %= _types;
			}

		private:
			qi::symbols<char, std::string>		_types;
			qi::rule<IteratorT, std::string()>	_rule;
		};


		template <typename IteratorT>
		class VariableDeclarationGrammar : public qi::grammar<IteratorT, ast::VariableDeclaration()>
		{
		public:
			VariableDeclarationGrammar()
				:	VariableDeclarationGrammar::base_type(_rule, "VariableDeclaration")
			{
				_rule.name("VariableDeclaration");
				_idsRule %= qi::skip(qi::space)[_identifierGrammar > *(qi::lit(',') > _identifierGrammar) > qi::lit(':')];
				_rule %= qi::skip(qi::space)[_idsRule > _typeSpecifierGrammar];
			}

		private:
			qi::rule<IteratorT, ast::VariableDeclaration()>		_rule;
			qi::rule<IteratorT, std::vector<std::string>()>		_idsRule;
			IdentifierGrammar<IteratorT>						_identifierGrammar;
			TypeSpecifierGrammar<IteratorT>						_typeSpecifierGrammar;
		};


		template <typename IteratorT>
		class DeclarationsGrammar : public qi::grammar<IteratorT, ast::Declarations()>
		{
		public:
			DeclarationsGrammar()
				:	DeclarationsGrammar::base_type(_rule, "Declarations")
			{
				_rule.name("Declarations");
				_rule %= qi::skip(qi::space)[qi::lit("Var") > qi::no_skip[qi::omit[+qi::space]] > +(_variableDeclarationGrammar > qi::lit(';'))];
			}

		private:
			qi::rule<IteratorT, ast::Declarations()>	_rule;
			VariableDeclarationGrammar<IteratorT>		_variableDeclarationGrammar;
		};


		template <typename IteratorT>
		class StatementGrammar : public qi::grammar<IteratorT>
		{
		public:
			StatementGrammar()
				:	StatementGrammar::base_type(_rule, "Statement")
			{
				_rule.name("Statement");
				_rule %= qi::lit("statement");
			}

		private:
			qi::rule<IteratorT>				_rule;
		};


		template <typename IteratorT>
		class StatementListGrammar : public qi::grammar<IteratorT>
		{
		public:
			StatementListGrammar()
				:	StatementListGrammar::base_type(_rule, "StatementList")
			{
				_rule.name("StatementList");
				_rule %= qi::skip(qi::space)[_statementGrammar >> -(_statementGrammar > ';' > _rule)];
			}

		private:
			qi::rule<IteratorT>				_rule;
			StatementGrammar<IteratorT>		_statementGrammar;
		};


		template <typename IteratorT>
		class CompoundStatementGrammar : public qi::grammar<IteratorT>
		{
		public:
			CompoundStatementGrammar()
				:	CompoundStatementGrammar::base_type(_rule, "CompoundStatement")
			{
				_rule.name("CompoundStatement");
				_rule %= qi::skip(qi::space)[qi::lit("Begin") >
					qi::no_skip[qi::omit[+qi::space]] >
					/* _statementListGrammar  > */
					qi::lit("End")];
			};

		private:
			qi::rule<IteratorT>					_rule;
			StatementListGrammar<IteratorT>		_statementListGrammar;
		};


		template <typename IteratorT>
		class BlockGrammar : public qi::grammar<IteratorT, ast::Block()>
		{
		public:
			BlockGrammar()
				:	BlockGrammar::base_type(_rule, "Block")
			{
				_rule.name("Block");
				_rule %= qi::skip(qi::space)[_declarationsGrammar > qi::no_skip[qi::omit[+qi::space]] > _compoundStatementGrammar];
			}

		private:
			qi::rule<IteratorT, ast::Block()>		_rule;
			DeclarationsGrammar<IteratorT>			_declarationsGrammar;
			CompoundStatementGrammar<IteratorT>		_compoundStatementGrammar;
		};

	}


	template <typename IteratorT>
	class PascalGrammar : public qi::grammar<IteratorT, ast::PascalProgram()>
	{
	public:
		PascalGrammar()
			:	PascalGrammar::base_type(_rule, "Pascal")
		{
			_rule %= qi::skip(qi::space)[_programNameGrammar > _blockGrammar > '.'];
		}

	private:
		qi::rule<IteratorT, ast::PascalProgram()>	_rule;
		detail::ProgramNameGrammar<IteratorT>		_programNameGrammar;
		detail::BlockGrammar<IteratorT>				_blockGrammar;
	};

}}

#endif /* parsing_rules_hpp */
