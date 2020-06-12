#ifndef BREAD_OS_BPRINT_H
#define BREAD_OS_BPRINT_H
enum class Color
{
  Black = 0x0,
  Blue = 0x1,
  Green = 0x2,
  Cyan = 0x3,
  Red = 0x4,
  Magenta = 0x5,
  Brown = 0x6,
  LightGray = 0x7,
  DarkGray = 0x8,
  LightBlue = 0x9,
  LightGreen = 0xA,
  LightCyan = 0xB,
  LightRed = 0xC,
  Pink = 0xD,
  Yellow = 0xE,
  White = 0xF
};

class ScreenChar
{
public:
  explicit ScreenChar(char ch,
                      Color fontColor = Color::Black,
                      Color backgroundColor = Color::White) : _code(ch), _fontColor(fontColor), _backgroundColor(backgroundColor) {}

  // fixme: remove this
  short getContent() const
  {
    return (static_cast<char>(_fontColor) << 12) | (static_cast<char>(_backgroundColor) << 8) | _code;
  }

private:
  char _code;
  Color _fontColor;
  Color _backgroundColor;
};

#endif //BREAD_OS_BPRINT_H
