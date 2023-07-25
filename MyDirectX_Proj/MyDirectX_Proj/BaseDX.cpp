//***************************************************************************************
// @author : zxz
// @file :
// @description :
//***************************************************************************************

#include "BaseDX.h"

using namespace Microsoft::WRL;


BaseDX::BaseDX(uint32_t width, uint32_t height, std::wstring name):
    m_width(width),
    m_height(height),
    m_title(name)
{
    m_aspectRatio = static_cast<float>(width / height);
}

BaseDX::~BaseDX()
{
}


void BaseDX::SetCustomWindowText(LPCWSTR text)
{
    std::wstring windowText = m_title + L": " + text;
    SetWindowText(WinApplication::GetHwnd(), windowText.c_str());
}