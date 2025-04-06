#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <DHT.h>

#define DHTPIN 5          // DHT11 on D1
#define DHTTYPE DHT11
#define RELAY_PIN 4       // Relay on D2
#define WIFI_SSID "Solace tel Touseef"
#define WIFI_PASSWORD "satti786"
#define FIREBASE_HOST "https://final-year-project-iot-88e97-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define FIREBASE_SECRET "AIzaSyB1oyNBslbCgeMod4WRLSoAhkg-xMznNXY"  //  web api or db secret

FirebaseData fbdo;
FirebaseConfig config;
FirebaseAuth auth;
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("Starting Firebase Integration...");

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);  // OFF (active-low)
  dht.begin();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi: ");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  config.database_url = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_SECRET;

  // Print host and secret
  Serial.print("Host: ");
  Serial.println(config.database_url.c_str());  // MB_String needs .c_str()
  Serial.print("Secret: ");
  Serial.println(config.signer.tokens.legacy_token);  // Already const char*, no .c_str()

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  Serial.print("Connecting to Firebase");
  unsigned long startTime = millis();
  while (!Firebase.ready() && (millis() - startTime < 30000)) {
    delay(500);
    Serial.print(".");
  }
  if (Firebase.ready()) {
    Serial.println(" Connected to Firebase");
    Firebase.setFloat(fbdo, "/sensors/humidity", 0);
    Firebase.setFloat(fbdo, "/sensors/temperature", 0);
    Firebase.setInt(fbdo, "/pump", 0);
    Firebase.setBool(fbdo, "/manualOverride", true);
  } else {
    Serial.println(" Failed to connect to Firebase");
    Serial.println("Error: " + String(fbdo.errorReason().c_str()));
  }
}

void loop() {
  if (Firebase.ready()) {
    // Read DHT11
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();
    if (isnan(humidity) || isnan(temperature)) {
      Serial.println("Failed to read DHT11!");
    } else {
      Serial.print("Humidity: ");
      Serial.print(humidity);
      Serial.print("%  Temperature: ");
      Serial.print(temperature);
      Serial.println("°C");
      Firebase.setFloat(fbdo, "/sensors/humidity", humidity);
      Firebase.setFloat(fbdo, "/sensors/temperature", temperature);
    }

    // Relay control
    if (Firebase.getBool(fbdo, "/manualOverride") && fbdo.boolData()) {
      if (Firebase.getInt(fbdo, "/pump")) {
        int pumpState = fbdo.intData();
        digitalWrite(RELAY_PIN, pumpState ? LOW : HIGH);  // Active-low
        Serial.print("Pump State: ");
        Serial.println(pumpState ? "ON" : "OFF");
      }
    } else {
      digitalWrite(RELAY_PIN, HIGH);  // OFF if manualOverride is false
      Serial.println("Pump: OFF (manual override disabled)");
    }
  } else {
    Serial.println("Firebase not connected");
  }
  delay(2000);
}