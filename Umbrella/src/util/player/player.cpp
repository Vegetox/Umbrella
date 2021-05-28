#include "player.h"

Player::Player() {
    this->m_dwBaseAddr = NULL;
    this->m_eTeam = eTeam::NONE;
    this->m_eLifeState = eLifeState::NONE;
    this->m_iLifePoints = NULL;
    this->m_iGlowIndex = NULL;
    this->m_bDormant = NULL;
    this->m_bSpotted = NULL;
    this->m_iFlags = NULL;
    this->m_fFlashMaxAlpha = NULL;
}

Player::Player(const DWORD& dwBaseAddr, const eTeam& eTeam, const eLifeState& eLifeState, const int& iLifePoints, const int& iGlowIndex, const bool& bDormant, const bool& bSpotted, const int& iFlags, const float& fFlashMaxAlpha) {
    this->m_dwBaseAddr = dwBaseAddr;
    this->m_eTeam = eTeam;
    this->m_eLifeState = eLifeState;
    this->m_iLifePoints = iLifePoints;
    this->m_iGlowIndex = iGlowIndex;
    this->m_bDormant = bDormant;
    this->m_bSpotted = bSpotted;
    this->m_iFlags = iFlags;
    this->m_fFlashMaxAlpha = fFlashMaxAlpha;
}

Player::~Player() {}

DWORD Player::getBaseAddr() const {
    return this->m_dwBaseAddr;
}

void Player::setBaseAddr(const DWORD& dwBaseAddr) {
    this->m_dwBaseAddr = dwBaseAddr;
}

eTeam Player::getTeam() const {
    return this->m_eTeam;
}

void Player::setTeam(const eTeam& eTeam) {
    this->m_eTeam = eTeam;
}

eLifeState Player::getLifeState() const {
    return this->m_eLifeState;
}

void Player::setLifeState(const eLifeState& eLifeState) {
    this->m_eLifeState = eLifeState;
}

int Player::getLifePoints() const {
    return this->m_iLifePoints;
}

void Player::setLifePoints(const int& iLifePoints) {
    this->m_iLifePoints = iLifePoints;
}

int Player::getGlowIndex() const {
    return this->m_iGlowIndex;
}

void Player::setGlowIndex(const int& unGlowIndex) {
    this->m_iGlowIndex = unGlowIndex;
}

bool Player::getDormant() const {
    return this->m_bDormant;
}

void Player::setDormant(const bool& bDormant) {
    this->m_bDormant = bDormant;
}

bool Player::getSpotted() const {
    return this->m_bSpotted;
}

void Player::setSpotted(const bool& bSpotted) {
    this->m_bSpotted = bSpotted;
}

int Player::getFlags() const {
    return this->m_iFlags;
}

void Player::setFlags(const int& iFlags) {
    this->m_iFlags = iFlags;
}

float Player::getFlashMaxAlpha() const {
    return this->m_fFlashMaxAlpha;
}

void Player::setFlashMaxAlpha(const float& fFlashMaxAlpha) {
    this->m_fFlashMaxAlpha = fFlashMaxAlpha;
}

bool Player::alive() const {
    return this->m_eLifeState == eLifeState::ALIVE;
}

bool Player::team(const Player& player) const {
    return this->m_eTeam == player.getTeam();
}

bool Player::equal(const Player& player) const {
    return this->m_dwBaseAddr == player.getBaseAddr();
}
