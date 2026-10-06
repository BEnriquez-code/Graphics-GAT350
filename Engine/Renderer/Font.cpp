#include "pch.h"
#include "Font.h"

#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>

nu::Font::~Font() {
	if (m_ttfFont != nullptr) {
		TTF_CloseFont(m_ttfFont);
	}
}

bool nu::Font::Load(const std::string& name, float fontSize) {
	m_ttfFont = TTF_OpenFont(name.c_str(), fontSize);
	if (m_ttfFont == nullptr) {
		std::cerr << "Could not load font: " << name << std::endl;
		return false;
	}

	return true;
}