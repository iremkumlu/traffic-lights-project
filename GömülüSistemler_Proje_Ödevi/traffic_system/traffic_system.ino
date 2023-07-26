#include <TimerOne.h>

// Pin atamaları
const int redLightPin = 13;
const int yellowLightPin = 12;
const int greenLightPin = 11;
const int pedestrianButtonPin = 2;

// Trafik ışıkları zamanlaması
const unsigned long redTime = 5000;
const unsigned long yellowTime = 1000;
const unsigned long greenTime = 4000;
const unsigned long pedestrianCrossingTime = 5000; 

// Trafik ışığı durumlarını ve zamanlamaları izlemek için değişkenler
volatile bool pedestrianButtonPressed = false; // yaya düğmesinin basıp basmadığını kontrol ediyor
volatile unsigned long currentPhaseStartTime = 0; // mevcut fazın başladığı zamanı saklar

void setup() {
  // sırasıyla pinleri yapılandırılır
  pinMode(redLightPin, OUTPUT);
  pinMode(yellowLightPin, OUTPUT); //ışıkları çıkış pini olarak almamızın nedeni, bunu tetiklicek yaya butonu girişe bağlanır
  pinMode(greenLightPin, OUTPUT);
  pinMode(pedestrianButtonPin, INPUT_PULLUP); 

  attachInterrupt(digitalPinToInterrupt(pedestrianButtonPin), handlePedestrianButtonInterrupt, FALLING); // eğer bir kesme olursa handlepedes. çağrılması gerektiğini bildiri
  Timer1.initialize(1000); // zamanlayıcı aralığı 1ms olarak ayarlanır.
  Timer1.attachInterrupt(handleTimerInterrupt); 
}

void loop() {
  // Trafik ışığı dizisi kesintiler ve zamanlayıcılar tarafından yönetildiğinden işlev boştur
}

// Yaya düğmesi kesme işlevi
void handlePedestrianButtonInterrupt() {
  pedestrianButtonPressed = true;
}

// zamanlayıcı kesme işlevi
void handleTimerInterrupt() {
  unsigned long elapsedTime = millis() - currentPhaseStartTime; //fonksiyon başladığından itibaren geçen sürede mevcut fazın başladığı süre çıkarılır
  //Bu geçen süre, hangi fazın aktif olması gerektiğini belirlemek ve buna göre trafik ışıklarının durumunu kontrol etmek için kullanılır.
  
  // Trafik ışığı aşamaları yönetimi
  if (elapsedTime >= 0 && elapsedTime < redTime) {
    // Kırmızı ışık fazı
    digitalWrite(redLightPin, HIGH);
    digitalWrite(yellowLightPin, LOW);
    digitalWrite(greenLightPin, LOW);
  } else if (elapsedTime >= redTime && elapsedTime < redTime + yellowTime) {
    // Sarı ışık fazı
    digitalWrite(redLightPin, LOW);
    digitalWrite(yellowLightPin, HIGH);
    digitalWrite(greenLightPin, LOW);
  } else if (elapsedTime >= redTime + yellowTime && elapsedTime < redTime + yellowTime + greenTime) {
    // Yeşil ışık fazı
    digitalWrite(redLightPin, LOW);
    digitalWrite(yellowLightPin, LOW);
    digitalWrite(greenLightPin, HIGH);
  } else if (elapsedTime >= redTime + yellowTime + greenTime) {
    //Yaya geçidi aşamasını başlat
    digitalWrite(redLightPin, LOW);
    digitalWrite(yellowLightPin, LOW);
    digitalWrite(greenLightPin, LOW);
    handlePedestrianCrossing();
  }
}

// Yaya geçidi aşamasını yönetme
void handlePedestrianCrossing() {
  // Sadece sarı ışık açılır
  digitalWrite(yellowLightPin, HIGH);
  delay(pedestrianCrossingTime); 
  digitalWrite(yellowLightPin, LOW);
  
  // Geçiş süresinden sonra sarı ışık söner ve trafik ışığı sıfırlanır.
  currentPhaseStartTime = millis();
  pedestrianButtonPressed = false;
}
