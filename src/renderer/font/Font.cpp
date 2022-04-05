//////////////////////////////////////////////////////////////////
// Orchid 2D Framework
// Version: Alpha 0.1
// File: Font.cpp
// Purpose: - This Class represents DirectX Fonts.
//////////////////////////////////////////////////////////////////

// Includes:
#include "Font.h"

// Functions:
// Public Functions:
Font::Font()
	:
	isInitialized(false)
{
}
Font::~Font()
{
}

void Font::Initialize(const LPDIRECT3DDEVICE9 d3dDevice, const std::string& font, const DWORD& fontSize,
	const bool& italic, const bool& bold)
{
	if (isInitialized)
	{
		// 1st: want to reinitialize it so shutdown it first:
		if (this->font != nullptr)
		{
			this->font->Release();
			this->font = nullptr;
		}
	}

	isInitialized = false;

	// 2nd: Create the font!
	D3DXCreateFontA(d3dDevice, fontSize, 0, bold ? FW_BOLD : FW_NORMAL, 1, italic, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, font.c_str(), &this->font);

	isInitialized = true;
}
void Font::Shutdown()
{
	if (font != nullptr)
	{
		font->Release();
		font = nullptr;
	}

	isInitialized = false;
}

void Font::Write(const std::string string, const int& x, const int& y, const D3DCOLOR& color)
{
	if (isInitialized == false)
	{
		// Not correctly initialized
		return;
	}

	RECT pos;
	SetRect(&pos, x, y, 1080, 1920);
	font->DrawTextA(NULL, string.c_str(), -1, &pos, DT_LEFT | DT_WORDBREAK, color);
}
bool Font::Initialized()
{
	return isInitialized;
}