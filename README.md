# Descripción del proyecto 

Este proyecto implementa un contador digital de 0 a 9 controlado por un microcontrolador ESP32.
El sistema cuenta con control de estado mediante botones físicos (START/STOP y RESET), 
visualización en un display de 7 segmentos, e indicadores visuales independientes 
(dos LED: uno para el parpadeo constante y otro para mostrar si el conteo está activo).

Todo el software está diseñado utilizando un enfoque no bloqueante con millis(), 
lo que permite que la lectura de botones, la actualización del display,
el parpadeo del LED y la lógica del conteo operen de manera simultánea y fluida.


# Requisitos y diseño

Componentes utilizados

ESP32.
LED Rojo.
LED Azul.
9 Resistencias de  220 Ω.
2 Botones mecánicos.
Display 7 segmentos.
Cables de conexión.

  
# 🔌 Tabla de Conexiones

| Función | GPIO | Conexión eléctrica | Observación |
| :--- | :---: | :--- | :--- |
| **LED 1** | 26 | GPIO → 220Ω → Ánodo; Cátodo → GND | Parpadeo independiente |
| **LED 2** | 33 | GPIO → 220Ω → Ánodo; Cátodo → GND | Indicador de conteo activo |
| **START/STOP** | 27 | Pulsador entre GPIO y GND | `INPUT_PULLUP` |
| **RESET** | 32 | Pulsador entre GPIO y GND | `INPUT_PULLUP` |
| **Segmento a** | 2 | GPIO → 220Ω → Pin 'a' | Display cátodo común |
| **Segmento b** | 4 | GPIO → 220Ω → Pin 'b' | Display cátodo común |
| **Segmento c** | 5 | GPIO → 220Ω → Pin 'c' | Display cátodo común |
| **Segmento d** | 19 | GPIO → 220Ω → Pin 'd' | Display cátodo común |
| **Segmento e** | 20 | GPIO → 220Ω → Pin 'e' | Display cátodo común |
| **Segmento f** | 21 | GPIO → 220Ω → Pin 'f' | Display cátodo común |
| **Segmento g** | 22 | GPIO → 220Ω → Pin 'g' | Display cátodo común |
| **Común Display** | GND | Terminal(es) común(es) a GND | Display cátodo común |

# 🔢 Tabla de Verdad del Display (Cátodo Común)

| Número | a | b | c | d | e | f | g |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| **0** | 1 | 1 | 1 | 1 | 1 | 1 | 0 |
| **1** | 0 | 1 | 1 | 0 | 0 | 0 | 0 |
| **2** | 1 | 1 | 0 | 1 | 1 | 0 | 1 |
| **3** | 1 | 1 | 1 | 1 | 0 | 0 | 1 |
| **4** | 0 | 1 | 1 | 0 | 0 | 1 | 1 |
| **5** | 1 | 0 | 1 | 1 | 0 | 1 | 1 |
| **6** | 1 | 0 | 1 | 1 | 1 | 1 | 1 |
| **7** | 1 | 1 | 1 | 0 | 0 | 0 | 0 |
| **8** | 1 | 1 | 1 | 1 | 1 | 1 | 1 |
| **9** | 1 | 1 | 1 | 0 | 0 | 1 | 1 |

# Montaje de laboratorio

<img width="580" height="354" alt="Montaje" src="https://github.com/user-attachments/assets/0c655dd5-07ff-4df4-9bf1-a7851c518292" />

# Análisis técnico

El uso de delay() bloquearía la ejecución del loop(), impidiendo atender simultáneamente los botones, el contador y el parpadeo del LED. Por esta razón, la versión implementada utiliza millis(), permitiendo ejecutar las tareas de forma simultánea.

El nivel representa el estado actual de una entrada, mientras que una transición permite detectar el cambio entre estados. En los pulsadores se utiliza INPUT_PULLUP, por lo que el estado es HIGH cuando no se presiona y LOW cuando se presiona. El evento se genera únicamente al detectar una transición estable HIGH a LOW, evitando múltiples eventos mientras el botón permanece presionado.

El display realiza el conteo de 0 a 9 y, después del 9, el contador vuelve a 0, produciendo la transición 9→0 sin mostrar un valor inválido.

Al presionar RESET, el contador se establece en 0 y el sistema queda detenido. Al reanudar mediante START/STOP, se reinicia la referencia temporal del contador para que el primer incremento ocurra después de 1000 ms y no inmediatamente. El parpadeo del LED utiliza una temporización independiente de 250 ms, por lo que continúa funcionando incluso cuando el contador está detenido.
