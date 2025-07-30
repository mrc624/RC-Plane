#ifndef COMMANDS_H
#define COMMANDS_H 

enum Serial_Color
{
  COLOR_Red,
  COLOR_Yellow,
  COLOR_Green,
  COLOR_Blue,
  NUM_COLORS
};

void Print_Color(Serial_Color color, String text);

#endif