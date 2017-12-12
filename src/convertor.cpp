#include "convertor.h"
#include "atn/PredictionMode.h"
#include "/home/delon/workspace/exe/cpp/6_BOOK_ALGORITHMS_VER4_Robert/include/include.h"

const char * Convertor::fileName = NULL;
const char * Convertor::errStr = NULL;
Langue Convertor::lang = VHDL;
bool Convertor::hierarchyOnly = false;
bool Convertor::debug = false;

ParserErrors Convertor::err = PERR_OK;

void parseFnVerilog(Verilog2001Parser * antlrParser,
		Source_textParser * hdlParser) {
	DENTER;
	Verilog2001Parser::Source_textContext * tree =
			antlrParser->source_text();
			DPRINT("hdlParser->visitSource_text(tree);");
	hdlParser->visitSource_text(tree);
	DRETURN;
}

void parseFnVHDL(vhdlParser * antlrParser, DesignFileParser * hdlParser) {
	vhdlParser::Design_fileContext* tree = antlrParser->design_file();
	hdlParser->visitDesign_file(tree);
}

void parseFnSystemVerilog(sv::sv2012Parser * antlrParser,
		Library_textParser * hdlParser) {
	sv::sv2012Parser::Library_textContext* tree =
				antlrParser->library_text();
		hdlParser->visitLibrary_text(tree);
}

Context * Convertor::parse(const char * _fileName, Langue _lang,
		bool _hierarchyOnly, bool _debug) {
	DENTER;
	fileName = _fileName;
	lang = _lang;
	hierarchyOnly = _hierarchyOnly;
	debug = _debug;
	DLOG(fileName);
	DLOG(hierarchyOnly);
	DLOG(debug);
	Context * c = NULL;
	try {
		if (lang == VHDL) {
			auto pc = new ParserContainer<vhdlLexer, vhdlParser,
					DesignFileParser>();
			err = pc->parseFile(fileName, hierarchyOnly, debug, parseFnVHDL);
			c = pc->context;
			delete pc;

		} else if (lang == VERILOG) {
			DEBUG_MARK;
			auto pc = new ParserContainer<Verilog2001Lexer, Verilog2001Parser,
					Source_textParser>();
			DEBUG_MARK;
			DPRINT("pc->parseFile()");
			err = pc->parseFile(fileName, hierarchyOnly, debug, parseFnVerilog);
			DEBUG_MARK;
			DPRINT("pc->context");
			DEBUG_MARK;
			delete pc;

		} else if (lang == SYSTEM_VERILOG) {
			auto pc = new ParserContainer<sv::sv2012Lexer, sv::sv2012Parser,
					Library_textParser>();
			err = pc->parseFile(fileName, hierarchyOnly, debug, parseFnSystemVerilog);
			c = pc->context;
			delete pc;

		} else {
			throw "Unsupported language.";
		}
	} catch (const char * _errStr) {
		err = PARSING_ERR;
		errStr = _errStr;
	DRETURN;
		return NULL;
	}
	DRETURN;
	return c;
}
