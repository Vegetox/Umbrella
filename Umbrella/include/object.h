#pragma once

#include <vector>
#include <Windows.h>

const std::vector<int> vecExcludedClass = { 36, 40, 52 };

class Object {
public:
     Object();

     Object(const DWORD& dwBaseAddr, const DWORD& dwEntBaseAddr, const int& iClassId, const int& iGlowIndex);

    ~Object();

    DWORD getBaseAddr() const;
    void setBaseAddr(const DWORD& dwBaseAddr);

    DWORD getEntBaseAddr() const;
    void setEntBaseAddr(const DWORD& dwEntBaseAddr);

    int getClassId() const;
    void setClassId(const int& iClassId);

    int getGlowIndex() const;
    void setGlowIndex(const int& iGlowIndex);

    bool exclude() const;

private:
    DWORD m_dwBaseAddr;
    DWORD m_dwEntBaseAddr;
    int m_iClassId;
    int m_iGlowIndex;
};
