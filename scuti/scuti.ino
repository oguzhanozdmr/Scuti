#include <DS3231.h>
DS3231 rtc(SDA, SCL);
Time time_clock;

#define SUN_PIN 9


//you can find the sun path on https://www.suncalc.org/#/-24.4684,133.0223,3/2021.02.09/03:18/1/3

#define DAWN_H 5
#define DAWN_M 57
#define DAWN_LIGHT_VALUE 254

#define SUNRISE_H 6
#define SUNRISE_M 21
#define SUNRISE_LIGHT_VALUE 252

#define CULMINATION_H 12
#define CULMINATION_M 53
#define CULMINATION_LIGHT_VALUE 120

#define SUNSET_H 19
#define SUNSET_M 25
#define SUNSET_LIGHT_VALUE 237

#define DUSK_H 19
#define DUSK_M 48
#define DUSK_LIGHT_VALUE 248

#define NIGHT_H 20
#define NIGHT_M 15
#define NIGHT_LIGHT_VALUE 255

int dawn = 0;
int sunrise = 0;
int culmination = 0;
int sunset = 0;
int dusk = 0;
int night = 0;

byte light_value = 0;

void setup()
{
  pinMode(SUN_PIN, OUTPUT);
  analogWrite(SUN_PIN, NIGHT_LIGHT_VALUE);

  Serial.begin(9600);

  rtc.begin();
  
  dawn = get_minute(DAWN_H, DAWN_M);
  sunrise = get_minute(SUNRISE_H, SUNRISE_M);
  culmination = get_minute(CULMINATION_H, CULMINATION_M);
  sunset = get_minute(SUNSET_H, SUNSET_M);
  dusk = get_minute(DUSK_H, DUSK_M);
  night = get_minute(NIGHT_H, NIGHT_M);
}

void loop()
{
  time_clock = rtc.getTime();
  int now_time = get_minute((byte)time_clock.hour, (byte)time_clock.min);

  
  if (now_time >= night || now_time < dawn)
  {
    change_light(NIGHT_LIGHT_VALUE);
  }
  else if (now_time >= dawn && now_time < sunrise)
  {
    change_light(calculate_light_value(now_time, dawn, DAWN_LIGHT_VALUE, sunrise, SUNRISE_LIGHT_VALUE));
  }
  else if (now_time >= sunrise && now_time < culmination)
  {
    change_light(calculate_light_value(now_time, sunrise, SUNRISE_LIGHT_VALUE, culmination, CULMINATION_LIGHT_VALUE));
  }
  else if (now_time >= culmination && now_time < sunset)
  {
    change_light(calculate_light_value(now_time, culmination, (byte)CULMINATION_LIGHT_VALUE, sunset, (byte)SUNSET_LIGHT_VALUE));
  }
  else if (now_time >= sunset && now_time < dusk)
  {
    change_light(calculate_light_value(now_time, sunset, SUNSET_LIGHT_VALUE, dusk, DUSK_LIGHT_VALUE));
  }
  else if (now_time >= dusk && now_time < night)
  {
    change_light(calculate_light_value(now_time, dusk, DUSK_LIGHT_VALUE, night, NIGHT_LIGHT_VALUE));
  }

  delay(60000);
}


void change_light(byte n_light_value)
{
  if (n_light_value != light_value)
  {
    light_value = n_light_value;
    analogWrite(SUN_PIN, light_value);
  }
}


byte calculate_light_value(int current_time, int min_time, byte min_light_value, int max_time, byte max_light_value)
{
  return (byte)map(current_time, min_time, max_time, min_light_value, max_light_value);
}


int get_minute(byte h, byte min) {
  return (60 * h) + min;
}
