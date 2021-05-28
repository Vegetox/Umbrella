#pragma once

#include <Windows.h>

enum class eTeam { NONE = -1, SPECTATOR = 1, TERRORIST = 2, COUNTER_TERRORIST = 3 };

enum class eLifeState { NONE = -1, ALIVE = 0, DYING = 1, DEAD = 2 };

class Player {
public:
    Player();

    Player(const DWORD& m_dwBaseAddress, const eTeam& m_eTeam, const eLifeState& m_eLifeState, const int& m_iLifePoints, const int& m_iGlowIndex, const bool& m_bDormant, const bool& m_bSpotted, const int& m_iFlags, const float& m_fFlashMaxAlpha);

    ~Player();

    DWORD getBaseAddr() const;
    void setBaseAddr(const DWORD& dwBaseAddr);

    eTeam getTeam() const;
    void setTeam(const eTeam& eTeam);

    eLifeState getLifeState() const;
    void setLifeState(const eLifeState& eLifeState);

    int getLifePoints() const;
    void setLifePoints(const int& iLifePoints);

    int getGlowIndex() const;
    void setGlowIndex(const int& iGlowIndex);

    bool getDormant() const;
    void setDormant(const bool& bDormant);

    bool getSpotted() const;
    void setSpotted(const bool& bSpotted);

    int getFlags() const;
    void setFlags(const int& iFlags);

    float getFlashMaxAlpha() const;
    void setFlashMaxAlpha(const float& fFlashMaxAlpha);

    bool alive() const;

    bool team(const Player& player) const;

    bool equal(const Player& player) const;

private:
    DWORD m_dwBaseAddr;
    eTeam m_eTeam;
    eLifeState m_eLifeState;
    int m_iLifePoints;
    int m_iGlowIndex;
    bool m_bDormant;
    bool m_bSpotted;
    int m_iFlags;
    float m_fFlashMaxAlpha;
};
