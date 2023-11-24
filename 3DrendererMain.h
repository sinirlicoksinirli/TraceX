#include "basic3DEngineUtility.h"
#include "screenController.h"
#include "soc/rtc_wdt.h"
#include "3DrendererUtils.h"
#define SVGA_800x600DefaultMultiplier 1
class TraceX1Class {
public:
  rendererType_t renderType = render_fragmentShader;
  struct cachedTriangle_t {
    double sortZ = 0;
    vector3I vertex0;
    vector3I vertex1;
    vector3I vertex2;
    RGB888 color;
  };
  struct ObjectData_t {
    struct vertex_t {
      char vertex;
      vector3I vertexLoc;
    };
    struct connection_t {
      char vertex0 = 0;
      char vertex1 = 0;
    };
    struct triangle_t {
      char vertex0 = 0;
      char vertex1 = 0;
      char vertex2 = 0;
      RGB888 color;
    };


    unsigned int countOfVertex = 0;
    unsigned int countOfConnection = 0;
    unsigned int countOfTriangles = 0;
    vertex_t vertexs[100];
    connection_t connections[120];
    triangle_t triangles[40];
    void addVertex(int _x, int _y, int _z, char _vertex) {
      vertexs[countOfVertex].vertex = _vertex;
      vertexs[countOfVertex].vertexLoc.x = _x;
      vertexs[countOfVertex].vertexLoc.y = _y;
      vertexs[countOfVertex].vertexLoc.z = _z;
      countOfVertex++;
    }
    void addConnection(char _vertex0, char _vertex1) {
      connections[countOfConnection].vertex0 = _vertex0;
      connections[countOfConnection].vertex1 = _vertex1;
      countOfConnection++;
    }
    void addTriangle(char _vertex0, char _vertex1, char _vertex2, RGB888 _color) {
      triangles[countOfTriangles].vertex0 = _vertex0;
      triangles[countOfTriangles].vertex1 = _vertex1;
      triangles[countOfTriangles].vertex2 = _vertex2;
      triangles[countOfTriangles].color = _color;
      countOfTriangles++;
    }
  };


  struct TDObjectAssetData_t {
    String assetName = "null";
    ObjectData_t *TDobject = NULL;
    void set3DObjectData(ObjectData_t *_TDobject) {
      TDobject = _TDobject;
    }
  };
private:
  vector3I cameraPos;
  angle cameraAngle;
  angle Fov;
  double multiplier = SVGA_800x600DefaultMultiplier;

  vector2I calculateVertexScreenPos(vector3I _Location, vector3I _AfLocation) {
    vector2I result(0, 0);
    vector3I Loc;
    vector3I camPos;
    int virtualScreenDistance = 0 /*360 / Fov.getAngle()*/;
    int x1 = 0;
    int x2 = 0;
    int y1 = 0;
    int y2 = 0;
    int z1 = 0;
    int z2 = 0;
    camPos.x = cameraPos.x * multiplier;
    camPos.y = cameraPos.y * multiplier;
    camPos.z = cameraPos.z * multiplier;
    Loc.x = _Location.x + _AfLocation.x;
    Loc.y = _Location.y + _AfLocation.y;
    Loc.z = _Location.z + _AfLocation.z;

    y2 = camPos.y + virtualScreenDistance;
    y1 = Loc.y - y2;
    if (Loc.x - camPos.x != 0) {
      x2 = Loc.x - camPos.x;
    } else {
      x2 = 0;
    }
    if ((y1 * x2) == 0 || (y1 + y2) == 0) {
      x1 = 0;
    } else {
      x1 = (y1 * x2) / (y1 + y2);
    }
    if (Loc.x >= 0) {
      if (x1 >= 0) {
        result.x = Loc.x + x1;
      } else {
        result.x = Loc.x - x1;
      }
    } else {
      if (x1 >= 0) {
        result.x = 0 - Loc.x + x1;
      } else {
        result.x = 0 - Loc.x - x1;
      }
    }
    result.x = Loc.x + x1;

    if (Loc.z - camPos.z != 0) {
      z2 = Loc.z - camPos.z;
    } else {
      z2 = 0;
    }

    if ((y1 * z2) == 0 || (y1 + y2) == 0) {
      z1 = 0;
    } else {
      z1 = (y1 * z2) / (y1 + y2);
    }
    result.y = abs(Loc.z) + z1;
    //Serial.println("calc result:" + String(result.x) + " ," + String(result.y));
    return result;
  }

public:
  void
  setCameraTransform(vector3I _Loc, angle _angle, angle _Fov) {
    cameraPos = _Loc;
    cameraAngle = _angle;
    Fov = _Fov;
  }
  void setCameraLocation(vector3I _Loc) {
    cameraPos = _Loc;
  }
  bool renderObjectAsset(TDObjectAssetData_t *objectData, vector3I _AfLocation) {
    Fov.setAngle(360);
    if (renderType == render_wireFrame) {
      for (int i = 0; i < objectData->TDobject->countOfConnection; i++) {
        rtc_wdt_feed();
        char vertex0 = objectData->TDobject->connections[i].vertex0;
        char vertex1 = objectData->TDobject->connections[i].vertex1;
        vector3I vertex0Loc;
        vector3I vertex1Loc;
        for (int b = 0; b < objectData->TDobject->countOfVertex; b++) {
          if (objectData->TDobject->vertexs[b].vertex == vertex0) {
            vector3I vert0(0, 0, 0);
            vert0.x = objectData->TDobject->vertexs[b].vertexLoc.x * multiplier;
            vert0.y = objectData->TDobject->vertexs[b].vertexLoc.y * multiplier;
            vert0.z = objectData->TDobject->vertexs[b].vertexLoc.z * multiplier;
            vertex0Loc = vert0;
          }
          if (objectData->TDobject->vertexs[b].vertex == vertex1) {
            vector3I vert1(0, 0, 0);
            vert1.x = objectData->TDobject->vertexs[b].vertexLoc.x * multiplier;
            vert1.y = objectData->TDobject->vertexs[b].vertexLoc.y * multiplier;
            vert1.z = objectData->TDobject->vertexs[b].vertexLoc.z * multiplier;
            vertex1Loc = vert1;
          }
        }
        //Serial.println("setting:" + String(i));
        vector2I calcVertex0 = calculateVertexScreenPos(vertex0Loc, _AfLocation);
        vector2I calcVertex1 = calculateVertexScreenPos(vertex1Loc, _AfLocation);

        cv.setPenColor(Color::White);
        // cv.drawLine(320 + calcVertex0.x, 175 - calcVertex0.y, 320 + calcVertex1.x, 175 - calcVertex1.y);
        cv.setPixel(320 + calcVertex0.x, 175 - calcVertex0.y, RGB888(255, 0, 0));
        cv.setPixel(320 + calcVertex1.x, 175 - calcVertex1.y, RGB888(255, 0, 0));
        //Serial.println("writePos:" + String(320 + calcVertex0.x) + "," + String(175 - calcVertex0.y) + "---" + String(320 + calcVertex1.x) + "," + String(175 - calcVertex1.y));
        //cv.drawText(320 + calcVertex0.x, 175 - calcVertex1.y, String(" ").c_str());
        //cv.drawText(320 + calcVertex0.x, 175 - calcVertex1.y, String(i).c_str());
      }
    } else if (renderType == render_fragmentShader) {
      vector3I camPos;
      unsigned int countOfRenderQueue = 0;
      cachedTriangle_t triangleRenderQueue[40];
      camPos.x = cameraPos.x * multiplier;
      camPos.y = cameraPos.y * multiplier;
      camPos.z = cameraPos.z * multiplier;
      for (int i = 0; i < objectData->TDobject->countOfTriangles; i++) {
        rtc_wdt_feed();
        char vertex0 = objectData->TDobject->triangles[i].vertex0;
        char vertex1 = objectData->TDobject->triangles[i].vertex1;
        char vertex2 = objectData->TDobject->triangles[i].vertex2;
        vector3I vertex0Loc;
        vector3I vertex1Loc;
        vector3I vertex2Loc;
        for (int b = 0; b < objectData->TDobject->countOfVertex; b++) {
          if (objectData->TDobject->vertexs[b].vertex == vertex0) {
            vector3I vert0(0, 0, 0);
            vert0.x = objectData->TDobject->vertexs[b].vertexLoc.x * multiplier;
            vert0.y = objectData->TDobject->vertexs[b].vertexLoc.y * multiplier;
            vert0.z = objectData->TDobject->vertexs[b].vertexLoc.z * multiplier;
            vertex0Loc = vert0;
          }
          if (objectData->TDobject->vertexs[b].vertex == vertex1) {
            vector3I vert1(0, 0, 0);
            vert1.x = objectData->TDobject->vertexs[b].vertexLoc.x * multiplier;
            vert1.y = objectData->TDobject->vertexs[b].vertexLoc.y * multiplier;
            vert1.z = objectData->TDobject->vertexs[b].vertexLoc.z * multiplier;
            vertex1Loc = vert1;
          }
          if (objectData->TDobject->vertexs[b].vertex == vertex2) {
            vector3I vert2(0, 0, 0);
            vert2.x = objectData->TDobject->vertexs[b].vertexLoc.x * multiplier;
            vert2.y = objectData->TDobject->vertexs[b].vertexLoc.y * multiplier;
            vert2.z = objectData->TDobject->vertexs[b].vertexLoc.z * multiplier;
            vertex2Loc = vert2;
          }
        }
        triangleRenderQueue[countOfRenderQueue].vertex0 = vertex0Loc;
        triangleRenderQueue[countOfRenderQueue].vertex1 = vertex1Loc;
        triangleRenderQueue[countOfRenderQueue].vertex2 = vertex2Loc;
        triangleRenderQueue[countOfRenderQueue].color = objectData->TDobject->triangles[i].color;

        vector3F toCamDistance0;
        vector3F toCamDistance1;
        vector3F toCamDistance2;

        toCamDistance0.x = vertex0Loc.x * -1 - camPos.x;
        toCamDistance0.y = vertex0Loc.y - camPos.y;
        toCamDistance0.z = vertex0Loc.z - camPos.z;

        toCamDistance1.x = vertex1Loc.x * -1 - camPos.x;
        toCamDistance1.y = vertex1Loc.y - camPos.y;
        toCamDistance1.z = vertex1Loc.z - camPos.z;

        toCamDistance2.x = vertex2Loc.x * -1 - camPos.x;
        toCamDistance2.y = vertex2Loc.y - camPos.y;
        toCamDistance2.z = vertex2Loc.z - camPos.z;

        double vert0Dist = pow((toCamDistance0.x * toCamDistance0.x) + (toCamDistance0.y * toCamDistance0.y) + (toCamDistance0.z * toCamDistance0.z), 0.5);
        double vert1Dist = pow((toCamDistance1.x * toCamDistance1.x) + (toCamDistance1.y * toCamDistance1.y) + (toCamDistance1.z * toCamDistance1.z), 0.5);
        double vert2Dist = pow((toCamDistance2.x * toCamDistance2.x) + (toCamDistance2.y * toCamDistance2.y) + (toCamDistance2.z * toCamDistance2.z), 0.5);

        if (vert0Dist < vert1Dist && vert0Dist < vert2Dist) {
          triangleRenderQueue[countOfRenderQueue].sortZ = vert0Dist;
        } else if (vert1Dist < vert0Dist && vert1Dist < vert2Dist) {
          triangleRenderQueue[countOfRenderQueue].sortZ = vert1Dist;
        } else if (vert2Dist < vert0Dist && vert2Dist < vert1Dist) {
          triangleRenderQueue[countOfRenderQueue].sortZ = vert2Dist;
        }
        countOfRenderQueue++;
      }
      //sorting sortZ vals
      unsigned int countOfSortedRenderQueue = 0;
      cachedTriangle_t sortedTriangleRenderQueue[40];
      int renderQueueTemp = countOfRenderQueue;
      for (int h = 0; h < renderQueueTemp; h++) {
        int maxVal = triangleRenderQueue[0].sortZ;
        int maxValIndex = 0;
        for (int item = 0; item < countOfRenderQueue; item++) {
          if (triangleRenderQueue[item].sortZ < maxVal) {
            maxVal = triangleRenderQueue[item].sortZ;
            maxValIndex = item;
          }
        }
        sortedTriangleRenderQueue[countOfSortedRenderQueue] = triangleRenderQueue[maxValIndex];
        countOfSortedRenderQueue++;
        for (int f = maxValIndex; f < countOfRenderQueue; f++) {
          triangleRenderQueue[f] = triangleRenderQueue[f + 1];
        }
        countOfRenderQueue--;
      }
      Serial.println("----------------------------------------");
      //for (int g = 0; g < countOfSortedRenderQueue; g++) {
      for (int g = countOfSortedRenderQueue; g > 0; g--) {
        Serial.println("sorted index: " + String(g) + ", value: " + String(sortedTriangleRenderQueue[g].sortZ));
        vector2I calcVertex0 = calculateVertexScreenPos(sortedTriangleRenderQueue[g].vertex0, _AfLocation);
        vector2I calcVertex1 = calculateVertexScreenPos(sortedTriangleRenderQueue[g].vertex1, _AfLocation);
        vector2I calcVertex2 = calculateVertexScreenPos(sortedTriangleRenderQueue[g].vertex2, _AfLocation);
        cv.setBrushColor(sortedTriangleRenderQueue[g].color);
        Point points[3];
        //   Serial.println("**************************");
        points[0] = { 320 + calcVertex0.x, 175 - calcVertex0.y };
        //  Serial.println(String(320 + calcVertex0.x) + "," + String(175 - calcVertex0.y));
        points[1] = { 320 + calcVertex1.x, 175 - calcVertex1.y };
        //  Serial.println(String(320 + calcVertex1.x) + "," + String(175 - calcVertex1.y));
        points[2] = { 320 + calcVertex2.x, 175 - calcVertex2.y };
        //  Serial.println(String(320 + calcVertex2.x) + "," + String(175 - calcVertex2.y));
        //  Serial.println("++++++++++++++++++++++++++++");
        cv.fillPath(points, 3);
        /*
        cv.setPenColor(Color::White);
        cv.drawLine(320 + calcVertex0.x, 175 - calcVertex0.y, 320 + calcVertex1.x, 175 - calcVertex1.y);
        cv.drawLine(320 + calcVertex1.x, 175 - calcVertex1.y, 320 + calcVertex2.x, 175 - calcVertex2.y);
        cv.drawLine(320 + calcVertex2.x, 175 - calcVertex2.y, 320 + calcVertex0.x, 175 - calcVertex0.y);
        */
      }
      Serial.println("*******************************************");
      cv.setBrushColor(RGB888(0, 0, 0));
      //final calcs
    }
    return true;
  }
} TraceX1;