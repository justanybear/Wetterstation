# ESP32 Wetterstation

Ein umfassendes IoT-Wetterüberwachungssystem basierend auf dem ESP32-Mikrocontroller, das Umweltdaten misst und Echtzeitüberwachung durch eine Webschnittstelle ermöglicht.

## 📋 Inhaltsverzeichnis

- [Überblick](#überblick)
- [Features](#features)
- [Hardware-Anforderungen](#hardware-anforderungen)
- [Installation](#installation)
- [Konfiguration](#konfiguration)
- [Verwendung](#verwendung)
- [Web-Interface](#web-interface)
- [Hardware-Setup](#hardware-setup)
- [Fehlerbehebung](#fehlerbehebung)

## 🌟 Überblick

Dieses Projekt implementiert eine voll funktionsfähige Wetterstation mit dem ESP32-C3 Mikrocontroller. Das System sammelt Umweltdaten wie Temperatur, Luftfeuchtigkeit und Magnetfelderkennung und stellt diese Informationen sowohl lokal auf einem OLED-Display als auch remote über eine responsive Webschnittstelle dar.

Die Wetterstation arbeitet als eigenständiges IoT-Gerät mit folgenden Funktionen:
- Echtzeit-Sensordatenerfassung und -verarbeitung
- Lokale Datenvisualisierung über OLED-Display
- Webbasierte Fernüberwachung und -steuerung
- Konfigurierbare Statusanzeige durch RGB-LED
- Persistente WiFi-Konfigurationsverwaltung
- Datenvalidierung und Fehlerbehandlung

## ✨ Features

### Sensorfunktionen
- **Temperaturmessung**: Präzise Temperaturerfassung mit DHT11-Sensor
- **Luftfeuchtigkeitsmessung**: Relative Luftfeuchtigkeit in Prozent
- **Magnetfelddetektion**: Reed-Kontakt B12_1 für Magnetpräsenz
- **Datenvalidierung**: Automatische Erkennung und Filterung ungültiger Messwerte

### Display & Visualisierung
- **OLED-Display**: Lokale Anzeige aller Messwerte und Systemstatus
- **RGB-LED-Statusanzeige**: Farbkodierte Systemzustände
  - 🔴 Rot: Keine WLAN-Verbindung
  - 🔵 Blau: Messung aktiv
  - 🟠 Orange: Temperatur zu hoch
- **Anpassbare LED-Farben**: Individuelle Farbkonfiguration über Web-Interface

### Netzwerk & Konnektivität
- **WiFi-Manager**: Automatische WLAN-Konfiguration mit Fallback auf Access Point
- **Webserver**: Integrierter HTTP-Server für Remote-Zugriff
- **SPIFFS-Dateisystem**: Lokale Speicherung von Web-Dateien
- **JSON-API**: RESTful-Schnittstelle für Datenabfrage

### Web-Interface
- **Responsive Design**: Optimiert für Desktop und Mobile
- **Echtzeit-Updates**: Live-Aktualisierung der Messwerte
- **RGB-Color-Picker**: Interaktive LED-Farbsteuerung
- **Zeitstempel**: Anzeige der letzten Messung
- **LED-Steuerung**: Ein-/Ausschalten der Status-LED

## 🔧 Hardware-Anforderungen

### Hauptkomponenten
| Komponente | Modell | Beschreibung | Anzahl |
|------------|---------|--------------|---------|
| Mikrocontroller | ESP32-C3 | Haupt-MCU mit WiFi/Bluetooth | 1 |
| Temperatursensor | DHT11 | Digital Temperatur & Luftfeuchtigkeit | 1 |
| Magnetsensor | Reed Contact B12_1 | Magnetfelderkennungssensor | 1 |
| Display | OLED (128x64) | Lokale Datenanzeige | 1 |
| LED | RGB-LED | Statusanzeige (auf ESP32 integriert) | 1 |

### Zusätzliche Materialien
- Breadboard oder PCB für Prototyping
- Jumper-Kabel (Male-to-Male, Male-to-Female)
- Pull-up Widerstände (10kΩ für Reed-Kontakt)
- Stromversorgung (USB oder 3.3V/5V Adapter)
- Gehäuse (optional, für Außeneinsatz)

### Datenfluss
1. **Sensorerfassung**: DHT11 und Reed-Kontakt werden in konfigurierbaren Intervallen abgefragt
2. **Datenvalidierung**: Ungültige Werte werden gefiltert und verworfen
3. **Lokale Anzeige**: Gültige Daten werden auf dem OLED-Display dargestellt
4. **Status-LED**: RGB-LED zeigt aktuellen Systemzustand an
5. **Web-Bereitstellung**: Daten werden über HTTP-Server als JSON bereitgestellt
6. **Remote-Zugriff**: Web-Interface ermöglicht Echtzeitüberwachung und -steuerung

## 📦 Installation

### Voraussetzungen
- Arduino IDE 2.0 oder höher
- ESP32 Board Package für Arduino IDE
- USB-Kabel für ESP32-Programmierung

### Arduino IDE Setup
1. **Board Manager konfigurieren**:
   ```
   Datei → Voreinstellungen → Zusätzliche Boardverwalter-URLs:
   https://dl.espressif.com/dl/package_esp32_index.json
   ```

2. **ESP32 Board installieren**:
   ```
   Werkzeuge → Board → Boardverwalter → "ESP32" suchen und installieren
   ```

3. **Erforderliche Bibliotheken installieren**:
   ```
   Bibliotheksverwalter → Folgende Bibliotheken installieren:
   - DHT sensor library (Adafruit)
   - Adafruit SSD1306
   - Adafruit GFX Library
   - ESPAsyncWebServer
   - AsyncTCP
   - WiFiManager
   ```

### Repository klonen
```bash
git clone https://github.com/[username]/esp32-wetterstation.git
cd esp32-wetterstation
```

### Upload-Konfiguration
1. Board auswählen: `ESP32C3 Dev Module`
2. Port auswählen: Entsprechender COM-Port
3. Upload-Geschwindigkeit: `921600`
4. Flash-Größe: `4MB`

## ⚙️ Konfiguration

### WiFi-Konfiguration
Das System verwendet WiFiManager für die automatische Netzwerkkonfiguration:

1. **Erster Start**: ESP32 erstellt Access Point "ESP32-WeatherStation"
2. **Konfiguration**: Mit AP verbinden und http://192.168.4.1 öffnen
3. **WLAN auswählen**: Verfügbare Netzwerke werden angezeigt
4. **Zugangsdaten eingeben**: SSID und Passwort eingeben
5. **Automatische Verbindung**: Bei Neustarts automatische Verbindung

### Sensor-Kalibrierung
```cpp
// DHT11 Kalibrierungsoffsets (falls erforderlich)
#define TEMP_OFFSET 0.0   // Temperatur-Offset in °C
#define HUM_OFFSET 0.0    // Luftfeuchtigkeits-Offset in %

// Messintervall anpassen
#define MEASUREMENT_INTERVAL 2000  // Millisekunden zwischen Messungen
```

### LED-Konfiguration
```cpp
// Standard-LED-Farben definieren
#define COLOR_NO_WIFI     0xFF0000  // Rot
#define COLOR_MEASURING   0x0000FF  // Blau  
#define COLOR_HIGH_TEMP   0xFF8000  // Orange
#define COLOR_NORMAL      0x00FF00  // Grün
```

## 🖥️ Verwendung

### Systemstart
1. ESP32 mit Strom versorgen
2. Warten auf OLED-Display-Initialisierung
3. WiFi-Verbindung wird automatisch hergestellt
4. IP-Adresse wird auf Display angezeigt
5. Webserver startet automatisch

### Lokale Überwachung
Das OLED-Display zeigt kontinuierlich:
- Aktuelle Temperatur (°C)
- Luftfeuchtigkeit (%)
- Magnetstatus (Erkannt/Nicht erkannt)
- WiFi-Verbindungsstatus
- IP-Adresse
- Letzte Aktualisierung

### Remote-Zugriff
1. IP-Adresse vom Display ablesen
2. Webbrowser öffnen: `http://[IP-ADRESSE]`
3. Dashboard mit Echtzeitdaten wird geladen

## 🌐 Web-Interface

### Dashboard-Features
- **Messwerte-Anzeige**: Große, gut lesbare Anzeigen für alle Sensordaten
- **Trend-Grafiken**: Historische Datenvisualisierung (optional)
- **Status-Indikatoren**: Farbkodierte Systemzustände
- **Zeitstempel**: Genaue Zeit der letzten Messung

### LED-Steuerung
- **RGB-Color-Picker**: Intuitive Farbauswahl mit Hex-Werten
- **Vordefinierte Farben**: Schnellauswahl für häufig verwendete Farben
- **Ein/Aus-Schalter**: LED komplett deaktivieren
- **Speichern**: Einstellungen bleiben nach Neustart erhalten

### Mobile Optimierung
- Responsive Design für Smartphones und Tablets
- Touch-optimierte Bedienelemente
- Automatische Skalierung für verschiedene Bildschirmgrößen

### Häufige Probleme

#### WiFi-Verbindungsprobleme
**Problem**: ESP32 kann sich nicht mit WLAN verbinden
```
Symptome:
- Rote LED leuchtet dauerhaft
- Display zeigt "No WiFi"
- Access Point wird erstellt

Lösungen:
1. WLAN-Zugangsdaten über AP neu eingeben
2. Router-Kompatibilität prüfen (2.4GHz erforderlich)
3. Signal-stärke überprüfen
4. Factory Reset: GPIO0 beim Boot gedrückt halten
```

#### Sensor-Leseprobleme
**Problem**: DHT11 liefert ungültige Werte
```
Symptome:
- NaN-Werte auf Display
- Sporadische Messwerte
- Fehlermeldungen im Serial Monitor

Lösungen:
1. Verkabelung überprüfen
2. Stromversorgung stabilisieren
3. Sensor ersetzen (DHT11 ist günstig aber ungenau)
4. Messintervall vergrößern (min. 2 Sekunden)
```

#### Display-Probleme
**Problem**: OLED-Display bleibt schwarz
```
Symptome:
- Keine Anzeige beim Start
- I2C-Kommunikationsfehler

Lösungen:
1. I2C-Adresse scannen (meist 0x3C oder 0x3D)
2. Pull-up Widerstände auf SDA/SCL (4.7kΩ)
3. Stromversorgung prüfen (3.3V stabil)
4. Display-Bibliothek aktualisieren
```

#### Webserver-Probleme
**Problem**: Webseite nicht erreichbar
```
Symptome:
- Browser zeigt "Seite nicht gefunden"
- Timeout-Fehler

Lösungen:
1. IP-Adresse vom Display ablesen
2. Firewall-Einstellungen prüfen
3. Gleiche Netzwerk-Subnet verwenden
4. SPIFFS-Upload überprüfen
```

### Debug-Modi

#### Serial Monitor aktivieren
```cpp
#define DEBUG_MODE 1  // Debug-Ausgaben aktivieren

void setup() {
  if (DEBUG_MODE) {
    Serial.begin(115200);
    Serial.println("=== ESP32 Weather Station Debug ===");
  }
}
```

### Testing
- Hardware-Tests auf echter ESP32-Hardware
- Web-Interface auf verschiedenen Browsern testen
- Dokumentation bei Änderungen aktualisieren

Copyright (c) 2025 Laurenz Hartenstein
```

### Libraries & Frameworks
- **Espressif Systems** - ESP32 Platform und Dokumentation
- **Adafruit** - Sensor-Libraries und Hardware-Support
- **Arduino Community** - IDE und Core-Libraries
- **AsyncWebServer** - Efficient Web Server Implementation

### Inspirationen & Ressourcen
- [Adafruit Learning System](https://learn.adafruit.com/) - Hardware-Tutorials
- [ESP32 Official Documentation](https://docs.espressif.com/projects/esp32/en/latest/)
- [Arduino Reference](https://www.arduino.cc/reference/en/) - Programming Reference
- **TGM E-Learning Platform** - Projektgrundlagen und Anforderungen

---

## 📊 Projektstatistiken

- **Entwicklungszeit**: ~20 Stunden
- **Code-Zeilen**: ~800 Zeilen C++/Arduino
- **Hardware-Kosten**: ~25-35€
- **Stromverbrauch**: ~150mA @ 3.3V (aktiv)
- **Reichweite**: WiFi-abhängig (typisch 10-50m)
- **Messgenauigkeit**: ±2°C, ±5% RH (DHT11-bedingt)



*Für Fragen, Anregungen oder Support besuchen Sie die [Issues](https://github.com/[username]/esp32-wetterstation/issues) Sektion oder kontaktieren Sie mich direkt.*
