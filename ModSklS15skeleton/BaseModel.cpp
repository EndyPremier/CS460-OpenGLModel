// BASEMODEL.CPP : Intended as a basis for new models with tests

#include "modelerview.h"
#include "modelerapp.h"
#include "modelerdraw.h"
#include <FL/gl.h>
#include <math.h>

#include "modelerglobals.h"


// main header for modeler
class BaseModel : public ModelerView
{
public:
  BaseModel(int x, int y, int w, int h, char *label)
    : ModelerView(x, y, w, h, label) { }

  virtual void draw();
};

// container for modeler away from API
ModelerView* createBaseModel(int x, int y, int w, int h, char *label)
{
  return new BaseModel(x, y, w, h, label);
}

// draw() definition for modeler
void BaseModel::draw()
{
  // main ModelerView::draw() interface
  ModelerView::draw();

  // draw the floor
  setAmbientColor(.1f, .1f, .1f);
  setDiffuseColor(COLOR_RED);

  glPushMatrix();
    glTranslated(-7.5, 0.0, -7.5);

    drawBox(15, 0.01f, 15);
  glPopMatrix();

  // draw primitives shapes
  setDiffuseColor(COLOR_BLUE);

  glPushMatrix();
    glTranslated(0, 1.0f + VAL(ALTITUDE), 0);
    drawSphere(1);
  glPopMatrix();

  glPushMatrix();
    glTranslated(2.5f - 1.0f, 0.0f + VAL(ALTITUDE), 0.0f - 1.0f);
    glTranslated( 1,  1,  1);
      glRotated(VAL(ANGLE), 0, 1, 0);
    glTranslated(-1, -1, -1);
    drawBox(2, 2, 2);
  glPopMatrix();

  glPushMatrix();
    glTranslated(-1.5f - 1.0f, 0.0f + VAL(ALTITUDE), 1.0f - 1.0f);
    glRotated(VAL(ANGLE) - 90, 1, 0, 0);
    drawCylinder(2, 1, 0);
  glPopMatrix();

  glPushMatrix();
    glTranslated(0, 1, 3);
    double delta = M_PI*(VAL(ALTITUDE) - 1);
    drawTriangle( 3, sin(         delta),  1,
                  3, sin(M_PI/2 + delta), -1,
                 -3, sin(M_PI/2 + delta),  1);
    drawTriangle(-3, sin(M_PI/1 + delta), -1,
                  3, sin(M_PI/2 + delta), -1,
                 -3, sin(M_PI/2 + delta),  1);
  glPopMatrix();

}

// main runtime (disable it when creating other)
/*
int main()
{
  // init controller
  ModelerControl controls[BASE_NUM_CONTROLS];
  // individual controls
  controls[ALTITUDE] = ModelerControl("Altitude", 0, 2, 0.01f, 0);
  controls[ANGLE   ] = ModelerControl("Angle"   , -180, 180, 1, 0);

  ModelerApplication::Instance()->Init(&createBaseModel, controls, BASE_NUM_CONTROLS);
  return ModelerApplication::Instance()->Run();
}
*/
