#include <ArduinoJson.h>
#include <DHT.h>
#include <LittleFS.h>
#include <WebServer.h>
#include <WiFi.h>

// TP1.0 page 50 / section 11.1
// Brochage impose pour garder le montage testable par l'enseignant.
const uint8_t PIN_DHT = 23;
const uint8_t PIN_LED_CLIM = 19;
const uint8_t PIN_LED_CHAUFFAGE = 21;
const uint8_t PIN_VENTILATEUR = 27;

const char* WIFI_SSID = "A_REMPLACER";
const char* WIFI_PASSWORD = "A_REMPLACER";
const char* WIFI_HOSTNAME = "esp32-rattrapage-iot";

// Si le capteur est un DHT22, remplacer DHT11 par DHT22.
#define DHT_TYPE DHT11
DHT dht(PIN_DHT, DHT_TYPE);

float seuilBas = 22.0;
float seuilHaut = 28.0;
const float HYSTERESIS = 1.0;
const float ECART_PLEINE_VITESSE = 4.0;

const unsigned long INTERVALLE_MESURE_MS = 2000;
const unsigned long DELAI_MIN_CHANGEMENT_MS = 5000;
const uint8_t NB_MESURES_FILTRE = 5;

// A laisser a 0 si le ventilateur est simplement simule par une LED.
// Passer a 1 seulement si le ventilateur est branche via un module/transistor
// compatible avec une commande PWM sur GPIO 27.
#define ACTIVER_PWM_VENTILATEUR 0

const char* IDENTIFIANT_ESP = "ESP32 rattrapage";
const char* UTILISATEUR = "Theo Delias";
const char* SALLE = "Maison";
const char* ADRESSE = "Non renseignee";
const char* LOCALISATION = "A definir";
const double GPS_LATITUDE = 43.62453842;
const double GPS_LONGITUDE = 7.050628185;

float mesures[NB_MESURES_FILTRE];
uint8_t indexMesure = 0;
uint8_t nbMesuresDisponibles = 0;
uint8_t vitesseVentilateur = 0;
float derniereTemperatureBrute = -999.0;
float derniereTemperatureFiltree = -999.0;
bool lectureCapteurOk = false;

enum ModeRegulation {
  MODE_ARRET,
  MODE_CHAUFFAGE,
  MODE_CLIMATISATION
};

ModeRegulation modeActuel = MODE_ARRET;
unsigned long derniereMesureMs = 0;
unsigned long dernierChangementModeMs = 0;

WebServer server(80);

void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(PIN_LED_CLIM, OUTPUT);
  pinMode(PIN_LED_CHAUFFAGE, OUTPUT);
  pinMode(PIN_VENTILATEUR, OUTPUT);

  appliquerMode(MODE_ARRET, 0.0);
  initialiserLittleFS();
  connecterWiFi();
  configurerRoutesHttp();
  server.begin();
}

void loop() {
  server.handleClient();
  unsigned long maintenant = millis();

  if (maintenant - derniereMesureMs < INTERVALLE_MESURE_MS) {
    return;
  }

  derniereMesureMs = maintenant;

  float temperature = dht.readTemperature();
  if (isnan(temperature)) {
    lectureCapteurOk = false;
    afficherErreurJson("dht_read");
    return;
  }

  lectureCapteurOk = true;
  derniereTemperatureBrute = temperature;
  ajouterMesure(temperature);
  float temperatureFiltree = calculerMoyenne();
  derniereTemperatureFiltree = temperatureFiltree;
  ModeRegulation modeDemande = determinerMode(temperatureFiltree);

  if (modeDemande != modeActuel &&
      maintenant - dernierChangementModeMs >= DELAI_MIN_CHANGEMENT_MS) {
    appliquerMode(modeDemande, temperatureFiltree);
    dernierChangementModeMs = maintenant;
  } else if (modeActuel == MODE_CLIMATISATION) {
    commanderVentilateur(temperatureFiltree);
  }

  afficherEtatJson(temperature, temperatureFiltree);
}

void ajouterMesure(float temperature) {
  mesures[indexMesure] = temperature;
  indexMesure = (indexMesure + 1) % NB_MESURES_FILTRE;

  if (nbMesuresDisponibles < NB_MESURES_FILTRE) {
    nbMesuresDisponibles++;
  }
}

float calculerMoyenne() {
  float somme = 0.0;

  for (uint8_t i = 0; i < nbMesuresDisponibles; i++) {
    somme += mesures[i];
  }

  return somme / nbMesuresDisponibles;
}

ModeRegulation determinerMode(float temperatureFiltree) {
  if (modeActuel == MODE_CLIMATISATION &&
      temperatureFiltree > seuilHaut - HYSTERESIS) {
    return MODE_CLIMATISATION;
  }

  if (modeActuel == MODE_CHAUFFAGE &&
      temperatureFiltree < seuilBas + HYSTERESIS) {
    return MODE_CHAUFFAGE;
  }

  if (temperatureFiltree > seuilHaut) {
    return MODE_CLIMATISATION;
  }

  if (temperatureFiltree < seuilBas) {
    return MODE_CHAUFFAGE;
  }

  return MODE_ARRET;
}

void appliquerMode(ModeRegulation mode, float temperatureFiltree) {
  modeActuel = mode;

  digitalWrite(PIN_LED_CLIM, mode == MODE_CLIMATISATION ? HIGH : LOW);
  digitalWrite(PIN_LED_CHAUFFAGE, mode == MODE_CHAUFFAGE ? HIGH : LOW);
  commanderVentilateur(temperatureFiltree);
}

void commanderVentilateur(float temperatureFiltree) {
  if (modeActuel != MODE_CLIMATISATION) {
    vitesseVentilateur = 0;
    digitalWrite(PIN_VENTILATEUR, LOW);
    return;
  }

#if ACTIVER_PWM_VENTILATEUR
  vitesseVentilateur = calculerVitesseVentilateur(temperatureFiltree);
  analogWrite(PIN_VENTILATEUR, vitesseVentilateur);
#else
  vitesseVentilateur = 255;
  digitalWrite(PIN_VENTILATEUR, HIGH);
#endif
}

uint8_t calculerVitesseVentilateur(float temperatureFiltree) {
  float depassement = temperatureFiltree - seuilHaut;

  if (depassement <= 0.0) {
    return 120;
  }

  if (depassement >= ECART_PLEINE_VITESSE) {
    return 255;
  }

  return 120 + (uint8_t)((depassement / ECART_PLEINE_VITESSE) * 135.0);
}

void afficherEtatJson(float temperatureBrute, float temperatureFiltree) {
  StaticJsonDocument<1500> doc;

  JsonObject status = doc["status"].to<JsonObject>();
  status["temperature"] = temperatureFiltree;
  status["temperature_raw"] = temperatureBrute;
  status["light"] = 0;
  status["regul"] = "RUNNING";
  status["fire"] = false;
  status["heat"] = modeActuel == MODE_CHAUFFAGE ? "ON" : "OFF";
  status["cold"] = modeActuel == MODE_CLIMATISATION ? "ON" : "OFF";
  status["fanspeed"] = vitesseVentilateur;
  status["mode"] = nomMode(modeActuel);

  JsonObject location = doc["location"].to<JsonObject>();
  location["room"] = SALLE;
  JsonObject gps = location["gps"].to<JsonObject>();
  gps["lat"] = GPS_LATITUDE;
  gps["lon"] = GPS_LONGITUDE;
  location["address"] = ADRESSE;

  JsonObject regul = doc["regul"].to<JsonObject>();
  regul["lt"] = seuilBas;
  regul["ht"] = seuilHaut;
  regul["hysteresis"] = HYSTERESIS;

  JsonObject info = doc["info"].to<JsonObject>();
  info["ident"] = IDENTIFIANT_ESP;
  info["user"] = UTILISATEUR;
  info["loc"] = LOCALISATION;

  JsonObject net = doc["net"].to<JsonObject>();
  net["uptime"] = millis() / 1000;
  net["ssid"] = WiFi.status() == WL_CONNECTED ? WiFi.SSID() : "NOP";
  net["mac"] = WiFi.macAddress();
  net["ip"] = WiFi.status() == WL_CONNECTED ? WiFi.localIP().toString() : "0.0.0.0";

  JsonObject reporthost = doc["reporthost"].to<JsonObject>();
  reporthost["target_ip"] = "127.0.0.1";
  reporthost["target_port"] = 1880;
  reporthost["sp"] = INTERVALLE_MESURE_MS / 1000;

  serializeJson(doc, Serial);
  Serial.println();
}

void afficherErreurJson(const char* codeErreur) {
  StaticJsonDocument<1500> doc;

  JsonObject status = doc["status"].to<JsonObject>();
  status["temperature"] = -999.0;
  status["temperature_raw"] = -999.0;
  status["light"] = 0;
  status["regul"] = "HALT";
  status["fire"] = false;
  status["heat"] = "OFF";
  status["cold"] = "OFF";
  status["fanspeed"] = 0;
  status["mode"] = "erreur";
  status["error"] = codeErreur;

  JsonObject location = doc["location"].to<JsonObject>();
  location["room"] = SALLE;
  JsonObject gps = location["gps"].to<JsonObject>();
  gps["lat"] = GPS_LATITUDE;
  gps["lon"] = GPS_LONGITUDE;
  location["address"] = ADRESSE;

  JsonObject regul = doc["regul"].to<JsonObject>();
  regul["lt"] = seuilBas;
  regul["ht"] = seuilHaut;
  regul["hysteresis"] = HYSTERESIS;

  JsonObject info = doc["info"].to<JsonObject>();
  info["ident"] = IDENTIFIANT_ESP;
  info["user"] = UTILISATEUR;
  info["loc"] = LOCALISATION;

  JsonObject net = doc["net"].to<JsonObject>();
  net["uptime"] = millis() / 1000;
  net["ssid"] = WiFi.status() == WL_CONNECTED ? WiFi.SSID() : "NOP";
  net["mac"] = WiFi.macAddress();
  net["ip"] = WiFi.status() == WL_CONNECTED ? WiFi.localIP().toString() : "0.0.0.0";

  JsonObject reporthost = doc["reporthost"].to<JsonObject>();
  reporthost["target_ip"] = "127.0.0.1";
  reporthost["target_port"] = 1880;
  reporthost["sp"] = INTERVALLE_MESURE_MS / 1000;

  serializeJson(doc, Serial);
  Serial.println();
}

String construireJsonEtat() {
  String sortie;
  StaticJsonDocument<1500> doc;

  JsonObject status = doc["status"].to<JsonObject>();
  status["temperature"] = derniereTemperatureFiltree;
  status["temperature_raw"] = derniereTemperatureBrute;
  status["light"] = 0;
  status["regul"] = lectureCapteurOk ? "RUNNING" : "HALT";
  status["fire"] = false;
  status["heat"] = modeActuel == MODE_CHAUFFAGE ? "ON" : "OFF";
  status["cold"] = modeActuel == MODE_CLIMATISATION ? "ON" : "OFF";
  status["fanspeed"] = vitesseVentilateur;
  status["mode"] = nomMode(modeActuel);

  JsonObject location = doc["location"].to<JsonObject>();
  location["room"] = SALLE;
  JsonObject gps = location["gps"].to<JsonObject>();
  gps["lat"] = GPS_LATITUDE;
  gps["lon"] = GPS_LONGITUDE;
  location["address"] = ADRESSE;

  JsonObject regul = doc["regul"].to<JsonObject>();
  regul["lt"] = seuilBas;
  regul["ht"] = seuilHaut;
  regul["hysteresis"] = HYSTERESIS;

  JsonObject info = doc["info"].to<JsonObject>();
  info["ident"] = IDENTIFIANT_ESP;
  info["user"] = UTILISATEUR;
  info["loc"] = LOCALISATION;

  JsonObject net = doc["net"].to<JsonObject>();
  net["uptime"] = millis() / 1000;
  net["ssid"] = WiFi.status() == WL_CONNECTED ? WiFi.SSID() : "NOP";
  net["mac"] = WiFi.macAddress();
  net["ip"] = WiFi.status() == WL_CONNECTED ? WiFi.localIP().toString() : "0.0.0.0";

  JsonObject reporthost = doc["reporthost"].to<JsonObject>();
  reporthost["target_ip"] = "127.0.0.1";
  reporthost["target_port"] = 1880;
  reporthost["sp"] = INTERVALLE_MESURE_MS / 1000;

  serializeJson(doc, sortie);
  return sortie;
}

void initialiserLittleFS() {
  LittleFS.begin(true);
}

void connecterWiFi() {
  if (String(WIFI_SSID) == "A_REMPLACER") {
    return;
  }

  WiFi.mode(WIFI_STA);
  WiFi.setHostname(WIFI_HOSTNAME);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  unsigned long debutConnexion = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - debutConnexion < 15000) {
    delay(250);
  }
}

void configurerRoutesHttp() {
  server.on("/", HTTP_GET, handleRoot);
  server.on("/status", HTTP_GET, handleStatus);
  server.on("/temperature", HTTP_GET, handleTemperature);
  server.on("/leds", HTTP_GET, handleLeds);
  server.on("/fan", HTTP_GET, handleFan);
  server.on("/set", HTTP_GET, handleSet);
  server.on("/esp.css", HTTP_GET, handleCss);
  server.onNotFound(handleNotFound);
}

void handleRoot() {
  if (!LittleFS.exists("/index.html")) {
    server.send(500, "text/plain", "index.html introuvable dans LittleFS");
    return;
  }

  File fichier = LittleFS.open("/index.html", "r");
  String page = fichier.readString();
  fichier.close();

  page.replace("%TEMPERATURE%", String(derniereTemperatureFiltree, 1));
  page.replace("%RAW_TEMPERATURE%", String(derniereTemperatureBrute, 1));
  page.replace("%MODE%", nomMode(modeActuel));
  page.replace("%HEAT%", modeActuel == MODE_CHAUFFAGE ? "ON" : "OFF");
  page.replace("%COLD%", modeActuel == MODE_CLIMATISATION ? "ON" : "OFF");
  page.replace("%FANSPEED%", String(vitesseVentilateur));
  page.replace("%LT%", String(seuilBas, 1));
  page.replace("%HT%", String(seuilHaut, 1));
  page.replace("%SSID%", WiFi.status() == WL_CONNECTED ? WiFi.SSID() : "Non connecte");
  page.replace("%IP%", WiFi.status() == WL_CONNECTED ? WiFi.localIP().toString() : "0.0.0.0");
  page.replace("%MAC%", WiFi.macAddress());
  page.replace("%UPTIME%", String(millis() / 1000));

  server.send(200, "text/html", page);
}

void handleStatus() {
  server.send(200, "application/json", construireJsonEtat());
}

void handleTemperature() {
  server.send(200, "text/plain", String(derniereTemperatureFiltree, 1));
}

void handleLeds() {
  String etat = String("{\"heat\":\"") +
                (modeActuel == MODE_CHAUFFAGE ? "ON" : "OFF") +
                "\",\"cold\":\"" +
                (modeActuel == MODE_CLIMATISATION ? "ON" : "OFF") +
                "\"}";
  server.send(200, "application/json", etat);
}

void handleFan() {
  server.send(200, "text/plain", String(vitesseVentilateur));
}

void handleSet() {
  if (server.hasArg("lt")) {
    seuilBas = server.arg("lt").toFloat();
  }

  if (server.hasArg("ht")) {
    seuilHaut = server.arg("ht").toFloat();
  }

  server.sendHeader("Location", "/");
  server.send(303);
}

void handleCss() {
  if (!LittleFS.exists("/esp.css")) {
    server.send(404, "text/plain", "esp.css introuvable");
    return;
  }

  File fichier = LittleFS.open("/esp.css", "r");
  server.streamFile(fichier, "text/css");
  fichier.close();
}

void handleNotFound() {
  server.send(404, "text/plain", "Route inconnue");
}

const char* nomMode(ModeRegulation mode) {
  switch (mode) {
    case MODE_CLIMATISATION:
      return "climatisation";
    case MODE_CHAUFFAGE:
      return "chauffage";
    default:
      return "arret";
  }
}
