#include "designFileParser.h"
#include "/home/delon/workspace/exe/cpp/6_BOOK_ALGORITHMS_VER4_Robert/include/include.h"

DesignFileParser::DesignFileParser(bool _hierarchyOnly) : BaseHdlParser(_hierarchyOnly) {}

void DesignFileParser::visitDesign_file(
		vhdlParser::Design_fileContext* ctx) {
	DENTER;
	if (!ctx)
		return;
	// design_file
	// : ( design_unit )* EOF
	// ;
	for (auto u : ctx->design_unit()) {
		DesignFileParser::visitDesign_unit(u);
	}
	DRETURN;
}

void DesignFileParser::visitDesign_unit(
		vhdlParser::Design_unitContext* ctx) {
	if (!ctx)
		return;
	// design_unit
	// : context_clause library_unit
	// ;
	DesignFileParser::visitContext_clause(ctx->context_clause());
	DesignFileParser::visitLibrary_unit(ctx->library_unit());
}
void DesignFileParser::visitLibrary_unit(
		vhdlParser::Library_unitContext* ctx) {
	if (!ctx)
		return;
	// library_unit
	// : secondary_unit | primary_unit
	// ;

	DesignFileParser::visitSecondary_unit(ctx->secondary_unit());
	DesignFileParser::visitPrimary_unit(ctx->primary_unit());
}
void DesignFileParser::visitSecondary_unit(
		vhdlParser::Secondary_unitContext* ctx) {
	if (!ctx)
		return;
	DENTER;
	// secondary_unit
	// : architecture_body
	// | package_body
	// ;
	auto arch = ctx->architecture_body();
	DEBUG_MARK;
	DLOG(arch);
	if (arch) {
		auto aparser = new ArchParser(hierarchyOnly);
		Arch * a = aparser->visitArchitecture_body(arch);
		delete aparser;
		DPRINT("context->architectures.push_back:"<<a);
		context->architectures.push_back(a);
	}
	auto pack = ctx->package_body();
	if (pack) {
		PackageParser * pparser = new PackageParser(hierarchyOnly);
		Package * p = pparser->visitPackage_body(pack);
		delete pparser;
		context->packages.push_back(p);

	}
	DRETURN;
}
void DesignFileParser::visitContext_clause(
		vhdlParser::Context_clauseContext* ctx) {
	if (!ctx)
		return;
	// context_clause
	// : ( context_item )*
	// ;
	for (auto item : ctx->context_item()) {
		visitContext_item(item);
	}
}
void DesignFileParser::visitPrimary_unit(
		vhdlParser::Primary_unitContext* ctx) {
	if (!ctx)
		return;
	// primary_unit
	// : entity_declaration
	// | configuration_declaration
	// | package_declaration
	// ;
	auto ed = ctx->entity_declaration();
	if (ed) {
		auto eParser = new EntityParser(hierarchyOnly);
		Entity * e = eParser->visitEntity_declaration(ed);
		context->entities.push_back(e);
		delete eParser;
		return;
	}
	auto cd = ctx->configuration_declaration();
	if (cd) {
		NotImplementedLogger::print(
				"DesignFileParser.visitConfiguration_declaration");
		return;
	}
	auto pd = ctx->package_declaration();
	if (pd) {
		auto php = new PackageHeaderParser(hierarchyOnly);
		PackageHeader * ph = php->visitPackage_declaration(pd);
		context->packageHeaders.push_back(ph);
		delete php;
	}

}
void DesignFileParser::visitContext_item(
		vhdlParser::Context_itemContext* ctx) {
	// context_item
	// : library_clause
	// | use_clause
	// ;
	auto l = ctx->library_clause();
	if (l) {
		return; //libraries are ignored
	}
	auto u = ctx->use_clause();
	if (u) {
		for (Expr * r : visitUse_clause(u)) {
			context->imports.push_back(r);
		}
	}
}

std::vector<Expr*> DesignFileParser::visitUse_clause(
		vhdlParser::Use_clauseContext* ctx) {
	// use_clause
	// : USE selected_name ( COMMA selected_name )* SEMI
	// ;
	std::vector<Expr*> refL;
	for (auto sn : ctx->selected_name()) {
		Expr * r = ReferenceParser::visitSelected_name(sn);
		refL.push_back(r);
	}
	return refL;
}
