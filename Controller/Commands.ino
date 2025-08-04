#include "Commands.h"
#include "Joystick_Helper.h"

enum Commands
{
  CMD_ESP_Now_Initialized,
  CMD_Help,
  CMD_Mac_Address,
  CMD_Set_Centering,
  CMD_Type,
  Num_Commands
};

String Commands_ToString[Num_Commands] = 
{
  "ESP Now Initilization State",
  "Help",
  "Mac Address",
  "Set Joystick Centering",
  "Type"
};

bool Debug_ESP_NOW = true;
bool Debug_Joystick = true;

bool Debug_Enabled(Debug_Flag flag)
{
  switch(flag)
  {
    case DBG_ESP_NOW:
      return Debug_ESP_NOW;
      break;
    case DBG_JOYSTICK:
      return Debug_Joystick;
      break;
  }
  return false;
}

void Serial_Println(String text, Debug_Flag flag)
{
  Serial_Print(text, flag);
  Serial.println();
}

void Serial_Println(String text, Debug_Flag flag, Serial_Color color)
{
  Serial_Print(text, flag, color);
  Serial.println();
}

void Serial_Print(String text, Debug_Flag flag)
{
  if (Debug_Enabled(flag))
  {
    Serial.print(text);
  }
}

void Serial_Print(String text, Debug_Flag flag, Serial_Color color)
{
  if (Debug_Enabled(flag))
  {
    Print_Color(text, color);
  }
}

void Print_Color(String text, Serial_Color color)
{
  switch (color)
  {
    case COLOR_Red:
      Serial.print("\033[31m" + text + "\033[0m");
      break;
    case COLOR_Green:
      Serial.print("\033[32m" + text + "\033[0m");
      break;
    case COLOR_Blue:
      Serial.print("\033[34m" + text + "\033[0m");
      break;
    case COLOR_Yellow:
      Serial.print("\033[33m" + text + "\033[0m");
      break;
    default:
      Serial.print(text);
      break;
  }
}

void Handle_Commands()
{
  if (Serial.available() > 0)
  {
    String data = Serial.readStringUntil('\n');
    data.trim();
    Commands command = Num_Commands;

    for (int i = 0; i < Num_Commands; i++)
    {
      if (data.equals(Commands_ToString[i]))
      {
        command = (Commands)i;
        i = Num_Commands;
      }
    }
    switch(command)
    {
      case CMD_ESP_Now_Initialized:
        Print_ESP_Initialized();
        break;
      case CMD_Mac_Address:
        Print_Mac_Address();
        break;
      case CMD_Set_Centering:
        Set_Center_Values();
        break;
      case CMD_Type:
        Print_Type();
        break;
      default:
        Serial.println("Command Not Found");
      case CMD_Help:
        Print_Commands();
        break;
    }
  }
}

void Print_Commands()
{
  Serial.println("Commands:");
  for (int i = 0; i < Num_Commands; i++)
  {
    Serial.println(Commands_ToString[i]);
  }
}

void Print_ESP_Initialized()
{
  Serial.println("ESP Now:       " + String(ESP_Now_Initialized ? "Initialized" : "Not Initialized"));
}

void Print_Mac_Address()
{
  uint8_t baseMac[6];
  esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA, baseMac);
  if (ret == ESP_OK)
  {
    Serial.printf("Mac Address:   %02x:%02x:%02x:%02x:%02x:%02x\n",
                  baseMac[0], baseMac[1], baseMac[2],
                  baseMac[3], baseMac[4], baseMac[5]);
  } 
  else
  {
    Serial.println("Failed to read MAC address");
  }
}

void Print_Type()
{
  Serial.println("Type:          Controller");
}