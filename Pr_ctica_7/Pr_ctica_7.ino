/*
  Fundacion Kinal
  Centro Educativo Tecnico Laborla Kinal
  Electronica
  Grado: Quinto
  Seccion: A
  Curso: Taller de eleectronica digital y reparacion de computadoras I
  Nombre: Juan Manuel Sebastian Ixen Coy
  Carne: 2019519
  Fecha: 12/05
*/
#include <Servo.h>
#include <Adafruit_NeoPixel.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define Neopixel 3
#define Sensor 4
#define R 5
#define B 6
#define G 7
#define Buzzer 8
#define Puerta 9
#define PIN_SERVO 10
#define Rele1 11
#define Rele2 12
#define Foco1 A0
#define Foco2 A1
int ValorPuerta = 0;
int ValorFoco1 = 0;
int ValorFoco2 = 0;

Servo servo;
Adafruit_NeoPixel neopixel(7, Neopixel, NEO_GRB + NEO_KHZ800);
OneWire ourWire(2);
DallasTemperature sensors(&ourWire);
LiquidCrystal_I2C LCD_Jixen(0x27, 16, 2);


//Caracteres para la LCD

byte OF[] = {
    B01110,
    B10001,
    B10001,
    B10001,
    B01010,
    B01110,
    B01110,
    B00100
};
byte ON[] = {
    B01110,
    B11111,
    B11111,
    B11111,
    B01110,
    B01110,
    B01110,
    B00100
};

byte Frio[] = {
    B00000,
    B00000,
    B01110,
    B11011,
    B10111,
    B11111,
    B01110,
    B00000
};
byte Media[] = {
    B00000,
    B00010,
    B00110,
    B01100,
    B01110,
    B11111,
    B11111,
    B01110
};
byte Calor[] = {
    B00000,
    B00000,
    B00010,
    B00100,
    B10010,
    B01111,
    B11111,
    B01110
};
byte Persona[] = {
    B01110,
    B01110,
    B01110,
    B00100,
    B01110, 
    B10101,
    B00100,
    B01010
};

void setup()
{
    pinMode(Neopixel, OUTPUT);
    pinMode(Sensor, INPUT);
    pinMode(Puerta, INPUT);
    pinMode(B, OUTPUT);
    pinMode(G, OUTPUT);
    pinMode(R, OUTPUT);
    pinMode(Buzzer, OUTPUT);
    pinMode(Rele1, OUTPUT);
    pinMode(Rele2, OUTPUT);
    pinMode(Foco1, INPUT);
    pinMode(Foco2, INPUT);
    digitalWrite(Rele1, HIGH);
    digitalWrite(Rele2, HIGH);
    servo.attach(PIN_SERVO);
    sensors.begin();
    LCD_Jixen.init();
    LCD_Jixen.backlight();
    LCD_Jixen.createChar(0, Frio);
    LCD_Jixen.createChar(1, Media);
    LCD_Jixen.createChar(2, Calor);
    LCD_Jixen.createChar(3, Persona);
    LCD_Jixen.createChar(4, OF);
    LCD_Jixen.createChar(5, ON);
    LCD_Jixen.setCursor(0, 1);
    LCD_Jixen.print("P:close");
    LCD_Jixen.setCursor(9, 1);
    LCD_Jixen.print("F:");
    LCD_Jixen.write(4);
    LCD_Jixen.setCursor(13, 1);
    LCD_Jixen.print("F:");
    LCD_Jixen.write(4);
}

void loop()
{
    int temperaturaCelsius = obtenerTemperaturaCelsius();
    controlPuerta();
    controlFoco1();
    controlFoco2();
    temperatura(temperaturaCelsius);
    Movimiento();
}




void temperatura(int temperaturaCelsius) //Contlor de la temperatura
{
    if (temperaturaCelsius > 14 && temperaturaCelsius < 22)
    {
        digitalWrite(R, HIGH);
        digitalWrite(B, HIGH);
        digitalWrite(G, LOW);
    }
    if (temperaturaCelsius > 21 && temperaturaCelsius < 26)
    {
        digitalWrite(R, LOW);
        digitalWrite(B, LOW);
        digitalWrite(G, HIGH);
    }
    if (temperaturaCelsius > 25 && temperaturaCelsius < 46)
    {
        digitalWrite(R, HIGH);
        digitalWrite(B, LOW);
        digitalWrite(G, LOW);
    }
}

int obtenerTemperaturaCelsius()
{
    sensors.requestTemperatures();
    float temp = sensors.getTempCByIndex(0);
    return static_cast<int>(temp);
}

void mostrarTemperaturaLCD(int temperaturaCelsius, int temperaturaFahrenheit, int icono)
{
    LCD_Jixen.setCursor(0, 0);
    LCD_Jixen.print("Temp:");
    LCD_Jixen.write(icono);
    LCD_Jixen.setCursor(6, 0);
    LCD_Jixen.print(temperaturaCelsius);
    LCD_Jixen.setCursor(12, 0);
    LCD_Jixen.print(temperaturaFahrenheit);
    LCD_Jixen.write(0xDF);
    LCD_Jixen.print("C");
}

void Movimiento()//Control del buzzer y el senso
{
    bool Mov = digitalRead(Sensor);
    if (Mov == 1)
    {
        LCD_Jixen.setCursor(9, 0);
        LCD_Jixen.print("Radar:");
        LCD_Jixen.write(3);
        for (int i = 0; i < 8; i++)
        {
            neopixel.setPixelColor(i, neopixel.Color(255, 0, 0));
            neopixel.show();
        }
        digitalWrite(Buzzer, HIGH);
        delay(250);
        for (int i = 0; i < 8; i++)
        {
            neopixel.setPixelColor(i, neopixel.Color(124, 252, 0));
            neopixel.show();
        }
        LCD_Jixen.setCursor(9, 0);
        LCD_Jixen.print("Radar: ");
        digitalWrite(Buzzer, LOW);
        delay(250);
    }
    if (Mov == 0)
    {
        for (int i = 0; i < 8; i++)
        {
            neopixel.setPixelColor(i, neopixel.Color(0, 0, 255));
            neopixel.show();
        }
        LCD_Jixen.setCursor(9, 0);
        LCD_Jixen.print("Radar: ");
    }
}


void controlFoco1()//control del foco 1
{
    if (digitalRead(Foco1) && ValorFoco1 == 0)
    {
        digitalWrite(Rele1, LOW);
        LCD_Jixen.setCursor(9, 1);
        LCD_Jixen.print("F:");
        LCD_Jixen.write(5);
        delay(500);
        ValorFoco1 = 1;
    }
    if (digitalRead(Foco1) && ValorFoco1 == 1)
    {
        digitalWrite(Rele1, HIGH);
        LCD_Jixen.setCursor(9, 1);
        LCD_Jixen.print("F:");
        LCD_Jixen.write(4);
        delay(500);
        ValorFoco1 = 0;
    }
}

void controlFoco2()//Control del foco 2
{
    if (digitalRead(Foco2) && ValorFoco2 == 0)
    {
        digitalWrite(Rele2, LOW);
        LCD_Jixen.setCursor(13, 1);
        LCD_Jixen.print("F:");
        LCD_Jixen.write(5);
        delay(500);
        ValorFoco2 = 1;
    }
    if (digitalRead(Foco2) && ValorFoco2 == 1)
    {
        digitalWrite(Rele2, HIGH);
        LCD_Jixen.setCursor(13, 1);
        LCD_Jixen.print("F:");
        LCD_Jixen.write(4);
        delay(500);
        ValorFoco2 = 0;
    }
}
void controlPuerta()// control del la puerta
{
    if (digitalRead(Puerta) && ValorPuerta == 0)
    {
        servo.write(0);
        LCD_Jixen.setCursor(0, 1);
        LCD_Jixen.print("P:Abierto ");
        delay(500);
        ValorPuerta = 1;
    }
    if (digitalRead(Puerta) && ValorPuerta == 1)
    {
        servo.write(90);
        LCD_Jixen.setCursor(0, 1);
        LCD_Jixen.print("P:Cerrado ");
        delay(500);
        ValorPuerta = 0;
    }
}
