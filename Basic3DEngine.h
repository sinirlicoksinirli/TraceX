#include "3DrendererMain.h"

class Basic3DEngineClass {
  class playerClass {
  public:
    vector3F Location;
    angle PlayerAngle;
    angle Fov;
  private:
    void updateCamera() {
      Fov.setAngle(90);
      TraceX1.setCameraTransform(Location.ToInt(), PlayerAngle, this->Fov);
    }
  };
  class dynamicObjectClass {
  private:
    TraceX1Class::TDObjectAssetData_t *mesh = NULL;
  public:
    vector3F Location;
    dynamicObjectClass(vector3F *_ConstructLoc, TraceX1Class::TDObjectAssetData_t *_defaultMesh) {
      Location.x = _ConstructLoc->x;
      Location.y = _ConstructLoc->y;
      Location.z = _ConstructLoc->z;
      mesh = _defaultMesh;
    }
    void UpdateObjectOnTheScreen() {
      TraceX1.renderObjectAsset(mesh, Location.ToInt());
    }
  };
public:
  bool debug = 1;
  unsigned int countOfSceneDynamicObjects = 0;
  dynamicObjectClass *sceneDynamicObjects[10];

  dynamicObjectClass *createNewDynamicObject(vector3F *_Loc, TraceX1Class::TDObjectAssetData_t *_mesh) {
    sceneDynamicObjects[countOfSceneDynamicObjects] = new dynamicObjectClass(_Loc, _mesh);
    countOfSceneDynamicObjects++;
    return sceneDynamicObjects[countOfSceneDynamicObjects - 1];
  }
  void updateAllObjects() {
    unsigned long RenderTimeS = 0;
    unsigned int RenderTime = 0;
    RenderTimeS = millis();
    cv.clear();
    for (int i = 0; i < countOfSceneDynamicObjects; i++) {
      sceneDynamicObjects[i]->UpdateObjectOnTheScreen();
    }
    RenderTime = millis() - RenderTimeS;
    int fps = 0;
    if (RenderTime != 0) {
      fps = 1000 / RenderTime;
    }
    if (debug) {
      String print = "RenderTime:" + String(RenderTime) + "ms FPS:" + String(fps) + " Dynamic Object Count:" + String(countOfSceneDynamicObjects);
      cv.drawText(0, 0, print.c_str());
    }
    cv.swapBuffers();
  }

} engine;