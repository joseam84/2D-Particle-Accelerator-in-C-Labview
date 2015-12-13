#include <cvirte.h>		
#include <userint.h>
#include "PartSim.h"
#include <gen_var.h>
#include <ansi_c.h>
#include <utils.h>
#include <gl.h>
#include <glu.h>

int main (int argc, char *argv[])
{
	int status;
	int i;//INIT 
	
	

	
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "PartSim.uir", PANEL)) < 0)
		return -1;
	if ((menuPanelHandle = LoadPanel (0, "PartSim.uir", MENU_PANEL)) < 0)
		return -1;
		
	SetCtrlVal(panelHandle,PANEL_SCALE,scaleFactor);
	GetCtrlAttribute(panelHandle,PANEL_CANVAS,ATTR_WIDTH,&canvasWidth);
	GetCtrlAttribute(panelHandle,PANEL_CANVAS,ATTR_HEIGHT,&canvasHeight);
	centerOffsetX = canvasWidth/2; //Half the canvas width
	centerOffsetY = canvasHeight/2; //Half the canvas width 	
	// Init threads
	timeThreadHandler=CreateThread(NULL,
                  0,(LPTHREAD_START_ROUTINE)timerThread,
				  (LPVOID)1,
				  0,
                  &tIdTime);
	
	displayThreadHandler=CreateThread(NULL,
                  0,(LPTHREAD_START_ROUTINE)displayRefresh,
				  (LPVOID)1,
				  0,
                  &tIdDisplay);
	
	trailSemHandler = CreateSemaphore(NULL,           // default security attributes
   									 1,  // initial count
   									 1,  // maximum count
   									 NULL);          // unnamed semaphore
	saveSemHandler = CreateSemaphore(NULL,           // default security attributes
   									 1,  // initial count
   									 1,  // maximum count
   									 NULL);          // unnamed semaphore
	//INIT end
	//TEST

	
	//END TEST
	DisplayPanel (panelHandle);
	RunUserInterface ();
	DiscardPanel (panelHandle);
	return 0;
}


int CVICALLBACK QuitCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			QuitUserInterface (0);
			TerminateThread(timeThreadHandler,0);
			TerminateThread(displayThreadHandler,0); 
			break;
	}
	return 0;
}
