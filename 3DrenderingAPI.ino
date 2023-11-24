#include "Basic3DEngine.h"

TraceX1Class::TDObjectAssetData_t cube;
TraceX1Class::ObjectData_t cubeData;
TraceX1Class::TDObjectAssetData_t model01;
TraceX1Class::ObjectData_t model01Data;
int a = 0;
int b = 20;


void TDApiProcess(void *param) {
  rtc_wdt_protect_off();
  rtc_wdt_disable();
  for (;;) {
    rtc_wdt_feed();
    if (a > 60) {
      a = -60;
    }
    if (b > 50) {
      b = 0;
    }
    TraceX1.setCameraLocation(vector3I(a, -150, b));
    engine.updateAllObjects();
  }
  vTaskDelete(NULL);
}
void setup() {
  Serial.begin(115200);
  cubeData.addVertex(10, 10, 10, 1);
  cubeData.addVertex(30, 10, 10, 2);
  cubeData.addVertex(10, 30, 10, 3);
  cubeData.addVertex(30, 30, 10, 4);
  cubeData.addVertex(10, 10, 30, 5);
  cubeData.addVertex(30, 10, 30, 6);
  cubeData.addVertex(10, 30, 30, 7);
  cubeData.addVertex(30, 30, 30, 8);

  cubeData.addConnection(1, 2);
  cubeData.addConnection(2, 4);
  cubeData.addConnection(4, 3);
  cubeData.addConnection(3, 1);

  cubeData.addConnection(5, 6);
  cubeData.addConnection(6, 8);
  cubeData.addConnection(8, 7);
  cubeData.addConnection(7, 5);

  cubeData.addConnection(1, 5);
  cubeData.addConnection(2, 6);
  cubeData.addConnection(3, 7);
  cubeData.addConnection(4, 8);

  cubeData.addTriangle(1, 2, 5, RGB888(0, 0, 255));
  cubeData.addTriangle(2, 5, 6, RGB888(0, 0, 255));

  cubeData.addTriangle(1, 3, 7, RGB888(0, 255, 0));
  cubeData.addTriangle(1, 5, 7, RGB888(0, 255, 0));

  cubeData.addTriangle(2, 4, 6, RGB888(0, 255, 255));
  cubeData.addTriangle(4, 6, 8, RGB888(0, 255, 255));

  cubeData.addTriangle(3, 4, 8, RGB888(255, 0, 0));
  cubeData.addTriangle(3, 7, 8, RGB888(255, 0, 0));

  cubeData.addTriangle(6, 7, 8, RGB888(255, 0, 255));
  cubeData.addTriangle(5, 6, 7, RGB888(255, 0, 255));

  cubeData.addTriangle(2, 3, 4, RGB888(255, 255, 0));
  cubeData.addTriangle(1, 2, 3, RGB888(255, 255, 0));

  cube.assetName = "Cube";
  cube.set3DObjectData(&cubeData);

  initScreenDriver();
  engine.createNewDynamicObject(new vector3F(-10, 40, -10), &cube);
  //engine.createNewDynamicObject(new vector3F(40, 0, -1), &cube);
  //engine.createNewDynamicObject(new vector3F(0, -70, -1), &cube);
  //engine.createNewDynamicObject(new vector3F(-40, 0, -1), &cube);

  xTaskCreatePinnedToCore(TDApiProcess, "TraceX1.1 shader Worker", 32000, NULL, 10, NULL, 0);
}
void loop() {
  delay(200);
  // b++;
  a++;
  if (Serial.available()) {
    a = Serial.readString().toInt();
  }
}
