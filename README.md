# 🚀 Dominando FreeRTOS: Comunicación Segura con Colas (Queues)

**Bienvenido al repositorio oficial del Club de Internet de las Cosas & Drones.** 
Este material de la asignatura de **Internet de las Cosas** está diseñado para los estudiantes del **Instituto Tecnológico Superior del Occidente del Estado de Hidalgo (ITSOEH)**. 
*Autor: MGTI Saul Isai Soto Ortiz*

---

## 🧠 ¿Qué son las Colas (Queues) y por qué son vitales?

En proyectos IoT avanzados, una tarea suele leer sensores (productor) mientras otra tarea envía esos datos por WiFi, MQTT o LoRa (consumidor). 

Si intentamos pasar esos datos usando una **variable global**, corremos un grave riesgo: si el sensor lee muy rápido y el envío por WiFi es lento, los datos nuevos sobrescribirán a los viejos antes de que puedan ser enviados. ¡Perdemos información crítica!

**Las Colas (Queues)** resuelven esto. Funcionan exactamente como una **Banda Transportadora en una fábrica**:
*   🍪 **Sin Banda (Variable Global):** El horno (Tarea 1) saca galletas y las avienta a la mesa del empacador (Tarea 2). Si el empacador está distraído armando una caja, el horno sigue tirando galletas encima, aplastando y perdiendo las anteriores.
*   🛤️ **Con Banda (Queue):** El horno pone las galletas ordenadamente en la banda transportadora. La banda las guarda y las mueve. El empacador las toma una por una en el exacto orden en que salieron (FIFO: First In, First Out). ¡Ningún dato se pierde!

---

## 🛠️ Materiales y Esquema de Conexiones

Para esta práctica simularemos la lectura de un sensor analógico que varía con el tiempo.

**Componentes:**
*   1x Placa de Desarrollo ESP32
*   1x Potenciómetro (10kΩ) o LDR (Fotorresistencia)
*   Cables jumper

**Diagrama de Cableado:**

      ESP32                       COMPONENTES (Potenciómetro)
    ┌───────┐
    │       │       
    │   3.3V│──────────────────> (Pin 1 - Extremo) VCC
    │       │
    │GPIO 34│──────────────────> (Pin 2 - Centro) Señal Analógica
    │       │
    │    GND│──────────────────> (Pin 3 - Extremo) GND
    │       │
    └───────┘

---

## 📂 Archivos de la Práctica (Explicación de Escenarios)

En este repositorio encontrarás dos archivos `.ino` que debes probar en tu ESP32 para comprender cómo las Queues protegen nuestra telemetría.

### ❌ Escenario 1: Pérdida de Datos (Archivo: `Sin_Queues_Perdida.ino`)
En este escenario, usamos una simple `variable global` para pasar la lectura del potenciómetro de la Tarea 1 a la Tarea 2.
*   **¿Qué sucede aquí?** La Tarea 1 (Sensor) lee datos muy rápido (cada 100 milisegundos). La Tarea 2 (Envío) es más lenta simulando un envío por red (cada 500 milisegundos).
*   **El Resultado:** La Tarea 1 sobrescribe la variable global 5 veces antes de que la Tarea 2 alcance a leerla. Si abres el Monitor Serie, verás que la Tarea 2 se "salta" muchísimos valores. En un dron o en agricultura de precisión, perder lecturas intermedias puede resultar en decisiones erróneas del sistema.

### ✅ Escenario 2: Telemetría Segura (Archivo: `Con_Queues_Seguro.ino`)
En este escenario implementamos nuestra "banda transportadora" creando un `QueueHandle_t`.
*   **¿Qué sucede aquí?** La Tarea 1 empaca sus lecturas y las pone en la banda usando `xQueueSend`. La banda tiene capacidad para guardar hasta 10 valores en fila. La Tarea 2 los va sacando de la banda usando `xQueueReceive`.
*   **El Resultado:** Aunque la Tarea 2 sea más lenta, cuando le toca leer, saca el dato más antiguo que estaba esperando en la fila. El Monitor Serie mostrará que **todos** los valores generados por el sensor son procesados, en el orden correcto, sin perderse ni uno solo (mientras no se llene la banda).

---
*Si lograste comunicar dos tareas de forma segura usando Queues, estás listo para crear sistemas IoT robustos y de grado industrial.*
