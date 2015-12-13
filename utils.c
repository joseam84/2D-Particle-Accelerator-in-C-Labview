#include <userint.h>
#include <ansi_c.h>
//==============================================================================
//
// Title:       utils.c
// Purpose:     A short description of the implementation.
//
// Created on:  10/29/2009 at 9:49:50 PM by Nobody.
// Copyright:   Non. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files

#include "utils.h"
#include "gen_var.h"
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


int numOfElements = 0;
int numOfAllocSpace = 0;

int addDataToDynamicArray(trailDot aDot,unsigned int id)
{
#if 0    /* formerly excluded lines */
	void *temp; 
	if(numOfElements == numOfAllocSpace)// Need more space
	{
		if(numOfElements == 0)
		{
			numOfAllocSpace = 2;	
		}else
		{
			numOfAllocSpace *=2;
		}
		temp = realloc(trailsArray[id], (numOfAllocSpace * sizeof(trailDot)));
		
		if(temp == NULL)
			MessagePopup("Error","allocation error, Insufficient Memory");
		
		trailsArray[id] = (trailDot*) temp;
	
	}
	
	trailsArray[id][numOfElements] = aDot;
	numOfElements++;
#endif   /* formerly excluded lines */
	
	return numOfElements;
}
