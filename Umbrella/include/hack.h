#pragma once

#include "memory.h"

class Hack : public Memory, Variable {
public:
	Hack();
	~Hack();
	void beginRadar() const;
	void beginPlayerGlow() const;
	void beginObjectGlow() const;
	void beginFlash() const;
	void beginSmoke() const;
	void beginBunnyHop() const;
	void run();
	bool end();
	void printKey() const;
};