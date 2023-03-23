#include <Arduino.h>
#include <Wire.h>
#include <ezButton.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Adafruit_VL53L0X.h"
#include <AMS_AS5600.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

ezButton button1(PC14);
ezButton button2(PC15);

#define TURN 69.0
#define CELL 315.0
#define BRAKEF 75.0
#define BRAKET 5.0
//#define SLEFT (240 - (8.3 - batteryLevel) * 25)
#define SLEFT 235
#define SRIGHT 255

float offsetL = 0, offsetR = 0;

long lastSample = 0;
float distL, distR;

float batteryLevel = 8.0;

// variable initialization
int unvisited[324] = {0};
int unvisited_i = 0;
bool maze[324][4] = {false};
bool visited[324] = {false};
int moves[324] = {0};
int moves_i = 0;
int path[324] = {0};
int path_i = 0;
int path_j = 0;
int current = 19;
int orient = 0;
char directions[] = {'n', 'e', 's', 'w'};
int dir[] = {18, 1, -18, -1};

bool ex = false;
int ocurrent;

int p[324] = {0};

void setup()
{
  Serial.begin(9600);
  pinMode(PC14, INPUT_PULLUP);
  pinMode(PC15, INPUT_PULLUP);

  Serial1.begin(9600);
  pinMode(PA7, INPUT);

  pinMode(PA0, OUTPUT);
  pinMode(PA1, OUTPUT);
  pinMode(PA2, OUTPUT);
  pinMode(PA3, OUTPUT);

  vlx_setup();

  checkMagnetPresence1(); //check the magnet (blocks until magnet is found)
  checkMagnetPresence2();

  while (digitalRead(PC14))
  {
    updateEncoder();
    Serial.println("Waiting...");
  }
  int timer = millis();
  while (millis() < timer + 1000)
  {
    updateEncoder();
  }
  offsetL = calculate_distanceL();
  offsetR = calculate_distanceR();

  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  button1.setDebounceTime(50);
  button2.setDebounceTime(50);

//  while(1)
//    _loop();
}

void loop()
{
  if (ex)
  {
    ex = false;
    ackReset();
    ocurrent = current;
    current = 19;
    orient = 0;
  }
  button1.loop();
  button2.loop();
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 1);
  display.println(" DESHRET MK 2   " + String(batteryLevel));
  display.drawLine(0, 10, display.width() - 1, 10, SSD1306_WHITE);
  display.setCursor(10, 14);
  display.println("Start Over");
  display.setCursor(10, 26);
  display.println("Resume Mapping");
  display.setCursor(10, 38);
  display.println("Run Shortest Path");
  display.setCursor(10, 50);
  display.println("(Testing Code)");
  int selection = 1;
  while (1)
  {
    display.setTextColor(SSD1306_BLACK);
    display.setCursor(2, 14);
    display.println(">");
    display.setCursor(2, 26);
    display.println(">");
    display.setCursor(2, 38);
    display.println(">");
    display.setCursor(2, 50);
    display.println(">");
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(2, 2 + 12 * selection);
    display.println(">");
    display.display();
    while (1)
    {
      button1.loop();
      button2.loop();
      if (button1.isPressed())
      {
        selection = selection % 4 + 1;
        break;
      }
      if (button2.isPressed())
      {
        display.clearDisplay();
        display.setCursor(12, 20);
        display.println("Press Right Button");
        display.setCursor(12, 32);
        display.println("To Confirm");
        display.display();

        while (button2.isPressed())
          button2.loop();

        while (1)
        {
          button1.loop();
          button2.loop();
          if (button1.isPressed())
          {
            return;
          }
          if (button2.isPressed())
          {
            display.setCursor(0, 1);
            display.clearDisplay();
            display.println("Starting in 3...");
            display.display();
            delay(1000);
            display.clearDisplay();
            display.println("Starting in 2...");
            display.display();
            delay(1000);
            display.clearDisplay();
            display.println("Starting in 1...");
            display.display();
            delay(1000);
            button2.loop();

            display.clearDisplay();
            display.setCursor(12, 20);
            display.println("Hold Left Button");
            display.setCursor(12, 32);
            display.println("To Reset");
            display.display();

            if (selection == 1)
            {
              for (int i = 0; i < 18; i++)
                for (int j = 0; j < 18; j++)
                {
                  setColor(j - 1, i - 1, 'k');
                  for (int k = 4; k--;)
                    clearWall(j - 1, i - 1, directions[k]);
                }
              memset(unvisited, 0, sizeof(unvisited));
              unvisited_i = 0;
              memset(maze, false, sizeof(maze));
              memset(visited, false, sizeof(visited));
              memset(moves, 0, sizeof(moves));
              moves_i = 0;
              memset(path, 0, sizeof(path));
              path_i = 0;
              path_j = 0;
              current = 19;
              orient = 0;
              memset(p, 0, sizeof(p));
              phase();
            }
            else if (selection == 2)
            {
              bfs(19);

              int v = ocurrent;
              path_i = 0, path_j = 0;
              while (v != -1)
              {
                path[path_i] = v;
                path_i++;
                v = p[v];
              }

              path_i--;
              sprint(path, path_j, path_i);

              phase();
            }
            else if (selection == 3)
            {
              phase2();
            }
            else if (selection == 4)
            {
              test();
            }

            return;
          }
        }
        break;
      }
    }
  }
}

void test()
{
  while(1)
    _loop();
}
