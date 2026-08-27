/* 
  Ejemplo 2: CON Queues - Telemetría Segura (Banda Transportadora)
  Club de Internet de las Cosas & Drones - ITSOEH
*/
#include <Arduino.h>

#define SENSOR_PIN 34

// 1. Declaramos nuestra "Banda Transportadora" (La Cola/Queue)
QueueHandle_t colaSensores;

// TAREA 1: El Horno (Lee el sensor muy rápido)
void TareaLeerSensor(void *parameter) {
  int contadorGalletas = 1;
  for (;;) {
    int valorSensor = analogRead(SENSOR_PIN) + contadorGalletas;
    
    Serial.print("🏭 Horno produce dato #");
    Serial.println(contadorGalletas);
    
    // 2. Ponemos el dato en la banda transportadora.
    // Pasamos la dirección de memoria (&valorSensor).
    // Esperamos máximo 10 ticks si la banda estuviera llena.
    if (xQueueSend(colaSensores, &valorSensor, 10) != pdPASS) {
      Serial.println("⚠️ La banda está llena, se cayó una galleta!");
    }
    
    contadorGalletas++;
    vTaskDelay(100 / portTICK_PERIOD_MS); // Produce muy rápido (cada 100ms)
  }
}

// TAREA 2: El Empacador (Procesa el dato lentamente)
void TareaProcesarDatos(void *parameter) {
  int valorRecibido; // Canasta vacía para recibir el dato
  
  for (;;) {
    // 3. Tomamos el dato de la banda transportadora.
    // portMAX_DELAY: Si la banda está vacía, el empacador espera pacientemente.
    if (xQueueReceive(colaSensores, &valorRecibido, portMAX_DELAY) == pdPASS) {
      Serial.print("📦 Empacador procesa dato en orden: ");
      Serial.println(valorRecibido);
    }
    
    // Simula que enviar por WiFi/LoRa toma mucho tiempo
    vTaskDelay(500 / portTICK_PERIOD_MS); // Procesa lento (cada 500ms)
  }
}

void setup() {
  Serial.begin(115200);

  // 4. Construimos la Banda Transportadora antes de arrancar la fábrica.
  // Indicamos cuántos espacios tiene (10 elementos) y de qué tamaño es cada uno (tamaño de un int).
  colaSensores = xQueueCreate(10, sizeof(int));

  if (colaSensores != NULL) {
    xTaskCreatePinnedToCore(TareaLeerSensor, "Lector", 2048, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(TareaProcesarDatos, "Procesador", 2048, NULL, 1, NULL, 1);
  }
}

void loop() {}
