#include <DHT.h>

// TP1.0 page 50 / section 11.1
// Brochage impose pour garder le montage testable par l'enseignant.
const uint8_t PIN_DHT = 23;
const uint8_t PIN_LED_CLIM = 19;
const uint8_t PIN_LED_CHAUFFAGE = 21;
const uint8_t PIN_VENTILATEUR = 27;

// Si le capteur est un DHT22, remplacer DHT11 par DHT22.
#define DHT_TYPE DHT11
DHT dht(PIN_DHT, DHT_TYPE);

const float SEUIL_BAS = 22.0;
const float SEUIL_HAUT = 28.0;
const float HYSTERESIS = 1.0;
const float ECART_PLEINE_VITESSE = 4.0;

const unsigned long INTERVALLE_MESURE_MS = 2000;
const unsigned long DELAI_MIN_CHANGEMENT_MS = 5000;
const uint8_t NB_MESURES_FILTRE = 5;

// A laisser a 0 si le ventilateur est simplement simule par une LED.
// Passer a 1 seulement si le ventilateur est branche via un module/transistor
// compatible avec une commande PWM sur GPIO 27.
#define ACTIVER_PWM_VENTILATEUR 0

float mesures[NB_MESURES_FILTRE];
uint8_t indexMesure = 0;
uint8_t nbMesuresDisponibles = 0;

enum ModeRegulation {
  MODE_ARRET,
  MODE_CHAUFFAGE,
  MODE_CLIMATISATION
};

ModeRegulation modeActuel = MODE_ARRET;
unsigned long derniereMesureMs = 0;
unsigned long dernierChangementModeMs = 0;

void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(PIN_LED_CLIM, OUTPUT);
  pinMode(PIN_LED_CHAUFFAGE, OUTPUT);
  pinMode(PIN_VENTILATEUR, OUTPUT);

  appliquerMode(MODE_ARRET, 0.0);
  Serial.println("TP1 - Regulation locale temperature");
}

void loop() {
  unsigned long maintenant = millis();

  if (maintenant - derniereMesureMs < INTERVALLE_MESURE_MS) {
    return;
  }

  derniereMesureMs = maintenant;

  float temperature = dht.readTemperature();
  if (isnan(temperature)) {
    Serial.println("Erreur lecture capteur DHT");
    return;
  }

  ajouterMesure(temperature);
  float temperatureFiltree = calculerMoyenne();
  ModeRegulation modeDemande = determinerMode(temperatureFiltree);

  if (modeDemande != modeActuel &&
      maintenant - dernierChangementModeMs >= DELAI_MIN_CHANGEMENT_MS) {
    appliquerMode(modeDemande, temperatureFiltree);
    dernierChangementModeMs = maintenant;
  } else if (modeActuel == MODE_CLIMATISATION) {
    commanderVentilateur(temperatureFiltree);
  }

  afficherEtat(temperature, temperatureFiltree);
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
      temperatureFiltree > SEUIL_HAUT - HYSTERESIS) {
    return MODE_CLIMATISATION;
  }

  if (modeActuel == MODE_CHAUFFAGE &&
      temperatureFiltree < SEUIL_BAS + HYSTERESIS) {
    return MODE_CHAUFFAGE;
  }

  if (temperatureFiltree > SEUIL_HAUT) {
    return MODE_CLIMATISATION;
  }

  if (temperatureFiltree < SEUIL_BAS) {
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
    digitalWrite(PIN_VENTILATEUR, LOW);
    return;
  }

#if ACTIVER_PWM_VENTILATEUR
  analogWrite(PIN_VENTILATEUR, calculerVitesseVentilateur(temperatureFiltree));
#else
  digitalWrite(PIN_VENTILATEUR, HIGH);
#endif
}

uint8_t calculerVitesseVentilateur(float temperatureFiltree) {
  float depassement = temperatureFiltree - SEUIL_HAUT;

  if (depassement <= 0.0) {
    return 120;
  }

  if (depassement >= ECART_PLEINE_VITESSE) {
    return 255;
  }

  return 120 + (uint8_t)((depassement / ECART_PLEINE_VITESSE) * 135.0);
}

void afficherEtat(float temperatureBrute, float temperatureFiltree) {
  Serial.print("temperature_brute=");
  Serial.print(temperatureBrute);
  Serial.print(" C | temperature_filtree=");
  Serial.print(temperatureFiltree);
  Serial.print(" C | mode=");
  Serial.println(nomMode(modeActuel));
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
