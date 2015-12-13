#ifndef _GEN_VAR_H
#define _GEN_VAR_H
#include <windows.h>
#include <winbase.h>
#include <utils.h>
// Math Macros
#define INFINITE   0xFFFFFFFF
#define SQUARE(X)   ((X)*(X))   //power of two 
#define TRUE       1
#define FALSE      0

#define SFIELD(sConst,factorA,factorB,dist) sConst*((factorA)*(factorB))/SQUARE(SQUARE(dist))
#define EVOLT(eConst,chargeA,chargeB,dist) eConst*((chargeA)*(chargeB))/(SQUARE(dist))
#define GFORCE(gConst,massA,massB,dist) gConst*((massA)*(massB))/SQUARE(dist)
#define SPFORCE(spConst,massA,massB,dist) spConst*((massA)*(massB))/(dist)
#define DISTANCE(y2,y1,x2,x1) sqrt(SQUARE(y2-y1)+SQUARE(x2 - x1))
//Particle  types
#define UNDEF_P       0
#define GENERIC_P     1
#define FIXED_P       2 //Cannot be moved
#define ELECTRON_P    3
#define DOTS          2
#define DASH          1
#define SOLID         0


#define STRANGE_CONST 100000
#define FAKE_G_CONST  100
#define FAKE_E_CONST  1
#define GRAVITY_CONST 0.0000000000667428
//General use definitions
#define MAX_PART_NUM 1000
#define MAX_TRAIL_PCKT_SIZE  10000
#define BUFFER       100000 //For text saving
//panels defines
#define P_INFO_PANEL 0x00000001
extern int panelHandle;
extern int menuPanelHandle;
extern int pInfoPanelHandle;
extern int pParticleEditPanel;
extern int colorPanelHandle;
typedef struct
{
	double x;
	double y;
	
}trailDot;

//This is the generic particle
typedef struct{
	
	unsigned int pFixed      :1;
	unsigned int pType       :1;
	unsigned int trailOn     :1;
	unsigned int firstDrawn  :1;
	unsigned int ignoreGrav  :1;
	unsigned int followMouse :1;
	unsigned int pId         :16;
	unsigned int reserved    :10;
	
	unsigned int trailLength;
	unsigned int trailMaxSize;
	unsigned int tColor;
	unsigned int tType;
	double pCharge;
	double pMass;
	double pSpin;
	double pSpForce;
	double relPosX;
	double relPosY;
	double oldPosX;
	double oldPosY;
	double pVelocityY;
	double pVelocityX;
	double pAccelX;
	double pAccelY;
	double pTime;
	double pSField;
	trailDot*  trailPoint;
	
}genericParticle;
//Saved state structure
typedef struct{
	genericParticle allParticles[MAX_PART_NUM];
	trailDot* trailPoint[MAX_PART_NUM];
	unsigned int pCount;
	unsigned int fileSize;
}saveStateStruct;

// A force vector
typedef struct{
	int    forceType;
	double xComp;// X Component
	double yComp;// Y component
}vectorForce;




extern int particleNumber;
extern float scaleFactor;
extern int centerOffsetX;
extern int centerOffsetY;
extern genericParticle testParticle;
extern genericParticle particles[MAX_PART_NUM];
extern trailDot *trailsArray[MAX_PART_NUM];
extern int canvasWidth;   
extern int canvasHeight; 
extern trailDot thisPoint;// trail to be recorded
extern int pFollowId;
extern int pointerIsAParticle;
//Thread handlers
//Real time
extern HANDLE timeThreadHandler;
extern HANDLE trailSemHandler;
extern HANDLE saveSemHandler;
extern unsigned int tIdTime;
unsigned long timerThread(int param);
//Display refresh
extern HANDLE displayThreadHandler;
extern unsigned int tIdDisplay;
unsigned long displayRefresh(int param);
//Function prototypes
int applyElecForce(genericParticle* particle,vectorForce force); 
int debugMoveParticle(genericParticle* particle);
int debugAccelParticle(genericParticle* particle);
int addParticle(genericParticle particle); 
int applyForcesOnParticles();
int applyStrangeFieldOnParticles();
void RefreshParticleInfo(void);
void updParticleEditInfo(void);
int addDataToDynamicArray(trailDot,unsigned int id);
#endif
