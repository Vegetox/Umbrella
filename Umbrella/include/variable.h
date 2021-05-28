#pragma once

#include <array>
#include <Windows.h>

#define PROCESS L"csgo.exe"
#define CLIENT L"client.dll"
#define ENGINE L"engine.dll"
#define MAX_PLAYERS 64
#define MAX_ENTITIES 8192 

enum class ePlayerGlow { OFF = 0, DEFAULT = 1, HEALTH_BASED = 2 };

class Variable {
public:
	Variable();

	~Variable();

	bool getUmbrella() const;
	void setUmbrella(const bool& bUmbrella);

	bool getRadar() const;
	void setRadar(const bool& bRadar);

	ePlayerGlow getPlayerGlow() const;
	void setPlayerGlow(const ePlayerGlow& ePlayerGlow);

	ePlayerGlow next(const ePlayerGlow& ePlayerGlow) const;

	bool getObjectGlow() const;
	void setObjectGlow(const bool& bObjectGlow);

	bool getFlash() const;
	void setFlash(const bool& bFlash);

	bool getSmoke() const;
	void setSmoke(const bool& bSmoke);

	bool getBunnyHop() const;
	void setBunnyHop(const bool& bBunnyHop);

	void listenVariable();

	void printVariable() const;
	
private:
	static bool m_bUmbrella;
	static bool m_bRadar;
	static ePlayerGlow m_ePlayerGlow;
	static bool m_bObjectGlow;
	static bool m_bFlash;
	static bool m_bSmoke;
	static bool m_bBunnyHop;
};

