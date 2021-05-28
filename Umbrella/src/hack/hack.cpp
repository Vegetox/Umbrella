#include "hack.h"

#include <iostream>
#include <thread>
#include <chrono>

std::array<float, 4> afDefaultTGlowColor = { 0.878431f, 0.686275f, 0.337255f, 0.6f };
std::array<float, 4> afDefaultCTGlowColor = { 0.447059f, 0.607843f, 0.866667f, 0.6f };
std::array<float, 4> afHealthBasedGlowColor;
std::array<bool, 2> abRender = { true, false };

std::array<float, 4> afObjectsGlowColor = { 0.75f, 0.25f, 0.75f, 0.6f };

std::array<float, 4> afResetGlowColor = { 0.0f, 0.0f, 0.0f, 0.0f };
std::array<bool, 2> abResetRender = { false, false };

enum knifeDefinitionIndex               // id
{
	WEAPON_KNIFE = 42,
	WEAPON_KNIFE_T = 59,
	WEAPON_KNIFE_BAYONET = 500,         // 0
	WEAPON_KNIFE_FLIP = 505,            // 1
	WEAPON_KNIFE_GUT = 506,             // 2
	WEAPON_KNIFE_KARAMBIT = 507,        // 3
	WEAPON_KNIFE_M9_BAYONET = 508,      // 4
	WEAPON_KNIFE_TACTICAL = 509,        // 5
	WEAPON_KNIFE_FALCHION = 512,        // 6
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,  // 7
	WEAPON_KNIFE_BUTTERFLY = 515,       // 8
	WEAPON_KNIFE_PUSH = 516,            // 9
	WEAPON_KNIFE_URSUS = 519,           // 10
	WEAPON_KNIFE_GYPSY_JACKKNIFE = 520, // 11
	WEAPON_KNIFE_STILETTO = 522,        // 12
	WEAPON_KNIFE_WIDOWMAKER = 523       // 13
};

Hack::Hack() {}

Hack::~Hack() {
	std::cout << std::endl << "Memory freed" << std::endl;
}

void Hack::beginRadar() const {
	bool lastValue = false;

	while (!getUmbrella()) {
		if (play() && getRadar()) {
			const Player localPlayer = getLocalPlayer(true, false, false, false, false, false, false, false);

			for (const auto& player : getPlayers(true, true, false, false, true, true, false, false)) {
				if (!player.team(localPlayer) && !player.getDormant() && player.alive() && !player.getSpotted()) {
					WPM<bool>(player.getBaseAddr() + hazedumper::netvars::m_bSpotted, true);
				}
			} 
			lastValue = true;
		}
		else if (play() && lastValue) {
			const Player localPlayer = getLocalPlayer();

			for (const auto& player : getPlayers(true, true, false, false, true, true, false, false)) {
				if (!player.team(localPlayer) && !player.getDormant() && player.alive() && player.getSpotted()) {
					WPM<bool>(player.getBaseAddr() + hazedumper::netvars::m_bSpotted, false);
				}
			}
			lastValue = false;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		
	}
}

void Hack::beginPlayerGlow() const {
	bool lastValue = false;

	while (!getUmbrella()) {
		if (play() && getPlayerGlow() != ePlayerGlow::OFF) {
			const DWORD dwGlowObjBaseAddr = RPM<DWORD>(this->getClientModBaseAddr() + hazedumper::signatures::dwGlowObjectManager);
			const Player localPlayer = getLocalPlayer(true, false, false, false, false, false, false, false);

			for (const auto& player : getPlayers(true, true, true, true, false, false, false, false)) {
				if (!player.getDormant() && player.alive() && !player.equal(localPlayer)) {

					if (getPlayerGlow() == ePlayerGlow::HEALTH_BASED && !player.team(localPlayer)) {
						afHealthBasedGlowColor = { 1.0f - player.getLifePoints() / 100.0f, player.getLifePoints() / 100.0f, 0.0f, 0.6f };

						WPM<std::array<float, 4>>(dwGlowObjBaseAddr + ((player.getGlowIndex() * 0x38) + 0x4), afHealthBasedGlowColor);
						WPM<std::array<bool, 2>>(dwGlowObjBaseAddr + ((player.getGlowIndex() * 0x38) + 0x24), abRender);
					}
					else {
						switch (player.getTeam()) {
						case eTeam::TERRORIST:
							WPM<std::array<float, 4>>(dwGlowObjBaseAddr + ((player.getGlowIndex() * 0x38) + 0x4), afDefaultTGlowColor);
							WPM<std::array<bool, 2>>(dwGlowObjBaseAddr + ((player.getGlowIndex() * 0x38) + 0x24), abRender);
							break;
						case eTeam::COUNTER_TERRORIST:
							WPM<std::array<float, 4>>(dwGlowObjBaseAddr + ((player.getGlowIndex() * 0x38) + 0x4), afDefaultCTGlowColor);
							WPM<std::array<bool, 2>>(dwGlowObjBaseAddr + ((player.getGlowIndex() * 0x38) + 0x24), abRender);
							break;
						}
					}
				}
			}
			lastValue = true;
		}
		else if (play() && lastValue) {
			const DWORD dwGlowObjBaseAddr = RPM<DWORD>(this->getClientModBaseAddr() + hazedumper::signatures::dwGlowObjectManager);
			const Player localPlayer = getLocalPlayer(true, false, false, false, false, false, false, false);

			for (const auto& player : getPlayers(true, true, true, true, false, false, false, false)) {
				if (!player.getDormant() && player.alive() && !player.equal(localPlayer)) {
					WPM<std::array<float, 4>>(dwGlowObjBaseAddr + ((player.getGlowIndex() * 0x38) + 0x4), afResetGlowColor);
					WPM<std::array<bool, 2>>(dwGlowObjBaseAddr + ((player.getGlowIndex() * 0x38) + 0x24), abResetRender);
				}
			}
			lastValue = false;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}
void Hack::beginObjectGlow() const {
	bool lastValue = false;

	while (!getUmbrella()) {
		if (play() && getObjectGlow()) {
			const DWORD dwGlowObjBaseAddr = RPM<DWORD>(this->getClientModBaseAddr() + hazedumper::signatures::dwGlowObjectManager);

			for (const auto& object : getObjects(true, true)) {
				if (!object.exclude()) {
					WPM<std::array<float, 4>>(object.getBaseAddr() + 0x4, afObjectsGlowColor);
					WPM<std::array<bool, 2>>(object.getBaseAddr() + 0x24, abRender);
				}
			}
			lastValue = true;
		}
		else if (play() && lastValue) {
			const DWORD dwGlowObjBaseAddr = RPM<DWORD>(this->getClientModBaseAddr() + hazedumper::signatures::dwGlowObjectManager);

			for (const auto& object : getObjects(true, true)) {
				if (!object.exclude()) {
					WPM<std::array<float, 4>>(object.getBaseAddr() + 0x4, afResetGlowColor);
					WPM<std::array<bool, 2>>(object.getBaseAddr() + 0x24, abResetRender);
				}
			}
			lastValue = false;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

void Hack::beginFlash() const {
	bool lastValue = false;

	while (!getUmbrella()) {
		if (play() && getFlash() != lastValue) {
			if (getFlash() != lastValue) {
				const Player& localPlayer = getLocalPlayer(false, false, false, false, false, false, false, true);

				WPM<float>(localPlayer.getBaseAddr() + hazedumper::netvars::m_flFlashMaxAlpha, getFlash() ? 0.0f : 255.0f);

				lastValue = !lastValue;
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

void Hack::beginSmoke() const {
	while (!getUmbrella()) {
		if (play() && getSmoke()) {
			// not implemented yet
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

void Hack::beginBunnyHop() const {
	while (!getUmbrella()) {
		if (play() && getBunnyHop()) {
			const Player& localPlayer = getLocalPlayer(false, false, false, false, false, false, true, false);

			if (GetAsyncKeyState(VK_SPACE) && (localPlayer.getFlags() & (1 << 0))) {
				WPM<int>(getClientModBaseAddr() + hazedumper::signatures::dwForceJump, 6);
			}
		}
		else {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
}

void Hack::run() {
	std::thread(&Hack::beginRadar, this).detach();
	std::thread(&Hack::beginPlayerGlow, this).detach();
	std::thread(&Hack::beginObjectGlow, this).detach();
	std::thread(&Hack::beginFlash, this).detach();
	std::thread(&Hack::beginBunnyHop, this).detach();

	printKey();

	this->end();
}

bool Hack::end() {
	while (!(GetAsyncKeyState(VK_F10) & (1 << 0))) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	this->setUmbrella(true);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	return true;
}

void Hack::printKey() const {
	std::cout << "Umbrella CS:GO" << std::endl << std::endl;
	std::cout << "[F3] Radar" << std::endl;
	std::cout << "[F4] Player glow" << std::endl;
	std::cout << "[F5] Object glow" << std::endl;
	std::cout << "[F6] Flash" << std::endl;
	std::cout << "[F7] Smoke (not implemented yet)" << std::endl;
	std::cout << "[F8] BunnyHop" << std::endl;
	std::cout << std::endl;
	std::cout << "[F10] Exit" << std::endl << std::endl;
}
