#include <ansi_c.h>
#include <userint.h>
#include <gen_var.h>
#include <PartSim.h>


Point point;
Point pointerParticle;
//Function: Set the pen color and draw the particle
void CVICALLBACK CreateParticle (int menuBar, int menuItem, void *callbackData,
		int panel)
{

	double mass,charge,sField,spForce;
	double velX,velY;
	int fixed,trails,color,type,ignGrav,follow;
	genericParticle particle;
	memset(&particle,0,sizeof(genericParticle));// init particle
	particle.trailMaxSize = 1024;
	particle.trailPoint = malloc(particle.trailMaxSize*sizeof(trailDot));
	particle.relPosX = (point.x - (centerOffsetX)) * scaleFactor ;
	particle.relPosY = (point.y - (centerOffsetY)) * scaleFactor ;
	GetCtrlVal(panelHandle,PANEL_PARTICLE_MASS,&mass);
	GetCtrlVal(panelHandle,PANEL_P_VELOCITY_X,&velX);
	GetCtrlVal(panelHandle,PANEL_P_VELOCITY_Y,&velY); 
	GetCtrlVal(panelHandle,PANEL_PARTICLE_CHARGE,&charge);
    GetCtrlVal(panelHandle,PANEL_STRANGE_FIELD ,&sField);	
	GetCtrlVal(panelHandle,PANEL_FIXED_P,&fixed);
	GetCtrlVal(panelHandle,PANEL_TRAILS_ON,&trails);
	GetCtrlVal(panelHandle,PANEL_TRAIL_COLOR,&color);    
	GetCtrlVal(panelHandle,PANEL_TRAIL_TYPE,&type);
	GetCtrlVal(panelHandle,PANEL_SP_FORCE,&spForce);
	GetCtrlVal(panelHandle,PANEL_IGNORE_G,&ignGrav);
	GetCtrlVal(panelHandle,PANEL_FOLLOW_MOUSE,&follow);
	switch(menuItem)
	{
		case POPUP_MENU_CREATE_PARTICLES_GENERIC_P:
			particle.pType = GENERIC_P;
			
			//Add other parameters
			particle.pFixed = fixed;
			particle.pType = GENERIC_P;
			particle.ignoreGrav =ignGrav;
			particle.pMass = mass;
			particle.pSpForce = spForce;
			particle.pCharge = charge;
			particle.pSField = sField;
			particle.pVelocityX = velX;
			particle.pVelocityY = velY;
			particle.trailOn = trails;
			particle.tColor = color;
			particle.tType = type;
			particle.followMouse =follow;
			
			if(particle.followMouse == TRUE)
			{
				pFollowId = particleNumber;
				pointerIsAParticle = TRUE;
			}
			
			addParticle(particle);
			break;
			
		case POPUP_MENU_CREATE_PARTICLES_ELECTRON_P:
			
			particle.pType = ELECTRON_P;
			//Add other parameters
			particle.pFixed = fixed; 
			particle.pMass = mass;
			particle.pCharge = charge;
			particle.pVelocityX = velX;
			particle.pVelocityY = velY;
			addParticle(particle);
			break;	
			
			
		default:
			break;
	}

}



 
// reads the position of the cursor  in the canvas
int CVICALLBACK CursorOnCanvas (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
  	if (event == EVENT_RIGHT_CLICK)
    {
	    int mouseX;
	    int mouseY;
	    int status;
	    int menuHandle;
	
	    menuHandle = GetPanelMenuBar (menuPanelHandle);
		GetRelativeMouseState (panel, PANEL_CANVAS, &mouseX, &mouseY, 0, 0, 0);
	
		point = MakePoint (mouseX, mouseY); // Global point containing coordinates 
		CanvasSetPenPosition (panel, PANEL_CANVAS, point);
		status = RunPopupMenu (menuHandle, POPUP_MENU_CREATE_PARTICLES, 
			panelHandle, (mouseY+20), mouseX+(180), 0,0, 0, 0);
    }
	if(pointerIsAParticle)
	{
	    int mouseX;
    	int mouseY;
		if(panelHandle)
		{
		GetRelativeMouseState (panel, PANEL_CANVAS, &mouseX, &mouseY, 0, 0, 0);
		pointerParticle = MakePoint(mouseX,mouseY);
		particles[pFollowId].relPosX = (pointerParticle.x - (centerOffsetX)) * scaleFactor ;
		particles[pFollowId].relPosY = (pointerParticle.y - (centerOffsetY)) * scaleFactor ;
		}
	}
		
	return 0;
}

int CVICALLBACK ClearScreen (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{   int status;
    char text[20];
	switch (event)
	{
		case EVENT_COMMIT:
			status = CanvasClear (panelHandle, PANEL_CANVAS, VAL_ENTIRE_OBJECT);
			particleNumber = 0;
			sprintf(text,"Particle Number: %d",particleNumber);
		    SetCtrlAttribute (panelHandle, PANEL_P_NUM, ATTR_CTRL_VAL, text);
			memset(particles,0,sizeof(particles));
			pointerIsAParticle = FALSE;
			break;
	}
	return 0;
}

void CVICALLBACK ViewParticleInfo (int menuBar, int menuItem, void *callbackData,
		int panel)
{
	if(pInfoPanelHandle <=0)
	{
		if ((pInfoPanelHandle  = LoadPanel (0, "PartSim.uir", P_INFO)) < 0)
			MessagePopup("Error loading panel", "Particle panel cannot be shown");
		else
		{
				DisplayPanel(pInfoPanelHandle);
				SetCtrlAttribute(panelHandle,PANEL_PANELS_TIMER,ATTR_ENABLED,1);
		}
	}
}

int CVICALLBACK RefreshPanels (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:

			if(pInfoPanelHandle > 0)
			{
				RefreshParticleInfo(); 	
			}
			
			break;
	}
	return 0;
}
//Refersh the information window
void RefreshParticleInfo(void)
{
	int index;
	char text[512];
	static int refresh = 0;
	
	GetCtrlVal(pInfoPanelHandle,P_INFO_P_ID,&index);
	sprintf(text,
		 "charge    : %f \n"
		 "mass      : %f \n"
		 "position X: %f \n"
		 "position Y: %f \n"
		 "velocity X: %f \n"
		 "velocity Y: %f \n"
		 "accel X   : %f \n"
		 "accel Y   : %f \n"
		 "isFixed   : %d \n",
		 particles[index].pCharge,
		 particles[index].pMass,
		 particles[index].relPosX,
		 particles[index].relPosY,
		 particles[index].pVelocityX,
		 particles[index].pVelocityY,
		 particles[index].pAccelX,
		 particles[index].pAccelY,
		 particles[index].pFixed);
	
		ResetTextBox(pInfoPanelHandle,P_INFO_PARTICLE_DATA,"");
		InsertTextBoxLine(pInfoPanelHandle,P_INFO_PARTICLE_DATA,0,text);

}
//Close the  program
int CVICALLBACK CloseInfoPanel (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			DiscardPanel(pInfoPanelHandle);
			pInfoPanelHandle = 0;//Disabled
			SetCtrlAttribute(panelHandle,PANEL_PANELS_TIMER,ATTR_ENABLED,0);
			break;
	}
	return 0;
}
//Update particle info every time the id is changed
int CVICALLBACK UpdateIdInfo (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			RefreshParticleInfo();
			break;
	}
	return 0;
}
//Save particle states
void CVICALLBACK saveState (int menuBar, int menuItem, void *callbackData,
		int panel)
{
	FILE *fd;                                /* file */
	size_t checkSize;
    char errorMsg[128];                      /* buffer for error message      */
    char filename[260];                      /* file to write                 */
    int status;
	unsigned int i,j;
	saveStateStruct save;
	
	WaitForSingleObject(trailSemHandler,INFINITE); 
	WaitForSingleObject(saveSemHandler,INFINITE);
	status = FileSelectPopup(".",            /* start at root directory       */
             "*.state",                      /* Display only files .state ext   */
             "*.state",                      /* Only one file type            */
             "Save State",                   /* Title of box             */
             VAL_SAVE_BUTTON,                /* Use Save as action button     */
             1,                              /* Can change directories        */
             1,                              /* Limit files to .dat ext       */
             1,                              /* User can cancel               */
             1,                              /* User can make new dir         */
             filename);                      /* Path name selected by user    */

   
	
	
	 memset(&save,0,sizeof(save));
	if(status)                               /* if file selected ...       */
    {
		//Fill the save file
		memcpy(save.allParticles, particles,sizeof(particles));
        for(j=0;j < particleNumber;j++)
		{
			 /* copy the pointer of particles array */   
			
			save.trailPoint[j] = malloc(sizeof(trailDot)*
				particles[j].trailMaxSize);//Save all the particle information except trails
			memset(save.trailPoint[j],0,
					sizeof(trailDot)*save.allParticles[j].trailMaxSize);//clear the allocated memory
			memcpy(save.trailPoint[j], particles[j].trailPoint,sizeof(trailDot)*
				particles[j].trailMaxSize);//Save every trail for every particle
			
		    save.pCount = particleNumber;
		
		}
		
		fd = fopen(filename, "wb+");         /* open file to write         */

        if(fd)                               /* if open successful ...     */
        {
            checkSize = fwrite((void*)&save,         /* write file to disk         */
                   sizeof(save),
                   1,
                   fd);
			if(checkSize != 1)
			{
				MessagePopup("Error","Save Error");
			}
			for(j=0;j < particleNumber;j++)
			{
				
				checkSize=fwrite((void*)save.trailPoint[j],     /* write file to disk         */
	                   sizeof(trailDot),
					   particles[j].trailMaxSize,
	                   fd);
				if(checkSize != particles[j].trailMaxSize)
				{
					MessagePopup("Error","Save Error");
				}
				
			
		    }
            fclose(fd);                      /* close file                    */
			
        }
        else                                 /* if open error ...             */
        {
            sprintf(errorMsg,                /* create error string           */
                    "Cannot save file %s.",
                    filename);

            MessagePopup("MCE Save Error",   /* display error message         */
                         errorMsg);
        }
    }
	ReleaseSemaphore(trailSemHandler,1,NULL);
	ReleaseSemaphore(saveSemHandler,1,NULL);
}
//Load file
void CVICALLBACK loadState (int menuBar, int menuItem, void *callbackData,
		int panel)
{
	FILE *fd;                                    /* file descripter            */
    char errorMsg[128];                      /* buffer for error message      */
    char filename[260];                      /* file to read                  */
    int status;                              /* function return status        */
	unsigned int i,j;
	saveStateStruct load;
	WaitForSingleObject(trailSemHandler,INFINITE);
    WaitForSingleObject(saveSemHandler,INFINITE);
	status = FileSelectPopup(".",            /* start at current directory    */
             "*.state",                      /* Display only files .rom ext   */
             "*.state",                      /* Only one file type            */
             "Load State",         /* Title of box                  */
             VAL_LOAD_BUTTON,                /* Use Load as action button     */
             1,                              /* Can change directories        */
             1,                              /* Limit files to .dat ext       */
             1,                              /* User can cancel               */
             1,                              /* User can make new dir         */
             filename);                      /* Path name selected by user    */

    //Load File after selecting file
    if(status)                               /* if file selected ...          */
    {
        fd = fopen(filename, "r");           /* open file to read             */

        if(fd)                               /* if open successful ...        */
        {
            fread((void*)&load,            /* read file from disk           */
                   sizeof(load),
                   1,
                   fd);

			for(j=0;j < load.pCount;j++)
			{
				load.trailPoint[j] = malloc(sizeof(trailDot)* //allocate memory first
				load.allParticles[j].trailMaxSize);
				
				fread((void*)load.trailPoint[j],     /* write file to disk         */
	                   sizeof(trailDot),
					   load.allParticles[j].trailMaxSize,
	                   fd);
		    }
            fclose(fd);	

                                  /* close file                    */
			
			
			for(j=0;j < load.pCount;j++)
			{
				// free trailPoint pointers from old particles table
				free(particles[j].trailPoint);
				
			}
			
			//Copy all data to new particles table
			memcpy(particles,load.allParticles,sizeof(particles));
		    particleNumber = load.pCount;
	
		
			
			
			for(j=0;j < load.pCount;j++)
			{
				//Fill all new pointers for particles trails
				particles[j].trailPoint = malloc(particles[j].trailMaxSize *
				sizeof(trailDot));
			
		    	memcpy(particles[j].trailPoint,load.trailPoint[j],
			    load.allParticles[j].trailMaxSize * sizeof(trailDot));
			}
		}
	}
    else                                 /* if open error ...             */
    {
        sprintf(errorMsg,                /* create error string           */
                "Cannot load file %s.",
                filename);

        MessagePopup("MCE Load Error",   /* display error message         */
                     errorMsg);
    }

	
	
	ReleaseSemaphore(trailSemHandler,1,NULL);
	ReleaseSemaphore(saveSemHandler,1,NULL);
}
	

//Unfix all particles
int CVICALLBACK unfixAlParticles (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int i;
	switch (event)
	{
		case EVENT_COMMIT:
			for(i = 0;i < particleNumber;i++)
			{
				particles[i].pFixed = FALSE;
			}
			break;
	}
	return 0;
}
//Fix all particles 
int CVICALLBACK fixAlParticles (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{   int i;
	switch (event)
	{
		case EVENT_COMMIT:
			for(i = 0;i < particleNumber;i++)
			{
				particles[i].pFixed = TRUE;
			}
			break;
			
	}
	return 0;
}
// Zoom in and out
int CVICALLBACK ChangeScale (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panelHandle,PANEL_SCALE,&scaleFactor);
			CanvasClear(panelHandle,PANEL_CANVAS,VAL_ENTIRE_OBJECT);
			break;
	}
	return 0;
}
//Edit particle menu
void CVICALLBACK editParticle (int menuBar, int menuItem, void *callbackData,
		int panel)
{
	if(pParticleEditPanel <=0)
		{
			if ((pParticleEditPanel  = LoadPanel (0, "PartSim.uir", EDIT_P)) < 0)
				MessagePopup("Error loading panel", "Particle panel cannot be shown");
			else
			{
					DisplayPanel(pParticleEditPanel);
					updParticleEditInfo();
			}
		}	
}

int CVICALLBACK OkParticleEdit (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	double mass,charge,sField;
	double velX,velY;
	int fixed,index,trailOn,velXEn,velYEn,follow;
	genericParticle particle;
	switch (event)
	{
		case EVENT_COMMIT:
			
			GetCtrlVal(panel,EDIT_P_INDEX,&index);
			GetCtrlVal(pParticleEditPanel,EDIT_P_VELX_EN,&velXEn);
			GetCtrlVal(pParticleEditPanel,EDIT_P_VELX_EN,&velYEn);	
			if(velXEn)
			{                                                                	
				GetCtrlVal(pParticleEditPanel,EDIT_P_VELOCITY_X,&velX); 
				particles[index].pVelocityX = velX; 
			}                                                                          	
			if(velYEn)
			{                                                                	
				GetCtrlVal(pParticleEditPanel,EDIT_P_VELOCITY_Y,&velY);
				particles[index].pVelocityY = velY;	
			} 
		
		 
			memset(&particle,0,sizeof(genericParticle));// init particle
			particle.relPosX = (point.x - (centerOffsetX)) * scaleFactor ;
			particle.relPosY = (point.y - (centerOffsetY)) * scaleFactor ;
			GetCtrlVal(panel,EDIT_P_PARTICLE_MASS,&mass);
		
		
			GetCtrlVal(panel,EDIT_P_PARTICLE_CHARGE,&charge);
	    	GetCtrlVal(panel,EDIT_P_STRANGE_FIELD ,&sField);	
			GetCtrlVal(panel,EDIT_P_FIXED_P,&fixed);
			GetCtrlVal(panel,EDIT_P_TRAILS_ON,&trailOn);
			GetCtrlVal(panel,EDIT_P_FOLLOW_MOUSE,&follow);
			particles[index].trailOn = trailOn;    
			particles[index].followMouse = follow;
			particles[index].pFixed = fixed;
			particles[index].pType = GENERIC_P;
			particles[index].pMass = mass;
			particles[index].pCharge = charge;
			particles[index].pSField = sField;
		   
			if(particles[index].followMouse == TRUE)
			{
				pFollowId = index;
				pointerIsAParticle = TRUE;
			}
			else if(pFollowId == index && 
					 particles[index].followMouse == FALSE)
			{
				pointerIsAParticle = FALSE;
			}
			
			pParticleEditPanel = 0; 
			DiscardPanel(panel); 	
			break;
	}
	return 0;
}

int CVICALLBACK updateParticleInfo (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{

	switch (event)
	{
		case EVENT_COMMIT:
			
			updParticleEditInfo();
			
			break;
			
	}
	return 0;
}

int CVICALLBACK CancelParticleEdit (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			pParticleEditPanel = 0;
			DiscardPanel(panel);
			break;
	}
	return 0;
}
void updParticleEditInfo(void)
{

	double mass,charge,sField;
	double velX,velY;
	int fixed,velXEn,velYEn;
	int i,trailOn,follow;
	
    GetCtrlVal(pParticleEditPanel,EDIT_P_INDEX,&i);
	GetCtrlVal(pParticleEditPanel,EDIT_P_VELX_EN,&velXEn);
	GetCtrlVal(pParticleEditPanel,EDIT_P_VELX_EN,&velYEn);
	mass=particles[i].pMass;
	velX=particles[i].pVelocityX; 
	velY=particles[i].pVelocityY;
	charge=particles[i].pCharge;
	sField = particles[i].pSField;
	fixed = particles[i].pFixed; 
	trailOn = particles[i].trailOn;
	follow = particles[i].followMouse;
	SetCtrlVal(pParticleEditPanel,EDIT_P_PARTICLE_MASS,mass);
	SetCtrlAttribute(pParticleEditPanel,EDIT_P_VELOCITY_X,ATTR_DIMMED,1);
	SetCtrlAttribute(pParticleEditPanel,EDIT_P_VELOCITY_Y,ATTR_DIMMED,1);
	if(velXEn){
		SetCtrlAttribute(pParticleEditPanel,EDIT_P_VELOCITY_X,ATTR_DIMMED,0);
		SetCtrlVal(pParticleEditPanel,EDIT_P_VELOCITY_X,velX);
	}
	if(velYEn){
		SetCtrlAttribute(pParticleEditPanel,EDIT_P_VELOCITY_Y,ATTR_DIMMED,0);
		SetCtrlVal(pParticleEditPanel,EDIT_P_VELOCITY_Y,velY);
	}
	SetCtrlVal(pParticleEditPanel,EDIT_P_PARTICLE_CHARGE,charge);
    SetCtrlVal(pParticleEditPanel,EDIT_P_STRANGE_FIELD ,sField);	
	SetCtrlVal(pParticleEditPanel,EDIT_P_FIXED_P,fixed);
	SetCtrlVal(pParticleEditPanel,EDIT_P_TRAILS_ON,trailOn);
	SetCtrlVal(pParticleEditPanel,EDIT_P_FOLLOW_MOUSE,follow); 
    GetCtrlVal(pParticleEditPanel,EDIT_P_INDEX,&i);
	
		

}

void CVICALLBACK loadText (int menuBar, int menuItem, void *callbackData,
		int panel)
{
	FILE *fd;                                    /* file descripter            */
    char errorMsg[128];                      /* buffer for error message      */
    char filename[260];                      /* file to read                  */
    int status;                              /* function return status        */
	unsigned int i,j,a,b;
	
	unsigned int pId;       
	int	pType;     
	int	trailOn;   
	int	firstDrawn;
	int	pFixed;
	int ignGrav;
	double x,y;
	
	WaitForSingleObject(trailSemHandler,INFINITE);
    WaitForSingleObject(saveSemHandler,INFINITE);
	status = FileSelectPopup(".",            /* start at current directory    */
             "*.txt",                      /* Display only files .rom ext   */
             "*.txt",                      /* Only one file type            */
             "Load State Text",         /* Title of box                  */
             VAL_LOAD_BUTTON,                /* Use Load as action button     */
             1,                              /* Can change directories        */
             1,                              /* Limit files to .dat ext       */
             1,                              /* User can cancel               */
             1,                              /* User can make new dir         */
             filename);                      /* Path name selected by user    */

    //Load File after selecting file
    if(status)                               /* if file selected ...          */
    {
        fd = fopen(filename, "r");           /* open file to read             */
		fscanf(fd,"PNUM %d ",&particleNumber);
		
		if(fd)                               /* if open successful ...        */
        {
            
			//Fill the string with data
			for(j=0;j < particleNumber;j++)
			{
		
				fscanf(fd,"PARTICLE %d\n",&pId);
				fscanf(fd,"pType %d \n",&pType);
				fscanf(fd,"ignGrav %d \n",&ignGrav);
				fscanf(fd,"pTrailOn %d\n",&trailOn);
				fscanf(fd,"firstDrawn %d\n",&firstDrawn);
				fscanf(fd,"pFixed %d\n",&pFixed);
				fscanf(fd,"pCharge %lf\n",&particles[j].pCharge);
				fscanf(fd,"pMass %lf\n",&particles[j].pMass);  
				fscanf(fd,"pSpForce %lf\n",&particles[j].pSpForce);
				fscanf(fd,"pSpin %lf\n",&particles[j].pSpin);  
				fscanf(fd,"relPosX %lf\n",&particles[j].relPosX);
				fscanf(fd,"relPosY %lf\n",&particles[j].relPosY);
				fscanf(fd,"oldPosX %lf\n",&particles[j].oldPosX);
				fscanf(fd,"oldPosY %lf\n",&particles[j].oldPosY);
				fscanf(fd,"pVelocityX %lf\n",&particles[j].pVelocityY);   
				fscanf(fd,"pVelocityY %lf\n",&particles[j].pVelocityX);   
				fscanf(fd,"pAccelX %lf\n", &particles[j].pAccelX);      
				fscanf(fd,"pAccelY %lf\n",&particles[j].pAccelY);      
				fscanf(fd,"pTime %lf\n",&particles[j].pTime);        
				fscanf(fd,"pSfield %lf\n",&particles[j].pSField);      
				fscanf(fd,"trailLength %d\n",&particles[j].trailLength); 
				fscanf(fd,"trailMaxSize %d\n",&particles[j].trailMaxSize);
				fscanf(fd,"trailColor %d\n",&particles[j].tColor);
				fscanf(fd,"trailType %d\n",&particles[j].tType);
				particles[j].pId = pId;       	
				particles[j].pType = pType;     	
				particles[j].trailOn = trailOn;   	
				particles[j].firstDrawn= firstDrawn;	
				particles[j].pFixed = pFixed;
				particles[j].ignoreGrav = ignGrav;
				
			}
			for(j=0;j<particleNumber;j++) 
			{
				free(particles[j].trailPoint);                               
				particles[j].trailPoint = malloc(particles[j].trailMaxSize*  
					sizeof(trailDot));                                       
				for(i=0;i< particles[j].trailLength;i++)
				{
					fscanf(fd,"pId %d tPoint %d: %lf,%lf\n",
						&j,
						&i,
						&particles[j].trailPoint[i].x,
						&particles[j].trailPoint[i].y);	  
				}
			}
			
            fclose(fd);	

       
		}
	}
    else                                 /* if open error ...             */
    {
        sprintf(errorMsg,                /* create error string           */
                "Cannot load file %s.",
                filename);

        MessagePopup("MCE Load Error",   /* display error message         */
                     errorMsg);
    }

	
	
	ReleaseSemaphore(trailSemHandler,1,NULL);
	ReleaseSemaphore(saveSemHandler,1,NULL);	
}

void CVICALLBACK saveText (int menuBar, int menuItem, void *callbackData,
		int panel)
{
	FILE *fd;                                /* file */
	size_t checkSize;
    unsigned int fileSize =0;
	unsigned int bufferSize =0;
	char errorMsg[128];                      /* buffer for error message      */
    char filename[260];                      /* file to write                 */
    int status;
	unsigned int i,j,totalTrailMaxSize=0;
	
				
	WaitForSingleObject(trailSemHandler,INFINITE); 
	WaitForSingleObject(saveSemHandler,INFINITE);

	status = FileSelectPopup(".",            /* start at root directory       */
             "*.txt",                      /* Display only files .state ext   */
             "*.txt",                      /* Only one file type            */
             "Save State as text",           /* Title of box             */
             VAL_SAVE_BUTTON,                /* Use Save as action button     */
             1,                              /* Can change directories        */
             1,                              /* Limit files to .dat ext       */
             1,                              /* User can cancel               */
             1,                              /* User can make new dir         */
             filename);                      /* Path name selected by user    */

   
	if(status)                               /* if file selected ...       */
    {
	   
		fd = fopen(filename, "w+");         /* open file to write         */

        if(fd)                               /* if open successful ...     */
        {
            for(j=0;j < particleNumber;j++)
			{
				totalTrailMaxSize += particles[j].trailMaxSize;
			}
			//Initialize a string the length of all the data
  
			fprintf(fd,"PNUM %d\n",	particleNumber); 
			
			//Fill the string with data
			for(j=0;j < particleNumber;j++)
			{
				fprintf(fd,"PARTICLE %d\n",	particles[j].pId);
				fprintf(fd,"pType %d \n",particles[j].pType);
				fprintf(fd,"ignGrav %d \n",particles[j].ignoreGrav);
				fprintf(fd,"pTrailOn %d\n",particles[j].trailOn);
				fprintf(fd,"firstDrawn %d\n",particles[j].firstDrawn);
				fprintf(fd,"pFixed %d\n",particles[j].pFixed);
				fprintf(fd,"pCharge %.16lf\n",particles[j].pCharge);
				fprintf(fd,"pMass %.16lf\n"	,particles[j].pMass);  
				fprintf(fd,"pSpForce %.16lf\n",particles[j].pSpForce);
				fprintf(fd,"pSpin %.16lf\n"	,particles[j].pSpin);  
				fprintf(fd,"relPosX %.16lf\n",particles[j].relPosX);
				fprintf(fd,"relPosY %.16lf\n",particles[j].relPosY);
				fprintf(fd,"oldPosX %.16lf\n",particles[j].oldPosX);
				fprintf(fd,"oldPosY %.16lf\n",particles[j].oldPosY);
				fprintf(fd,"pVelocityX %.16lf\n",particles[j].pVelocityY);   
				fprintf(fd,"pVelocityY %.16lf\n",particles[j].pVelocityX);   
				fprintf(fd,"pAccelX %.16lf\n", particles[j].pAccelX);      
				fprintf(fd,"pAccelY %.16lf\n",particles[j].pAccelY);      
				fprintf(fd,"pTime %.16lf\n",particles[j].pTime);        
				fprintf(fd,"pSfield %.16lf\n",particles[j].pSField);      
				fprintf(fd,"trailLength %d\n",particles[j].trailLength); 
				fprintf(fd,"trailMaxSize %d\n", particles[j].trailMaxSize);
				fprintf(fd,"trailColor %d\n",particles[j].tColor);
				fprintf(fd,"trailType %d\n",particles[j].tType);
			}
	
			for(j=0;j < particleNumber;j++)
			{
		
				for(i=0;i < particles[j].trailLength; i++) 
				{
					if(particles[j].trailLength == 0)  // If the length is zero
						continue;
					fprintf(fd,"pId %d tPoint %d: %.16lf,%.16lf\n",
						j,
						i,
						particles[j].trailPoint[i].x,
						particles[j].trailPoint[i].y);
				
			
					}
				}
			}
			
			
            fclose(fd);                      /* close file                    */
			
        }
        else                                 /* if open error ...             */
        {
            sprintf(errorMsg,                /* create error string           */
                    "Cannot save file %s.",
                    filename);

            MessagePopup("MCE Save Error",   /* display error message         */
                         errorMsg);
        }

	
	ReleaseSemaphore(trailSemHandler,1,NULL);
    ReleaseSemaphore(saveSemHandler,1,NULL);
}
