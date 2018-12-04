/* Kynky Daphne
 *  //El nombre deriva de la Ninfa del mismo nombre del mito griego Apollo y Daphne, 
 *  el adjetivo Kynky es agregado en honor del RedBull RB7 (2011) de Sebastian Vettel
 *  nombrado 'Kynky Kylie' por el piloto alemán.
 *  
 * 
 *  
 *  ------------------------------------------------
 *  Este sistema es básicamente un sistema de acceso simple donde el sensor ultrasónico
 *  detecta la presencia de alguien, posteriormente lee la entrada de un tag, si el tag 
 *  esta autorizado mostrara el mensaje correspondiente y activara el sistema de acceso,
 *  sino el sistema negara el acceso hasta que dicha tag sea registrada por tag.
 *  
 *  Se utiliza la comunicación serial por Bluetooth, originalmente se planeaba usar un LCD16X02
 *  sin embargo, debido a las limitantes presentadas por el Arduino Uno en cuetion de pines de salida
 *  se optó por usar dicha comunciación.
 *  ------------------------------------------------
 *  
 *  Conexión de módulo a Arduino uno
 *   * ---------------------------------------------
 *             MFRC522      Arduino       
 *             Reader/PCD   Uno/101       
 * Signal      Pin          Pin           
 * -------------------------------------------------
 * RST/Reset   RST          9             
 * SPI SS      SDA(SS)      10            
 * SPI MOSI    MOSI         11 / ICSP-4   
 * SPI MISO    MISO         12 / ICSP-1   
 * SPI SCK     SCK          13 / ICSP-3   
 */

#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define RST_PIN         9         
#define SS_PIN          10 
#define L_Azul          8
#define L_Rojo          7 
#define L_Verde         6
#define trigger         5
#define echo            4

MFRC522 mfrc522(SS_PIN, RST_PIN);     //Create MFRC522 instance

Servo DaphneServo;      //Define nombre del servo

float tiempo, distancia;
void setup() {
  Serial.begin(9600); // Inicializa la comunicación serial 
  SPI.begin();
  mfrc522.PCD_Init();
  DaphneServo.attach(3);  ///donde esta conectado el servo motor (la señal del servo pues)
  DaphneServo.write(1);
  pinMode(L_Azul,OUTPUT); 
  pinMode(L_Rojo,OUTPUT); //No acceso
  pinMode(L_Verde,OUTPUT);  //Si a
  pinMode(trigger,OUTPUT);  //trigger de sensor ultrasónico
  pinMode(echo,INPUT);    //echo para el sensor utlrasónico
  Serial.println("Kinky Daphne");
  delay(1000);
  Serial.println();
}

void loop() {
  digitalWrite (trigger,LOW);
  delayMicroseconds (2);
  digitalWrite(trigger,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger,LOW);

   tiempo = pulseIn(echo,HIGH);

   distancia = tiempo/58;

   if (distancia <= 20){
    digitalWrite(L_Azul,LOW);
    Serial.println("Presente TAG");
    Serial.println();
    delay(1000);
    //Busca tarjeta
    if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // lee tarjeta
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Muestra código UID en la serial
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  
  content.toUpperCase();
  Serial.println();
  
  if (content.substring(1) == "C7 8C 0D 04") 
  {
    Serial.println("Bienvenido");
    Serial.println("Augusto Rivera");
    Serial.println();
    Serial.println("El comunismo vivirá");
    Serial.println();
    delay(500);
    digitalWrite(L_Verde, HIGH);
    delay(1000);
    DaphneServo.write(90);
    delay(2000);
    DaphneServo.write(1);
    digitalWrite(L_Verde, LOW);
    delay(500);
  }
  else if (content.substring(1) == "0C 58 D9 74")
  {
    Serial.println("Bienvenido");
    Serial.println("Fernando Alonso Lara");
    Serial.println();
    Serial.println("Ten un excelente día");
    Serial.println();
    delay(500);
    digitalWrite(L_Verde, HIGH);
    delay(1000);
    DaphneServo.write(90);
    delay(2000);
    DaphneServo.write(1);
    digitalWrite(L_Verde, LOW);
    delay(500);
  }
 
 else   {
    Serial.println("Usted no puede acceder");
    Serial.println();
    digitalWrite(L_Rojo, HIGH);
    delay(1000);
    digitalWrite(L_Rojo, LOW);
    
  }
    ////////
   }else{
    digitalWrite(L_Azul,HIGH);
   }
 //  Serial.print(distancia);
   //Serial.println("cm");
   //delay(1000);
}
