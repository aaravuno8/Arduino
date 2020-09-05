int LEDport = 12;
int Nextword = 4;
int DotTime = 1000;
int DashTime = 1500;
String type = "Dot";
bool repeat = false;
int repeatTime = 3000;
bool nextword = false;
void setup() {
  // put your setup code here, to run once:
  pinMode(LEDport, OUTPUT);
  pinMode(Nextword, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(Nextword, HIGH);
  delay(3000);
  digitalWrite(Nextword, LOW);
  morsecode();
  morsecode();
  morsecode();
  morsecode();
  nextword = true;
  morsecode();
  morsecode();
}
void morsecode (){
  Serial.println(nextword);
  if (nextword == true){
    digitalWrite(Nextword, HIGH);
    nextword = false;
    delay(300);
    digitalWrite(Nextword, LOW);
  }
  delay(200);
  if (type == "Dot"){
    digitalWrite(LEDport, HIGH);
    delay(DotTime);
    digitalWrite(LEDport, LOW);}
  if (type == "Dash"){
    digitalWrite(LEDport, HIGH);
    delay(DashTime);
    digitalWrite(LEDport, LOW);}}
