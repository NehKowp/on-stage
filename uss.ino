  /*
  av : avancer                  re : reculer
  td : tourner clockwise        tg : tourner counter clockwise
  hg : diagona  le haut-gauche    hd : diagonale haut-droite
  bd : diagonale bas-gauche     bd : diagonale bas-droite
  gg : avancer vers gauche      dd : avancer vers droite

  0, 1 (Serial classique)
  2, 3, 4, 5 Moteurs DC
  6, 7 Libres
  9,10,11,12,13 Servomoteurs
  8 tete
  18, 19 pour la caméra 
  20 pour la distance doublée (nano)
  22-29 pour les directions de moteurs

  Branchements caméra : (TX)18 - P5(RX), (RX)19 - P4(TX)
  Violet Vin - Gris GND - Blanc -> P5 - Noir ->P4
  
  Branchements Xbee : (TX)16 - Din(RX), 17(RX) - Dout(TX)
*/

#include <Wire.h>
#include <Servo.h>

// Initialise le servomoteur de chaque patte et la queue de la tortue
Servo paAG, paAD, paBG, paBD, paQU;

// Pins moteurs    1   2   3   4 
const int in1[] = {22, 24, 26, 28};
const int in2[] = {23, 25, 27, 29};
const int ena[] = {2,  3,  4,  5};

/////////////////////////////// Fonctions supplémentaires ///////////////////////////////////////

// Fonction pour faire se déplacer la tortue
// Chaque moteur est controlé avec 3 connections :
// in1 et in2 controllent la direction du moteur (respectivement HIGH LOW pour avancer
// LOW HIGH pour reculer, HIGH HIGH pour freiner brutalement, LOW LOW pour ne rien faire)
// enfin la connection ena s'occupe de la vitesse, 0 étant l'arrêt, 255 la vitesse max

void moteurs(String dir, int vit){
  if(analogRead(A0) <= 500)
  {
    // Pour avancer
    if(dir == "av"){
      // Moteurs : + + + +
      for(int i=0; i<4;i++)
      {
        digitalWrite(in1[i], HIGH);
        digitalWrite(in2[i], LOW);
        // Vitesse des moteurs
        analogWrite(ena[i], vit);
      } return;
    }
  
    // Pour reculer
    if(dir == "re"){
      // Moteurs : - - - -
      for(int i=0; i<4;i++)
      {
        digitalWrite(in1[i], LOW);
        digitalWrite(in2[i], HIGH);
        // Vitesse des moteurs
        analogWrite(ena[i], vit);
      } return;
    }
  
    // Pour tourner clockise (sens aiguille montre)
    if(dir == "td"){
      // Moteurs : + - + -
      digitalWrite(in1[0], HIGH); // M1 +
      digitalWrite(in2[0], LOW);
      digitalWrite(in1[1], LOW);  // M2 -
      digitalWrite(in2[1], HIGH);
      digitalWrite(in1[2], HIGH); // M3 +
      digitalWrite(in2[2], LOW);
      digitalWrite(in1[3], LOW);  // M4 -
      digitalWrite(in2[3], HIGH);
      // Vitesse des moteurs
      for(int i=0; i<4;i++) {
        analogWrite(ena[i], vit);
      } return;
    }
  
    // Pour tourner counter clockwise (sens inverse aiguille montre)
    if(dir == "tg"){
      // Moteurs : - + - +
      digitalWrite(in1[0], LOW);  // M1 -
      digitalWrite(in2[0], HIGH);
      digitalWrite(in1[1], HIGH); // M2 +
      digitalWrite(in2[1], LOW);
      digitalWrite(in1[2], LOW);  // M3 -
      digitalWrite(in2[2], HIGH);
      digitalWrite(in1[3], HIGH); // M4 +
      digitalWrite(in2[3], LOW);
      // Vitesse des moteurs
      for(int i=0; i<4;i++) {
        analogWrite(ena[i], vit);
      } return;
    }
  
    // Pour se déplacer à gauche
    if(dir == "gg"){
      // Moteurs : - + + -
      digitalWrite(in1[0], LOW); // M1 -
      digitalWrite(in2[0], HIGH);
      digitalWrite(in1[1], HIGH); // M2 +
      digitalWrite(in2[1], LOW);
      digitalWrite(in1[2], HIGH); // M3 +
      digitalWrite(in2[2], LOW);
      digitalWrite(in1[3], LOW); // M4 -
      digitalWrite(in2[3], HIGH);
      // Vitesse des moteurs
      for(int i=0; i<4;i++) {
        analogWrite(ena[i], vit);
      } return;
    }
  
    // Pour se déplacer à droite
    if(dir == "dd"){
      // Moteurs : + - - +
      digitalWrite(in1[0], HIGH); // M1 +
      digitalWrite(in2[0], LOW);
  
      digitalWrite(in1[1], LOW); // M2 -
      digitalWrite(in2[1], HIGH);
  
      digitalWrite(in1[2], LOW); // M3 -
      digitalWrite(in2[2], HIGH);
  
      digitalWrite(in1[3], HIGH); // M4 +
      digitalWrite(in2[3], LOW);
      // Vitesse des moteurs
      for(int i=0; i<4;i++) {
        analogWrite(ena[i], vit);
      } return;
    }
  
    // Pour avancer en diagonale haut gauche
    if(dir == "hg"){
      // Moteurs : 0 + + 0
      for(int i=0; i<4;i++)
      {
        digitalWrite(in1[i], HIGH);
        digitalWrite(in2[i], LOW);
      }
      analogWrite(ena[0], 0);
      analogWrite(ena[1], vit);
      analogWrite(ena[2], vit);
      analogWrite(ena[3], 0);
       return;
    }
  
    // Pour avancer en diagonale haut droit
    if(dir == "hd"){
      // Moteurs : + 0 0 +
      for(int i=0; i<4;i++)
      {
        digitalWrite(in1[i], HIGH);
        digitalWrite(in2[i], LOW);
      }
      analogWrite(ena[0], vit);
      analogWrite(ena[1], 0);
      analogWrite(ena[2], 0);
      analogWrite(ena[3], vit);
       return;
    }
  
    // Pour avancer en diagonale bas gauche
    if(dir == "bg"){
      // Moteurs : - 0 0 -
      for(int i=0; i<4;i++)
      {
        digitalWrite(in1[i], LOW);
        digitalWrite(in2[i], HIGH);
      }
      analogWrite(ena[0], vit);
      analogWrite(ena[1], 0);
      analogWrite(ena[2], 0);
      analogWrite(ena[3], vit);
       return;
    }
  
    // Pour avancer en diagonale bas droit
    if(dir == "bd"){
      // Moteurs : - 0 0 -
      for(int i=0; i<4;i++)
      {
        digitalWrite(in1[i], LOW);
        digitalWrite(in2[i], HIGH);
      }
      analogWrite(ena[0], 0);
      analogWrite(ena[1], vit);
      analogWrite(ena[2], vit);
      analogWrite(ena[3], 0);
    }
  } else {
    Serial.println("Stop");
  }
}


// Vérifie l'état du Serial1 (caméra) pendant forMillis millisecondes
int checkSerial(int forMillis, int moteur_active){
  long tzero = millis();
  int didrecieve = 0;

  // Tant que le délai n'est pas dépassé ou qu'un message est recu dans le Serial1 (caméra)
  while(millis()-tzero < forMillis && didrecieve == 0){
    // Si Serial1 (caméra) a envoyé des informations
    if(Serial1.available()>0){
      String message = "";
      //Récupère tous les caractères et les concatène dans "message"
      while(Serial1.available()>0){
        char c = Serial1.read();
        message.concat(c);
      }
      // Debug
      Serial.println(message);
      // Si on a demandé à la fonction de déplacer les moteurs
      if(moteur_active == 1){
        // 1 -> Tourne dans le sens inverse des aiguilles d'une 
        // 2 -> Tourne dans le sens des aiguilles d'une 
        // 3 -> Avance (l'objet détecté est au centre le l'image)
        if(message.indexOf("1")!=-1){
          moteurs("tg", 50);
        } else if(message.indexOf("2")!=-1){
          moteurs("td", 50);
        } else if(message.indexOf("3")!=-1){
          moteurs("av", 100);
        }
      }
      // Mesure de sécurité pour éviter les messages vides
      if(message.length()>0){
        didrecieve = 1;
      }
      message = "";
    }
    delay(10);
  }
  delay(50);
  // Check Debug
  Serial.println("rien");
  return didrecieve;
}


// Fonction qui permet à la caméra de balayer la scène pour trouver des objets (méduse ou sac)
int turnTime = 1000;
int waitTime = 400;
int ampliPat = 20;
int ampliAvan = 25;
void scanne(){
  if(digitalRead(A1) >= 800){
    turnTime = 2500;
    waitTime = 400;
  }
  int atrouve = 0;  
  // Utilise while et des if pour réagir le plus vite et ne pas perdre de temps
  // Appelle la fonction "checkSerial" comme équivalent de "delay()" mais qui permet de se terminer
  // dès qu'on recoit un message dans le Serial1 pour finir "scanne" le plus vite possible
/*
  paAD.write(90+ampliAvan);
  paBD.write(90+ampliPat);
  moteurs("tg", 150);
  delay(turnTime);

  moteurs("tg", 0);
  paAD.write(90);
  paBD.write(90);
  delay(waitTime);
  
  paAG.write(90-ampliAvan);
  paBG.write(90-ampliPat);
  moteurs("td", 150);
  delay(turnTime);

  paAG.write(90);
  paBG.write(90);
  moteurs("td", 0);
  delay(waitTime);

  paAG.write(90-ampliAvan);
  paBG.write(90-ampliPat);
  moteurs("td", 150 );
  delay(turnTime);

  paAG.write(90);
  paBG.write(90);
  moteurs("td", 0);
  delay(waitTime);

  paAD.write(90-ampliAvan);
  paBD.write(90-ampliPat);
  moteurs("tg", 150 );
  delay(turnTime);

  paAD.write(90);
  paBD.write(90);
  moteurs("tg", 0);
  delay(waitTime);*/

  while(atrouve==0){
    // Regarde a gauche
    if(atrouve==0){
      paAD.write(90+ampliAvan);
      paBD.write(90+ampliPat);
      moteurs("tg", 150);
      atrouve = checkSerial(turnTime, 0);
    }
    if(atrouve==0){
      moteurs("tg", 0);
      paAD.write(90);
      paBD.write(90);
      atrouve = checkSerial(waitTime, 0);
    }
    // Reviens
    if(atrouve==0){
      paAG.write(90-ampliAvan);
      paBG.write(90-ampliPat);
      moteurs("td", 150);
      atrouve = checkSerial(turnTime, 0);
    }
    if(atrouve==0){
      paAG.write(90);
      paBG.write(90);
      moteurs("td", 0);
      atrouve = checkSerial(waitTime, 0);
    }
    // Regarde a droite
    if(atrouve==0){
      paAG.write(90-ampliAvan);
      paBG.write(90-ampliPat);
      moteurs("td", 150 );
      atrouve = checkSerial(turnTime, 0);
    }
    if(atrouve==0){
      paAG.write(90);
      paBG.write(90);
      moteurs("td", 0);
      atrouve = checkSerial(waitTime, 0);
    }
    // Reviens
    if(atrouve==0){
      paAD.write(90+ampliAvan);
      paBD.write(90+ampliPat);
      moteurs("tg", 150 );
      atrouve = checkSerial(turnTime, 0);
    }
    if(atrouve==0){
      paAD.write(90);
      paBD.write(90);
      moteurs("tg", 0);
      atrouve = checkSerial(waitTime, 0);
    }
    Serial.println("Recherche");
  }
  return;
}


// Similaire à checkSerial mais ne s'éxecute pas pour un temps donné, le test se fait jusqu'a ce 
// qu'il n'y ait plus de messages durant un temps donné "threshold"
void suis_camera(){
  long compteur = 0;
  long threshold = 1000;
  while(compteur < threshold && analogRead(A2) <= 500){
    if(Serial1.available()>0){
      String message = "";
      while(Serial1.available()>0){
        char c = Serial1.read();
        message.concat(c);
      }

      Serial.println(message);
      // 1 -> Tourne dans le sens inverse des aiguilles d'une 
      // 2 -> Tourne dans le sens des aiguilles d'une 
      // 3 -> Avance (l'objet détecté est au centre le l'image)
      if(message.indexOf("1")!=-1){
        Serial.println("Gauche");
        moteurs("tg", 80);
        delay(100);
      } else if(message.indexOf("2")!=-1){
        Serial.println("Droite");
        moteurs("td", 80);
        delay(100);
      } else if(message.indexOf("3")!=-1){
        Serial.println("avance");
        moteurs("av", 200);
        delay(100);
      } else {
        Serial.println("Message pas compris");
        moteurs("av", 0);
      }
      compteur = 0;
    } else {
      delay(1);
      compteur ++;
      //Serial.println(compteur);
    }
  }
  if(analogRead(A2) >= 500){
    Serial.println("Obstacle");
  }
  moteurs("av", 0);
  Serial.println("finSuivicamera");
  return;
}

//////////////////////////////////// Fonctions principales  /////////////////////////////////////

void setup() {
  Serial.begin(38400); // Juste pour le débug

  Serial1.begin(9600); // Camera, TX en 18 et RX en 19
                      // UART(3): (TX, RX) = (P4, P5)
                      // Donc connecter 18-P5 et 19-P4
  Serial2.begin(9600); // Coraux, TX en 16, RX en 17
                      // Xbee : Dout et Din
                      // Donc connecter 16-Din et 17-Dout

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);

  // Un objet pour chaque servomoterus, simple etèfiable
  paAG.attach(12);
  paAD.attach(9);
  paBG.attach(11);
  paBD.attach(10);
  paQU.attach(13);
  
  for(int i=0;i<4;i++)
  {
    // Initialisation des pins pour chaque moteur,
    // pas besoin de modifier ça si on change les pins moteurs
    pinMode(in1[i], OUTPUT);
    pinMode(in2[i], OUTPUT);
    pinMode(ena[i], OUTPUT);
  }
  // Debug
  Serial.println("Pins ok");

  // Initialisation à 90 degrés de chaque servo
  paAG.write(90);
  delay(200);
  paAD.write(90);
  delay(200);
  paBG.write(90);
  delay(200);
  paBD.write(90);
  delay(200);
  paQU.write(90);
  delay(200);

  // Debug
  Serial.println("Lance loop");

  // Montre les mecanum

  

  //delay(30*1000);
}

void loop() {
  // Attend une détection de la caméra
  scanne();
  Serial.println("Fini scan");
  
  // Suis les instructions de la caméra
  suis_camera();
  Serial.print("Fini suivi");

  moteurs("re", 180);
  delay(500);
  moteurs("re", 0);
  delay(10);
  moteurs("re", 0);
    
  
  // Attend 10 secondes avant de recommencer un scan
  delay(10*1000);
  turnTime = 2500;
  waitTime = 400;
}
