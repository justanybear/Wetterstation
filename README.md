# Wetterstation
SYT-Aufgabe-2025


 
Inhaltsverzeichnis:
Einführung	
Projektbeschreibung	
Theorie
Arbeitsschritte	
Sensoren beschrieben	
WLAN-Verbindung	
Webserver	
Status RGB-LED	
Code ESP32C3	
Code Erklärung	
Zusammenfassung	
Quellen	


 
Einführung
Im Rahmen des SYT-Unterrichts haben wir eine Wetterstation entwickelt, die Umweltdaten wie Temperatur, Luftfeuchtigkeit sowie den Status eines Magnetsensors misst und über eine Webseite anzeigt. Die Daten werden vom Mikrocontroller (ESP32) über WLAN gesammelt, verarbeitet und dargestellt. Ziel war es, ein funktionales IoT-Gerät zu bauen, das Messwerte bereinigt und mit visueller Rückmeldung (LED & Display) ausgibt.

Die Schaltung wurde rund um den ESP32-Mikrocontroller aufgebaut. Angeschlossen wurden folgende Komponenten:
1.	DHT11
2.	Reed Contact B12_1
3.	Display
4.	Auf dem ESP integrierte LED
Projektbeschreibung
Die Wetterstation misst Temperatur, Luftfeuchtigkeit sowie den Status eines Magnetkontaktsensors. Die gesammelten Daten werden über WLAN auf einer Webseite angezeigt, wobei auch eine Status-LED sowie ein OLED-Display zur direkten Ausgabe verwendet werden.
Theorie
Für die Umsetzung des Projekts waren Kenntnisse im Bereich Mikrocontrollerprogrammierung, Webserverbetrieb, Sensorauswertung sowie digitaler Schaltungen notwendig. Die Temperatur- und Feuchtigkeitsmessung erfolgt über den digitalen Sensor DHT11. Ein Reed Contact B12_1 wird zur Erkennung eines magnetischen Feldes genutzt. Zur Visualisierung dient ein OLED-Display, während der ESP32 die zentrale Steuerung übernimmt. Die Verbindung zum WLAN erfolgt über die Bibliothek WiFiManager, die es ermöglicht, Netzwerkeinstellungen über einen Access Point vorzunehmen. Die Webseite wird vom ESP32 als Webserver bereitgestellt und zeigt aktuelle Messwerte sowie weitere Bedienelemente wie einen RGB-Colorpicker zur Steuerung der Status-LED.
Arbeitsschritte
Zuerst haben wir die elektrische Schaltung rund um den ESP32 erstellt. Dabei wurden der DHT11-Sensor, der Reed-Contact, das RGB-LED-Display angeschlossen Anschließend wurde der Code für die Sensorabfrage und Displayausgabe entwickelt.
Die WLAN-Verbindung erfolgt über WiFi.begin(), wobei das Netzwerk "Iphone 14" genutzt wurde. Nach erfolgreicher Verbindung wird die IP-Adresse auf dem OLED-Display angezeigt. Der ESP32 startet dann einen Webserver, welcher HTML-Dateien aus dem SPIFFS-Dateisystem lädt und unter anderem einen JSON-Endpunkt zur Verfügung stellt.
Die Webseite zeigt die aktuellen Messwerte an, stellt einen RGB-Colorpicker zur Steuerung der LED bereit und erlaubt das Ein- und Ausschalten der Statusanzeige. Die RGB-LED signalisiert Zustände wie "Kein WLAN" (Rot), "Messung aktiv" (Blau) oder "Temperatur zu hoch" (Orange).
Bei jedem Schleifendurchlauf werden die Sensorwerte aktualisiert, auf dem Display angezeigt und bei Verfügbarkeit über die Webseite ausgeliefert. Eine einfache Fehlerbehandlung ist implementiert, die ungültige Sensorwerte erkennt und ignoriert.



Sensoren beschrieben:
DHT11: Der DHT11 misst die Temperatur (in Grad Celsius) und die Luftfeuchtigkeit (in %) digital und eignet sich für einfache Wetteranwendungen.
Reed Contact B12_1: Der Reed Contact B12_1 erkennt ob ein Magnet in seiner Nähe anliegt und lässt eine LED leuchten wenn einer da ist.
WLAN-Verbindung
Der ESP32 nutzt WiFiManager, um sich entweder mit einem vorhandenen WLAN zu verbinden oder selbst als Access Point aufzutreten. Die Verbindungseinstellungen bleiben auch nach einem Neustart gespeichert. Die IP-Adresse wird angezeigt und ermöglicht den Zugriff auf das Webinterface.


Webserver
Der ESP32 betreibt einen einfachen Webserver, der über WLAN erreichbar ist und eine Webseite bereitstellt, auf der alle aktuellen Messdaten wie Temperatur, Luftfeuchtigkeit und der Status des Magnetsensors angezeigt werden. Zusätzlich kann über einen RGB-Colorpicker die Farbe der Status-LED individuell angepasst und dauerhaft gespeichert werden. Die Webseite zeigt außerdem Zeitstempel der letzten Messung an, ermöglicht das Ein- und Ausschalten der LED und funktioniert sowohl im WLAN-Betrieb als auch im Access-Point-Modus.
Status RGB-LED
Die RGB-LED des ESP32 zeigt verschiedene Systemzustände durch unterschiedliche Farben an, z. B. Rot bei fehlender WLAN-Verbindung, Blau während einer Messung und Orange bei zu hoher Temperatur. Die Farben lassen sich über das Webinterface individuell anpassen und dauerhaft speichern.
Code Erklärung
Der oben angegebene Code startet mit der Initialisierung des Displays und der WLAN-Verbindung. Danach werden die Sensorwerte in festen Intervallen ausgelesen und sowohl auf dem Display als auch per Webserver bereitgestellt. Fehlerhafte Sensorwerte werden dabei ignoriert.
Zusammenfassung
Im Projekt wurde erfolgreich eine Wetterstation mit einem ESP32-Mikrocontroller realisiert, die Temperatur, Luftfeuchtigkeit sowie einen Magnetkontakt ausliest und visuell auf einem Display sowie über eine Webseite darstellt. Schwierigkeiten traten anfangs bei der Ansteuerung des OLED-Displays sowie beim Einbinden des SPIFFS-Dateisystems auf. Diese konnten durch gezielte Recherche und Codeanpassungen behoben werden.
Auch die Implementierung der WLAN-Verbindung erforderte wiederholte Tests, insbesondere beim Übergang vom Access Point in den Client-Modus. Insgesamt wurde das Projekt erfolgreich umgesetzt und erfüllt die gestellten Anforderungen.

 
Quellen
OpenAI.com
Adafruit arduino
https://learn.adafruit.com/adafruit-qt-py-esp32-c3-wifi-dev-board/arduino-ide-setup
Github	 
https://github.com/me-no-dev/ESPAsyncWebServer
Espressif Systems datasheet https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf
Elearning kurs
https://elearning.tgm.ac.at/mod/assign/view.php?id=153854
