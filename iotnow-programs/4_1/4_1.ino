#include<iotnow.h>
int programId = 4;
int versionId = 1;
void setup() {
  Serial.begin(115200);
  iotnow(programId,versionId);
}

void loop() {
  Serial.println("Hello World!");
  delay(1000);
}
