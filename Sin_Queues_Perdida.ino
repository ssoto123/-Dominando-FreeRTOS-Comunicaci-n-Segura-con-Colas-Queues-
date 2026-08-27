/* 
  Ejemplo 1: SIN Queues - La Pérdida de Datos (Uso de Variables Globales)
  Club de Internet de las Cosas & Drones - ITSOEH
*/
#include <Arduino.h>

#define SENSOR_PIN 34

// Nuestra "Mesa" donde aventamos las galletas (Variable Global)
int lecturaGlobal = 0; 

// TAREA 1: El Horno (Lee el sensor muy rápido)
void TareaLeerSensor(void *parameter) {
  int contadorGalletas = 1;
  for (;;) {
    // Simulamos leer el sensor (usamos un contador sumado a la lectura para notar la pérdida)
    int valorSensor = analogRead(SENSOR_PIN) + contadorGalletas;
    
    // Sobrescribe la mesa sin preguntar si el empacador ya terminó
    lecturaGlobal = valorSensor;
    Serial.print("🏭 Horno produce dato #");
    Serial.println(contadorGalletas);
    
    contadorGalletas++;
    vTaskDelay(100 / portTICK_PERIOD_MS); // Produce muy rápido (cada 100ms)
  }
}

// TAREA 2: El Empacador (Procesa el dato lentamente)
void TareaProcesarDatos(void *parameter) {
  for (;;) {
    // El empacador toma lo que sea que esté en la mesa en este momento
    int valorProcesado = lecturaGlobal;
    
    Serial.print("📦 Empacador procesa dato: ");
    Serial.println(valorProcesado);
    
    // Simula que enviar por WiFi/LoRa toma mucho tiempo
    vTaskDelay(500 / portTICK_PERIOD_MS); // Procesa lento (cada 500ms)
  }
}

void setup() {
  Serial.begin(115200);

  // Soltamos ambas tareas
  xTaskCreatePinnedToCore(TareaLeerSensor, "Lector", 2048, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(TareaProcesarDatos, "Procesador", 2048, NULL, 1, NULL, 1);
}

void loop() {}
