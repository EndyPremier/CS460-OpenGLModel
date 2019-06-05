// RkAlphaModel.cpp

// main components
#include "modelerview.h"
#include "modelerapp.h"
#include "modelerdraw.h"
#include <FL/gl.h>
#include <math.h>

// local variables
#include "RkAlphaValues.h"


// main header for modeler
class RkAlphaModel : public ModelerView
{
public:
  RkAlphaModel(int x, int y, int w, int h, char *label)
    : ModelerView(x, y, w, h, label) { }

  // main driver
  virtual void draw();
  virtual void animate();

  // COMPONENT
  virtual void TopHead();
    virtual void TopEye(double eyeShift, double browTilt);
    virtual void TopEar();
  virtual void Muzzle();
    virtual void BackTuft();
    virtual void TopTeeth();
    virtual void Snout(double muzzleWidth);
    virtual void JawSupport();
    virtual void BottomJaw();
    virtual void BottomTeeth();

  // Helpers
  virtual void drawOrigin();
  virtual void drawPoint();
  virtual void drawEdges(double x, double y1, double y2, double t);

private:
  int ticks = 0;

};

// container for modeler away from API
ModelerView* createRkAlphaModel(int x, int y, int w, int h, char *label)
{
  return new RkAlphaModel(x, y, w, h, label);
}

// draw() definition for modeler
void RkAlphaModel::draw()
{
  // main ModelerView::draw() interface
  ModelerView::draw();

  glPushMatrix();
    /* ANIMATION */
    animate();

    /* PRELOAD MATRIX */
    glTranslated(0,0.7,-HEAD_RAD+0.3);
    
    /* INITIAL TRANSFORMATION */
    glTranslated(VAL(X_POS), VAL(Y_POS), VAL(Z_POS));
    EULER_ROT(VAL(X_ROT), VAL(Y_ROT), VAL(Z_ROT));
    UNI_SCALE(VAL(SCALE));

    /* SETUP ORIGIN MODEL */
    drawOrigin();
    
    /* MAIN RENDER */
    glTranslated(0,-0.7,HEAD_RAD-0.3);
    TopHead();
    Muzzle();

  glPopMatrix();
}

void RkAlphaModel::animate()
{
  if (ModelerApplication::Instance()->IsAnimated())
  {
    double sines1   = sin(M_PI * ticks / 120);
    double sinessq1 = pow(sines1,2);
    double possin1 = sines1 > 0 ?  sines1 : 0;
    double possin2 = sines1 < 0 ? -sines1 : 0;


    SET(Z_ROT, -sines1*15);
    SET(Y_ROT, sines1*180/8);
    SET(X_ROT, -sinessq1*15);

    SET( LEFT_EAR_SHIFT,  sines1);
    SET(RIGHT_EAR_SHIFT, -sines1);

    SET( LEFT_EYE_SHIFT,  sines1);
    SET(RIGHT_EYE_SHIFT,  sines1);

    SET( LEFT_BROW_TILT, -sines1*10);
    SET(RIGHT_BROW_TILT, -sines1*10);

    SET(JAW_OPEN, possin1 * 15);

    SET(BOT_TEETH, possin1);
    SET(TOP_TEETH, possin2);

    glRotated(   -sines1*60, 0,1,0);
    glRotated(-sinessq1*2.5-10, 1,0,0);

    ticks++;
  }
}

/* COMPONENTS */
void RkAlphaModel::TopHead()
{
  // VARIABLES
  double headHeight = 1.0;
  double eyePop = 0.05;

  // RENDER
  glPushMatrix();
    //drawPoint();

    // base head
    SET_COLOR(COLOR_BASE);
    glPushMatrix();
      glRotated(-90, 1,0,0);
      drawCylinder(headHeight, HEAD_RAD, HEAD_RAD);
    glPopMatrix();
    
    glPushMatrix();
      glTranslated(-HEAD_RAD, 0, -HEAD_RAD);
      drawBox(HEAD_DIAM, headHeight, HEAD_RAD);
    glPopMatrix();

    // ear base
    glPushMatrix();
      glTranslated(0, headHeight, 0);
      // left ear
      glPushMatrix();
        glTranslated(HEAD_RAD, 0, -HEAD_RAD + 0.7 + VAL(RIGHT_EAR_SHIFT)*0.2);
        glRotated(90, 0,1,0);
        TopEar();
      glPopMatrix();
      // right ear
      glPushMatrix();
        glTranslated(-HEAD_RAD, 0, -HEAD_RAD + 0.7 + VAL(LEFT_EAR_SHIFT)*0.2);
        glRotated(-90, 0,1,0);
        TopEar();
      glPopMatrix();

    glPopMatrix();

    // base eyes
    glPushMatrix();
      glTranslated(0, headHeight-0.3, 0);
      
      // left eye
      glPushMatrix();
        glRotated(VAL(LEFT_EYE_SHIFT)*15-35, 0,1,0);
        glTranslated(0, 0, HEAD_RAD+eyePop);
        TopEye(VAL(LEFT_EYE_SHIFT), -VAL(LEFT_BROW_TILT));
      glPopMatrix();
      
      // left eye
      glPushMatrix();
        glRotated(VAL(RIGHT_EYE_SHIFT)*15+35, 0,1,0);
        glTranslated(0, 0, HEAD_RAD+eyePop);
        TopEye(VAL(RIGHT_EYE_SHIFT), VAL(RIGHT_BROW_TILT));
      glPopMatrix();
      
    glPopMatrix();
   
  glPopMatrix();
}

void RkAlphaModel::TopEye(double eyeShift, double browTilt)
{
  ModelerDrawState *mds = ModelerDrawState::Instance();

  double browSize = 0.1;
  double browLength = 1.0;
  double scaleraRad = browLength/2;
  double    irisRad = scaleraRad-0.1;
  double   pupilRad = irisRad-0.1;
  double depth = browLength;

  int fidelity;

  switch (mds->m_quality)
  {
    case HIGH:
      fidelity = 32; break;
    case MEDIUM:
      fidelity = 20; break;
    case LOW:
      fidelity = 12; break;
    case POOR:
      fidelity = 8; break;
  }

  glPushMatrix();
    //drawPoint();

    // brow
    SET_COLOR(COLOR_DARK);
    glPushMatrix();
      glTranslated(-browLength/2, 0.1,-depth);
      // centered rotation
      glTranslated( browLength/2,  browSize/2, 0);
      glRotated(browTilt, 0,0,1);
      glTranslated(-browLength/2, -browSize/2, 0);

      drawBox(browLength, browSize, depth);
    glPopMatrix();

    // eye-top
    SET_COLOR(COLOR_WHITE);
    drawTriangle(-scaleraRad,0,0, scaleraRad,0,0, -scaleraRad,0,-depth);
    drawTriangle(scaleraRad,0,0, scaleraRad,0,-depth, -scaleraRad,0,-depth);
    
    // scalera
    glPushMatrix();
      glScaled(scaleraRad,scaleraRad,1);

      for (int i = 0; i < fidelity; ++i)
      {
        double x1 = -cos(M_PI/fidelity*i);
        double y1 = -sin(M_PI/fidelity*i);
        double x2 = -cos(M_PI/fidelity*(i+1));
        double y2 = -sin(M_PI/fidelity*(i+1));

        drawTriangle(x1,y1,0, x2,y2,0, 0,0,0);
        drawTriangle(x1,y1,0, x1,y1,-depth, x2,y2,0);
        drawTriangle(x2,y2,0, x1,y1,-depth, x2,y2,-depth);
      }
    glPopMatrix();
    // iris
    SET_COLOR(COLOR_IRIS);
    glPushMatrix();
      glTranslated(eyeShift*0.1,0,0.01);
      glScaled(irisRad,irisRad,1);

      for (int i = 0; i < fidelity; ++i)
      {
        double x1 = -cos(M_PI/fidelity*i);
        double y1 = -sin(M_PI/fidelity*i);
        double x2 = -cos(M_PI/fidelity*(i+1));
        double y2 = -sin(M_PI/fidelity*(i+1));

        drawTriangle(x1,y1,0, x2,y2,0, 0,0,0);
      }
    glPopMatrix();
    // pupil
    SET_COLOR(COLOR_PUPIL);
    glPushMatrix();
      glTranslated(eyeShift*0.2,0,0.02);
      glScaled(pupilRad,pupilRad,1);

      for (int i = 0; i < fidelity; ++i)
      {
        double x1 = -cos(M_PI/fidelity*i);
        double y1 = -sin(M_PI/fidelity*i);
        double x2 = -cos(M_PI/fidelity*(i+1));
        double y2 = -sin(M_PI/fidelity*(i+1));

        drawTriangle(x1,y1,0, x2,y2,0, 0,0,0);
      }
    glPopMatrix();

  glPopMatrix();
}

void RkAlphaModel::TopEar()
{
  double earWidth  = 1;
  double earHeight = 1.5;
  double  inWidth  = 0.6;
  double  inHeight = 0.9;

  double baseLength = 0.5;
  double baseWidth  = 0.4;
  double baseRadius = baseWidth/2;
  double boltRadius = baseRadius-0.1;

  double thickness = 0.2;
  double     thick = 0.1;
  double    margin = 0.01;

  // RENDER
  glPushMatrix();
    //drawPoint();

    SET_COLOR(COLOR_BASE);
    drawTriangle(0,earHeight,0, -earWidth/2,0,0, earWidth/2,0,0);
    drawTriangle(-earWidth/2,0,-thickness, 0,earHeight,-thickness, earWidth/2,0,-thickness);
    drawTriangle(0,earHeight,-thickness, -earWidth/2,0,-thickness, -earWidth/2,0,0);
    drawTriangle(0,earHeight,0, 0,earHeight,-thickness, -earWidth/2,0,0);
    drawTriangle(0,earHeight,0, 0,earHeight,-thickness, earWidth/2,0,0);
    drawTriangle(earWidth/2,0,0, 0,earHeight,-thickness, earWidth/2,0,-thickness);

    glPushMatrix();
      glTranslated(-baseRadius, 0, -baseLength);
      drawBox(baseWidth, thick, baseLength);
    glPopMatrix();

    glPushMatrix();
      glTranslated(0,0,-baseLength);
      glRotated(-90, 1,0,0);
      drawCylinder(thick, baseRadius, baseRadius);
      // bolt
      SET_COLOR(COLOR_BOLT);
      glTranslated(0,0,thick);
      drawCylinder(thick/2, boltRadius, boltRadius);
    glPopMatrix();

    SET_COLOR(COLOR_DARK);
    drawTriangle(0,inHeight,margin, -inWidth/2,0,margin, inWidth/2,0,margin);
    
  glPopMatrix();
}

void RkAlphaModel::Muzzle()
{
  // VARABLES
  double muzzleLength = 4;
  double muzzleWidth  = 3.3;  // 0.5*2 + 2.3
  double muzzleHeight = 0.9;

  // RENDER
  glPushMatrix();
    //drawPoint();

    SET_COLOR(COLOR_BASE);
    // Muzzle Base
    glPushMatrix();
      glTranslated(-muzzleWidth/2, -muzzleHeight, -HEAD_RAD);
      drawBox(muzzleWidth, muzzleHeight, muzzleLength);
    glPopMatrix();

    // snout
    glPushMatrix();
      glTranslated(0, -0.6, muzzleLength -1.05 -0.9);
      Snout(muzzleWidth);
    glPopMatrix();

    // teeth
    glPushMatrix();
      glTranslated(0, -muzzleHeight, muzzleLength-HEAD_RAD-0.25);
      // left teeth
      glPushMatrix();
        glTranslated(-muzzleWidth/2+0.25, 0, 0);
        TopTeeth();
      glPopMatrix();
      // right teeth
      glPushMatrix();
        glTranslated( muzzleWidth/2-0.25, 0, 0);
        TopTeeth();
      glPopMatrix();
    glPopMatrix();

    // back-tuft
    glPushMatrix();
      glRotated(90, 0,1,0);
      glTranslated(HEAD_RAD, 0, 0);
      // left
      glPushMatrix();
        glTranslated(0, 0, -muzzleWidth/2+0.1);
        BackTuft();
      glPopMatrix();
      // right
      glPushMatrix();
        glTranslated(0, 0,  muzzleWidth/2-0.1);
        BackTuft();
      glPopMatrix();
    glPopMatrix();

    // right reinforce
    glPushMatrix();
      glTranslated(muzzleWidth/2, -muzzleHeight+0.1, -1.05);
      glRotated(90, 0,1,0);
      JawSupport();
    glPopMatrix();
    // left reinforce
    glPushMatrix();
      glTranslated(-muzzleWidth/2, -muzzleHeight+0.1, -1.05);
      glRotated(-90, 0,1,0);
      JawSupport();
    glPopMatrix();

    // Bottom Jaw
    glPushMatrix();
      glTranslated(0, -muzzleHeight-0.3, -HEAD_RAD+0.3);
      BottomJaw();
    glPopMatrix();

  glPopMatrix();
}

void RkAlphaModel::Snout(double muzzleWidth)
{
  double snoutLength = muzzleWidth + 0.2;
  double snoutWidth = 0.7;
  double snoutHeight = snoutWidth;

  double nostrilLen = 0.5;
  double nostrilWid = 0.2;
  double nostrilCtr = snoutWidth - nostrilLen + 0.01;

  double shiftDelta = (snoutLength - nostrilWid) / 2;
  double shiftRatio = pow(VAL(SNOUT_DELTA), 1.6);

  glPushMatrix();
    drawPoint();
    // base snout
    SET_COLOR(COLOR_DARK);
    glTranslated(-snoutLength/2, 0, 0);
    drawBox(snoutLength, snoutHeight, snoutWidth);
  glPopMatrix();

  // nostrils
  glPushMatrix();
    SET_COLOR(COLOR_GS_00);
    glTranslated(-nostrilWid/2, nostrilCtr, nostrilCtr);

    // left nostril
    glPushMatrix();
      glTranslated(shiftDelta * shiftRatio, 0, 0);
      drawBox(nostrilWid, nostrilLen, nostrilLen);
    glPopMatrix();
    // left nostril
    glPushMatrix();
      glTranslated(-shiftDelta * shiftRatio, 0, 0);
      drawBox(nostrilWid, nostrilLen, nostrilLen);
    glPopMatrix();
  glPopMatrix();
}

void RkAlphaModel::BackTuft()
{
  double x[] = { 0.7,  0.9,  1.2};
  double y[] = { 0.0, -0.3, -0.4, -0.6, -0.8, -1.2};
  double t = 0.1;

  glPushMatrix();
    //drawPoint();
    // edge
    SET_COLOR(COLOR_BASE);
    drawEdges(x[0],y[0],y[2],t);
    drawEdges(x[1],y[1],y[4],t);
    drawEdges(x[2],y[3],y[5],t);
  glPopMatrix();
}

void RkAlphaModel::TopTeeth()
{
  // VARIABLES
  double bigDiam   = 0.2 ;
  double smallDiam = 0.15; 
  double bigHigh   = 0.4 ;
  double smallHigh = 0.3 ;

  double shift = (1.0-VAL(TOP_TEETH))*bigHigh;

  // RENDER
  glPushMatrix();
    //drawPoint();

    // first teeth
    SET_COLOR(COLOR_WHITE);
    glPushMatrix();
      glTranslated(0,shift,0);
      glRotated(180, 0,0,1);
      glRotated(-90, 1,0,0);

      drawCylinder(bigHigh, bigDiam, 0);
      // second teeth
      glPushMatrix();
        glTranslated(0, bigDiam+smallDiam+0.1, 0);
        drawCylinder(smallHigh, smallDiam, 0);
        // third teeth
        glPushMatrix();
          glTranslated(0, smallDiam*2+0.1, 0);
          drawCylinder(smallHigh, smallDiam, 0);
        glPopMatrix();
      glPopMatrix();
    glPopMatrix();
  glPopMatrix();
}

void RkAlphaModel::BottomJaw()
{
  // VARIABLES
  double jawLength = 3.5;
  double jawWidth  = 2.3;  // 3.3 - 0.5*2
  double rotRadius = JAW_HEIGHT/2;

  // RENDER
  glPushMatrix();
    glRotated(VAL(JAW_OPEN), 1,0,0);
    //drawPoint();

    SET_COLOR(COLOR_BASE);
    glPushMatrix();
      glTranslated(-jawWidth/2, -JAW_HEIGHT/2, 0);
      drawBox(jawWidth, JAW_HEIGHT, jawLength);
    glPopMatrix();

    glPushMatrix();
      glTranslated(-jawWidth/2, 0, 0);
      glRotated(90, 0,1,0);
      drawCylinder(jawWidth, rotRadius, rotRadius);
    glPopMatrix();

    // teeth
    SET_COLOR(COLOR_WHITE);
    glPushMatrix();
      glTranslated(0, rotRadius, jawLength-0.65);
      // left teeth
      glPushMatrix();
        glTranslated(-jawWidth/2+0.2, 0, 0);
        BottomTeeth();
      glPopMatrix();
      // left teeth
      glPushMatrix();
        glTranslated( jawWidth/2-0.2, 0, 0);
        BottomTeeth();
      glPopMatrix();
    glPopMatrix();

  glPopMatrix();
}

void RkAlphaModel::JawSupport()
{
  double reinSize = JAW_HEIGHT;
  double reinRadius = reinSize/2;
  double boltRadius = reinRadius - 0.1;
  double reinThick = 0.3;
  double reinBase = reinRadius + 0.1;
  double boltThick = 0.1;
  double boltDepth = 0.3 + reinThick;

  glPushMatrix();
    //drawPoint();
    glTranslated(0, -reinBase, -reinThick);

    SET_COLOR(COLOR_BASE);
    drawCylinder(reinThick, reinRadius, reinRadius);
    glPushMatrix();
      glTranslated(-reinRadius, 0, 0);
      drawBox(reinSize, reinBase, reinThick);
    glPopMatrix();

    SET_COLOR(COLOR_BOLT);
    glPushMatrix();
      glTranslated(0, 0, boltThick+reinThick-boltDepth);
      drawCylinder(boltDepth, boltRadius, boltRadius);
    glPopMatrix();

  glPopMatrix();
}

void RkAlphaModel::BottomTeeth()
{
  // VARIABLES
  double radius = 0.15; 
  double height = 0.3 ;
  double delta  = radius*2 + 0.1;

  double shift = -(1.0-VAL(BOT_TEETH))*height;

  // RENDER
  glPushMatrix();
    //drawPoint();

    // first teeth
    SET_COLOR(COLOR_WHITE);
    glPushMatrix();
      glTranslated(0,shift,0);
      glRotated(-90, 1,0,0);

      drawCylinder(height, radius, 0);
      // second teeth
      glPushMatrix();
        glTranslated(0, delta, 0);
        drawCylinder(height, radius, 0);
        // third teeth
        glPushMatrix();
          glTranslated(0, delta, 0);
          drawCylinder(height, radius, 0);
        glPopMatrix();

      glPopMatrix();

    glPopMatrix();

  glPopMatrix();
}


/* HELPERS */
void RkAlphaModel::drawOrigin()
{
  float size = 0.4f;
  float length = 6.0f;
  float thick = 0.06f;

  if (VAL(ORIGIN) == 1)
  {
    SET_COLOR(COLOR_BASE);
    //drawSphere(size);

    // X-AXIS (RED)
    glPushMatrix();
      SET_COLOR(COLOR_RED);
      glTranslated(-length/2, -thick/2, -thick/2);
      drawBox(length, thick, thick);
    glPopMatrix();

    // Y-AXIS (GREEN)
    glPushMatrix();
      SET_COLOR(COLOR_GREEN);
      glTranslated(-thick/2, -length/2, -thick/2);
      drawBox(thick, length, thick);
    glPopMatrix();

    // Z-AXIS (BLUE)
    glPushMatrix();
      SET_COLOR(COLOR_BLUE);
      glTranslated(-thick/2, -thick/2, -length/2);
      drawBox(thick, thick, length);
    glPopMatrix();
  }
}

void RkAlphaModel::drawPoint()
{
  double rad = 0.125;
  double off = -rad * 0.7;
  double len = -off * 2;

  SET_COLOR(COLOR_CYAN);
  drawSphere(rad);
  glPushMatrix();
    glTranslated(off, off, off);
    drawBox(len, len, len);
  glPopMatrix();
}

void RkAlphaModel::drawEdges(double x, double y1, double y2, double t)
{
  // front
  drawTriangle(0,y2, t, x,y2, t, 0,y1, t);
  // back
  drawTriangle(x,y2,-t, 0,y2,-t, 0,y1,-t);
  // edge
  drawTriangle(x,y2, t, x,y2,-t, 0,y1,-t);
  drawTriangle(0,y1, t, x,y2, t, 0,y1,-t);
  drawTriangle(x,y2, t, 0,y2, t, x,y2,-t);
  drawTriangle(0,y2, t, 0,y2,-t, x,y2,-t);
}

// main runtime (disable it when creating other)
int main()
{
  // init controller
  ModelerControl controls[NUM_CONTROLS];

  // ALL CONTROLS
  controls[HEADER] = ModelerControl("*** Rk.Alpha-Head Model ***", 0,1,1,0);
  // INITIAL TRANSFORM
  controls[INIT_HEAD] = ModelerControl("** Initial Transformation **", 0,1,1,0);
    // UNIVERSAL SCALAR
    controls[SCALE] = ModelerControl("  Uniform Scalar", 0, 2, 0.05f, 1);
    // ROTATION
    controls[ROT_HEAD] = ModelerControl("  * Rotation Controls *", 0,1,1,0);
      controls[Z_ROT] = ModelerControl("    Roll (Z-Rotation)", -180, 180, 1, 0);
      controls[X_ROT] = ModelerControl("    Pitch (X-Rotation)", -180, 180, 1, 0);
      controls[Y_ROT] = ModelerControl("    Yaw (Y-Rotation)", -180, 180, 1, 0);
    // TRANSLATION
    controls[POS_HEAD] = ModelerControl("  * Position Controls *", 0,1,1,0);
      controls[X_POS] = ModelerControl("    X-Position", -5, 5, 0.01f, 0);
      controls[Y_POS] = ModelerControl("    Y-Position", -5, 5, 0.01f, 0);
      controls[Z_POS] = ModelerControl("    Z-Position", -5, 5, 0.01f, 0);

  // INITIAL TRANSFORM
  controls[INTER_HEAD] = ModelerControl("** Internal Transformation **", 0, 1, 1, 0);
    // HEAD
    controls[HEAD] = ModelerControl("  * Head *", 0, 1, 1, 0);
      controls[ LEFT_EAR_SHIFT] = ModelerControl("    Left Ear Shift",   -1,  1, 0.01f, 0);
      controls[RIGHT_EAR_SHIFT] = ModelerControl("    Right Ear Shift",  -1,  1, 0.01f, 0);
      controls[ LEFT_EYE_SHIFT] = ModelerControl("    Left Eye Shift",   -1,  1, 0.01f, 0);
      controls[RIGHT_EYE_SHIFT] = ModelerControl("    Right Eye Shift",  -1,  1, 0.01f, 0);
      controls[ LEFT_BROW_TILT] = ModelerControl("    Left Brow Tilt",  -12, 12, 0.01f, 0);
      controls[RIGHT_BROW_TILT] = ModelerControl("    Right Brow Tilt", -12, 12, 0.01f, 0);
    // MUZZLE
    controls[MUZZLE] = ModelerControl("  * Muzzle *", 0, 1, 1, 0);
      controls[SNOUT_DELTA] = ModelerControl("    Snout Delta", 0.35, 0.65, 0.01f, 0.5);
      controls[JAW_OPEN] = ModelerControl("    Jaw Open", 0, 15, 0.01f, 0);
      controls[TOP_TEETH] = ModelerControl("    Top Teeth", 0, 1, 0.01f, 0);
      controls[BOT_TEETH] = ModelerControl("    Bottom Teeth", 0, 1, 0.01f, 0);

  // DEBUGGER
  controls[DEBUGGER] = ModelerControl("!!!! Debugger !!!!", 0,1,1,0);
    controls[ORIGIN] = ModelerControl("  !! Origin Visible !!", 0,1,1,0);

  ModelerApplication::Instance()->Init(&createRkAlphaModel, controls, NUM_CONTROLS);
  return ModelerApplication::Instance()->Run();
}
