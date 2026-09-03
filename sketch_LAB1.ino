constexpr uint8_t LED1 = 26;
constexpr uint8_t LED2 = 33;
constexpr uint8_t BUTTON_PIN = 27;
constexpr uint8_t BUTTON_RESET = 32;
int segmentos[] = {2, 4, 5, 18, 19, 21, 22};


const unsigned long debounceDelay = 30; // Tiempo de estabilización en ms
bool Gobalstate = false;


// Tiempo de estabilización en ms
bool buttonPressedEvent = false; 
bool reset = false;
bool ConteoDisplay = true;

int contador = 0; // Contador en el Display
int Contadordereseteo = 0;

//        a  b  c  d  e  f  g
byte numeros[10][7] = {
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1}  // 9
};

void LED_Blinking();
void updateButton();
void reportStateChanges();
void updateCounter();
void updateDisplay();


void setup() {
  Serial.begin(115200);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUTTON_RESET, INPUT_PULLUP);

  for (int i = 0; i < 7; i++) {
    pinMode(segmentos[i], OUTPUT);
  }
}

void loop() {
  LED_Blinking();
  updateButton();
  reportStateChanges();
  updateCounter();
  updateDisplay();
}

void LED_Blinking(){
  static unsigned long PreviousTime= 0; // Guarda el último instante en que cambió el LED
  const long Interval = 250;      // Intervalo de parpadeo en milisegundos 
  unsigned long Now = millis();

  static bool State_LED = LOW;              // Estado actual del LED
  

  if (Now - PreviousTime >= Interval){
    PreviousTime = Now;
    State_LED = !State_LED;
    digitalWrite(LED1,State_LED);
  }

}
void updateButton(){
  // Variables de estado y anti-rebote locales (persistentes) 
  static bool lastRawReading  = HIGH;      // Última lectura cruda (sin filtrar)
  static bool lastButtonState = HIGH;    // Última lectura estable del pulsador (HIGH = no presionado)
  static unsigned long lastDebounceTime = 0; //último cambio detectado

  const unsigned long now = millis();
  bool rawReading = digitalRead(BUTTON_PIN);

  // Si la lectura cruda cambió, reinicia el temporizador de anti-rebote
  if (rawReading != lastRawReading) {
    lastDebounceTime = now;
    lastRawReading = rawReading; // Guarda el estado actual para la siguiente comparación
  }

  // Si ya pasó el tiempo de estabilización, se considera lectura válida
  if ((now - lastDebounceTime) > debounceDelay) {

    // Solo actuar si el estado estable realmente cambió
    if (rawReading != lastButtonState) {
      lastButtonState = rawReading;

      // Con pull-up: LOW = pulsador presionado
      if (lastButtonState == LOW) {
        Gobalstate =!Gobalstate;
        digitalWrite(LED2, Gobalstate);

        // bandera para evitar llamar la funcion aqui
        buttonPressedEvent = true;
      }
    }
  }
}
void reportStateChanges(){
  if(buttonPressedEvent == true){
  Serial.print("Pulsador presionado -> LED ");
  Serial.println(Gobalstate ? "ENCENDIDO" : "APAGADO");

  if (Gobalstate == LOW && reset == true ){
      Serial.println("Display reseteado");
      Serial.print("Numero en que se reseteo : ");
      Serial.println(Contadordereseteo);
      reset = false;

  }
  buttonPressedEvent = false; // Consumimos el evento para que solo se imprima una vez
  }
  if(ConteoDisplay == true) {
      Serial.print("Numero del Display : ");
      Serial.println(contador);
    ConteoDisplay = false;
  }
  
}
void updateCounter(){
  static bool lastRawReading  = HIGH;      // Última lectura cruda (sin filtrar)
  static bool lastButton_ResetState = HIGH;
  static unsigned long lastDebounceTime = 0; //último cambio detectado

  const unsigned long now = millis();
  bool rawReading = digitalRead(BUTTON_RESET);

  // Si la lectura cruda cambió, reinicia el temporizador de anti-rebote
  if (rawReading != lastRawReading) {
    lastDebounceTime = now;
    lastRawReading = rawReading; // Guarda el estado actual para la siguiente comparación
  }

  // Si ya pasó el tiempo de estabilización, se considera lectura válida
  if ((now - lastDebounceTime) > debounceDelay) {

    // Solo actuar si el estado estable realmente cambió
    if (rawReading != lastButton_ResetState) {
      lastButton_ResetState = rawReading;

      // Con pull-up: LOW = pulsador presionado
      if (lastButton_ResetState == LOW) {
        Contadordereseteo = contador;
        contador = 0;

        if(Gobalstate == HIGH){
          Gobalstate = LOW;
          digitalWrite(LED2, Gobalstate);
          reset = true;
        }
        else{
          reset = true;
        }
        buttonPressedEvent = true;
      }
    }
  }
}
void updateDisplay(){
  const unsigned long now = millis();
  static unsigned long tiempoAnteriorConteo = 0;
  const unsigned long intervaloConteo = 1000;

  if (Gobalstate){
    if (now - tiempoAnteriorConteo >= intervaloConteo) {
      tiempoAnteriorConteo = now;
      contador++;
      if (contador > 9) {
        contador = 0;
      }
      ConteoDisplay = true;
    }
  } else {
    // sincroniza el tiempo pasado y actual. para evitar saltos buscos 
    tiempoAnteriorConteo = now;
  }

      // muestra el numero en el 7segmentos
  for (int i = 0; i < 7; i++) {
    digitalWrite(segmentos[i], numeros[contador][i]);
  }
}
