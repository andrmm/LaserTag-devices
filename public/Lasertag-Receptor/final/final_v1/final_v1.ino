#include <driver/timer.h>
#define BUTTON1_PIN  14  // Пин, к которому подключена кнопка
#define BUTTON2_PIN  13  // Пин, к которому подключена кнопка
#define BUTTON3_PIN  15  // Пин, к которому подключена кнопка
#define BUTTON4_PIN  4  // Пин, к которому подключена кнопка

hw_timer_t *My_timer = NULL;

// Счётчик тактов
int numberTicks = 0;
// Счётчик вспышек
int numberFlash = 0;
// Счётчик попаданий
int numberStrikes = 0;
// Стартовая позиция
bool start = true;

void IRAM_ATTR onTimer()
{
  // Прерывание по таймеру
  numberTicks++;  // Инкримент счётчика тактов
  if (numberTicks == 1000) // Достижение примерно 1 мс
  {
    digitalWrite(2, !digitalRead(2)); // Моргание светодиодом
    numberTicks = 0;  // Сброс счётчика тактов
    numberFlash++;  // Инкремент счётчика вспышек

    if (numberFlash == 6) // Проверка количества вспышек
    {
      numberFlash = 0;  // Обнуление счётчика
      timerStop(My_timer);  // Остановка таймера
    }
  }
}

// Далее при срабатывании прерываний по кнопке запускается функция обработки "попадания"
void IRAM_ATTR handleButtonPress1() {
  StrikeFunction();
}
void IRAM_ATTR handleButtonPress2() {  
  StrikeFunction();
}
void IRAM_ATTR handleButtonPress3() {  
  StrikeFunction();
}
void IRAM_ATTR handleButtonPress4() {  
  StrikeFunction();
}

// Функция обработки "попадания"
void StrikeFunction()
{
  numberStrikes++;  // Инкремент счётчика попаданий

  // Запуск таймера
  if (!start) {timerStart(My_timer);}  
  else{
    start = false;
    timerAlarm(My_timer, 1000, true, 0);
  }

  // При 6 попаданиях сигнализация о "смерти"
  if (numberStrikes == 6)
  {    
    digitalWrite(2, HIGH);
    detachInterrupt(digitalPinToInterrupt(BUTTON1_PIN));
    detachInterrupt(digitalPinToInterrupt(BUTTON2_PIN));
    detachInterrupt(digitalPinToInterrupt(BUTTON3_PIN));
    detachInterrupt(digitalPinToInterrupt(BUTTON4_PIN));    
  }
}

void setup() {
  // сигнализация о включении
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  delay(2000);
  digitalWrite(2, LOW);
   
  // Инициализация Таймера
  My_timer = timerBegin(10000000);
  timerAttachInterrupt(My_timer, &onTimer);
  
  // Настраиваем прерывания по кнопке (попаданию)
  pinMode(BUTTON1_PIN, INPUT_PULLUP); // Настраиваем пин кнопки как вход с подтяжкой
  pinMode(BUTTON2_PIN, INPUT_PULLUP); // Настраиваем пин кнопки как вход с подтяжкой
  pinMode(BUTTON3_PIN, INPUT_PULLUP); // Настраиваем пин кнопки как вход с подтяжкой
  pinMode(BUTTON4_PIN, INPUT_PULLUP); // Настраиваем пин кнопки как вход с подтяжкой
  attachInterrupt(digitalPinToInterrupt(BUTTON1_PIN), handleButtonPress1, RISING); // Настраиваем прерывание на Нарастающий фронт
  attachInterrupt(digitalPinToInterrupt(BUTTON2_PIN), handleButtonPress2, RISING); // Настраиваем прерывание на Нарастающий фронт
  attachInterrupt(digitalPinToInterrupt(BUTTON3_PIN), handleButtonPress3, RISING); // Настраиваем прерывание на Нарастающий фронт
  attachInterrupt(digitalPinToInterrupt(BUTTON4_PIN), handleButtonPress4, RISING); // Настраиваем прерывание на Нарастающий фронт
}

void loop() {
  
}