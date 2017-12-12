#pragma once
#include "named.h"
#include "variable.h"
#include "port.h"

class Entity: public Named {
public:
	std::vector<Variable*> generics;
	std::vector<Port*> ports;
	Entity();
	Port * getPortByName(const char * name);
#ifdef USE_PYTHON
	PyObject * toJson() const;
#endif
	void dump(int indent) const;
	~Entity();
};
