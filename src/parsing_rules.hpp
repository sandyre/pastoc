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

	template <typename IteratorT = std::string::iterator, typename SkipperT>
	class PascalGrammar : public qi::grammar<Iterator, ast::Object(), SkipperT>
	{
	public:
		PascalGrammar()
			:	PascalGrammar::base_type(_objectRule, "Object")
		{
			// whole program structure
			_programRule %= _programNameRule > _blockRule > '.';

			_programNameRule %= qi::lit("Program ") > _variableRule;

			_blockRule %= _declarationListRule > _compoundStatementRule;

			_declarationsRule %= -(qi::lit("Var ") > +_variableDeclarationRule);

			_variableDeclarationRule %= _identifierRule >> *(',' >> _identifierRule) > ':' > _typeSpecifierRule;

			_typeSpecifierRule %= qi::lit("Integer") | qi::lit("Real") | qi::lit("String") | qi::lit("Boolean");

			_compoundStatementRule %= qi::lit("Begin ") > _statementListRule > qi::lit("End");

			_statementListRule %= _statementRule | (_statementRule > ';' > _statementListRule));

			_statementRule %= -(_conditionalStatement | _writeLnStatementRule | _forLoopStatementRule | _assignmentStatementRule | _compoundStatementRule);

			_writeLnStatementRule %= qi::lit("writeln") > '(' >> _expressionRule >> *(_expressionRule > ',') > ')';

		}

	private:
		qi::rule<IteratorT, ast::Object(), SkipperT>				_objectRule;
		qi::rule<IteratorT, ast::Program(), SkipperT>				_programRule;
		qi::rule<IteratorT, ast::ProgramName(), SkipperT>			_programNameRule;
		qi::rule<IteratorT, ast::Block(), SkipperT>					_blockRule;
		qi::rule<IteratorT, ast::Declarations(), SkipperT>			_declarationsRule;
		qi::rule<IteratorT, ast::VariableDeclaration(), SkipperT>	_variableDeclarationRule;

		qi::rule<IteratorT, ast::CompoundStatement(), SkipperT>		_compoundStatementRule;
		qi::rule<IteratorT, ast::Statement(), SkipperT>				_statementRule;
		qi::rule<IteratorT, ast::StatementList(), SkipperT>			_statementListRule;
		qi::rule<IteratorT, ast::WriteLnStatement(), SkipperT>		_writeLnStatementRule;
		qi::rule<IteratorT, ast::ConditionalStatement(), SkipperT>	_conditionalStatement;
		qi::rule<IteratorT, ast::ForLoopStatement(), SkipperT>		_forLoopStatementRule;
		qi::rule<IteratorT, ast::AssignmentStatement(), SkipperT>	_assignmentStatementRule;

		qi::rule<IteratorT, ast::Expression(), SkipperT>			_expressionRule;
		qi::rule<IteratorT, ast::SimpleExpression(), SkipperT>		_simpleExpessionRule;
		qi::rule<IteratorT, ast::BooleanExpression(), SkipperT>		_booleanExpressionRule;
		qi::rule<IteratorT, ast::StringExpression(), SkipperT>		_stringExpressionRule;

		qi::rule<IteratorT, ast::Term(), SkipperT>					_termRule;
		qi::rule<IteratorT, ast::Factor(), SkipperT>				_factorRule;
		qi::rule<IteratorT, ast::RelationalOpeator(), SkipperT>		_relationalOperatorRule;
		qi::rule<IteratorT, ast::Variable(), SkipperT>				_variableRule;

		qi::rule<IteratorT, ast::Identifier(), SkipperT>			_identifierRule;
		qi::rule<IteratorT, ast::TypeSpecifier(), SkipperT>			_typeSpecifierRule;
	};

}}

#endif /* parsing_rules_hpp */
