#include <Arduino.h>
#include "DHT.h"

// ================= PIN CONFIGURATION =================
#define DHTPIN 15          // DHT22 data pin
#define DHTTYPE DHT22      // DHT22 sensor type
#define LED_PIN 2          // Heater LED indicator
#define BUZZER_PIN 4       // Buzzer alarm pin

// ================= SENSOR OBJECT =====================
DHT dht(DHTPIN, DHTTYPE);

// ================= SYSTEM STATES =====================
enum State {
  IDLE,
  HEATING,
  STABILIZING,
  TARGET_REACHED,
  OVERHEAT
};

volatile State currentState = IDLE;
volatile float currentTemp = 0.0;

// ================= FREE RTOS TASK HANDLES ============
TaskHandle_t tempTaskHandle;
TaskHandle_t controlTaskHandle;

// ================= FUNCTION DECLARATIONS =============
void tempTask(void *parameter);
void controlTask(void *parameter);
void printState(State state);

// ====================== SETUP ========================
void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  dht.begin();

  Serial.println("ESP32 Heater Control System (FreeRTOS)");

  // Create temperature monitoring task (Core 1)
  xTaskCreatePinnedToCore(
    tempTask, "TempTask", 4096, NULL, 1, &tempTaskHandle, 1);

  // Create control task (Core 0)
  xTaskCreatePinnedToCore(
    controlTask, "ControlTask", 2048, NULL, 1, &controlTaskHandle, 0);
}

void loop() {
  // Main loop is empty (FreeRTOS tasks handle everything)
}

// ================= TEMP TASK =========================
// Reads temperature and updates the system state
void tempTask(void *parameter) {
  for (;;) {
    float temp = dht.readTemperature();

    if (!isnan(temp)) {
      currentTemp = temp;

      // State machine logic
      if (temp < 25) currentState = IDLE;
      else if (temp < 35) currentState = HEATING;
      else if (temp < 45) currentState = STABILIZING;
      else if (temp < 55) currentState = TARGET_REACHED;
      else currentState = OVERHEAT;

      Serial.print("Temp: ");
      Serial.print(temp);
      Serial.print(" °C, State: ");
      printState(currentState);
    } else {
      Serial.println("Failed to read from DHT sensor!");
    }

    vTaskDelay(pdMS_TO_TICKS(2000)); // Run every 2 seconds
  }
}

// ================ CONTROL TASK =======================
// Controls LED and Buzzer based on current state
void controlTask(void *parameter) {
  for (;;) {
    if (currentState == OVERHEAT) {
      digitalWrite(LED_PIN, LOW);
      tone(BUZZER_PIN, 1000); // Alarm
    } else if (currentState == HEATING || currentState == STABILIZING) {
      digitalWrite(LED_PIN, HIGH);
      noTone(BUZZER_PIN);
    } else {
      digitalWrite(LED_PIN, LOW);
      noTone(BUZZER_PIN);
    }

    vTaskDelay(pdMS_TO_TICKS(500)); // Check every 0.5 seconds
  }
}

// ================ PRINT STATE ========================
void printState(State state) {
  switch (state) {
    case IDLE: Serial.println("IDLE"); break;
    case HEATING: Serial.println("HEATING"); break;
    case STABILIZING: Serial.println("STABILIZING"); break;
    case TARGET_REACHED: Serial.println("TARGET_REACHED"); break;
    case OVERHEAT: Serial.println("OVERHEAT"); break;
  }
}
