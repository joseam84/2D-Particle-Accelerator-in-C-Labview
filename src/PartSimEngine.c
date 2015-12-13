#include <ansi_c.h>
#include <userint.h>
#include <gen_var.h>
#include <PartSim.h>
#include <utils.h>
int debugAccelParticle(genericParticle* particle)
{
	
	particle->pVelocityX += particle->pAccelX;
	particle->pVelocityY += particle->pAccelY;
	
	return 0;
}

int debugMoveParticle(genericParticle* particle)
{
	
	particle->relPosX += particle->pVelocityX;
	particle->relPosY += particle->pVelocityY;
	//if(particle->pId ==2)
		//printf("%d \n",particle->pId);
	return 0;
}

int applyForce(genericParticle* particle,vectorForce force)
{
	if(particle->pMass != 0 && particle->followMouse == FALSE)
	{
		particle->pAccelX = force.xComp/particle->pMass;
		particle->pAccelY = force.yComp/particle->pMass;
		debugAccelParticle(particle); 
	    debugMoveParticle(particle); 
	}
	

	return 0;
}

int applyElecForce(genericParticle* particle,vectorForce force)
{
	if(particle->pCharge != 0)
	{
		particle->pAccelX = force.xComp/particle->pCharge;
		particle->pAccelY = force.yComp/particle->pCharge;
	
	}
	return 0;
}

int applySFieldForce(genericParticle* particle,vectorForce force)
{
	if(particle->pSField != 0)
	{
		particle->pAccelX = force.xComp/particle->pSField;
		particle->pAccelY = force.yComp/particle->pSField;
	
	}
	return 0;
}
// Apply force on every particle.

int applyForcesOnParticles()
{
	int i,j;
	double distance,force = 0;
	vectorForce forceV;
	genericParticle thisParticle;
	memset(&forceV,0,sizeof(vectorForce));
	for(j=0; j< particleNumber; j++)
	{
		thisParticle = particles[j];// particle evaluated
		
		for(i=0; i< particleNumber; i++)
		{
			if((i == j)|| (particles[i].pFixed == TRUE))
			continue;// if evaluating the same particle
		
			//Calculate the Gravitational force betwen particles
		    distance =  sqrt(SQUARE(particles[i].relPosY - thisParticle.relPosY)+
				        SQUARE(particles[i].relPosX - thisParticle.relPosX));
			 
			//Spring force  (1/r^2)
			if(particles[i].pSpForce != 0.0){
			force += -SPFORCE(FAKE_G_CONST,
						thisParticle.pSpForce,
						particles[i].pSpForce,
					    (distance));
			}
			//Gravity force  (1/r^2)
			if(particles[i].pMass != 0.0 &&
				particles[i].ignoreGrav == FALSE){
			force += -GFORCE(FAKE_G_CONST,
						thisParticle.pMass,
						particles[i].pMass,
					    (distance));
			}
			// Electrical force  (1/r^2)
			if(particles[i].pCharge != 0.0){
			force += EVOLT(FAKE_E_CONST,
						thisParticle.pCharge,
						particles[i].pCharge,
					    (distance));
			}
			//Strange Force   (1/r^4)
			if(particles[i].pSField != 0.0){
			force += SFIELD(STRANGE_CONST,
						thisParticle.pSField,
						particles[i].pSField,
					    (distance));
			}
			forceV.yComp = (force/distance)*(particles[i].relPosY - thisParticle.relPosY);
			forceV.xComp = (force/distance)*(particles[i].relPosX - thisParticle.relPosX); 
			
			//printf(" %f,%d, %d, %d \n",force,i,j,particles[i].pId);
			applyForce(&particles[i], forceV);
			force = 0.0; //clear the old force once applied to particles
			
		}
	
	}
	return 0;
}
//Add a particle to the space
int addParticle(genericParticle particle)
{
	char text[32];
	if(particleNumber < MAX_PART_NUM)
	{
		particle.pId = particleNumber;
		particles[particleNumber] = particle;
		particleNumber++;
		sprintf(text,"Particle Number: %d",particleNumber);
		SetCtrlAttribute (panelHandle, PANEL_P_NUM, ATTR_CTRL_VAL, text);
	}
	else
	{
		MessagePopup("Warning","Exceeded number of allowed particles");
	}
	return particleNumber;
}
// Refresh the screen
unsigned long displayRefresh(int param) 
{
	Point partPositionPt,oldPosition;
	static int dispWritten; // Allow XOR masking  
	int i,j;
	int status;
	int particleType;
	int color,penWidth;
	Point pnt,pnt2;
	char text[64];
	while(1)
	{
		WaitForSingleObject(trailSemHandler,INFINITE);
		CanvasStartBatchDraw (panelHandle, PANEL_CANVAS);
		
	//	status = CanvasClear  (panelHandle, PANEL_CANVAS, VAL_ENTIRE_OBJECT);  
		for(i=0; i< particleNumber; i++)
		{
			if(particles[i].pType!= UNDEF_P)// If the particle is not undefined
			{
			    particleType= particles[i].pType;
				switch(particleType)
				{
					case GENERIC_P:
						color = VAL_WHITE;
						penWidth = 4;
						break;
					case ELECTRON_P:
						color = VAL_YELLOW;
						penWidth = 3;
						break;
					default:
						break;
				}
				
				//Set Pen
				
				SetCtrlAttribute(panelHandle,PANEL_CANVAS,ATTR_PEN_MODE, VAL_XOR_MODE);        
				SetCtrlAttribute(panelHandle,PANEL_CANVAS,ATTR_PEN_STYLE,VAL_SOLID);           
				SetCtrlAttribute(panelHandle,PANEL_CANVAS,ATTR_PEN_COLOR,color);               
				SetCtrlAttribute(panelHandle,PANEL_CANVAS,ATTR_PEN_WIDTH,penWidth);
				//Erase old
				if(particles[i].firstDrawn)// Avoid being drawn at 0,0
				{
					oldPosition.x = ((particles[i].oldPosX / scaleFactor) + (centerOffsetX)) ;
			    	oldPosition.y = ((particles[i].oldPosY / scaleFactor) + (centerOffsetY)) ;
					status = CanvasDrawPoint (panelHandle, PANEL_CANVAS, oldPosition);
					
				}
				
				partPositionPt.x = ((particles[i].relPosX / scaleFactor) + (centerOffsetX)) ;
				partPositionPt.y = ((particles[i].relPosY / scaleFactor) + (centerOffsetY)) ;
				particles[i].firstDrawn = TRUE;
				//Draw new Point                                                                   
				status = CanvasDrawPoint (panelHandle, PANEL_CANVAS, partPositionPt);          
				
				
				
					
				if(particles[i].trailOn == 1)
				{
					thisPoint.x = particles[i].relPosX;
					thisPoint.y = particles[i].relPosY;
					
					oldPosition.x = ((particles[i].oldPosX / scaleFactor) + (centerOffsetX)) ;
			     	oldPosition.y = ((particles[i].oldPosY / scaleFactor) + (centerOffsetY)) ;
					
					//Store Trail points on particle tables, if size is exceed, it will 
					//double the size of the tables.
					
					//We add a particle trail to the array
				
					particles[i].trailPoint[particles[i].trailLength]= thisPoint;
					
					particles[i].trailLength++; 
					
					// Resize the trail data structure if it reach the limit
					if(particles[i].trailLength >= particles[i].trailMaxSize-1)
					{  
						particles[i].trailMaxSize *= 2;
						particles[i].trailPoint = realloc(particles[i].trailPoint,
						particles[i].trailMaxSize * sizeof(trailDot));
					}
					if(particles[i].trailLength >=2)//guarantee at least two points
					{
						//Set the pen for drawing
						status =  CanvasDefaultPen (panelHandle,PANEL_CANVAS);
						SetCtrlAttribute(panelHandle,
							PANEL_CANVAS,
							ATTR_PEN_MODE,
							VAL_COPY_MODE);
						SetCtrlAttribute(panelHandle,
							PANEL_CANVAS,
							ATTR_PEN_STYLE,
							particles[i].tType);
						SetCtrlAttribute(panelHandle,
							PANEL_CANVAS,
							ATTR_PEN_COLOR,
							(~particles[i].tColor)&0xFFFFFF);
						SetCtrlAttribute(panelHandle,PANEL_CANVAS,ATTR_PEN_WIDTH,1);
						//Set the dots for drawing
						pnt.x= partPositionPt.x;
						pnt.y= partPositionPt.y;
						pnt2.x= oldPosition.x;
						pnt2.y= oldPosition.y;
						//DEBUG
#if 0    /* formerly excluded lines */
						sprintf(text,"Trail Number %d Max Size %d",
							particles[i].trailLength,particles[i].trailMaxSize);
					    SetCtrlAttribute(panelHandle,PANEL_DEBUG_MSG,ATTR_CTRL_VAL,text);
#endif   /* formerly excluded lines */
						if(particles[i].tType == SOLID)
						CanvasDrawLine(panelHandle,PANEL_CANVAS,pnt,pnt2);
					    //SetCtrlAttribute(panelHandle,PANEL_CANVAS,ATTR_PEN_MODE, VAL_AND_MODE); 
					    if(particles[i].tType == DOTS)
						status = CanvasDrawPoint (panelHandle, PANEL_CANVAS, pnt);
						
						 
					}
				}
				particles[i].oldPosX = particles[i].relPosX;
				particles[i].oldPosY = particles[i].relPosY;
			}
		}
	 	CanvasEndBatchDraw (panelHandle, PANEL_CANVAS);
		status = CanvasUpdate (panelHandle, PANEL_CANVAS, VAL_ENTIRE_OBJECT);
		ReleaseSemaphore(trailSemHandler,1,NULL) ; 
		Sleep(16);
	}
	return 0;
}

//Update the state of the particles
unsigned long timerThread(int param) 
{
	while(1)
	{
		//WaitForSingleObject(trailSemHandler,INFINITE); 
		WaitForSingleObject(saveSemHandler,INFINITE);
		WaitForSingleObject(trailSemHandler,INFINITE); 
		applyForcesOnParticles();
		ReleaseSemaphore(trailSemHandler,1,NULL) ;  
		Sleep(5);
		ReleaseSemaphore(saveSemHandler,1,NULL) ; 
	}
}

int CVICALLBACK drawTrailOnce (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int status,j, i = 0;
    static int canvasElements = 0;
	Point pnt,pnt2;
	switch (event)
	{
		case EVENT_COMMIT:
			WaitForSingleObject(trailSemHandler,INFINITE);
			status = CanvasStartBatchDraw (panelHandle, PANEL_CANVAS);   
			status = CanvasClear  (panelHandle, PANEL_CANVAS, VAL_ENTIRE_OBJECT);  
			status =  CanvasDefaultPen (panelHandle,PANEL_CANVAS);
			//Set Pen
			SetCtrlAttribute(panelHandle,PANEL_CANVAS,ATTR_PEN_MODE, VAL_COPY_MODE);
			
			SetCtrlAttribute(panelHandle,PANEL_CANVAS,ATTR_PEN_WIDTH,1);
		    canvasElements = 0;
			
			for(j=0;j < particleNumber;j++)
			{
				if(particles[j].trailOn == FALSE)
					continue;
				
				SetCtrlAttribute(panelHandle,PANEL_CANVAS,ATTR_PEN_COLOR,particles[j].tColor);
				SetCtrlAttribute(panelHandle,PANEL_CANVAS,ATTR_PEN_STYLE,particles[j].tType); 
				for(i=0;i < particles[j].trailLength - 1;i++)
				{
					if(particles[j].trailLength <= 1 || i >= particles[j].trailLength)
						continue;
					
					pnt.x=((particles[j].trailPoint[i].x / scaleFactor) + (centerOffsetX));	
					pnt.y=((particles[j].trailPoint[i].y / scaleFactor) + (centerOffsetY));
					pnt2.x=((particles[j].trailPoint[i+1].x / scaleFactor) + (centerOffsetX));	
					pnt2.y=((particles[j].trailPoint[i+1].y / scaleFactor) + (centerOffsetY));
				    
					if(particles[j].tType == SOLID)
					status = CanvasDrawLine(panelHandle,PANEL_CANVAS,pnt,pnt2);
					if(particles[j].tType == DOTS)
					status = CanvasDrawPoint (panelHandle, PANEL_CANVAS, pnt);
					canvasElements++;
		    	}
			}
			CanvasEndBatchDraw (panelHandle, PANEL_CANVAS);
	    	status = CanvasUpdate (panelHandle, PANEL_CANVAS, VAL_ENTIRE_OBJECT);
			ReleaseSemaphore(trailSemHandler,1,NULL) ;      	   
													   
			break;
	}
	return 0;
}
 
