#include "variable.h"

#include <iostream>
#include <thread>

bool Variable::m_bUmbrella = false;
bool Variable::m_bRadar = false;
ePlayerGlow Variable::m_ePlayerGlow = ePlayerGlow::OFF;
bool Variable::m_bObjectGlow = false;
bool Variable::m_bFlash = false;
bool Variable::m_bSmoke = false;
bool Variable::m_bBunnyHop = false;

Variable::Variable() {
	std::thread(&Variable::listenVariable, this).detach();
}

Variable::~Variable() {}

bool Variable::getUmbrella() const {
	return this->m_bUmbrella;
}
	
void Variable::setUmbrella(const bool& bUmbrella) {
	this->m_bUmbrella = bUmbrella;
}

bool Variable::getRadar() const {
	return this->m_bRadar;
}
	
void Variable::setRadar(const bool& bRadar) {
	this->m_bRadar = bRadar;
}

ePlayerGlow Variable::getPlayerGlow() const {
	return this->m_ePlayerGlow;
}
	
void Variable::setPlayerGlow(const ePlayerGlow& ePlayerGlow) {
	this->m_ePlayerGlow = ePlayerGlow;
}

ePlayerGlow Variable::next(const ePlayerGlow& ePlayerGlow) const {
	switch (ePlayerGlow) {
	case ePlayerGlow::OFF:
			return ePlayerGlow::DEFAULT;
	case ePlayerGlow::DEFAULT:
		return ePlayerGlow::HEALTH_BASED;
	case ePlayerGlow::HEALTH_BASED:
		return ePlayerGlow::OFF;
	default:
		return ePlayerGlow::OFF;
	}
}

bool Variable::getObjectGlow() const {
	return this->m_bObjectGlow;
}

void Variable::setObjectGlow(const bool& bObjectGlow) {
	this->m_bObjectGlow = bObjectGlow;
}

bool Variable::getFlash() const {
	return this->m_bFlash;
}

void Variable::setFlash(const bool& bFlash) {
	this->m_bFlash = bFlash;
}

bool Variable::getSmoke() const {
	return this->m_bSmoke;
}

void Variable::setSmoke(const bool& bSmoke) {
	this->m_bSmoke = bSmoke;
}

bool Variable::getBunnyHop() const {
	return this->m_bBunnyHop;
}

void Variable::setBunnyHop(const bool& bBunnyHop) {
	this->m_bBunnyHop = bBunnyHop;
}

void Variable::listenVariable() {
	while (!this->m_bUmbrella) {
		bool bChange = false;

		if (GetAsyncKeyState(VK_F3) & (1 << 0)) {
			this->m_bRadar = !this->m_bRadar;
			bChange = true;
		}
		if (GetAsyncKeyState(VK_F4) & (1 << 0)) {
			this->m_ePlayerGlow = next(this->m_ePlayerGlow);
			bChange = true;
		}
		if (GetAsyncKeyState(VK_F5) & (1 << 0)) {
			this->m_bObjectGlow = !this->m_bObjectGlow;
			bChange = true;
		}
		if (GetAsyncKeyState(VK_F6) & (1 << 0)) {
			this->m_bFlash = !this->m_bFlash;
			bChange = true;
		}
		if (GetAsyncKeyState(VK_F7) & (1 << 0)) {
			this->m_bSmoke = !this->m_bSmoke;
			bChange = true;
		}
		if (GetAsyncKeyState(VK_F8) & (1 << 0)) {
			this->m_bBunnyHop = !this->m_bBunnyHop;
			bChange = true;
		}
		if (bChange) {
			printVariable();
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void Variable::printVariable() const {
	std::cout << "---" << std::endl;
	std::cout << "Radar: " << this->m_bRadar << std::endl;
	std::cout << "Player glow: " << static_cast<std::underlying_type<ePlayerGlow>::type>(this->m_ePlayerGlow) << std::endl;
	std::cout << "Object glow: " << this->m_bObjectGlow << std::endl;
	std::cout << "Flash: " << this->m_bFlash << std::endl;
	std::cout << "Smoke: " << this->m_bSmoke << std::endl;
	std::cout << "BunnyHop: " << this->m_bBunnyHop << std::endl;
}
