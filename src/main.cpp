#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C display(0x27, 16, 2);

const float SOUND_SPEED = 0.0343;

const short trig_pin = 13;
const short echo_pin = 14;

unsigned long last_time = 0;

void setup()
{
  // На ESP8266 за замовчуванням Wire використовує SDA=SDA(D2), SCL=SCL(D1)
  // Ініціалізуємо I2C шину
  Wire.begin(D2, D1);

  // Ініціалізація дисплея
  display.init(); // Для деяких версій бібліотеки використовуйте lcd.init();

  // Вмикаємо підсвічування екрана
  display.backlight();

  // Очищаємо екран про всяк випадок
  display.clear();

  // Встановлюємо курсор на початок (0-й стовпчик, 0-й рядок)
  // display.setCursor(0, 0);
  // display.print("Hello, Wemos!"); // Виводимо текст у першому рядку

  // // Переносимо курсор на другий рядок (0-й стовпчик, 1-й рядок)
  // display.setCursor(0, 1);
  // display.print("ESP8266 I2C LCD");

  pinMode(trig_pin, OUTPUT); // Установлюємо trigPin як Вихід
  pinMode(echo_pin, INPUT);  // Установлюємо echoPin як Вхід
}

void loop()
{
  unsigned long cur_time = millis();
  if (cur_time - last_time >= 750)
  {
    last_time = cur_time;
    // Очищаємо trigPin перед початком імпульсу
    digitalWrite(trig_pin, LOW);
    delayMicroseconds(2);

    // Генеруємо ультразвуковий імпульс: подаємо HIGH на 10 мікросекунд
    digitalWrite(trig_pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig_pin, LOW);

    long duration = pulseIn(echo_pin, HIGH);

    // Розраховуємо відстань: (час * швидкість звуку) ділимо на 2
    // Ділимо на 2, оскільки звук летить до об'єкта і назад
    short distanceCm = duration * SOUND_SPEED / 2;

    display.setCursor(0, 0);
    display.print("Distance:       "); // Пробіли затирають старі довгі числа
    display.setCursor(10, 0);          // Зміщуємо курсор туди, де мають бути цифри

    if (distanceCm > 0 && distanceCm < 400)
    {
      display.print(distanceCm);
      display.print(" cm");
    }
    else
    {
      display.print("Error"); // Якщо датчик поза зоною або не підключений
    }
  }
}