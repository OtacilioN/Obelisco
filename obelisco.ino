#include <Arduino.h>
#include <WiFi.h>
#include <FirebaseESP32.h>
#include <ESP32Servo.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "uaifai-apolo"
#define WIFI_PASSWORD "bemvindoaocesar"

/* 2. If work with RTDB, define the RTDB URL and database secret */
#define DATABASE_URL "https://excellent-hue-425111-b0-default-rtdb.firebaseio.com/"
#define DATABASE_SECRET "1y9kBFaYK1vapVvKoeUBWtFAlORROdocrbxt1kps"

/* 3. Define the Firebase Data object */
FirebaseData fbdo;

/* 4, Define the FirebaseAuth data for authentication data */
FirebaseAuth auth;

/* Define the FirebaseConfig data for config data */
FirebaseConfig config;

unsigned long dataMillis = 0;
int count = 0;
int servoPin = 22;
Servo myservo;
int ADC_Max = 4096;

void setup()
{
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50); // Standard 50hz servo
  myservo.attach(servoPin, 500, 2400);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(34, INPUT);
  Serial.begin(9600);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the certificate file (optional) */
  // config.cert.file = "/cert.cer";
  // config.cert.file_storage = StorageType::FLASH;

  /* Assign the database URL and database secret(required) */
  config.database_url = DATABASE_URL;
  config.signer.tokens.legacy_token = DATABASE_SECRET;

  // Comment or pass false value when WiFi reconnection will control by your code or third party library e.g. WiFiManager
  Firebase.reconnectNetwork(true);

  // Since v4.4.x, BearSSL engine was used, the SSL buffer need to be set.
  // Large data transmission may require larger RX buffer, otherwise connection issue or data read time out can be occurred.
  fbdo.setBSSLBufferSize(4096 /* Rx buffer size in bytes from 512 - 16384 */, 1024 /* Tx buffer size in bytes from 512 - 16384 */);

  /* Initialize the library with the Firebase authen and config */
  Firebase.begin(&config, &auth);

  // Or use legacy authenticate method
  // Firebase.begin(DATABASE_URL, DATABASE_SECRET);
}
int lastVal = 0;
int potLastVal = 0;
int val;
void loop()
{
  if (millis() - dataMillis > 1000)
  {
    dataMillis = millis();
    Serial.printf("Set int... %s\n", Firebase.setInt(fbdo, "/test/timestamp", count++) ? "ok" : fbdo.errorReason().c_str());
    int iVal = 0;
    Firebase.getInt(fbdo, F("/test/position"), &iVal);
    digitalWrite(LED_BUILTIN, HIGH);
    if (iVal != lastVal)
    {
      lastVal = iVal;
      if (iVal > 150)
      {
        iVal = 150;
      }
      if (iVal < 65)
      {
        iVal = 65;
      }
      myservo.write(iVal);
      delay(4000);
    }

    val = analogRead(34);
    int rangePlus = potLastVal + 50;
    int rangeMinus = potLastVal - 50;
    Firebase.setInt(fbdo, "/test/pot", val);
    if (val > rangePlus || val < rangeMinus)
    {
      potLastVal = val;
      val = map(val, 0, ADC_Max, 65, 150); // scale it to use it with the servo (value between 0 and 180)
      myservo.write(val);
      Firebase.setInt(fbdo, "/test/position", val);
      delay(30);
    }
  }
}
