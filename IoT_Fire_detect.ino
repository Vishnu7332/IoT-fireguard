#define BLYNK_TEMPLATE_ID "TMPL32807va4w"
#define BLYNK_TEMPLATE_NAME "Forest Fire Detector"
#define BLYNK_AUTH_TOKEN "8ErdtriC3OvwjVBdq2z_1Zol0yW7aw4Z"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

// ----------- Blynk Credentials ----------
char auth[] = "8ErdtriC3OvwjVBdq2z_1Zol0yW7aw4Z";
char ssid[] = "OnePlus Nord CE3 5G";
char pass[] = "00000000";

// ----------- Pins and Sensors -----------
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define MQ2_PIN 34
#define FLAME_PIN 32

#define BUZZER_PIN 27

#define RGB_GREEN 26

void setup() 
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  dht.begin();

  pinMode(MQ2_PIN, INPUT);
  pinMode(FLAME_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RGB_GREEN, OUTPUT);

}

void loop() 
{
  Blynk.run();

  float dhtTemp = dht.readTemperature();                       // DHT11 Temp
  float humidity = dht.readHumidity();                         // DHT11 Humidity
  int mq2Value = analogRead(MQ2_PIN);                          // MQ-2 Gas
  int flameState = analogRead(FLAME_PIN);                      // Flame sensor

  // Send sensor data to Blynk
  Blynk.virtualWrite(V1, dhtTemp);
  Blynk.virtualWrite(V2, humidity);
  Blynk.virtualWrite(V3, mq2Value);
  Blynk.virtualWrite(V4, flameState);


  //serial moniter
  Serial.println("===== Sensor Readings =====");
  Serial.print("MQ2 Smoke Level     : "); Serial.println(mq2Value);
  Serial.print("DHT11 Temp (°C)     : "); Serial.println(dhtTemp);
  Serial.print("DHT11 Humidity (%)  : "); Serial.println(humidity);
  Serial.print("Flame Detected?     : "); Serial.print(flameState);
  if (flameState < 100) Serial.println("   🔥 YES");
  else Serial.println("   ❌ NO");

  Serial.println("===========================\n");
  
  // Fire detection logic
  int fireDetected;
  if((humidity < 30)||(mq2Value > 1000)||(flameState<100))
  {
    fireDetected = 1;
  }
  else 
  {
    fireDetected = 0;
  }
  
  if (fireDetected==1) 
  {
    digitalWrite(BUZZER_PIN, HIGH);
    Blynk.logEvent("fire_alert", "🔥 Fire Detected! Check Forest Area Immediately");
  } 
  else 
  {
    digitalWrite(BUZZER_PIN, LOW);
    analogWrite(RGB_GREEN, 255); // Safe - Green
  }

  delay(5000); // Adjust based on real-time needs
}
