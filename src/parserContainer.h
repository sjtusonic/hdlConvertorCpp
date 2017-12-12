#pragma once

#include <iostream>
#include <fstream>
#include <functional>

#include "hdlObjects/context.h"
#include "antlr4-runtime.h"
#include "/home/delon/workspace/exe/cpp/6_BOOK_ALGORITHMS_VER4_Robert/include/include.h"


using namespace antlr4;
using namespace vhdl;

enum ParserErrors {
	PERR_OK = 0, PERR_FILE, PARSING_ERR, CONVERTING_ERR
};

inline bool file_exists(const char * name) {
	if (FILE *file = fopen(name, "r")) {
		fclose(file);
		return true;
	} else {
		return false;
	}
}

template<class antlrLexerT, class antlrParserT, class hdlParserT>
class ParserContainer {
	ANTLRErrorListener * syntaxErrLogger;
	antlrLexerT * lexer;
	CommonTokenStream * tokens;
	antlrParserT * antlrParser;
	hdlParserT * hdlParser;

	void initParser(ANTLRFileStream * input) {
		// create a lexer that feeds off of input CharStream
		lexer = new antlrLexerT(input);

		// create a buffer of tokens pulled from the lexer
		tokens = new CommonTokenStream(lexer);

		// create a parser that feeds off the tokens buffer
		antlrParser = new antlrParserT(tokens);

		antlrParser->removeErrorListeners();
		syntaxErrLogger = new SyntaxErrorLogger();
		antlrParser->addErrorListener(syntaxErrLogger);
	}
public:
	Context * context;
	ParserErrors parseFile(
			const char * fileName,
			bool hierarchyOnly,
			bool debug,
			std::function<
					void(antlrParserT * antlrParser, hdlParserT * hdlParser)> parseFn) {
		DENTER;
		// create a CharStream that reads from standard input
		if (!file_exists(fileName))
			return PERR_FILE;

DEBUG_MARK;
		ANTLRFileStream * input = new ANTLRFileStream(fileName);
DEBUG_MARK;
		initParser(input);
DEBUG_MARK;

		hdlParser = new hdlParserT(hierarchyOnly);
DEBUG_MARK;

		// begin parsing at init rule
		parseFn(antlrParser, hdlParser);
DEBUG_MARK;

		context = hdlParser->getContext();
DEBUG_MARK;
		delete hdlParser;
		delete syntaxErrLogger;
		delete antlrParser;
		delete tokens;
		delete lexer;
		delete input;

DRETURN;
		return PERR_OK;
	}
};
