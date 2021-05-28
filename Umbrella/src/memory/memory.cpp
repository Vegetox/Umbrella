#include "memory.h"

#include <iostream>
#include <thread>
#include <TlHelp32.h>

const int iPlayerAddrDist = 0x10;

HANDLE Memory::m_hProcess = NULL;
DWORD Memory::m_dwClientModBaseAddr = NULL;
DWORD Memory::m_dwEngineModBaseAddr = NULL;

Memory::Memory() {
	setProcessHandle(PROCESS);
	setClientModBaseAddr(CLIENT);
	setEngineModBaseAddr(ENGINE);
}

Memory::~Memory() {
	CloseHandle(this->m_hProcess);
	std::cout << "Handle closed" << std::endl;
}

HANDLE Memory::getProcessHandle() const {
	return this->m_hProcess;
}

void Memory::setProcessHandle(const std::wstring& wsProcess) {
	while (this->m_hProcess == NULL) {
		HANDLE hProcessSnap;
		if (hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL), hProcessSnap == INVALID_HANDLE_VALUE) {
			throw std::runtime_error("CreateToolhelp32Snapshot failed");
		}

		PROCESSENTRY32 pe32;
		pe32.dwSize = sizeof(PROCESSENTRY32);

		if (!Process32First(hProcessSnap, &pe32)) {
			throw std::runtime_error("Process32First failed");
		}
		do {
			if (!wsProcess.compare(pe32.szExeFile)) {
				if (this->m_hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID), this->m_hProcess == NULL) {
					throw std::runtime_error("OpenProcess failed");
				}
			}
		} while (this->m_hProcess == NULL && Process32Next(hProcessSnap, &pe32));

		if (!Process32Next(hProcessSnap, &pe32)) {
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
		if (!CloseHandle(hProcessSnap)) {
			throw std::runtime_error("CloseHandle failed");
		}
	};
}

DWORD Memory::getClientModBaseAddr() const {
	return this->m_dwClientModBaseAddr;
}

void Memory::setClientModBaseAddr(const std::wstring& wsModule) {
	while (this->m_dwClientModBaseAddr == NULL) {
		HANDLE hModuleSnap;
		if (hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetProcessId(this->m_hProcess)), hModuleSnap == INVALID_HANDLE_VALUE) {
			throw std::runtime_error("CreateToolhelp32Snapshot failed");
		}

		MODULEENTRY32 me32;
		me32.dwSize = sizeof(MODULEENTRY32);

		if (!Module32First(hModuleSnap, &me32)) {
			throw std::runtime_error("Module32First failed");
		}
		do {
			if (!wsModule.compare(me32.szModule)) {
				this->m_dwClientModBaseAddr = (DWORD)me32.modBaseAddr;
			}
		} while (this->m_dwClientModBaseAddr == NULL && Module32Next(hModuleSnap, &me32));

		if (!CloseHandle(hModuleSnap)) {
			throw std::runtime_error("CloseHandle failed");
		}
	};
}

DWORD Memory::getEngineModBaseAddr() const {
	return this->m_dwEngineModBaseAddr;
}

void Memory::setEngineModBaseAddr(const std::wstring& wsModule) {
	while (this->m_dwEngineModBaseAddr == NULL) {
		HANDLE hModuleSnap;
		if (hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetProcessId(this->m_hProcess)), hModuleSnap == INVALID_HANDLE_VALUE) {
			throw std::runtime_error("CreateToolhelp32Snapshot failed");
		}

		MODULEENTRY32 me32;
		me32.dwSize = sizeof(MODULEENTRY32);

		if (!Module32First(hModuleSnap, &me32)) {
			throw std::runtime_error("Module32First failed");
		}
		do {
			if (!wsModule.compare(me32.szModule)) {
				this->m_dwEngineModBaseAddr = (DWORD)me32.modBaseAddr;
			}
		} while (this->m_dwEngineModBaseAddr == NULL && Module32Next(hModuleSnap, &me32));

		if (!CloseHandle(hModuleSnap)) {
			throw std::runtime_error("CloseHandle failed");
		}
	};
}

bool Memory::play() const {
	const auto uClientState = RPM<int>(RPM<DWORD>(this->getEngineModBaseAddr() + hazedumper::signatures::dwClientState) + hazedumper::signatures::dwClientState_State);
	return uClientState == 6;
}

Player Memory::getLocalPlayer() const {
	const DWORD dwBaseAddr = RPM<DWORD>(this->m_dwClientModBaseAddr + hazedumper::signatures::dwLocalPlayer);
	return Player(
		dwBaseAddr,
		RPM<eTeam>(dwBaseAddr + hazedumper::netvars::m_iTeamNum),
		RPM<eLifeState>(dwBaseAddr + hazedumper::netvars::m_lifeState),
		RPM<bool>(dwBaseAddr + hazedumper::netvars::m_iHealth),
		RPM<int>(dwBaseAddr + hazedumper::netvars::m_iGlowIndex),
		RPM<bool>(dwBaseAddr + hazedumper::signatures::m_bDormant),
		RPM<bool>(dwBaseAddr + hazedumper::netvars::m_bSpotted),
		RPM<int>(dwBaseAddr + hazedumper::netvars::m_fFlags),
		RPM<float>(dwBaseAddr + hazedumper::netvars::m_flFlashMaxAlpha)
	);
}

Player Memory::getLocalPlayer(const bool bTeam, const bool bLifeState, const bool bLifePoints, const bool bGlowIndex, const bool bDormant, const bool bSpotted, const bool bFlags, const bool bFlashMaxAlpha) const {
	const DWORD dwBaseAddr = RPM<DWORD>(this->m_dwClientModBaseAddr + hazedumper::signatures::dwLocalPlayer);
	return Player(
		dwBaseAddr,
		bTeam ? RPM<eTeam>(dwBaseAddr + hazedumper::netvars::m_iTeamNum) : eTeam::NONE,
		bLifeState ? RPM<eLifeState>(dwBaseAddr + hazedumper::netvars::m_lifeState) : eLifeState::NONE,
		bLifePoints ? RPM<bool>(dwBaseAddr + hazedumper::netvars::m_iHealth) : NULL,
		bGlowIndex ? RPM<int>(dwBaseAddr + hazedumper::netvars::m_iGlowIndex) : NULL,
		bDormant ? RPM<bool>(dwBaseAddr + hazedumper::signatures::m_bDormant) : NULL,
		bSpotted ? RPM<bool>(dwBaseAddr + hazedumper::netvars::m_bSpotted) : NULL,
		bFlags ? RPM<int>(dwBaseAddr + hazedumper::netvars::m_fFlags) : NULL,
		bFlashMaxAlpha ? RPM<float>(dwBaseAddr + hazedumper::netvars::m_flFlashMaxAlpha) : NULL
	);
}

std::vector<Player> Memory::getPlayers() const {
	std::vector<Player> vecPlayers;

	for (auto i = 0; i < MAX_PLAYERS; i++) {
		DWORD dwBaseAddr = NULL;
		if (dwBaseAddr = RPM<DWORD>(this->m_dwClientModBaseAddr + hazedumper::signatures::dwEntityList + i * iPlayerAddrDist), dwBaseAddr != NULL) {
			vecPlayers.push_back(Player(
				dwBaseAddr,
				RPM<eTeam>(dwBaseAddr + hazedumper::netvars::m_iTeamNum),
				RPM<eLifeState>(dwBaseAddr + hazedumper::netvars::m_lifeState),
				RPM<bool>(dwBaseAddr + hazedumper::netvars::m_iHealth),
				RPM<int>(dwBaseAddr + hazedumper::netvars::m_iGlowIndex),
				RPM<bool>(dwBaseAddr + hazedumper::signatures::m_bDormant),
				RPM<bool>(dwBaseAddr + hazedumper::netvars::m_bSpotted),
				RPM<int>(dwBaseAddr + hazedumper::netvars::m_fFlags),
				RPM<float>(dwBaseAddr + hazedumper::netvars::m_flFlashMaxAlpha)
			));
		}
	}

	return vecPlayers;
}

std::vector<Player> Memory::getPlayers(const bool bTeam, const bool bLifeState, const bool bLifePoints, const bool bGlowIndex, const bool bDormant, const bool bSpotted, const bool bFlags, const bool bFlashMaxAlpha) const {
	std::vector<Player> vecPlayers;

	for (auto i = 0; i < MAX_PLAYERS; i++) {
		DWORD dwBaseAddr = NULL;
		if (dwBaseAddr = RPM<DWORD>(this->m_dwClientModBaseAddr + hazedumper::signatures::dwEntityList + i * iPlayerAddrDist), dwBaseAddr != NULL) {
			vecPlayers.push_back(Player(
				dwBaseAddr,
				bTeam ? RPM<eTeam>(dwBaseAddr + hazedumper::netvars::m_iTeamNum) : eTeam::NONE,
				bLifeState ? RPM<eLifeState>(dwBaseAddr + hazedumper::netvars::m_lifeState) : eLifeState::NONE,
				bLifePoints ? RPM<bool>(dwBaseAddr + hazedumper::netvars::m_iHealth) : NULL,
				bGlowIndex ? RPM<int>(dwBaseAddr + hazedumper::netvars::m_iGlowIndex) : NULL,
				bDormant ? RPM<bool>(dwBaseAddr + hazedumper::signatures::m_bDormant) : NULL,
				bSpotted ? RPM<bool>(dwBaseAddr + hazedumper::netvars::m_bSpotted) : NULL,
				bFlags ? RPM<int>(dwBaseAddr + hazedumper::netvars::m_fFlags) : NULL,
				bFlashMaxAlpha ? RPM<float>(dwBaseAddr + hazedumper::netvars::m_flFlashMaxAlpha) : NULL
			));
		}
	}

	return vecPlayers;
}

std::vector<Object> Memory::getObjects() const {
	std::vector<Object> vecObjects;

	const auto& maxObjects = RPM<DWORD>(this->m_dwClientModBaseAddr + hazedumper::signatures::dwGlowObjectManager + 0xC);

	for (auto i = 0; i < maxObjects; i++) {
		DWORD dwBaseAddr = NULL, dwEntBaseAddr = NULL;
		if (dwBaseAddr = RPM<DWORD>(this->m_dwClientModBaseAddr + hazedumper::signatures::dwGlowObjectManager) + i * 0x38, dwBaseAddr != NULL) {
			if (dwEntBaseAddr = RPM<DWORD>(dwBaseAddr), dwEntBaseAddr != NULL) {
				vecObjects.push_back(Object(
					dwBaseAddr,
					dwEntBaseAddr,
					RPM<int>(RPM<DWORD>(RPM<DWORD>(RPM<DWORD>(dwEntBaseAddr + 0x8) + 0x8) + 0x1) + 0x14),
					i
				));
			}
		}
	}

	return vecObjects;
}

std::vector<Object> Memory::getObjects(const bool bClassId, const bool bGlowIndex) const {
	std::vector<Object> vecObjects;

	const auto& maxObjects = RPM<DWORD>(this->m_dwClientModBaseAddr + hazedumper::signatures::dwGlowObjectManager + 0xC);

	for (auto i = 0; i < maxObjects; i++) {
		DWORD dwBaseAddr = NULL, dwEntBaseAddr = NULL;
		if (dwBaseAddr = RPM<DWORD>(this->m_dwClientModBaseAddr + hazedumper::signatures::dwGlowObjectManager) + i * 0x38, dwBaseAddr != NULL) {
			if (dwEntBaseAddr = RPM<DWORD>(dwBaseAddr), dwEntBaseAddr != NULL) {
				vecObjects.push_back(Object(
					dwBaseAddr,
					dwEntBaseAddr,
					bClassId ? RPM<int>(RPM<DWORD>(RPM<DWORD>(RPM<DWORD>(dwEntBaseAddr + 0x8) + 0x8) + 0x1) + 0x14) : NULL,
					bGlowIndex ? i : NULL
				));
			}
		}
	}

	return vecObjects;
}
