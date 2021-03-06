#include <Arduboy2.h>
#include "VectorMaths.h"
#include "Mesh.h"
#include "Camera.h"
#include "RenderedObject.h"
#include "RenderDithered.h"

#define TARGET_FRAMERATE 50
#define NEAR 0.01f
#define FAR 100.0f
#define DEG_2_RAD 0.01745329f
#define FOV_RADIANS (60.0f * DEG_2_RAD)

Arduboy2 arduboy;

Matrix4f projectionMat;

extern Vector3f lightVector;

RenderedObject* testObject;

float delta = 0.0f;

bool justRenderedFlag = false;

int numSkips = 0;

void setup() {
  arduboy.begin();
  arduboy.setFrameRate(TARGET_FRAMERATE);
  delta = 1 / 50.0f;
  setupProjectionMatrix();

  lightVector = {0.0f, -1.0f, 0.0f};

  //Test
  testObject = new RenderedObject({0.0f, 0.0f, 5.0f}, {0.0f, 0.0f, 0.0f}, &primitiveCube);
}

void loop() {
  if (!arduboy.nextFrame()) {
    numSkips++;
    return;
  }

  //Arduboy2Base::pollButtons();

  testObject->rotation.x += (120.0f * delta) * DEG_2_RAD;
  testObject->rotation.y += (180.0f * delta) * DEG_2_RAD;
  testObject->rotation.z += (20.0f * delta) * DEG_2_RAD;
  testObject->markUpdated();

  Serial.print("Num skips between renders: ");
  Serial.println(numSkips);
  
  render();

  numSkips = 0;
}

void render() {
  arduboy.clear();

  renderDithered(arduboy, projectionMat, testObject);

  arduboy.display();
}

void setupProjectionMatrix() {
  float aspect = WIDTH / HEIGHT;
  float tanHalfFov = tan(FOV_RADIANS / 2.0f);

  projectionMat = {
    1.0f / (aspect * tanHalfFov), 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f / tanHalfFov, 0.0f, 0.0f,
    0.0f, 0.0f, -(FAR + NEAR) / (FAR - NEAR), -(2.0f * FAR * NEAR) / (FAR - NEAR),
    0.0f, 0.0f, -1.0f, 0.0f
  };
}
