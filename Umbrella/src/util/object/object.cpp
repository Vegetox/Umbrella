#include "object.h"

Object::Object() {
    this->m_dwBaseAddr = NULL;
    this->m_dwEntBaseAddr = NULL;
    this->m_iClassId = NULL;
    this-> m_iGlowIndex = NULL;
}

Object::Object(const DWORD& dwBaseAddr, const DWORD& dwEntBaseAddr, const int& iClassId, const int& iGlowIndex) {
    this->m_dwBaseAddr = dwBaseAddr;
    this->m_dwEntBaseAddr = dwEntBaseAddr;
    this->m_iClassId = iClassId;
    this->m_iGlowIndex = iGlowIndex;
}

Object::~Object() {}

DWORD Object::getBaseAddr() const {
    return this->m_dwBaseAddr;
}

void Object::setBaseAddr(const DWORD& dwBaseAddr) {
    this->m_dwBaseAddr = dwBaseAddr;
}

DWORD Object::getEntBaseAddr() const {
    return this->m_dwEntBaseAddr;
}

void Object::setEntBaseAddr(const DWORD& dwEntBaseAddr) {
    this->m_dwEntBaseAddr = dwEntBaseAddr;
}

int Object::getClassId() const {
    return this->m_iClassId;
}

void Object::setClassId(const int& iClassId) {
    this->m_iClassId = iClassId;
}

int  Object::getGlowIndex() const {
    return this->m_iGlowIndex;
}

void  Object::setGlowIndex(const int& iGlowIndex) {
    this->m_iGlowIndex = iGlowIndex;
}

bool Object::exclude() const {
    return std::find(vecExcludedClass.begin(), vecExcludedClass.end(), this->m_iClassId) != vecExcludedClass.end();
}
