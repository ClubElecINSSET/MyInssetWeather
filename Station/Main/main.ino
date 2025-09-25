#include "DFRobot_BME280.h"

// Broches
static const byte broche_pluviometre    = 34;
static const byte broche_anemometre     = 35;
static const byte broche_girouette      = A0;

// Constantes
static const byte delai_anti_rebond_anemometre_ms   = 10;
static const byte delai_anti_rebond_pluviometre_ms  = 50;
static const float longueur_tige_anemometre_cm      = 7.0f;
static const byte temps_abandon_pluviometre_min     = 1;

static const unsigned int temps_abandon_pluviometre_min_ms =
    temps_abandon_pluviometre_min * 60000U;

static const float pre_calcul_vitesse_vent =
    (longueur_tige_anemometre_cm / 100.0f) * 2.0f * M_PI * 3.6f;

unsigned long dernier_temps_envoi = 1000UL;

// Variables anémomètre
float vitesse_vent_km_h = 0.0f;
unsigned long tableau_temps_anemometre[2] = {0, 0};
byte indice_tableau_anemometre = 0;

// Variables pluviomètre
float valeur_pluie = 0.0f; // mm
unsigned long tableau_temps_pluviometre[2] = {0, 0};
byte indice_tableau_pluviometre = 0;

// Variables interruptions
volatile unsigned long dernier_temps_declanchement_anemometre = 0UL;
volatile boolean anemometre_declanche = 0;
volatile unsigned long dernier_temps_declanchement_pluviometre = 0UL;
volatile boolean pluviometre_declanche = 0;

// Capteur BME280
typedef DFRobot_BME280_IIC BME;
BME bme(&Wire, 0x76);

// Fonction d'interruption pour l'anémomètre
void interrupt_anemometre() {
    dernier_temps_declanchement_anemometre = millis();
    anemometre_declanche = true;
}

// Fonction d'interruption pour le pluviomètre
void interrupt_pluviometre() {
    dernier_temps_declanchement_pluviometre = millis();
    pluviometre_declanche = true;
}

// Fonction pour déterminer l'angle de la girouette
int girouette(float valeur) {
    if (valeur > 720.0f && valeur < 850.0f)
        return 0; // Nord
    if (valeur > 400.0f && valeur < 500.0f)
        return 45; // Nord-Est
    if (valeur > 50.0f && valeur < 120.0f)
        return 90; // Est
    if (valeur > 140.0f && valeur < 210.0f)
        return 135; // Sud-Est
    if (valeur > 250.0f && valeur < 310.0f)
        return 180; // Sud
    if (valeur > 600.0f && valeur < 670.0f)
        return 225; // Sud-Ouest
    if (valeur > 930.0f && valeur < 990.0f)
        return 270; // Ouest
    if (valeur > 870.0f && valeur < 930.0f)
        return 315; // Nord-Ouest
    return 0;     // Par défaut
}


void setup() {
    Serial.begin(115200);
    pinMode(broche_anemometre, INPUT);
    pinMode(broche_pluviometre, INPUT);

    attachInterrupt(digitalPinToInterrupt(broche_anemometre),
                                   interrupt_anemometre, FALLING);
    attachInterrupt(digitalPinToInterrupt(broche_pluviometre),
                                    interrupt_pluviometre, FALLING);

    Wire.begin();

    // Initialisation du capteur BME280
    while (bme.begin() != BME::eStatusOK) {
        Serial.println("BME280 initialization failed");
        delay(2000);
    }
    Serial.println("Sensor initialized successfully!");
}

void loop(){}

/*

void loop() {
    // Lecture de la girouette
    int tension = analogRead(broche_girouette);
    int angle = girouette(tension);

    // Gestion de l'anémomètre
    if (dernier_temps_declanchement_anemometre + delai_anti_rebond_anemometre_ms <=
                    millis() &&
            anemometre_declanche) {
        tableau_temps_anemometre[indice_tableau_anemometre] =
                dernier_temps_declanchement_anemometre;
        indice_tableau_anemometre ^= 1;
        anemometre_declanche = false;
    }

    if (tableau_temps_anemometre[!indice_tableau_anemometre] -
                            tableau_temps_anemometre[indice_tableau_anemometre] <
                    3000 &&
            tableau_temps_anemometre[1]) {
        vitesse_vent_km_h = pre_calcul_vitesse_vent /
                                     (float(tableau_temps_anemometre[!indice_tableau_anemometre] -
                                                    tableau_temps_anemometre[indice_tableau_anemometre]) *
                                        0.002);
    } else {
        vitesse_vent_km_h = 0.0f;
    }

    // Gestion du pluviomètre
    if (dernier_temps_declanchement_pluviometre + delai_anti_rebond_pluviometre_ms <=
                    millis() &&
            pluviometre_declanche) {
        tableau_temps_pluviometre[indice_tableau_pluviometre] =
                dernier_temps_declanchement_pluviometre;
        indice_tableau_pluviometre ^= 1;
        pluviometre_declanche = false;
    }

    if ((tableau_temps_pluviometre[!indice_tableau_pluviometre] +
             temps_abandon_pluviometre_min_ms) > millis() &&
            tableau_temps_pluviometre[0] && tableau_temps_pluviometre[1]) {
        valeur_pluie =
                (1.0f / float(tableau_temps_pluviometre[!indice_tableau_pluviometre] -
                                         tableau_temps_pluviometre[indice_tableau_pluviometre])) *
                3600.0f * 0.2794f;
    } else {
        valeur_pluie = 0;
    }

    // Lecture des données environnementales
    float temperature = bme.getTemperature();
    float humidite = bme.getHumidity();
    int pression = bme.getPressure() / 100;

    // Envoi des données toutes les 5 secondes
    if (dernier_temps_envoi <= millis()) {
        dernier_temps_envoi = millis() + 5000UL;
        Serial.println(
                "Temperature:" + String(temperature) + ",Humidite:" + String(humidite) +
                ",Pression:" + String(pression) + ",Vitesse:" + String(vitesse_vent_km_h) +
                ",Direction:" + String(angle) + ",Pluie:" + String(valeur_pluie));
    }
}
*/