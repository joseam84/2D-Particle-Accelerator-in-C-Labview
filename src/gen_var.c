//==============================================================================
//
// Title:       gen_var.c
// Purpose:     A short description of the implementation.
//
// Created on:  10/22/2009 at 7:42:33 PM by Nobody.
// Copyright:   Non. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files

#include "gen_var.h"
#include <userint.h>


//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// Static global variables

//==============================================================================
// Static functions

//==============================================================================
// Global variables

//==============================================================================
// Global functions


//Panel handlers
int panelHandle = 0;
int menuPanelHandle = 0;
int pInfoPanelHandle = 0;
int pParticleEditPanel = 0;
int colorPanelHandle = 0;
//Variables
unsigned int panelEnable;
int centerOffsetX = 200;
int centerOffsetY = 400;
int canvasWidth;
int canvasHeight;
float scaleFactor = 10.0;
int particleNumber = 0;
int pFollowId;
int pointerIsAParticle;
trailDot thisPoint;// trail to be recorded 
//Thread handlers
HANDLE timeThreadHandler;
HANDLE trailSemHandler;
HANDLE saveSemHandler;
unsigned int tIdTime;
HANDLE displayThreadHandler;
unsigned int tIdDisplay;

genericParticle testParticle;
genericParticle particles[MAX_PART_NUM];


