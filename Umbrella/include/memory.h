#pragma once

#include "object.h"
#include "offset.h"
#include "player.h"
#include "variable.h"

#include <stdexcept>
#include <string>
#include <vector>
#include <Windows.h>

class Memory {
public:
	Memory();

	~Memory();

	template<typename T>
	T RPM(const DWORD& dwBaseAddress) const {
		T buffer;

		if (!ReadProcessMemory(this->m_hProcess, (LPCVOID)dwBaseAddress, &buffer, sizeof(T), NULL)) {
			throw std::runtime_error("RPM failed");
		}

		return buffer;
	}

	template<typename T>
	void WPM(const DWORD& dwBaseAddress, T buffer) const {
		if (!WriteProcessMemory(this->m_hProcess, (LPVOID)dwBaseAddress, &buffer, sizeof(T), NULL)) {
			throw std::runtime_error("WPM failed");
		}
	}

	HANDLE getProcessHandle() const;
	void setProcessHandle(const std::wstring& wsProcess);

	DWORD getClientModBaseAddr() const;
	void setClientModBaseAddr(const std::wstring& wsModule);

	DWORD getEngineModBaseAddr() const;
	void setEngineModBaseAddr(const std::wstring& wsModule);

	bool play() const;

	Player getLocalPlayer() const;
	Player getLocalPlayer(const bool bTeam, const bool bLifeState, const bool bLifePoints, const bool bGlowIndex, const bool bDormant, const bool bSpotted, const bool bFlags, const bool bFlashMaxAlpha) const;

	std::vector<Player> getPlayers() const;
	std::vector<Player> getPlayers(const bool bTeam, const bool bLifeState, const bool bLifePoints, const bool bGlowIndex, const bool bDormant, const bool bSpotted, const bool bFlags, const bool bFlashMaxAlpha) const;

	std::vector<Object> getObjects() const;
	std::vector<Object> getObjects(const bool bClassId, const bool bGlowIndex) const;

private:
	static HANDLE m_hProcess;
	static DWORD m_dwClientModBaseAddr;
	static DWORD m_dwEngineModBaseAddr;
};
