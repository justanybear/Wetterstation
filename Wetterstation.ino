#include <DHT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <Preferences.h>
#include <Adafruit_NeoPixel.h>


#define DHTPIN       3
#define LED_PIN     8       // Der Pin, an dem der NeoPixel angeschlossen ist
#define NUMPIXELS   1       // Anzahl der LEDs
Adafruit_NeoPixel pixels(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

#define MAGNET_PIN   2
#define OLED_SDA     4 
#define OLED_SCL     5 
#define DHTTYPE DHT11
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64



TwoWire myWire = TwoWire(0);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &myWire, -1);
DHT dht(DHTPIN, DHTTYPE);
AsyncWebServer server(80);
Preferences preferences;

float humidity, tempC;

// RGB-Struktur für LED-Farben
struct RGBColor {
  int r, g, b;
};

// Status-Farben (Standard-Werte)
struct StatusColors {
  String off = "#000000";        // Display aus (Magnet entfernt)
  String normal = "#28a745";     // Normal Betrieb
  String error = "#dc3545";      // Sensor Fehler  
  String connecting = "#007bff"; // Verbindung läuft
} statusColors;

enum SystemState { STATE_OFF, STATE_NORMAL, STATE_ERROR, STATE_CONNECTING };
SystemState currentState = STATE_CONNECTING;

String getStateString() {
  switch (currentState) {
    case STATE_OFF: return "aus";
    case STATE_NORMAL: return "normal";
    case STATE_ERROR: return "fehler";
    case STATE_CONNECTING: return "verbindet";
  }
  return "unbekannt";
}

String getCurrentStatusColor() {
  switch (currentState) {
    case STATE_OFF: return statusColors.off;
    case STATE_NORMAL: return statusColors.normal;
    case STATE_ERROR: return statusColors.error;
    case STATE_CONNECTING: return statusColors.connecting;
  }
  return "#666666";
}

// Funktion um Hex-Farbe in RGB zu konvertieren

RGBColor hexToRGB(String hexColor) {
  RGBColor color = {0, 0, 0};
  
  // Entferne '#' falls vorhanden
  if (hexColor.startsWith("#")) {
    hexColor = hexColor.substring(1);
  }
  
  // Konvertiere Hex zu RGB
  if (hexColor.length() == 6) {
    color.r = strtol(hexColor.substring(0, 2).c_str(), NULL, 16);
    color.g = strtol(hexColor.substring(2, 4).c_str(), NULL, 16);
    color.b = strtol(hexColor.substring(4, 6).c_str(), NULL, 16);
  }
  
  return color;
}

// LED entsprechend dem Status setzen
void updateLED() {
  pinMode(LED_BUILTIN, OUTPUT);
  String currentColor = getCurrentStatusColor();
  RGBColor rgb = hexToRGB(currentColor);
  
  // PWM-Werte direkt verwenden (0-255)
  int redValue = rgb.r;
  int greenValue = rgb.g;
  int blueValue = rgb.b;

   pixels.setPixelColor(0, pixels.Color(rgb.r, rgb.g, rgb.b));
  pixels.show();
  
  
  Serial.printf("🔴 LED Status: %s | RGB(%d,%d,%d)\n", 
                getStateString().c_str(), rgb.r, rgb.g, rgb.b);
}

void loadColorSettings() {
  preferences.begin("colors", false);
  
  statusColors.off = preferences.getString("off", "#000000");
  statusColors.normal = preferences.getString("normal", "#28a745");
  statusColors.error = preferences.getString("error", "#dc3545");
  statusColors.connecting = preferences.getString("connecting", "#007bff");
  
  preferences.end();
  
  Serial.println("✅ Farb-Einstellungen geladen:");
  Serial.println("  Display Aus: " + statusColors.off);
  Serial.println("  Normal: " + statusColors.normal);
  Serial.println("  Fehler: " + statusColors.error);
  Serial.println("  Verbindung: " + statusColors.connecting);
}

void saveColorSetting(String state, String color) {
  preferences.begin("colors", false);
  preferences.putString(state.c_str(), color);
  preferences.end();
  
  Serial.println("💾 Farbe gespeichert: " + state + " = " + color);
  
  // LED-Status nach Farbänderung aktualisieren
  updateLED();
}

void setup() {
  Serial.begin(9600);
  Serial.println("🚀 ESP32 Umwelt-Monitor startet...");
  pixels.begin();       // NeoPixel starten
  pixels.setBrightness(50);
  
  
  // PWM für RGB LED einrichten (ESP32C3 kompatibel)
  ledcSetup(0, 5000, 8);  // Kanal 0, 5kHz, 8-bit für C3
  ledcSetup(1, 5000, 8);  // Kanal 1, 5kHz, 8-bit für C3
  ledcSetup(2, 5000, 8);  // Kanal 2, 5kHz, 8-bit für C3

  // Farb-Einstellungen laden
  loadColorSettings();
  
  dht.begin();
  pinMode(MAGNET_PIN, INPUT);
  myWire.begin(OLED_SDA, OLED_SCL);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("❌ OLED nicht gefunden!"));
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("ESP32 Monitor");
  display.println("Display startet...");
  display.display();
  delay(2000);

  // WiFi-Konfiguration
  const char* ssid = "Iphone 14";
  const char* password = "12345678";

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Verbinde WLAN...");
  display.display();

  // LED auf "connecting" setzen
  currentState = STATE_CONNECTING;
  updateLED();

  // Netzwerk-Scan
  Serial.println("🔍 Suche verfügbare Netzwerke...");
  int n = WiFi.scanNetworks();
  Serial.println("Verfügbare Netzwerke:");
  for (int i = 0; i < n; ++i) {
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(WiFi.SSID(i));
    Serial.print(" (");
    Serial.print(WiFi.RSSI(i));
    Serial.println(" dBm)");
  }

  WiFi.begin(ssid, password);
  Serial.print("🔌 Verbinde mit: ");
  Serial.println(ssid);
  
  // WiFi-Verbindung mit Timeout
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(1000);
    Serial.print(".");
    display.print(".");
    display.display();
    attempts++;
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\n❌ WLAN-Verbindung fehlgeschlagen!");
    Serial.print("WiFi Status: ");
    Serial.println(WiFi.status());
    
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("WLAN Fehler!");
    display.println("Prüfe Netzwerk");
    display.display();
    
    currentState = STATE_ERROR;
    updateLED();
  } else {
    Serial.println("\n✅ WLAN verbunden!");
    Serial.print("📶 Netzwerk: ");
    Serial.println(WiFi.SSID());
    Serial.print("📡 IP-Adresse: ");
    Serial.println(WiFi.localIP());
    Serial.print("📊 Signalstärke: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");

    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("✅ WLAN OK!");
    display.print("IP: ");
    display.println(WiFi.localIP());
    display.display();
    delay(3000);
    
    currentState = STATE_NORMAL;
    updateLED();
  }

  if (!SPIFFS.begin()) {
    Serial.println("❌ SPIFFS konnte nicht gestartet werden");
  } else {
    Serial.println("✅ SPIFFS gestartet");
  }

  // Web-Server Routen
  
  // HTML-Seite bereitstellen
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });

  // Farb-Einstellungen empfangen und speichern
  server.on("/setColor", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("state") && request->hasParam("value")) {
      String state = request->getParam("state")->value();
      String color = request->getParam("value")->value();
      
      // Farbe in Struct und Preferences speichern
      if (state == "off") {
        statusColors.off = color;
      } else if (state == "normal") {
        statusColors.normal = color;
      } else if (state == "error") {
        statusColors.error = color;
      } else if (state == "connecting") {
        statusColors.connecting = color;
      }
      
      saveColorSetting(state, color);
      Serial.println("🎨 Farbe aktualisiert: " + state + " = " + color);
    }
    request->send(200, "text/plain", "OK");
  });

  // Sensor-Daten und Status als JSON
  server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request){
    String json = "{";
    json += "\"temp\":" + String(tempC, 1) + ",";
    json += "\"hum\":" + String(humidity, 1) + ",";
    json += "\"pres\":1013,";
    json += "\"state\":\"" + getStateString() + "\",";
    json += "\"statusColor\":\"" + getCurrentStatusColor() + "\",";
    json += "\"colors\":{";
    json += "\"off\":\"" + statusColors.off + "\",";
    json += "\"normal\":\"" + statusColors.normal + "\",";
    json += "\"error\":\"" + statusColors.error + "\",";
    json += "\"connecting\":\"" + statusColors.connecting + "\"";
    json += "},";
    json += "\"wifi\":{";
    json += "\"connected\":" + String(WiFi.status() == WL_CONNECTED ? "true" : "false") + ",";
    json += "\"rssi\":" + String(WiFi.RSSI()) + ",";
    json += "\"ip\":\"" + WiFi.localIP().toString() + "\"";
    json += "}";
    json += "}";
    
    request->send(200, "application/json", json);
  });

  // Alle Farben zurücksetzen
  server.on("/resetColors", HTTP_GET, [](AsyncWebServerRequest *request){
    preferences.begin("colors", false);
    preferences.clear();
    preferences.end();
    
    // Standard-Farben wiederherstellen
    statusColors.off = "#000000";
    statusColors.normal = "#28a745";
    statusColors.error = "#dc3545";
    statusColors.connecting = "#007bff";
    
    // LED aktualisieren
    updateLED();
    
    Serial.println("🔄 Farben auf Standard zurückgesetzt");
    request->send(200, "text/plain", "Farben zurückgesetzt");
  });

  server.begin();
  Serial.println("🌐 Webserver gestartet auf: http://" + WiFi.localIP().toString());
}

void loop() {
  delay(2000);

  // WiFi-Status überwachen
  if (WiFi.status() != WL_CONNECTED) {
    currentState = STATE_CONNECTING;
    updateLED();
    Serial.println("⚠️ WiFi-Verbindung verloren, versuche Reconnect...");
    
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("WiFi verloren");
    display.println("Reconnect...");
    display.display();
    
    WiFi.reconnect();
    delay(5000);
    
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("❌ Reconnect fehlgeschlagen");
      currentState = STATE_ERROR;
      updateLED();
    } else {
      Serial.println("✅ Reconnect erfolgreich");
      currentState = STATE_NORMAL;
      updateLED();
    }
    return;
  }

  // Magnet-Status prüfen
  int magnetStatus = digitalRead(MAGNET_PIN);
  if (magnetStatus == HIGH) {
    currentState = STATE_OFF;
    updateLED();
    display.clearDisplay();
    display.display();
    Serial.println("🔕 Kein Magnet erkannt – Display aus");
    return;
  }

  // Sensor-Daten lesen
  humidity = dht.readHumidity();
  tempC = dht.readTemperature();

  if (isnan(humidity) || isnan(tempC)) {
    currentState = STATE_ERROR;
    updateLED();
    Serial.println("⚠️ Fehler beim Lesen vom DHT11!");
    
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Sensor Fehler!");
    display.println("DHT11 prüfen");
    display.println("");
    display.println("Status: " + getStateString());
    display.display();
    return;
  }

  // Normal-Betrieb
  currentState = STATE_NORMAL;
  updateLED();
  Serial.printf("🌡️ %.1f°C | 💧 %.1f%% | Status: %s\n", tempC, humidity, getStateString().c_str());

  // Display aktualisieren
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  
  display.println("ESP32 Monitor");
  display.println("==============");
  display.printf("Temp: %.1f C\n", tempC);
  display.printf("Hum:  %.1f %%\n", humidity);
  display.println("");
  display.printf("Status: %s\n", getStateString().c_str());
  
  // WiFi-Info
  if (WiFi.status() == WL_CONNECTED) {
    display.printf("WiFi: %d dBm", WiFi.RSSI());
  } else {
    display.println("WiFi: Fehler");
  }
  
  display.display();
}
