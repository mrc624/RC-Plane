#ifndef COMMANDS_H
#define COMMANDS_H 

enum Debug_Flag
{
  DBG_ESP_NOW,
  DBG_MAIN,
  NUM_DEBUGS
};

enum Serial_Color
{
  COLOR_Red,
  COLOR_Yellow,
  COLOR_Green,
  COLOR_Blue,
  NUM_COLORS
};

bool Debug_Enabled(Debug_Flag flag);
void Serial_Print(String text, Debug_Flag flag);
void Serial_Print(String text, Debug_Flag flag, Serial_Color color);
void Serial_Println(String text, Debug_Flag flag);
void Serial_Println(String text, Debug_Flag flag, Serial_Color color);

#endif