enum class Color {
  Black = 0,
  Blue = 1,
  Green = 2,
  Cyan = 3,
  Red = 4,
  Magenta = 5,
  Brown = 6,
  LightGray = 7,
  DarkGray = 8,
  LightBlue = 9,
  LightGreen = 10,
  LightCyan = 11,
  LightRed = 12,
  Pink = 13,
  Yellow = 14,
  White = 15
};

class ScreenChar {
public:
  explicit ScreenChar(char ch, Color color = Color::White) : _code(ch), _color(color) {}
private:
  char _code;
  Color _color;
};