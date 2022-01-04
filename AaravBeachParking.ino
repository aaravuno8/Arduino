#include <Servo.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

Servo servo;
LiquidCrystal c(13, 12, 8, 7, 4, 2);
SoftwareSerial bluetooth(9, 10);

int count = 0;
int sensor;
int msensor;
int oc = 3;
String input;

int carIn_Buzzer_Freq = 1;
int carOut_Buzzer_Freq = 1;

int PORT_SERVO_MOTOR = 11;
int PORT_BUZZER = 1;


void setup() {
  //power LCD
  c.begin(16, 2);

  //configure IR Sensor Ports
  pinMode(A1, INPUT); //add sensor
  pinMode(A2, INPUT); //exit sensor


  //configure bluetooth baud rate
  bluetooth.begin(9600);
  Serial.begin(9600);

  //configure output port
  pinMode(PORT_BUZZER, OUTPUT);  // Buzzer

  showData_LCD_App();

  //configure servo port
  servo.attach(PORT_SERVO_MOTOR);

  lightOn();

}


void loop() {
  // put your main code here, to run repeatedly:

  //read sensor
  msensor = digitalRead(A2); //exit
  sensor = digitalRead(A1); //add

  readBluetooth();

  //check max occupancy
  checkCarMaxOccupancy();

  checkCarInSensor();
  checkCarOutSensor();
}

void showData_LCD_App()
{
  //show initial data in LCD, Bluetooth data to App
  display_Count_On_LCD();
  Bluetooth_CarCount_Data_To_App();
}


void display_Count_On_LCD() {
  c.clear();
  c.setCursor(0, 0);
  c.print("Car Count: " + String(count));
  c.setCursor(0, 1);
  c.print("Max Occupacy: " + String(oc));
}

void Bluetooth_Light_Data_To_App()
{
  if (String(digitalRead(PORT_PAIR_ONE_LED)) == LOW) {
    bluetooth.println("LED: off");
  } else {
    bluetooth.println("LED: on");
  }
}

void Bluetooth_CarCount_Data_To_App()
{

  bluetooth.println("Car Count: " + String(count));
  bluetooth.println("Max Car Count: " + String(oc));
}


void checkCarMaxOccupancy()
{
  if ((count > (oc - 1)) == 1)
  {
    //open gate
    servo.write(90);
  }
  else
  {
    //close gate
    servo.write(0);
  }
}


void checkCarInSensor()
{
  //add sensor
  if (sensor == HIGH)
  {
    count++;
    while (sensor == HIGH)
    {
      sensor = digitalRead(A1);
    }

    beepIt(carIn_Buzzer_Freq);
    showData_LCD_App();

  }
}

void checkCarOutSensor()
{
  if (msensor == HIGH)
  {
    //check if any car in parking
    if ((count - 1) != -1)
    {
      count = count - 1;
      while (msensor == HIGH)
      {
        msensor = digitalRead(A2);
      }
      beepIt(carOut_Buzzer_Freq);
      showData_LCD_App();

    }
  }
}

void beepIt(int freqHz) {
  Serial.println("In Beep It! freqHz : " + freqHz);
  tone(PORT_BUZZER, freqHz); //1 Hz frequency
  delay(1000);
}

void readBluetooth() {
  //read bluetooth
  if (bluetooth.available())
  {
    //read input from app
    input = bluetooth.readString();
    Serial.println("Input: "+input);
    
    //read reset from app
    if (input == "reset_count_21349")
    {
      count = 0;
    }
    //read max occupancy from app
    else if (input.substring(0, 5) != "+CONNE")
    {
      if (input.substring(0, 5) != "CONNEC")
      {
        if (input != "")
        {
          oc = input.toInt();
        }
      }
    }//else - end
    showData_LCD_App();
  } //bluetooth.available - end
}
