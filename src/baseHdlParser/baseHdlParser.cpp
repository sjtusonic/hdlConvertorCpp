#include "baseHdlParser.h"
#include "/home/delon/workspace/exe/cpp/6_BOOK_ALGORITHMS_VER4_Robert/include/include.h"

BaseHdlParser::BaseHdlParser(bool _hierarchyOnly) {
	DENTER;
	context = new Context();
	hierarchyOnly = _hierarchyOnly;
	DRETURN;
}
Context * BaseHdlParser::getContext() {
	return context;
}
