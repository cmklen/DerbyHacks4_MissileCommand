#ifndef MASTER_H
#define MASTER_H

#include <ESP8266WiFi.h>
#include <Wire.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include "SSD1306Wire.h"
#include <stdint.h>
#include "defender.h"
#include "attacker.h"
#include "bitmaps.h"

//#define F_CPU 160000000L

//Beginning State definitions
#define INIT_STATE    0
#define DEFEND_INIT   1
#define DEFEND_STATE  2
#define DEFEND_EXIT   3
#define ATTACK_INIT   4
#define ATTACK_STATE  5
#define ATTACK_EXIT   6


//base defs
#define BASE_L      0x10
#define BASE_C      0x20
#define BASE_R      0x30

#define BASE_000HP  1
#define BASE_025HP  2
#define BASE_050HP  3
#define BASE_075HP  4
#define BASE_100HP  5

#endif
