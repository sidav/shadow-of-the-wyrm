#pragma once
#include <string>
#include <memory>
#include "ScreenComponent.hpp"

class TextComponent : public ScreenComponent
{
  public:
    TextComponent(const std::string& str);
    TextComponent(const std::string& str, const Colour c);
};

using TextComponentPtr = std::shared_ptr<TextComponent>;
