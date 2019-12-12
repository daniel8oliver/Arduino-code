const int ledPin = 13; // the pin that the LED is attached to
const byte buffSize = 40;
unsigned int inputBuffer[buffSize];
const char startMarker = '<';
const char endMarker = '>';
byte bytesRecvd = 0;
boolean readInProgress = false;
boolean newDataFromPC = false;
byte coordinates[4];

Servo servo1;
Servo servo2;
Servo servo3;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  servo1.attach(9);
  servo2.attach(10);
  servo3.attach(11);
}

void loop()
{
  getDataFromPC();
  if (newDataFromPC)
  {
    sendSuspendCmd();
    digitalWrite(ledPin, HIGH);
    delay(5000);
    digitalWrite(ledPin, LOW);
    servo1.write(coordinates[0]);
    servo2.write(coordinates[1]);
    servo3.write(coordinates[2]);
    
    sendEnableCmd();
    sendCoordinatesToPC();
    newDataFromPC = false;
  }
}

void sendSuspendCmd()
{
  // send the suspend-true command
  Serial.println("<S1>");
}
void sendEnableCmd()
{
  // send the suspend-false command
  Serial.println("<S0>");
}
void sendCoordinatesToPC()
{
  // send the point data to the PC
  Serial.print("<P");
  Serial.print(coordinates[0]);
  Serial.print(",");
  Serial.print(coordinates[1]);
  Serial.print(",");
  Serial.print(coordinates[2]);
  Serial.print(",");
  Serial.print(coordinates[3]);
  Serial.println(">");
}
// alternative to the readBytes function:
void getDataFromPC()
{
  // receive data from PC and save it into inputBuffer
  if (Serial.available() > 0)
  {
    char x = Serial.read();
    // the order of these IF clauses is significant
    if (x == endMarker)
    {
      readInProgress = false;
      newDataFromPC = true;
      inputBuffer[bytesRecvd] = 0;
      coordinates[0] = inputBuffer[0];
      coordinates[1] = inputBuffer[1];
      coordinates[2] = inputBuffer[2];
      coordinates[3] = inputBuffer[3];
    }
    if (readInProgress)
    {
      inputBuffer[bytesRecvd] = x;
      bytesRecvd ++;
      if (bytesRecvd == buffSize)
      {
        bytesRecvd = buffSize - 1;
      }
    }
    if (x == startMarker)
    {
      bytesRecvd = 0;
      readInProgress = true;
    }
  }
}
