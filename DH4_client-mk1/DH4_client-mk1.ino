#include "master.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <math.h>
#include <vector> 

const char* ssid     = "miscom_host";   //missile command host
//const char* password = "l33t";

char *to_disp;
Base base(0x14, 0x24, 0x34);
bool has_missile = 0;

uint8_t state = 0;
uint8_t x_pos = 0;
uint8_t y_pos = 0;

IPAddress host_IP(13,37,6,9);
IPAddress local_IP(13,37,6,20);
IPAddress gateway(13,37,6,0);
IPAddress subnet(255,255,255,0);

WiFiClient client;
WiFiServer server(80);

//SSD1306Wire  display(0x3C, 5, 4);
Adafruit_SSD1306 display(-1);



void run_scheduler( void );
void task_display( void );
void task_display_baseL( void );
void task_display_baseC( void );
void task_display_baseR( void );
void task_serial_rx( void );
void is_damaged( uint8_t missile_x );

void check_state( void );
void host_setup( void );
void client_setup( void );
void reset_state( void );
void poll_connected( void );
void recieve_fb( void );
void write_fb( uint8_t damaged_base );

void defend( void );
void attack( void );
void animateMissile( void );
void victory( void );
void defeat( void );


//missile time
Missile m(10,0,0);
Missile m1(10,0,0);
Missile m2(10,0,0);
Missile m3(10,0,0);
Missile m4(10,0,0);
Missile m5(10,0,0);
Missile m6(10,0,0);
Missile m7(10,0,0);
Missile m8(10,0,0);
Missile m9(10,0,0);
Missile m10(10,0,0);
Missile missileContainer []= {m1,m2,m3,m4,m5,m6,m7,m8,m9,m10};


int16_t x_1=0,x_2=0,y_1=0,y_2=0;

typedef struct
{
  uint32_t previous_millis;
  uint32_t elapsed_millis;
  uint32_t timeout_millis;
  void (*callback)();
} timer_type;

static timer_type scheduler_table[] =
{
  {0, 0, 100,  &task_display        },
  {0, 0, 130,  &task_display_baseL  },
  {0, 0, 150,  &task_display_baseC  },
  {0, 0, 170,  &task_display_baseR  },
  {0, 0, 300,  &task_serial_rx      },
  {0, 0, 270,  &check_state         },
  {0, 0, 230,  &animateMissile      }
  
};

void scheduler_run()
{
  // Run each timer in the scheduler table, and call
  for (uint32_t i = 0; i < sizeof(scheduler_table)/sizeof(timer_type); i++)
  {
    // Note: millis() will overflow after ~50 days.  
    uint32_t currentMillis = millis();
    timer_type *t = &scheduler_table[i];    
    t->elapsed_millis += currentMillis - t->previous_millis;
    t->previous_millis = currentMillis;
    if (t->elapsed_millis >= t->timeout_millis)
    {
      t->elapsed_millis = 0;
      t->callback();
    }
  }
}

void task_display( void )
{
  //Serial.println("trying graphics");
	// Display bitmap
	display.drawBitmap(0, 0,  screen_no_bases, 128, 64, WHITE);
	display.display();
  
}

void is_damaged( uint8_t missile_x )
{
  if (missile_x >27 && missile_x < 34)
  {
    base.attacc_base(BASE_L);
    write_fb(BASE_L | base.getBaseHP(BASE_L));
  }
  else if (missile_x >58 && missile_x < 66)
  {
    base.attacc_base(BASE_C);
    write_fb(BASE_C | base.getBaseHP(BASE_C));
  }
  else if (missile_x >90 && missile_x < 100)
  {
    base.attacc_base(BASE_R);
    write_fb(BASE_R | base.getBaseHP(BASE_R));
  }
  
}

void task_display_baseL( void )
{
  display.fillScreen(BLACK);
  switch (base.getBaseHP(BASE_L) & 0x0F)
  {
    case 0x00:
      display.drawBitmap(29, 56, bmp_base_00 , 5, 5, WHITE);
      break;
    case 0x01:
      display.drawBitmap(29, 56, bmp_base_25 , 5, 5, WHITE);
      break;
    case 0x02:
      display.drawBitmap(29, 56, bmp_base_50 , 5, 5, WHITE);
      break;
    case 0x03:
      display.drawBitmap(29, 56, bmp_base_75 , 5, 5, WHITE);
      break;
    case 0x04:
      display.drawBitmap(29, 56, bmp_base_100 , 5, 5, WHITE);
      break;
  }
  
}

void task_display_baseC( void )
{
  switch (base.getBaseHP(BASE_C) & 0x0F)
  {
    case 0x00:
      display.drawBitmap(61, 56, bmp_base_00 , 5, 5, WHITE);
      break;
    case 0x01:
      display.drawBitmap(61, 56, bmp_base_25 , 5, 5, WHITE);
      break;
    case 0x02:
      display.drawBitmap(61, 56, bmp_base_50 , 5, 5, WHITE);
      break;
    case 0x03:
      display.drawBitmap(61, 56, bmp_base_75 , 5, 5, WHITE);
      break;
    case 0x04:
      display.drawBitmap(61, 56, bmp_base_100 , 5, 5, WHITE);
      break;
  }
}

void task_display_baseR( void )
{
  switch (base.getBaseHP(BASE_R) & 0x0F)
  {
    case 0x00:
      display.drawBitmap(94, 56, bmp_base_00 , 5, 5, WHITE);
      break;
    case 0x01:
      display.drawBitmap(94, 56, bmp_base_25 , 5, 5, WHITE);
      break;
    case 0x02:
      display.drawBitmap(94, 56, bmp_base_50 , 5, 5, WHITE);
      break;
    case 0x03:
      display.drawBitmap(94, 56, bmp_base_75 , 5, 5, WHITE);
      break;
    case 0x04:
      display.drawBitmap(94, 56, bmp_base_100 , 5, 5, WHITE);
      break;
  }
}

void task_serial_rx( void )
{
  uint8_t serial_rx = Serial.parseInt();
  char flag = Serial.read();

  switch (flag)
  {
    case 's':
      state = uint8_t(serial_rx);
      break;
    case 'x':
      x_pos = serial_rx;
      break;
    case 'y':
      y_pos = serial_rx;
      break;
    case 't':
      to_disp = "got t";
      break;
    case 'i':
      Serial.println(WiFi.localIP());
      break;
    case 'n':
      Serial.println(WiFi.softAPgetStationNum());
      break;
    case 'b':
      switch (serial_rx)
      {
        case 1:
          base.attacc_base(BASE_L);
          break;
        case 2:
          base.attacc_base(BASE_C);
          break;
        case 3:
          base.attacc_base(BASE_R);
          break;
      }
      break;
    default:
      //Serial.print("\nDefault flag\n");
      //to_disp = "got naught";
      break;
  }
  
}

void host_setup()
{
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid);
  WiFi.softAPConfig(host_IP,gateway,subnet);
  
  server.begin();
  
  Serial.print("Set up using this IP: ");
  Serial.println(WiFi.softAPIP());

  client = server.available();
  while (!client)
  {
    Serial.print("\n[Client not connected]\n");
//    delay(500);
    client = server.available();
  }
  if (client)
  {
    Serial.print("\n[Client connected]\n");
    state += 1;
//    player_obj(DEFENDER);
  }
  else
  {
    Serial.print("no client...\n");
  }
  
  
}

void client_setup()
{

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid);
  Serial.print("Conencting to: ");
  Serial.print(host_IP);
  while(WiFi.status() != WL_CONNECTED)
  {
      delay(500);
      Serial.print(".");
  }
  //client.connect(local_IP,80);
  
  if(!client.connect(host_IP,80))
  {
    Serial.println("connection failed, we'll get em next time");
    return;
  }
  else
  {
    Serial.println("Connection Success, Beginning attack state");
    state += 1;
  }
  
}

void reset_state( void )
{
  state = 0;
  WiFi.softAPdisconnect(true);
  //client disconnect
  WiFi.disconnect(true);
  //reset other stuff when it exists
  
}

void check_state( void )
{
  switch (state)
  {
    case INIT_STATE:
      to_disp = "Menu State\n1s -> defend\n4s -> attack";
      
      break;
    case DEFEND_INIT:
      to_disp = "Defend init";
      host_setup();
      //state += 1;
      break;
    case DEFEND_STATE:
      to_disp = "Defend";
      defend();
      break;
    case DEFEND_EXIT:
      to_disp = "Defend exit";
      reset_state();
      defeat();
      break;
    case ATTACK_INIT:
      to_disp = "Attack init";
      client_setup();
      //state += 1;
      break;
    case ATTACK_STATE:
      to_disp = "Attack";
      attack();
      break;
    case ATTACK_EXIT:
      to_disp = "Attack exit";
      reset_state();
      victory();
      break;
    
    default:
      to_disp = "default";
      break;
  }
}

void attack( void ) //this handles sending data to the defender
{ //missile(vel, ang, xpos);
  Missile new_missile(10, 0, 0);
  uint8_t *to_send = new_missile.send_arr();
  Serial.print("Writing this data:");
//  for (uint8_t c = 0; c<3; c++)
//  {
//    client.write(to_send[c]);
//    Serial.print(to_send[c]);
//  }
  client.write(new_missile.getX());
  Serial.print(new_missile.getX());
  client.write('M');
  Serial.print("M\n");
  client.flush();
  //delay(2000);
  recieve_fb();
}

void animateMissile( void )
{
  if(has_missile)
  {
    Serial.print("has_missile in animate\n");
    for(int c=0; c<20;c++)
    {
      x_1 = int16_t(missileContainer[c].getX());
      y_1 = int16_t(missileContainer[c].getY());
      
      x_2 = int16_t((missileContainer[c].getVel()+x_1));
      y_2 = int16_t((missileContainer[c].getVel()+y_1));
  
      display.drawLine(x_1,y_1,x_2,y_2,WHITE);
      
      if(y_2 >= 56)
      {
        missileContainer[c].setVel(0);
        display.drawCircle(x_2,y_2,3,WHITE);
  //      delay(5);
        display.drawCircle(x_2,y_2,6,WHITE);
  //      delay(5);
        display.drawCircle(x_2,y_2,9,WHITE);
        is_damaged(x_2);
        missileContainer[c].setX(0);
        missileContainer[c].setY(0);
        break;
      }
      missileContainer[c].setX(x_2);
      missileContainer[c].setY(y_2);
    }
    
    has_missile = false;
    
  }
}

void defend( void ) //this handles recieving data from attacker 
{                   //it will also handle/talk to the handler for the display

  if (!has_missile)
  {
    byte data = client.read();
    byte flag = client.read();
    Serial.print(data, HEX);
    Serial.print(flag, HEX);
    
    if (flag == 0x4D)
    {
      has_missile = true;
      Serial.print("Fired a missile w/ x pos =");
      Serial.print(data);
      missileContainer[0].setX(data);
      
    }
  }
}

void recieve_fb( void )
{
  uint8_t data_rx = 0;
  uint8_t flag = 0;

  data_rx = client.read();
  flag = client.read();

  if (flag == 0x48)
  {
    Serial.print("Data byte:");
    Serial.println(data_rx, HEX);
    base.set_damage(data_rx & 0xF0, data_rx & 0x0F);
  }

  if (data_rx == 0x20)
  {
    state = 6; //exit and victory
  }
}

void write_fb( uint8_t base_data )
{
  client.write(base_data);
  client.write('H');

  if (base_data == 0x20)
  {
    state = 3;  //defeat and exit
  }
}

void victory( void )
{
  while(true)
  {
    display.clearDisplay();
    // Display bitmap
    display.drawBitmap(0, 0,  bmp_victory, 128, 64, WHITE);
    display.display();
  }
}

void defeat( void )
{
  while(true)
  {
    display.clearDisplay();
    // Display bitmap
    display.drawBitmap(0, 0,  bmp_defeat, 128, 64, WHITE);
    display.display();
  }
}

void setup() 
{ 
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println();
  Serial.print("Starting...\n");
  
  //host_setup();
  
  // initialize with the I2C addr 0x3C
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);



  // Clear the buffer.
  display.clearDisplay();
  // Display bitmap
  display.drawBitmap(0, 0,  bmp_title, 128, 64, WHITE);
  display.display();
  delay(2000);
  display.clearDisplay();

//  task_display();
//  display.drawBitmap(29, 56, bmp_base_100 , 5, 5, WHITE);
//  display.drawBitmap(61, 56, bmp_base_100 , 5, 5, WHITE);
//  display.drawBitmap(94, 56, bmp_base_100 , 5, 5, WHITE);
//  display.display();
  
}

void loop() 
{
  scheduler_run();
  
  
}
