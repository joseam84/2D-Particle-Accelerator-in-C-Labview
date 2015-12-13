/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2009. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  COLOR                            1
#define  COLOR_COLOR_PICKER               2
#define  COLOR_SELECT_COLOR               3       /* callback function: SelectTrailColor */
#define  COLOR_CLOSE_COLOR_P              4       /* callback function: CloseColorPanel */

#define  EDIT_P                           2
#define  EDIT_P_STRANGE_FIELD             2
#define  EDIT_P_VELOCITY_Y                3
#define  EDIT_P_PARTICLE_CHARGE           4
#define  EDIT_P_VELOCITY_X                5
#define  EDIT_P_PARTICLE_MASS             6
#define  EDIT_P_INDEX                     7       /* callback function: updateParticleInfo */
#define  EDIT_P_TRAILS_ON                 8
#define  EDIT_P_OK_P_EDIT                 9       /* callback function: OkParticleEdit */
#define  EDIT_P_CANCEL_P_EDIT             10      /* callback function: CancelParticleEdit */
#define  EDIT_P_FOLLOW_MOUSE              11
#define  EDIT_P_FIXED_P                   12
#define  EDIT_P_VELY_EN                   13      /* callback function: updateParticleInfo */
#define  EDIT_P_VELX_EN                   14      /* callback function: updateParticleInfo */

#define  MENU_PANEL                       3

#define  P_INFO                           4
#define  P_INFO_PARTICLE_DATA             2
#define  P_INFO_P_ID                      3       /* callback function: UpdateIdInfo */
#define  P_INFO_CLOSE_INFO_PANEL          4       /* callback function: CloseInfoPanel */

#define  PANEL                            5
#define  PANEL_CANVAS                     2       /* callback function: CursorOnCanvas */
#define  PANEL_QUITBUTTON                 3       /* callback function: QuitCallback */
#define  PANEL_STRANGE_FIELD              4
#define  PANEL_SP_FORCE                   5
#define  PANEL_PARTICLE_CHARGE            6
#define  PANEL_PARTICLE_MASS              7
#define  PANEL_SCALE                      8       /* callback function: ChangeScale */
#define  PANEL_P_VELOCITY_Y               9
#define  PANEL_P_VELOCITY_X               10
#define  PANEL_CLEAR_SCREEN               11      /* callback function: ClearScreen */
#define  PANEL_FOLLOW_MOUSE               12
#define  PANEL_IGNORE_G                   13
#define  PANEL_FIXED_P                    14
#define  PANEL_PANELS_TIMER               15      /* callback function: RefreshPanels */
#define  PANEL_P_NUM                      16
#define  PANEL_FIX_BUTTON                 17      /* callback function: fixAlParticles */
#define  PANEL_UNFIX_BUTTON               18      /* callback function: unfixAlParticles */
#define  PANEL_DRAW_TRAIL                 19      /* callback function: drawTrailOnce */
#define  PANEL_TRAILS_ON                  20
#define  PANEL_TRAIL_COLOR                21
#define  PANEL_TRAIL_TYPE                 22
#define  PANEL_DEBUG_MSG                  23


     /* Menu Bars, Menus, and Menu Items: */

#define  MAIN_MENU                        1
#define  MAIN_MENU_FILE_MENU              2
#define  MAIN_MENU_FILE_MENU_LOAD_TEXT    3       /* callback function: loadText */
#define  MAIN_MENU_FILE_MENU_SAVE_TEXT    4       /* callback function: saveText */
#define  MAIN_MENU_FILE_MENU_LOAD_STATE   5       /* callback function: loadState */
#define  MAIN_MENU_FILE_MENU_SAVE_STATE   6       /* callback function: saveState */
#define  MAIN_MENU_VIEW                   7
#define  MAIN_MENU_VIEW_P_INFO            8       /* callback function: ViewParticleInfo */
#define  MAIN_MENU_EDIT_MENU              9
#define  MAIN_MENU_EDIT_MENU_PARTICLE_EDIT 10     /* callback function: editParticle */

#define  POPUP_MENU                       2
#define  POPUP_MENU_CREATE_PARTICLES      2
#define  POPUP_MENU_CREATE_PARTICLES_GENERIC_P 3  /* callback function: CreateParticle */
#define  POPUP_MENU_CREATE_PARTICLES_ELECTRON_P 4 /* callback function: CreateParticle */


     /* Callback Prototypes: */

int  CVICALLBACK CancelParticleEdit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ChangeScale(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ClearScreen(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CloseColorPanel(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CloseInfoPanel(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK CreateParticle(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK CursorOnCanvas(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK drawTrailOnce(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK editParticle(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK fixAlParticles(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK loadState(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK loadText(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK OkParticleEdit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK QuitCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK RefreshPanels(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK saveState(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK saveText(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK SelectTrailColor(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK unfixAlParticles(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK UpdateIdInfo(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK updateParticleInfo(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK ViewParticleInfo(int menubar, int menuItem, void *callbackData, int panel);


#ifdef __cplusplus
    }
#endif
