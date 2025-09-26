#include "DFRobot_BME280.h"

// Pins
static const byte rain_gauge_pin     = 34;
static const byte anemometer_pin     = 35;
static const byte wind_vane_pin      = A0;

// Constants
static const byte anemometer_debounce_delay_ms   = 10;
static const byte rain_gauge_debounce_delay_ms   = 50;
static const float anemometer_arm_length_cm      = 7.0f;
static const byte rain_gauge_timeout_min         = 1;

static const unsigned int rain_gauge_timeout_min_ms =
    rain_gauge_timeout_min * 60000U;

static const float wind_speed_pre_calc =
    (anemometer_arm_length_cm / 100.0f) * 2.0f * M_PI * 3.6f;

unsigned long last_send_time = 1000UL;

// Anemometer variables
float wind_speed_kmh = 0.0f;
unsigned long anemometer_time_array[2] = {0, 0};
byte anemometer_array_index = 0;

// Rain gauge variables
float rain_value = 0.0f; // mm
unsigned long rain_gauge_time_array[2] = {0, 0};
byte rain_gauge_array_index = 0;

// Interrupt variables
volatile unsigned long last_anemometer_trigger_time = 0UL;
volatile boolean anemometer_triggered = 0;
volatile unsigned long last_rain_gauge_trigger_time = 0UL;
volatile boolean rain_gauge_triggered = 0;

// BME280 sensor
typedef DFRobot_BME280_IIC BME;
BME bme(&Wire, 0x76);

// Interrupt function for anemometer
void anemometer_interrupt() {
    last_anemometer_trigger_time = millis();
    anemometer_triggered = true;
}

// Interrupt function for rain gauge
void rain_gauge_interrupt() {
    last_rain_gauge_trigger_time = millis();
    rain_gauge_triggered = true;
}

// Function to determine wind vane angle
int wind_vane(float value) {
    if (value > 720.0f && value < 850.0f)
        return 0; // North
    if (value > 400.0f && value < 500.0f)
        return 45; // North-East
    if (value > 50.0f && value < 120.0f)
        return 90; // East
    if (value > 140.0f && value < 210.0f)
        return 135; // South-East
    if (value > 250.0f && value < 310.0f)
        return 180; // South
    if (value > 600.0f && value < 670.0f)
        return 225; // South-West
    if (value > 930.0f && value < 990.0f)
        return 270; // West
    if (value > 870.0f && value < 930.0f)
        return 315; // North-West
    return 0;     // Default
}


void setup() {
    Serial.begin(115200);
    pinMode(anemometer_pin, INPUT);
    pinMode(rain_gauge_pin, INPUT);

    attachInterrupt(digitalPinToInterrupt(anemometer_pin),
                                   anemometer_interrupt, FALLING);
    attachInterrupt(digitalPinToInterrupt(rain_gauge_pin),
                                    rain_gauge_interrupt, FALLING);

    Wire.begin();

    // BME280 sensor initialization
    while (bme.begin() != BME::eStatusOK) {
        Serial.println("BME280 initialization failed");
        delay(2000);
    }
    Serial.println("Sensor initialized successfully!");
}


void loop() {
    // Wind vane reading
    int voltage = analogRead(wind_vane_pin);
    int angle = wind_vane(voltage);

    // Anemometer management
    if (last_anemometer_trigger_time + anemometer_debounce_delay_ms <=
                    millis() &&
            anemometer_triggered) {
        anemometer_time_array[anemometer_array_index] =
                last_anemometer_trigger_time;
        anemometer_array_index ^= 1;
        anemometer_triggered = false;
    }

    if (anemometer_time_array[!anemometer_array_index] -
                            anemometer_time_array[anemometer_array_index] <
                    3000 &&
            anemometer_time_array[1]) {
        wind_speed_kmh = wind_speed_pre_calc /
                                     (float(anemometer_time_array[!anemometer_array_index] -
                                                    anemometer_time_array[anemometer_array_index]) *
                                        0.002);
    } else {
        wind_speed_kmh = 0.0f;
    }

    // Rain gauge management
    if (last_rain_gauge_trigger_time + rain_gauge_debounce_delay_ms <=
                    millis() &&
            rain_gauge_triggered) {
        rain_gauge_time_array[rain_gauge_array_index] =
                last_rain_gauge_trigger_time;
        rain_gauge_array_index ^= 1;
        rain_gauge_triggered = false;
    }

    if ((rain_gauge_time_array[!rain_gauge_array_index] +
             rain_gauge_timeout_min_ms) > millis() &&
            rain_gauge_time_array[0] && rain_gauge_time_array[1]) {
        rain_value =
                (1.0f / float(rain_gauge_time_array[!rain_gauge_array_index] -
                                         rain_gauge_time_array[rain_gauge_array_index])) *
                3600.0f * 0.2794f;
    } else {
        rain_value = 0;
    }

    // Environmental data reading
    float temperature = bme.getTemperature();
    float humidity = bme.getHumidity(); 
    int pressure = bme.getPressure() / 100;

    // Send data every 5 seconds
    if (last_send_time <= millis()) {
        last_send_time = millis() + 5000UL;
        Serial.println(
                "Temperature:" + String(temperature) + ",Humidity:" + String(humidity) +
                ",Pressure:" + String(pressure) + ",WindSpeed:" + String(wind_speed_kmh) +
                ",Direction:" + String(angle) + ",Rain:" + String(rain_value));
    }
}