#include<iotnow.h>
int programId = 4;
int versionId = 2;
void setup() {
  Serial.begin(115200);
  iotnow(programId,versionId);
}

void loop() {
  Serial.println("Hello World! (new)");
  delay(1000);
}
