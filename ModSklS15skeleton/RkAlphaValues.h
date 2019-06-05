// Values for Rk.Alpha model

#ifndef _RK_ALPHA_VALUES_H
#define _RK_ALPHA_VALUES_H

// List of controls of RkAlpha Controller
enum RkAlphaModelConbrotrols
{
  /* MAIN HEADER */
  HEADER,
  /* INITIAL TRANSFORM */
  INIT_HEAD,
    // 1. Uniform Scale
    SCALE,
    // 2. Rotation (Roll, Pitch, Yaw)
    ROT_HEAD, Z_ROT, X_ROT, Y_ROT,
    // 3. XYZ Origin
    POS_HEAD, X_POS, Y_POS, Z_POS,
  /* INTERNAL TRANSFORM */
  INTER_HEAD,
    // Head
    HEAD, LEFT_EAR_SHIFT, RIGHT_EAR_SHIFT,
          LEFT_EYE_SHIFT, RIGHT_EYE_SHIFT,
          LEFT_BROW_TILT, RIGHT_BROW_TILT,
    // Muzzle
    MUZZLE, SNOUT_DELTA, JAW_OPEN,
            TOP_TEETH, BOT_TEETH,
  /* DEBUG */
  DEBUGGER, ORIGIN,

  /* COUNTER */
  NUM_CONTROLS
};

/* COLORS */
// BASE RGB
#define COLOR_RED     1.0f, 0.0f, 0.0f
#define COLOR_GREEN   0.0f, 1.0f, 0.0f
#define COLOR_BLUE    0.0f, 0.0f, 1.0f
// BASE CMY
#define COLOR_YELLOW  1.0f, 1.0f, 0.0f
#define COLOR_CYAN    0.0f, 1.0f, 1.0f
#define COLOR_MAGENTA 1.0f, 0.0f, 1.0f
// GRAYSCALE
#define COLOR_GS_00   0.0f, 0.0f, 0.0f
#define COLOR_GS_80   0.5f, 0.5f, 0.5f
#define COLOR_GS_FF   1.0f, 1.0f, 1.0f

// COLOR PALLET
#define COLOR_BASE    COLOR_GS_80
#define COLOR_BOLT    0.8f, 0.8f, 0.8f
#define COLOR_DARK    0.1f, 0.1f, 0.1f

#define COLOR_WHITE   COLOR_GS_FF
#define COLOR_IRIS    112.0f/255, 173.0f/255, 71.0f/255
#define COLOR_PUPIL   84.0f/255, 130.0f/255, 53.0f/255

/* MAIN CONSTANTS */
#define HEAD_DIAM 2.7
#define HEAD_RAD HEAD_DIAM/2
#define JAW_HEIGHT 0.6

// Macro for accessing control values
#define VAL(x) (ModelerApplication::Instance()->GetControlValue(x))
#define SET(x,v) (ModelerApplication::Instance()->SetControlValue(x,v))
#define SET_COLOR(x) setAmbientColor(.1f, .1f, .1f); setDiffuseColor(x)
#define EULER_ROT(x,y,z) glRotated(z,0,0,1); glRotated(x,1,0,0); glRotated(y,0,1,0)
#define UNI_SCALE(x) glScaled(x,x,x)

#endif
