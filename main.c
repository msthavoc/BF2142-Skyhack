#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>	// for itoa() call
#include <conio.h>
#include "resource.h"





#pragma comment(linker,"/FILEALIGN:512 /MERGE:.rdata=.text /MERGE:.data=.text /SECTION:.text,EWR /IGNORE:4078")

BOOL GameRunning;

/** Start of Declarations here **/
BOOL GetProcessList( );

// Below is the about text that is shown when "About" button is clicked

char *about   =
"BF2142 Multihack v0.2 mst\n"
"Created by msthavoc/mstbigboss\n"
" \n"
"Greets\n"
"PSYCH0_DAD\n"
" \n"
"2010 | mst-gaming.de\n"
"----------------------\n";




char *hotkeylist  =
"BF2142 Multihack v0.2 mst\n"
"Created by msthavoc/mstbigboss\n"
"HotKey List:\n"
"CLTR + R    -> DemoFix\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"2010 | mst-gaming.de\n"
"----------------------\n";





/////////////////////////////////////////////////////////////////////

char *gameWindow = "BF2142.exe"; // exe name here
DWORD pid; HWND hwndWindow; DWORD bytes; HANDLE hand = NULL;


HANDLE pFile; //Used for logging address to file (not implimented in this build)

//below you will list the BOOLs for function toggles
BOOL IsHack1On, FirstTime1;

///////////////////////////////////////////////////////
////Global Variables
int * fogPtr;
float floatvalue;





//ADRESS LIST
int BF2142 = 0x400000;  //BF2142.exe Adress
int RendDX9; //RendDX9.dll Adress
int mem_adr[50];  //Adresslist
int * addrPtr;
BYTE unlock_console=0xEB;
int dem_fix=0x90;
int demotimev;
int demojumpv;




int msthavoc;

//ADRESS LIST POINTER


	char * nullPtr;
	float * floatPtr;
	BYTE NOPon[2] = {0x90, 0x90};							//Generic 2Byte NOP


	BYTE original_code[4] = {0}; //defines our variable name and gives it a value of 0
	BYTE sky_x[2] = {0}; //defines our variable name and gives it a value of 0
	BYTE sky_y[2] = {0}; //defines our variable name and gives it a value of 0
	BYTE sky_z[2] = {0}; //defines our variable name and gives it a value of 0
	BYTE hascloudlayer;
	BYTE hascloudlayer2;
	BYTE testbyte[32];

	
	float skyx_s = 0;
	float skyy_s = 0;
	float skyz_s = 0;
	float skyx_e = 0;
	float skyy_e = 0;
	float skyz_e = 0;

	long double skyxyz = 0;
	float steps = 0.1f;
	int skyposition;
	


	char char_adress[50];
	char char_skyx_start[50];
	char char_skyx_end[50];
	char char_skyy_start[50];
	char char_skyy_end[50];
	char char_skyz_start[50];
	char char_skyz_end[50];
	char char_timer[50];



//////////////////////////////
	HWND hwndEdit; 
	BYTE timing;
	BYTE timing2;
	char tempVar[50];
	HWND hwndEdit2;
	HWND hwndTimer;	
	HWND hwndDemoTime;	
	char tempVar2[50];
	float tempFloat;
	float tempFloat2;
	BYTE tempByte;
	long double tempDouble;
	int tempInt;
	float sundirection[3] = {0};
	float domeposition[4] = {0};
	float scrolldirection[4] = {0};
	float flaredirection[3] = {0};
	int adressen[2] = {0};
	int animated=0;
	int testzaehler=0;
	int fps[3];//fps[1]=time(sec) | fps[2]=fps | fps[3]=frames //fps[1]*fps[2]=fps[3]


	struct element* vl_sundirectionx = NULL;
	struct element* vl_sundirectiony = NULL;
	struct element* vl_sundirectionz = NULL;
	struct element* vl_flaredirectionx = NULL;
	struct element* vl_flaredirectiony = NULL;
	struct element* vl_flaredirectionz = NULL;
	struct element* vl_domepositionx = NULL;
	struct element* vl_domepositiony = NULL;
	struct element* vl_domepositionz = NULL;
	struct element* vl_domerotation = NULL;
	struct element* vl_scrolldirect1x = NULL;
	struct element* vl_scrolldirect1y = NULL;
	struct element* vl_scrolldirect2x = NULL;
	struct element* vl_scrolldirect2y = NULL;
	float targ_sundirection[3];
	float targ_dome[4];
	float targ_sc[4];
	struct element* e = NULL;









	


	
///////////////////////////////////////////////////////

	/** End of Declarations here **/
//verkettete liste

	
struct element 
{
	float wert;
	struct element* naechstes;
};

struct element* lElement(struct element* kopf)
{
	struct element* e = NULL;
	e=kopf;
	while(e->naechstes != NULL)
	{
		e=e->naechstes;
		
	}
	return e;
}


void einfuegen_hinten(struct element* kopf, float wert) {
	struct element* e = NULL;
	struct element* neuElement = (struct element*) malloc(sizeof(struct element));
	neuElement->wert=wert;
	neuElement->naechstes = NULL;
	e=lElement(kopf);
	e->naechstes = neuElement;
}

void lEloeschen(struct element* kopf)
{
	if (kopf->naechstes != NULL)
	{
		struct element* le = NULL; //letztes Element
		struct element* de = NULL; //derzeitiges Element

		le = lElement(kopf); //-> definiert letztes Element
		de = kopf; //übergabe der Liste an das derzeitige Element

		while( (de->naechstes != NULL) && (de->naechstes != le) ) //suche nach vorletztem Element Pointer adresse naechste == letztem Element
		{
			de=de->naechstes;
		}
		free(le); //speicher freigeben fürs letzte Element
		de->naechstes = NULL; //Pointerverbindung löschen
	}
}










struct element* neueVerketteteListe(float wert)
	{
		struct element* liste = NULL;
		liste = (struct element*) malloc(sizeof(struct element));
		liste->naechstes = NULL;
		liste->wert=wert;
		return liste;
	};








///////////////////////////////////////////////////////
void initAdress(HWND hwnd){
/* Adress Overview

mem_adr[0] //unlock console
mem_adr[1] //timing
mem_adr[2] //sundirection[x]
mem_adr[3] //sundirection[y]
mem_adr[4] //sundirection[z]
mem_adr[5] //domeposition[x]
mem_adr[6] //domeposition[y]
mem_adr[7] //domeposition[z]
mem_adr[8] //domerotation
mem_adr[9] //scrolldirection 1 x
mem_adr[10] //scrolldirection 1 y
mem_adr[11] //scrolldirection 2 x
mem_adr[12] //scrolldirection 2 y
mem_adr[13] //scrolldirection 1 enabled
mem_adr[14] //scrolldirection 2 enabled
mem_adr[15] //flaredirection x
mem_adr[16] //flaredirection y
mem_adr[17] //flaredirection z
mem_adr[18] = //Demo Fix Adress 1
mem_adr[19] = //Demo Fix Adress 1
mem_adr[20] = //Demo Fix Adress 1
mem_adr[21] = //Demo Fix Adress 1
mem_adr[22] = //Demo Fix Adress 1
mem_adr[23] = //Demo Fix Adress 1
mem_adr[24] = //Demo Fix Adress 1
mem_adr[25] = //Demo Fix Adress 1
mem_adr[26] = //Demo Fix Adress 1
mem_adr[27] = //Demo Fix Adress 1
mem_adr[28] = //Demo Fix Adress 1
mem_adr[29] = //Demo Fix Adress 1
mem_adr[30] = //Demo Fix Adress 1
mem_adr[31] = //Demo Fix Adress 1
mem_adr[32] //Demo Position Adress


*/




//* addrPtr;

	
	mem_adr[0] = BF2142+0x276B48; //unlock console
	mem_adr[1] = BF2142-0x371F30; //timing
	




	//adress RendDX9
	RendDX9 = BF2142 + 0x62B7A0; //RendDX9.dll Adress
	ReadProcessMemory(hand, (void*)+(int)RendDX9, &addrPtr, sizeof(int), NULL);
	addrPtr = (int) addrPtr + 0xC;
	ReadProcessMemory(hand, (void*)+(int)addrPtr, &addrPtr, sizeof(int), NULL);
	RendDX9 = (int) addrPtr;
	
	

	//adress sundirection[x]
	addrPtr = BF2142+0x62B79C;
	ReadProcessMemory(hand, (void*)+(int)addrPtr , &addrPtr, sizeof(int), NULL);
	addrPtr = (int)addrPtr + (int)0x74;
	ReadProcessMemory(hand, (void*)+(int)addrPtr , &addrPtr, sizeof(int), NULL);
	addrPtr = (int)addrPtr + (int)0x30;
	ReadProcessMemory(hand, (void*)+(int)addrPtr , &addrPtr, sizeof(int), NULL);
	addrPtr = (int)addrPtr + (int)0x8;
	ReadProcessMemory(hand, (void*)+(int)addrPtr , &addrPtr, sizeof(int), NULL);
	addrPtr = (int)addrPtr + (int)0xC;
	ReadProcessMemory(hand, (void*)+(int)addrPtr , &addrPtr, sizeof(int), NULL);
	addrPtr = (int)addrPtr  + (int)0x164;
	mem_adr[2] = (int)addrPtr; 
	//adress sundirection[y]
	mem_adr[3] = mem_adr[2]+0x4;
	//adress sundirection[z]
	mem_adr[4] = mem_adr[2]+0x8;

	//temprary
	//adressen[0] = (int)mem_adr[2] ;

	

	//adresse domepos x
	addrPtr = (int)RendDX9+0x22496C;
	ReadProcessMemory(hand, (void*)+(int)addrPtr , &addrPtr, sizeof(int), NULL);
	addrPtr = (int)addrPtr + (int)0x238;
	ReadProcessMemory(hand, (void*)+(int)addrPtr , &addrPtr, sizeof(int), NULL);
	addrPtr = (int)addrPtr + (int)0xC;
	mem_adr[5] = (int)addrPtr;

	//temprary
	//adressen[1] = mem_adr[5];

	//adresse domepos y
	mem_adr[6] = mem_adr[5]+0x4;
	//adresse domepos z
	mem_adr[7] = mem_adr[5]+0x8;
	//adresse domerot
	mem_adr[8] = mem_adr[5]+0x10;

	//adresse scrolldirection 1 x
	mem_adr[9] = mem_adr[5]+0xC4;
	//adresse scrolldirection 1 y
	mem_adr[10] = mem_adr[5]+0xC8;
	//adresse scrolldirection 2 x
	mem_adr[11] = mem_adr[5]+0xCC;
	//adresse scrolldirection 2 y
	mem_adr[12] = mem_adr[5]+0xD0;
	//adresse scrolldirection 1 enabled
	mem_adr[13] = mem_adr[5]+0x68;
	//adresse scrolldirection 2 enabled
	mem_adr[14] = mem_adr[5]+0x69;

	//adresse flaredirection x
	mem_adr[15] = mem_adr[5]+0x188;
	//adresse flaredirection y
	mem_adr[16] = mem_adr[5]+0x18C;
	//adresse flaredirection y
	mem_adr[17] = mem_adr[5]+0x190;
	//Demo Fix Adress 1
	mem_adr[18] = BF2142 + 0x08CCF8;
	mem_adr[19] = BF2142 + 0x08CCF9;
	mem_adr[20] = BF2142 + 0x08CCFA;
	mem_adr[21] = BF2142 + 0x08CCFB;
	mem_adr[22] = BF2142 + 0x08CCFC;
	mem_adr[23] = BF2142 + 0x08CCFD;
	mem_adr[24] = BF2142 + 0x08CCFE;
	//Demo Fix Adress 2
	mem_adr[25] = BF2142 + 0x0B78A2;
	mem_adr[26] = BF2142 + 0x0B78A3;
	mem_adr[27] = BF2142 + 0x0B78A4;
	mem_adr[28] = BF2142 + 0x0B78A5;
	mem_adr[29] = BF2142 + 0x0B78A6;
	mem_adr[30] = BF2142 + 0x0B78A7;
	mem_adr[31] = BF2142 + 0x0B78A8;

	//Demo Position Adress
	mem_adr[32] = BF2142 + 0x6884D0;

	



	mem_adr[33] = 0;


}



void fixdemo(HWND hwnd){
		initAdress(hwnd);

		WriteProcessMemory(hand, (void*)mem_adr[18], &dem_fix,1, &bytes);
		WriteProcessMemory(hand, (void*)mem_adr[19], &dem_fix,1, &bytes);
		WriteProcessMemory(hand, (void*)mem_adr[20], &dem_fix,1, &bytes);
		WriteProcessMemory(hand, (void*)mem_adr[21], &dem_fix,1, &bytes);
		WriteProcessMemory(hand, (void*)mem_adr[22], &dem_fix,1, &bytes);
		WriteProcessMemory(hand, (void*)mem_adr[23], &dem_fix,1, &bytes);
		WriteProcessMemory(hand, (void*)mem_adr[24], &dem_fix,1, &bytes);
				
		WriteProcessMemory(hand, (void*)mem_adr[25], &dem_fix,1, &bytes);
		WriteProcessMemory(hand, (void*)mem_adr[26], &dem_fix,1, &bytes);	
		WriteProcessMemory(hand, (void*)mem_adr[27], &dem_fix,1, &bytes);	
		WriteProcessMemory(hand, (void*)mem_adr[28], &dem_fix,1, &bytes);	
		WriteProcessMemory(hand, (void*)mem_adr[29], &dem_fix,1, &bytes);	
		WriteProcessMemory(hand, (void*)mem_adr[30], &dem_fix,1, &bytes);	
		WriteProcessMemory(hand, (void*)mem_adr[31], &dem_fix,1, &bytes);


}


void unlockconsole(HWND hwnd){
		initAdress(hwnd);

		//ReadProcessMemory(hand, (void*)+(int)mem_adr[0] , &unlock_console, 1, &bytes);
		WriteProcessMemory(hand, (void*)+(int)mem_adr[0], &unlock_console, 1, &bytes);

}



void demoskip(HWND hwnd)
{
	initAdress(hwnd);
	tempInt=demotimev+200;
	WriteProcessMemory(hand, (void*)+(int)mem_adr[32] , &tempInt, 4, &bytes);
	


}


void demojump(HWND hwnd){
	initAdress(hwnd);

	hwndEdit = GetDlgItem(hwnd, IDC_DEMOJUMPER_VALUE);
	GetWindowText(hwndEdit, tempVar, 50);
	demojumpv=atoi(tempVar);
	if (demojumpv < demotimev){
			demojumpv = demotimev;
			sprintf(tempVar, "%i", demojumpv);
			SetWindowText(hwndEdit, tempVar);
		}
	else
	{
	WriteProcessMemory(hand, (void*)+(int)mem_adr[32] , &demojumpv, 4, &bytes);
	}
}







void aboutButton(HWND hwnd)
{
	
	MessageBox(hwnd,about,"About",MB_ICONINFORMATION);

}

void hkl(HWND hwnd)
{
	//MessageBox(hwnd,hotkeylist,"HotKeyList",MB_ICONINFORMATION);
	//
	Beep(500,120);
	Beep(600,120);
	Beep(700,120);
	Beep(800,120);
}






void Initialize_Menu(HWND hwnd){
			

			tempVar[0]='X';
			hwndEdit = GetDlgItem(hwnd, IDC_SUNDIRECTION_X); 
			SetWindowText(hwndEdit,tempVar);
			EnableWindow(hwndEdit, FALSE);
			hwndEdit = GetDlgItem(hwnd, IDC_FLAREDIRECTION_X); 
			SetWindowText(hwndEdit,tempVar);
			EnableWindow(hwndEdit, FALSE);
			hwndEdit = GetDlgItem(hwnd, IDC_DOMEPOS_X); 
			SetWindowText(hwndEdit,tempVar);
			EnableWindow(hwndEdit, FALSE);
			hwndEdit = GetDlgItem(hwnd, IDC_SCROLLD1_X); 
			SetWindowText(hwndEdit,tempVar);
			EnableWindow(hwndEdit, FALSE);
			hwndEdit = GetDlgItem(hwnd, IDC_SCROLLD2_X); 
			SetWindowText(hwndEdit,tempVar);
			EnableWindow(hwndEdit, FALSE);

			hwndEdit = GetDlgItem(hwnd, IDC_SUNDIRECTION_X2); 
			SetWindowText(hwndEdit,tempVar);
			EnableWindow(hwndEdit, FALSE);
			hwndEdit = GetDlgItem(hwnd, IDC_FLAREDIRECTION_X2); 
			SetWindowText(hwndEdit,tempVar);

			hwndEdit = GetDlgItem(hwnd, IDC_DOMEPOS_X2); 
			SetWindowText(hwndEdit,tempVar);

			hwndEdit = GetDlgItem(hwnd, IDC_SCROLLD1_X2); 
			SetWindowText(hwndEdit,tempVar);
			EnableWindow(hwndEdit, FALSE);
			hwndEdit = GetDlgItem(hwnd, IDC_SCROLLD2_X2); 
			SetWindowText(hwndEdit,tempVar);
			EnableWindow(hwndEdit, FALSE);


			tempVar[0]='Y';
			hwndEdit = GetDlgItem(hwnd, IDC_SUNDIRECTION_Y); 
			SetWindowText(hwndEdit,tempVar);
			EnableWindow(hwndEdit, FALSE);
			hwndEdit = GetDlgItem(hwnd, IDC_FLAREDIRECTION_Y); 
			SetWindowText(hwndEdit,tempVar);
			EnableWindow(hwndEdit, FALSE);
			hwndEdit = GetDlgItem(hwnd, IDC_DOMEPOS_Y); 
			SetWindowText(hwndEdit,tempVar);
			EnableWindow(hwndEdit, FALSE);
			hwndEdit = GetDlgItem(hwnd, IDC_SCROLLD1_Y); 
			SetWindowText(hwndEdit,tempVar);
			EnableWindow(hwndEdit, FALSE);
			hwndEdit = GetDlgItem(hwnd, IDC_SCROLLD2_Y); 
			SetWindowText(hwndEdit,tempVar);
			EnableWindow(hwndEdit, FALSE);

			hwndEdit = GetDlgItem(hwnd, IDC_SUNDIRECTION_Y2); 
			SetWindowText(hwndEdit,tempVar);
			EnableWindow(hwndEdit, FALSE);
			hwndEdit = GetDlgItem(hwnd, IDC_FLAREDIRECTION_Y2); 
			SetWindowText(hwndEdit,tempVar);

			hwndEdit = GetDlgItem(hwnd, IDC_DOMEPOS_Y2); 
			SetWindowText(hwndEdit,tempVar);

			hwndEdit = GetDlgItem(hwnd, IDC_SCROLLD1_Y2); 
			SetWindowText(hwndEdit,tempVar);
			EnableWindow(hwndEdit, FALSE);
			hwndEdit = GetDlgItem(hwnd, IDC_SCROLLD2_Y2); 
			SetWindowText(hwndEdit,tempVar);
			EnableWindow(hwndEdit, FALSE);

			tempVar[0]='Z';
			hwndEdit = GetDlgItem(hwnd, IDC_SUNDIRECTION_Z); 
			SetWindowText(hwndEdit,tempVar);
			EnableWindow(hwndEdit, FALSE);
			hwndEdit = GetDlgItem(hwnd, IDC_FLAREDIRECTION_Z); 
			SetWindowText(hwndEdit,tempVar);
			EnableWindow(hwndEdit, FALSE);
			hwndEdit = GetDlgItem(hwnd, IDC_DOMEPOS_Z); 
			SetWindowText(hwndEdit,tempVar);
			EnableWindow(hwndEdit, FALSE);

			hwndEdit = GetDlgItem(hwnd, IDC_SUNDIRECTION_Z2); 
			SetWindowText(hwndEdit,tempVar);
			EnableWindow(hwndEdit, FALSE);
			hwndEdit = GetDlgItem(hwnd, IDC_FLAREDIRECTION_Z2); 
			SetWindowText(hwndEdit,tempVar);
			
			hwndEdit = GetDlgItem(hwnd, IDC_DOMEPOS_Z2); 
			SetWindowText(hwndEdit,tempVar);
	

			tempVar[0]='H';
			hwndEdit = GetDlgItem(hwnd, IDC_DOMEROT_H); 
			SetWindowText(hwndEdit,tempVar);
			EnableWindow(hwndEdit, FALSE);
			hwndEdit = GetDlgItem(hwnd, IDC_DOMEROT_H2); 
			SetWindowText(hwndEdit,tempVar);


			hwndEdit = GetDlgItem(hwnd, IDC_SUNDIRECTION); 
			EnableWindow(hwndEdit, FALSE);
			
			hwndEdit = GetDlgItem(hwnd, IDC_SCROLLD2); 
			EnableWindow(hwndEdit, FALSE);
			
			hwndEdit = GetDlgItem(hwnd, IDC_SCROLLD1); 
			EnableWindow(hwndEdit, FALSE);
			
			hwndEdit = GetDlgItem(hwnd, IDC_DOMEROT); 
			EnableWindow(hwndEdit, FALSE);

			hwndEdit = GetDlgItem(hwnd, IDC_DOMEPOS); 
			EnableWindow(hwndEdit, FALSE);
			
			hwndEdit = GetDlgItem(hwnd, IDC_FLAREDIRECTION); 
			EnableWindow(hwndEdit, FALSE);			

			hwndEdit = GetDlgItem(hwnd, IDC_SUNDIRECTION); 
			EnableWindow(hwndEdit, FALSE);	

			hwndEdit = GetDlgItem(hwnd, IDC_SET); 
			EnableWindow(hwndEdit, FALSE);	

			hwndEdit = GetDlgItem(hwnd, IDC_UNLOCK); 
			EnableWindow(hwndEdit, FALSE);	
			hwndEdit = GetDlgItem(hwnd, IDC_DEMOFIX); 
			EnableWindow(hwndEdit, FALSE);	
			hwndEdit = GetDlgItem(hwnd, IDC_DEMOSKIP); 
			EnableWindow(hwndEdit, FALSE);	
			hwndEdit = GetDlgItem(hwnd, IDC_DEMOJUMPER); 
			EnableWindow(hwndEdit, FALSE);	
			hwndEdit = GetDlgItem(hwnd, IDC_DEMOSKIP_VALUE); 
			EnableWindow(hwndEdit, FALSE);	
			hwndEdit = GetDlgItem(hwnd, IDC_DEMOJUMPER_VALUE); 
			EnableWindow(hwndEdit, FALSE);	
			hwndEdit = GetDlgItem(hwnd, IDC_DEMOPOS); 
			EnableWindow(hwndEdit, FALSE);

			
			


	
}





void Set_Menu(HWND hwnd){
			hwndEdit = GetDlgItem(hwnd, IDC_SUNDIRECTION_X); 
			EnableWindow(hwndEdit, TRUE);
			hwndEdit = GetDlgItem(hwnd, IDC_FLAREDIRECTION_X); 
			EnableWindow(hwndEdit, TRUE);
			hwndEdit = GetDlgItem(hwnd, IDC_DOMEPOS_X); 
			EnableWindow(hwndEdit, TRUE);
			hwndEdit = GetDlgItem(hwnd, IDC_DOMEPOS_X2); 
			EnableWindow(hwndEdit, TRUE);
			hwndEdit = GetDlgItem(hwnd, IDC_SCROLLD1_X); 
			EnableWindow(hwndEdit, TRUE);
			hwndEdit = GetDlgItem(hwnd, IDC_SCROLLD2_X); 
			EnableWindow(hwndEdit, TRUE);
			hwndEdit = GetDlgItem(hwnd, IDC_SCROLLD1_X2); 
			EnableWindow(hwndEdit, TRUE);
			hwndEdit = GetDlgItem(hwnd, IDC_SCROLLD2_X2); 
			EnableWindow(hwndEdit, TRUE);
			hwndEdit = GetDlgItem(hwnd, IDC_SUNDIRECTION_Y); 
			EnableWindow(hwndEdit, TRUE);
			hwndEdit = GetDlgItem(hwnd, IDC_FLAREDIRECTION_Y); 
			EnableWindow(hwndEdit, TRUE);
			hwndEdit = GetDlgItem(hwnd, IDC_DOMEPOS_Y); 
			EnableWindow(hwndEdit, TRUE);
			hwndEdit = GetDlgItem(hwnd, IDC_DOMEPOS_Y2); 
			EnableWindow(hwndEdit, TRUE);
			hwndEdit = GetDlgItem(hwnd, IDC_SCROLLD1_Y); 
			EnableWindow(hwndEdit, TRUE);
			hwndEdit = GetDlgItem(hwnd, IDC_SCROLLD2_Y); 
			EnableWindow(hwndEdit, TRUE);
			hwndEdit = GetDlgItem(hwnd, IDC_SCROLLD1_Y2); 
			EnableWindow(hwndEdit, TRUE);
			hwndEdit = GetDlgItem(hwnd, IDC_SCROLLD2_Y2); 
			EnableWindow(hwndEdit, TRUE);
			hwndEdit = GetDlgItem(hwnd, IDC_SUNDIRECTION_Z); 
			EnableWindow(hwndEdit, TRUE);
			hwndEdit = GetDlgItem(hwnd, IDC_FLAREDIRECTION_Z); 
			EnableWindow(hwndEdit, TRUE);
			hwndEdit = GetDlgItem(hwnd, IDC_DOMEPOS_Z); 
			EnableWindow(hwndEdit, TRUE);
			hwndEdit = GetDlgItem(hwnd, IDC_DOMEPOS_Z2); 
			EnableWindow(hwndEdit, TRUE);
			hwndEdit = GetDlgItem(hwnd, IDC_DOMEROT_H); 
			EnableWindow(hwndEdit, TRUE);
			hwndEdit = GetDlgItem(hwnd, IDC_DOMEROT_H2); 
			EnableWindow(hwndEdit, TRUE);

			hwndEdit = GetDlgItem(hwnd, IDC_SUNDIRECTION); 
			EnableWindow(hwndEdit, TRUE);
			hwndEdit = GetDlgItem(hwnd, IDC_SCROLLD2); 
			EnableWindow(hwndEdit, TRUE);
			hwndEdit = GetDlgItem(hwnd, IDC_SCROLLD1); 
			EnableWindow(hwndEdit, TRUE);
			hwndEdit = GetDlgItem(hwnd, IDC_DOMEROT); 
			EnableWindow(hwndEdit, TRUE);
			hwndEdit = GetDlgItem(hwnd, IDC_DOMEPOS); 
			EnableWindow(hwndEdit, TRUE);
			hwndEdit = GetDlgItem(hwnd, IDC_FLAREDIRECTION); 
			EnableWindow(hwndEdit, TRUE);
			hwndEdit = GetDlgItem(hwnd, IDC_SUNDIRECTION); 
			EnableWindow(hwndEdit, TRUE);	
			hwndEdit = GetDlgItem(hwnd, IDC_SET); 
			EnableWindow(hwndEdit, TRUE);	
			hwndEdit = GetDlgItem(hwnd, IDC_ANIMATION_START); 
			EnableWindow(hwndEdit, TRUE);	
			hwndEdit = GetDlgItem(hwnd, IDC_SUNDIRECTION_X2); 
			EnableWindow(hwndEdit, TRUE);
			hwndEdit = GetDlgItem(hwnd, IDC_SUNDIRECTION_Y2); 
			EnableWindow(hwndEdit, TRUE);
			hwndEdit = GetDlgItem(hwnd, IDC_SUNDIRECTION_Z2); 
			EnableWindow(hwndEdit, TRUE);
			hwndEdit = GetDlgItem(hwnd, IDC_SCROLLD1_LAYER); 
			EnableWindow(hwndEdit, TRUE);
			hwndEdit = GetDlgItem(hwnd, IDC_SCROLLD2_LAYER); 
			EnableWindow(hwndEdit, TRUE);


			hwndEdit = GetDlgItem(hwnd, IDC_TIME); 
			EnableWindow(hwndEdit, TRUE);	
			hwndEdit = GetDlgItem(hwnd, IDC_FPS); 
			EnableWindow(hwndEdit, TRUE);
			hwndEdit = GetDlgItem(hwnd, IDC_TIME_L); 
			EnableWindow(hwndEdit, TRUE);	
			hwndEdit = GetDlgItem(hwnd, IDC_FPS_L); 
			EnableWindow(hwndEdit, TRUE);	





			hwndEdit = GetDlgItem(hwnd, IDC_UNLOCK); 
			EnableWindow(hwndEdit, TRUE);	
			hwndEdit = GetDlgItem(hwnd, IDC_DEMOFIX); 
			EnableWindow(hwndEdit, TRUE);	
			hwndEdit = GetDlgItem(hwnd, IDC_DEMOSKIP); 
			EnableWindow(hwndEdit, TRUE);	
			hwndEdit = GetDlgItem(hwnd, IDC_DEMOJUMPER); 
			EnableWindow(hwndEdit, TRUE);	
			hwndEdit = GetDlgItem(hwnd, IDC_DEMOSKIP_VALUE); 
			EnableWindow(hwndEdit, TRUE);	
			hwndEdit = GetDlgItem(hwnd, IDC_DEMOJUMPER_VALUE); 
			EnableWindow(hwndEdit, TRUE);
			SetWindowText(hwndEdit, tempVar);
			hwndEdit = GetDlgItem(hwnd, IDC_DEMOPOS); 
			EnableWindow(hwndEdit, TRUE);




			//hwndEdit = GetDlgItem(hwnd, IDC_CHECK1); 
			//hwndEdit.GetCheck();
			//SetWindowValue(hwndEdit,"1");
			//SetValue(hwndEdit,1);
			//.GetCheck();
		//CheckDlgButton(hwndEdit, BST_CHECKED);
		// CheckDlgButton(IDC_CHECK1, BST_UNCHECKED);


	

		 //CheckDlgButton(hwnd, IDC_CHECK1, BST_CHECKED);
		 //CButton *pCheck=(CButton*)GetDlgItem(hwnd, IDC_CHECK1);

		


		

}








void Read_Values(HWND hwnd){



	////////////////SUNDIRECTION//////////////////////////////
	//sundirection x 
	hwndEdit = GetDlgItem(hwnd, IDC_SUNDIRECTION_X);
	ReadProcessMemory(hand, (void*)+(int)mem_adr[2] , &tempFloat, sizeof(float), NULL);
	sundirection[0]=tempFloat;
	sprintf(tempVar, "%f", tempFloat);
	SetWindowText(hwndEdit,tempVar);
	hwndEdit = GetDlgItem(hwnd, IDC_SUNDIRECTION_X2);
	SetWindowText(hwndEdit,tempVar);
	//sundirection y
	hwndEdit = GetDlgItem(hwnd, IDC_SUNDIRECTION_Y);
	ReadProcessMemory(hand, (void*)+(int)mem_adr[3] , &tempFloat, sizeof(float), NULL);
	sundirection[1]=tempFloat;
	sprintf(tempVar, "%f", tempFloat);
	SetWindowText(hwndEdit,tempVar);
	hwndEdit = GetDlgItem(hwnd, IDC_SUNDIRECTION_Y2);
	SetWindowText(hwndEdit,tempVar);
	//sundirection z 
	hwndEdit = GetDlgItem(hwnd, IDC_SUNDIRECTION_Z);
	ReadProcessMemory(hand, (void*)+(int)mem_adr[4] , &tempFloat, sizeof(float), NULL);
	sundirection[2]=tempFloat;
	sprintf(tempVar, "%f", tempFloat);
	SetWindowText(hwndEdit,tempVar);
	hwndEdit = GetDlgItem(hwnd, IDC_SUNDIRECTION_Z2);
	SetWindowText(hwndEdit,tempVar);
	////////////////SUNDIRECTION//////////////////////////////

	////////////////DOMEPOSITION/DOMEROTATION//////////////////////////////
	//domepos x 
	hwndEdit = GetDlgItem(hwnd, IDC_DOMEPOS_X);
	ReadProcessMemory(hand, (void*)+(int)mem_adr[5] , &tempFloat, sizeof(float), NULL);
	domeposition[0]=tempFloat;
	sprintf(tempVar, "%f", tempFloat);
	SetWindowText(hwndEdit,tempVar);
	hwndEdit = GetDlgItem(hwnd, IDC_DOMEPOS_X2);
	SetWindowText(hwndEdit,tempVar);
	//domepos y
	hwndEdit = GetDlgItem(hwnd, IDC_DOMEPOS_Y);
	ReadProcessMemory(hand, (void*)+(int)mem_adr[6] , &tempFloat, sizeof(float), NULL);
	domeposition[1]=tempFloat;
	sprintf(tempVar, "%f", tempFloat);
	SetWindowText(hwndEdit,tempVar);
	hwndEdit = GetDlgItem(hwnd, IDC_DOMEPOS_Y2);
	SetWindowText(hwndEdit,tempVar);
	//domepos z 
	hwndEdit = GetDlgItem(hwnd, IDC_DOMEPOS_Z);
	ReadProcessMemory(hand, (void*)+(int)mem_adr[7] , &tempFloat, sizeof(float), NULL);
	domeposition[2]=tempFloat;
	sprintf(tempVar, "%f", tempFloat);
	SetWindowText(hwndEdit,tempVar);
	hwndEdit = GetDlgItem(hwnd, IDC_DOMEPOS_Z2);
	SetWindowText(hwndEdit,tempVar);
	//domerotation 
	hwndEdit = GetDlgItem(hwnd, IDC_DOMEROT_H);
	ReadProcessMemory(hand, (void*)+(int)mem_adr[8] , &tempFloat, sizeof(float), NULL);
	domeposition[3]=tempFloat;
	sprintf(tempVar, "%f", tempFloat);
	SetWindowText(hwndEdit,tempVar);
	hwndEdit = GetDlgItem(hwnd, IDC_DOMEROT_H2);
	SetWindowText(hwndEdit,tempVar);
	////////////////DOMEPOSITION/DOMEROTATION//////////////////////////////

	////////////////Scrolldirection1&2//////////////////////////////
	//scrolldirection x
	hwndEdit = GetDlgItem(hwnd, IDC_SCROLLD1_X);
	ReadProcessMemory(hand, (void*)+(int)mem_adr[9] , &tempFloat, sizeof(float), NULL);
	scrolldirection[0]=tempFloat;
	sprintf(tempVar, "%f", tempFloat);
	SetWindowText(hwndEdit,tempVar);
	hwndEdit = GetDlgItem(hwnd, IDC_SCROLLD1_X2);
	SetWindowText(hwndEdit,tempVar);
	//scrolldirection y
	hwndEdit = GetDlgItem(hwnd, IDC_SCROLLD1_Y);
	ReadProcessMemory(hand, (void*)+(int)mem_adr[10] , &tempFloat, sizeof(float), NULL);
	scrolldirection[1]=tempFloat;
	sprintf(tempVar, "%f", tempFloat);
	SetWindowText(hwndEdit,tempVar);
	hwndEdit = GetDlgItem(hwnd, IDC_SCROLLD1_Y2);
	SetWindowText(hwndEdit,tempVar);
	//scrolldirection2 x
	hwndEdit = GetDlgItem(hwnd, IDC_SCROLLD2_X);
	ReadProcessMemory(hand, (void*)+(int)mem_adr[11] , &tempFloat, sizeof(float), NULL);
	scrolldirection[2]=tempFloat;
	sprintf(tempVar, "%f", tempFloat);
	SetWindowText(hwndEdit,tempVar);
	hwndEdit = GetDlgItem(hwnd, IDC_SCROLLD2_X2);
	SetWindowText(hwndEdit,tempVar);
	//scrolldirection2 y
	hwndEdit = GetDlgItem(hwnd, IDC_SCROLLD2_Y);
	ReadProcessMemory(hand, (void*)+(int)mem_adr[12] , &tempFloat, sizeof(float), NULL);
	scrolldirection[3]=tempFloat;
	sprintf(tempVar, "%f", tempFloat);
	SetWindowText(hwndEdit,tempVar);
	hwndEdit = GetDlgItem(hwnd, IDC_SCROLLD2_Y2);
	SetWindowText(hwndEdit,tempVar);

	//scrolldirection1 enabled
	hwndEdit = GetDlgItem(hwnd, IDC_SCROLLD1_LAYER);
	ReadProcessMemory(hand, (void*)+(int)mem_adr[13] , &hascloudlayer, 1, NULL);
	tempInt =hascloudlayer;
	sprintf(tempVar, "%i", tempInt);
	SetWindowText(hwndEdit,tempVar);
	//scrolldirection2 enabled
	hwndEdit = GetDlgItem(hwnd, IDC_SCROLLD2_LAYER);
	ReadProcessMemory(hand, (void*)+(int)mem_adr[14] , &hascloudlayer2, 1, NULL);
	tempInt =hascloudlayer2;
	sprintf(tempVar, "%i", tempInt);
	SetWindowText(hwndEdit,tempVar);

	////////////////Scrolldirection1&2//////////////////////////////

	////////////////flaredirection//////////////////////////////
	//flaredirection x
	hwndEdit = GetDlgItem(hwnd, IDC_FLAREDIRECTION_X);
	ReadProcessMemory(hand, (void*)+(int)mem_adr[15] , &tempFloat, sizeof(float), NULL);
	flaredirection[0]=tempFloat;
	sprintf(tempVar, "%f", tempFloat);
	SetWindowText(hwndEdit,tempVar);
	hwndEdit = GetDlgItem(hwnd, IDC_FLAREDIRECTION_X2);
	SetWindowText(hwndEdit,tempVar);
	//flaredirection y
	hwndEdit = GetDlgItem(hwnd, IDC_FLAREDIRECTION_Y);
	ReadProcessMemory(hand, (void*)+(int)mem_adr[16] , &tempFloat, sizeof(float), NULL);
	flaredirection[1]=tempFloat;
	sprintf(tempVar, "%f", tempFloat);
	SetWindowText(hwndEdit,tempVar);
	hwndEdit = GetDlgItem(hwnd, IDC_FLAREDIRECTION_Y2);
	SetWindowText(hwndEdit,tempVar);
	//flaredirection Z
	hwndEdit = GetDlgItem(hwnd, IDC_FLAREDIRECTION_Z);
	ReadProcessMemory(hand, (void*)+(int)mem_adr[17] , &tempFloat, sizeof(float), NULL);
	flaredirection[2]=tempFloat;
	sprintf(tempVar, "%f", tempFloat);
	SetWindowText(hwndEdit,tempVar);
	hwndEdit = GetDlgItem(hwnd, IDC_FLAREDIRECTION_Z2);
	SetWindowText(hwndEdit,tempVar);
	
	////////////////flaredirection//////////////////////////////



	////////////////Demoposition//////////////////////////////
	ReadProcessMemory(hand, (void*)+(int)mem_adr[32] , &demotimev, 4, &bytes);
	demojumpv = demotimev;
	hwndEdit = GetDlgItem(hwnd, IDC_DEMOJUMPER_VALUE); 
	sprintf(tempVar, "%i", demojumpv);
	SetWindowText(hwndEdit, tempVar);
	////////////////Demoposition//////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*	////////////////Demoposition//////////////////////////////
	//demoposition
	hwndEdit = GetDlgItem(hwnd, IDC_DEMOJUMPER_VALUE);
	ReadProcessMemory(hand, (void*)+(int)mem_adr[32] , &tempInt, sizeof(int), NULL);
	sprintf(tempVar, "%i", tempInt);
	SetWindowText(hwndEdit,tempVar);
	







*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	



	

	
	/*
	//sundirection x 
	hwndEdit = GetDlgItem(hwnd, IDC_SUNDIRECTION_X);
	ReadProcessMemory(hand, (void*)+(int)adressen[0] , &tempFloat, sizeof(float), NULL);
	sundirection[0]=tempFloat;
	sprintf(tempVar, "%i", msthavoc);
	SetWindowText(hwndEdit,tempVar);
	*/


}
void initDialog(HWND hwnd)
{
	//ADRESSEN Setzen

	//adressen auslesen über eingabefelder
	//hwndEdit = GetDlgItem(hwnd, IDC_ADR_SUNDIRECT); 
	//GetWindowText(hwndEdit, tempVar, 50);
	//hwndEdit2 = GetDlgItem(hwnd, IDC_ADR_DOMEPOS); 
	//GetWindowText(hwndEdit2, tempVar2, 50);
	//adresse 1 sundirection
	//fogPtr = BF2142+0x62B79C;
	//ReadProcessMemory(hand, (void*)+(int)fogPtr , &fogPtr, sizeof(int), NULL);
	//fogPtr = (int)fogPtr + (int)0x74;
	//ReadProcessMemory(hand, (void*)+(int)fogPtr , &fogPtr, sizeof(int), NULL);
	//fogPtr = (int)fogPtr + (int)0x30;
	//ReadProcessMemory(hand, (void*)+(int)fogPtr , &fogPtr, sizeof(int), NULL);
	//fogPtr = (int)fogPtr + (int)0x8;
	//ReadProcessMemory(hand, (void*)+(int)fogPtr , &fogPtr, sizeof(int), NULL);
	//fogPtr = (int)fogPtr + (int)0xC;
	//ReadProcessMemory(hand, (void*)+(int)fogPtr , &fogPtr, sizeof(int), NULL);
	//fogPtr = (int)fogPtr + (int)0x164;
	//adressen[0] = (int)fogPtr ;




	//adresse RendDX9
	//msthavoc = RendDX9;
	//ReadProcessMemory(hand, (void*)+(int)RendDX9, &fogPtr, sizeof(int), NULL);
	//fogPtr = (int) fogPtr + 0xC;
	//ReadProcessMemory(hand, (void*)+(int)fogPtr, &fogPtr, sizeof(int), NULL);
	//fogPtr = (int) fogPtr;


	//adresse 2 domepos1
	//fogPtr = (int)fogPtr+0x22496C;
	//ReadProcessMemory(hand, (void*)+(int)fogPtr , &fogPtr, sizeof(int), NULL);
	//fogPtr = (int)fogPtr + (int)0x238;
	//ReadProcessMemory(hand, (void*)+(int)fogPtr , &fogPtr, sizeof(int), NULL);
	//fogPtr = (int)fogPtr + (int)0xC;
	//adressen[1] = (int)fogPtr ;
	initAdress(hwnd);
	Set_Menu(hwnd);
	Read_Values(hwnd);



	
	//adressen[1] = (int)fogPtr ;


	


	/*if (tempVar[0]!='\0' && tempVar2[0]!='\0'){ //wenn nicht leer
		Set_Menu(hwnd);
		//umwandlung des char in int und ablegen im adressen array
		tempDouble=strtol(tempVar,NULL,16);		
		adressen[0] = (int)tempDouble;
		
		tempDouble=strtol(tempVar2,NULL,16);		
		adressen[1] = (int)tempDouble;
		
		
		
		Read_Values(hwnd);

	}*/
}




void animate(HWND hwnd)
{
	hwndEdit = GetDlgItem(hwnd, IDC_TIME);
	hwndEdit2 = GetDlgItem(hwnd, IDC_FPS);
	
	GetWindowText(hwndEdit, tempVar, 50);
	fps[0]=atoi(tempVar);
	GetWindowText(hwndEdit2, tempVar, 50);
	fps[1]=atoi(tempVar);




	if (fps[0]>0 && fps[1]>0){
		fps[2]=fps[0]*fps[1];

		//auslesen anfangswerte

		hwndEdit = GetDlgItem(hwnd, IDC_SUNDIRECTION_X); 
		GetWindowText(hwndEdit, tempVar, 50);
		tempFloat = atof(tempVar);
		vl_sundirectionx = neueVerketteteListe(tempFloat);
		hwndEdit = GetDlgItem(hwnd, IDC_SUNDIRECTION_Y); 
		GetWindowText(hwndEdit, tempVar, 50);
		tempFloat = atof(tempVar);
		vl_sundirectiony = neueVerketteteListe(tempFloat);
		hwndEdit = GetDlgItem(hwnd, IDC_SUNDIRECTION_Z); 
		GetWindowText(hwndEdit, tempVar, 50);
		tempFloat = atof(tempVar);
		vl_sundirectionz = neueVerketteteListe(tempFloat);

		hwndEdit = GetDlgItem(hwnd, IDC_FLAREDIRECTION_X); 
		GetWindowText(hwndEdit, tempVar, 50);
		tempFloat = atof(tempVar);
		vl_flaredirectionx = neueVerketteteListe(tempFloat);
		hwndEdit = GetDlgItem(hwnd, IDC_FLAREDIRECTION_Y); 
		GetWindowText(hwndEdit, tempVar, 50);
		tempFloat = atof(tempVar);
		vl_flaredirectiony = neueVerketteteListe(tempFloat);
		hwndEdit = GetDlgItem(hwnd, IDC_FLAREDIRECTION_Z); 
		GetWindowText(hwndEdit, tempVar, 50);
		tempFloat = atof(tempVar);
		vl_flaredirectionz = neueVerketteteListe(tempFloat);

		hwndEdit = GetDlgItem(hwnd, IDC_DOMEPOS_X); 
		GetWindowText(hwndEdit, tempVar, 50);
		tempFloat = atof(tempVar);
		vl_domepositionx = neueVerketteteListe(tempFloat);
		hwndEdit = GetDlgItem(hwnd, IDC_DOMEPOS_Y); 
		GetWindowText(hwndEdit, tempVar, 50);
		tempFloat = atof(tempVar);
		vl_domepositiony = neueVerketteteListe(tempFloat);
		hwndEdit = GetDlgItem(hwnd, IDC_DOMEPOS_Z); 
		GetWindowText(hwndEdit, tempVar, 50);
		tempFloat = atof(tempVar);
		vl_domepositionz = neueVerketteteListe(tempFloat);
		hwndEdit = GetDlgItem(hwnd, IDC_DOMEROT_H); 
		GetWindowText(hwndEdit, tempVar, 50);
		tempFloat = atof(tempVar);
		vl_domerotation = neueVerketteteListe(tempFloat);

		hwndEdit = GetDlgItem(hwnd, IDC_SCROLLD1_X); 
		GetWindowText(hwndEdit, tempVar, 50);
		tempFloat = atof(tempVar);
		vl_scrolldirect1x = neueVerketteteListe(tempFloat);
		hwndEdit = GetDlgItem(hwnd, IDC_SCROLLD1_Y); 
		GetWindowText(hwndEdit, tempVar, 50);
		tempFloat = atof(tempVar);
		vl_scrolldirect1y = neueVerketteteListe(tempFloat);

		hwndEdit = GetDlgItem(hwnd, IDC_SCROLLD2_X); 
		GetWindowText(hwndEdit, tempVar, 50);
		tempFloat = atof(tempVar);
		vl_scrolldirect2x = neueVerketteteListe(tempFloat);
		hwndEdit = GetDlgItem(hwnd, IDC_SCROLLD2_Y); 
		GetWindowText(hwndEdit, tempVar, 50);
		tempFloat = atof(tempVar);
		vl_scrolldirect2y = neueVerketteteListe(tempFloat);

		hwndEdit = GetDlgItem(hwnd, IDC_SUNDIRECTION_X2); 
		GetWindowText(hwndEdit, tempVar, 50);
		tempFloat = atof(tempVar);
		targ_sundirection[0]=tempFloat;
		hwndEdit = GetDlgItem(hwnd, IDC_SUNDIRECTION_Y2); 
		GetWindowText(hwndEdit, tempVar, 50);
		tempFloat = atof(tempVar);
		targ_sundirection[1]=tempFloat;
		hwndEdit = GetDlgItem(hwnd, IDC_SUNDIRECTION_Z2); 
		GetWindowText(hwndEdit, tempVar, 50);
		tempFloat = atof(tempVar);
		targ_sundirection[2]=tempFloat;



		hwndEdit = GetDlgItem(hwnd, IDC_DOMEPOS_X2); 
		GetWindowText(hwndEdit, tempVar, 50);
		tempFloat = atof(tempVar);
		targ_dome[0]=tempFloat;
		hwndEdit = GetDlgItem(hwnd, IDC_DOMEPOS_Y2); 
		GetWindowText(hwndEdit, tempVar, 50);
		tempFloat = atof(tempVar);
		targ_dome[1]=tempFloat;
		hwndEdit = GetDlgItem(hwnd, IDC_DOMEPOS_Z2); 
		GetWindowText(hwndEdit, tempVar, 50);
		tempFloat = atof(tempVar);
		targ_dome[2]=tempFloat;
		hwndEdit = GetDlgItem(hwnd, IDC_DOMEROT_H2); 
		GetWindowText(hwndEdit, tempVar, 50);
		tempFloat = atof(tempVar);
		targ_dome[3]=tempFloat;
		
		hwndEdit = GetDlgItem(hwnd, IDC_SCROLLD1_X2); 
		GetWindowText(hwndEdit, tempVar, 50);
		tempFloat = atof(tempVar);
		targ_sc[0]=tempFloat;
		hwndEdit = GetDlgItem(hwnd, IDC_SCROLLD1_Y2); 
		GetWindowText(hwndEdit, tempVar, 50);
		tempFloat = atof(tempVar);
		targ_sc[1]=tempFloat;
		hwndEdit = GetDlgItem(hwnd, IDC_SCROLLD2_X2); 
		GetWindowText(hwndEdit, tempVar, 50);
		tempFloat = atof(tempVar);
		targ_sc[2]=tempFloat;
		hwndEdit = GetDlgItem(hwnd, IDC_SCROLLD2_Y2); 
		GetWindowText(hwndEdit, tempVar, 50);
		tempFloat = atof(tempVar);
		targ_sc[3]=tempFloat;
		


		tempFloat=(targ_sundirection[0]-vl_sundirectionx->wert)/fps[2];
		for (tempInt=0;tempInt<fps[2];tempInt++){
			e=lElement(vl_sundirectionx);
			tempFloat2=e->wert+tempFloat;
			einfuegen_hinten(vl_sundirectionx, tempFloat2);

		}
		for (tempInt=0;tempInt<fps[2];tempInt++){
			e=lElement(vl_flaredirectionx);
			tempFloat2=e->wert+tempFloat;
			einfuegen_hinten(vl_flaredirectionx, tempFloat2);

		}
		




		tempFloat=(targ_sundirection[1]-vl_sundirectiony->wert)/fps[2];
		for (tempInt=0;tempInt<fps[2];tempInt++){
			e=lElement(vl_sundirectiony);
			tempFloat2=e->wert+tempFloat;
			einfuegen_hinten(vl_sundirectiony, tempFloat2);

		}
		for (tempInt=0;tempInt<fps[2];tempInt++){
			e=lElement(vl_flaredirectiony);
			tempFloat2=e->wert+tempFloat;
			einfuegen_hinten(vl_flaredirectiony, tempFloat2);

		}


		tempFloat=(targ_sundirection[2]-vl_sundirectionz->wert)/fps[2];
		for (tempInt=0;tempInt<fps[2];tempInt++){
			e=lElement(vl_sundirectionz);
			tempFloat2=e->wert+tempFloat;
			einfuegen_hinten(vl_sundirectionz, tempFloat2);
		}
		for (tempInt=0;tempInt<fps[2];tempInt++){
			e=lElement(vl_flaredirectionz);
			tempFloat2=e->wert+tempFloat;
			einfuegen_hinten(vl_flaredirectionz, tempFloat2);

		}


		tempFloat=(targ_dome[0]-vl_domepositionx->wert)/fps[2];
		for (tempInt=0;tempInt<fps[2];tempInt++){
			e=lElement(vl_domepositionx);
			tempFloat2=e->wert+tempFloat;
			einfuegen_hinten(vl_domepositionx, tempFloat2);

		}
		tempFloat=(targ_dome[1]-vl_domepositiony->wert)/fps[2];
		for (tempInt=0;tempInt<fps[2];tempInt++){
			e=lElement(vl_domepositiony);
			tempFloat2=e->wert+tempFloat;
			einfuegen_hinten(vl_domepositiony, tempFloat2);

		}
		tempFloat=(targ_dome[2]-vl_domepositionz->wert)/fps[2];
		for (tempInt=0;tempInt<fps[2];tempInt++){
			e=lElement(vl_domepositionz);
			tempFloat2=e->wert+tempFloat;
			einfuegen_hinten(vl_domepositionz, tempFloat2);

		}
		tempFloat=(targ_dome[3]-vl_domerotation->wert)/fps[2];
		for (tempInt=0;tempInt<fps[2];tempInt++){
			e=lElement(vl_domerotation);
			tempFloat2=e->wert+tempFloat;
			einfuegen_hinten(vl_domerotation, tempFloat2);

		}
		

		tempFloat=(targ_sc[0]-vl_scrolldirect1x->wert)/fps[2];
		for (tempInt=0;tempInt<fps[2];tempInt++){
			e=lElement(vl_scrolldirect1x);
			tempFloat2=e->wert+tempFloat;
			einfuegen_hinten(vl_scrolldirect1x, tempFloat2);

		}
		tempFloat=(targ_sc[1]-vl_scrolldirect1y->wert)/fps[2];
		for (tempInt=0;tempInt<fps[2];tempInt++){
			e=lElement(vl_scrolldirect1y);
			tempFloat2=e->wert+tempFloat;
			einfuegen_hinten(vl_scrolldirect1y, tempFloat2);

		}
		tempFloat=(targ_sc[2]-vl_scrolldirect2x->wert)/fps[2];
		for (tempInt=0;tempInt<fps[2];tempInt++){
			e=lElement(vl_scrolldirect2x);
			tempFloat2=e->wert+tempFloat;
			einfuegen_hinten(vl_scrolldirect2x, tempFloat2);

		}
		tempFloat=(targ_sc[3]-vl_scrolldirect2y->wert)/fps[2];
		for (tempInt=0;tempInt<fps[2];tempInt++){
			e=lElement(vl_scrolldirect2y);
			tempFloat2=e->wert+tempFloat;
			einfuegen_hinten(vl_scrolldirect2y, tempFloat2);

		}



	

	animated=1;	
	hwndEdit = GetDlgItem(hwnd, IDC_ANIMATION_STOP); 
	EnableWindow(hwndEdit, TRUE);

	}
		//fps[1]*fps[2]=fps[3]

	
	//initDialog(hwnd);
}





void setDialog(HWND hwnd)
{

	///SUNDIRECTION/////////////////////////////////////////////
	hwndEdit = GetDlgItem(hwnd, IDC_SUNDIRECTION_X); 
	GetWindowText(hwndEdit, tempVar, 50);
	tempFloat = atof(tempVar);
	sundirection[0]=tempFloat;
	WriteProcessMemory(hand, (void*)+(int)mem_adr[2], &sundirection[0], sizeof(float), NULL);

	hwndEdit = GetDlgItem(hwnd, IDC_SUNDIRECTION_Y); 
	GetWindowText(hwndEdit, tempVar, 50);
	tempFloat = atof(tempVar);
	sundirection[1]=tempFloat;
	WriteProcessMemory(hand, (void*)+(int)mem_adr[3] , &sundirection[1], sizeof(float), NULL);

	hwndEdit = GetDlgItem(hwnd, IDC_SUNDIRECTION_Z); 
	GetWindowText(hwndEdit, tempVar, 50);
	tempFloat = atof(tempVar);
	sundirection[2]=tempFloat;
	WriteProcessMemory(hand, (void*)+(int)mem_adr[4] , &sundirection[2], sizeof(float), NULL);
	///SUNDIRECTION/////////////////////////////////////////////

	///DOMEPOSITION/ROTATION/////////////////////////////////////////////
	hwndEdit = GetDlgItem(hwnd, IDC_DOMEPOS_X); 
	GetWindowText(hwndEdit, tempVar, 50);
	tempFloat = atof(tempVar);
	domeposition[0]=tempFloat;
	WriteProcessMemory(hand, (void*)+(int)mem_adr[5] , &domeposition[0], sizeof(float), NULL);

	hwndEdit = GetDlgItem(hwnd, IDC_DOMEPOS_Y); 
	GetWindowText(hwndEdit, tempVar, 50);
	tempFloat = atof(tempVar);
	domeposition[1]=tempFloat;
	WriteProcessMemory(hand, (void*)+(int)mem_adr[6] , &domeposition[1], sizeof(float), NULL);

	hwndEdit = GetDlgItem(hwnd, IDC_DOMEPOS_Z); 
	GetWindowText(hwndEdit, tempVar, 50);
	tempFloat = atof(tempVar);
	domeposition[2]=tempFloat;
	WriteProcessMemory(hand, (void*)+(int)mem_adr[7] , &domeposition[2], sizeof(float), NULL);

	hwndEdit = GetDlgItem(hwnd, IDC_DOMEROT_H); 
	GetWindowText(hwndEdit, tempVar, 50);
	tempFloat = atof(tempVar);
	domeposition[3]=tempFloat;
	WriteProcessMemory(hand, (void*)+(int)mem_adr[8] , &domeposition[3], sizeof(float), NULL);
	///DOMEPOSITION/ROTATION/////////////////////////////////////////////


	///SCROLLDIRECTION/////////////////////////////////////////////
	hwndEdit = GetDlgItem(hwnd, IDC_SCROLLD1_X); 
	GetWindowText(hwndEdit, tempVar, 50);
	tempFloat = atof(tempVar);
	scrolldirection[0]=tempFloat;
	WriteProcessMemory(hand, (void*)+(int)mem_adr[9] , &scrolldirection[0], sizeof(float), NULL);

	hwndEdit = GetDlgItem(hwnd, IDC_SCROLLD1_Y); 
	GetWindowText(hwndEdit, tempVar, 50);
	tempFloat = atof(tempVar);
	scrolldirection[1]=tempFloat;
	WriteProcessMemory(hand, (void*)+(int)mem_adr[10], &scrolldirection[1], sizeof(float), NULL);

	hwndEdit = GetDlgItem(hwnd, IDC_SCROLLD2_X); 
	GetWindowText(hwndEdit, tempVar, 50);
	tempFloat = atof(tempVar);
	scrolldirection[2]=tempFloat;
	WriteProcessMemory(hand, (void*)+(int)mem_adr[11] , &scrolldirection[2], sizeof(float), NULL);

	hwndEdit = GetDlgItem(hwnd, IDC_SCROLLD2_Y); 
	GetWindowText(hwndEdit, tempVar, 50);
	tempFloat = atof(tempVar);
	scrolldirection[3]=tempFloat;
	WriteProcessMemory(hand, (void*)+(int)mem_adr[12] , &scrolldirection[3], sizeof(float), NULL);

	///SCROLLDIRECTION/////////////////////////////////////////////



	///FLAREDIRECTION/////////////////////////////////////////////
	hwndEdit = GetDlgItem(hwnd, IDC_FLAREDIRECTION_X); 
	GetWindowText(hwndEdit, tempVar, 50);
	tempFloat = atof(tempVar);
	flaredirection[0]=tempFloat;
	WriteProcessMemory(hand, (void*)+(int)mem_adr[15] , &flaredirection[0], sizeof(float), NULL);

	hwndEdit = GetDlgItem(hwnd, IDC_FLAREDIRECTION_Y); 
	GetWindowText(hwndEdit, tempVar, 50);
	tempFloat = atof(tempVar);
	flaredirection[1]=tempFloat;
	WriteProcessMemory(hand, (void*)+(int)mem_adr[16] , &flaredirection[1], sizeof(float), NULL);

	hwndEdit = GetDlgItem(hwnd, IDC_FLAREDIRECTION_Z); 
	GetWindowText(hwndEdit, tempVar, 50);
	tempFloat = atof(tempVar);
	flaredirection[2]=tempFloat;
	WriteProcessMemory(hand, (void*)+(int)mem_adr[17] , &flaredirection[2], sizeof(float), NULL);

	///FLAREDIRECTION/////////////////////////////////////////////
	hwndEdit = GetDlgItem(hwnd, IDC_SCROLLD1_LAYER); 
	GetWindowText(hwndEdit, tempVar, 50);
	for (tempInt=1; tempInt<50; tempInt++){
		tempVar[tempInt]=' ';
	}
	tempFloat=atof(tempVar);
	if (0<(int)tempFloat){
		tempByte=(int)1;
	}
	else{
		tempByte=(int)0;
	}
	 
	hascloudlayer=tempByte;
	WriteProcessMemory(hand, (void*)+(int)mem_adr[13] , &hascloudlayer, 1, NULL);


	hwndEdit = GetDlgItem(hwnd, IDC_SCROLLD2_LAYER); 
	GetWindowText(hwndEdit, tempVar, 50);
	for (tempInt=1; tempInt<50; tempInt++){
		tempVar[tempInt]=' ';
	}
	tempFloat=atof(tempVar);
	if (0<(int)tempFloat){
		tempByte=(int)1;
	}
	else{
		tempByte=(int)0;
	}
	hascloudlayer2=tempByte;
	WriteProcessMemory(hand, (void*)+(int)mem_adr[14] , &hascloudlayer2, 1, NULL);




	
	demojump(hwnd);
	//update Dialog
	initDialog(hwnd);
}






void Initialize(HWND hwnd,WPARAM wParam, LPARAM lParam) {
	
	
	GetProcessList();
	
	FirstTime1=TRUE; //This is the true / false flag for "is this the first time the trainers read the game code

	IsHack1On=FALSE; // used in our example hack function below


	GetProcessList();
	
	
	if(GameRunning==TRUE)
	{		 
         GetWindowThreadProcessId(hwndWindow, &pid);
		 hand = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION,0,pid);
		 SetTimer(hwnd, 1, 10, NULL);	//Timer speed is 500ms, you can change it here
		// sprintf(tempVar, "%d", pid);
		 

		 Initialize_Menu(hwnd);
		 initDialog(hwnd);
		//initAdress(hwnd);

		
		 ReadProcessMemory(hand, (void*)+(int)mem_adr[1] , &timing, 1, &bytes);
		 timing2 = timing;
		 

		


		 


//		 WriteProcessMemory(hand, (void*)0x0a09db66, &original_code,4, &bytes); // Write the original code into memory
		 
	} 
	else 
	{ //Error message for when game not found in process list

		MessageBox(NULL, "BF2142 not detected, please run the game before running the trainer", "Error", MB_OK + MB_ICONWARNING);
	}


	GetProcessList();
	
	
	
	
	//initDialog(hwnd);
}

void HookExe() //This function ensures we are attatched to the game at all times
{
	
	CloseHandle(hand);
    GetProcessList( );
    GetWindowThreadProcessId(hwndWindow, &pid);
	hand = OpenProcess(PROCESS_ALL_ACCESS,0,pid);


}

	/*----- Here comes the good stuff -----*/


void timerCall(HWND hwnd) //functions in here run according to timer above
{	
	
	
	HookExe(); //Call to function above (game always attatched)


/////////////////////////////////////////////////////////////////////////
/////ReadProcMem arrays are used to read and store original code so we 
/////toggle the code on and off

	
	//IDC_TIMER

	hwndTimer = GetDlgItem(hwnd, IDC_TIMER);
	ReadProcessMemory(hand, (void*)+(int)mem_adr[1] , &timing, 1, &bytes);
	if (timing!=timing2){
		timing2 = timing;
		tempInt = timing;
		sprintf(tempVar, "%i", tempInt);
		SetWindowText(hwndTimer,tempVar);

		hwndDemoTime = GetDlgItem(hwnd, IDC_DEMOJUMPER_VALUE2);
		ReadProcessMemory(hand, (void*)+(int)mem_adr[32] , &demotimev, 4, &bytes);
		sprintf(tempVar, "%i", demotimev);


		if (demojumpv < demotimev){
			demojumpv = demotimev;
		}

		SetWindowText(hwndDemoTime,tempVar);
		

		//
	
		if (animated==1){

			testzaehler++;


			//ReadProcessMemory(hand, (void*)+(int)adressen[0] , &tempFloat, sizeof(float), NULL);
			//sundirection[0]=tempFloat;
			//sundirection[0]=sundirection[0]+0.001f;
			tempFloat=vl_sundirectionx->wert;
			WriteProcessMemory(hand, (void*)+(int)mem_adr[2] , &tempFloat, sizeof(float), NULL);

			tempFloat=vl_sundirectiony->wert;
			WriteProcessMemory(hand, (void*)+(int)mem_adr[3] , &tempFloat, sizeof(float), NULL);
			tempFloat=vl_sundirectionz->wert;
			WriteProcessMemory(hand, (void*)+(int)mem_adr[4] , &tempFloat, sizeof(float), NULL);
			
			tempFloat=vl_flaredirectionx->wert;
			WriteProcessMemory(hand, (void*)+(int)mem_adr[15] , &tempFloat, sizeof(float), NULL);
			tempFloat=vl_flaredirectiony->wert;
			WriteProcessMemory(hand, (void*)+(int)mem_adr[16] , &tempFloat, sizeof(float), NULL);
			tempFloat=vl_flaredirectionz->wert;
			WriteProcessMemory(hand, (void*)+(int)mem_adr[17] , &tempFloat, sizeof(float), NULL);
			
			tempFloat=vl_domepositionx->wert;
			WriteProcessMemory(hand, (void*)+(int)mem_adr[5] , &tempFloat, sizeof(float), NULL);

			tempFloat=vl_domepositiony->wert;
			WriteProcessMemory(hand, (void*)+(int)mem_adr[6] , &tempFloat, sizeof(float), NULL);

			tempFloat=vl_domepositionz->wert;
			WriteProcessMemory(hand, (void*)+(int)mem_adr[7] , &tempFloat, sizeof(float), NULL);

			tempFloat=vl_domerotation->wert;
			WriteProcessMemory(hand, (void*)+(int)mem_adr[8] , &tempFloat, sizeof(float), NULL);


			
			tempFloat=vl_scrolldirect1x->wert;
			WriteProcessMemory(hand, (void*)+(int)mem_adr[9] , &tempFloat, sizeof(float), NULL);

			tempFloat=vl_scrolldirect1y->wert;
			WriteProcessMemory(hand, (void*)+(int)mem_adr[10] , &tempFloat, sizeof(float), NULL);

			tempFloat=vl_scrolldirect2x->wert;
			WriteProcessMemory(hand, (void*)+(int)mem_adr[11] , &tempFloat, sizeof(float), NULL);

			tempFloat=vl_scrolldirect2y->wert;
			WriteProcessMemory(hand, (void*)+(int)mem_adr[12] , &tempFloat, sizeof(float), NULL);

			vl_sundirectionx = vl_sundirectionx->naechstes;
			vl_sundirectiony = vl_sundirectiony->naechstes;
			vl_sundirectionz = vl_sundirectionz->naechstes;

			vl_flaredirectionx = vl_flaredirectionx->naechstes;
			vl_flaredirectiony = vl_flaredirectiony->naechstes;
			vl_flaredirectionz = vl_flaredirectionz->naechstes;
	
			vl_domepositionx = vl_domepositionx->naechstes;
			vl_domepositiony = vl_domepositiony->naechstes;
			vl_domepositionz = vl_domepositionz->naechstes;
			vl_domerotation = vl_domerotation->naechstes;

			vl_scrolldirect1x = vl_scrolldirect1x->naechstes;
			vl_scrolldirect1y = vl_scrolldirect1y->naechstes;
			vl_scrolldirect2x = vl_scrolldirect2x->naechstes;
			vl_scrolldirect2y = vl_scrolldirect2y->naechstes;


			if (testzaehler>fps[2]){
				animated=0;
				testzaehler=0;
				hwndEdit = GetDlgItem(hwnd, IDC_ANIMATION_STOP); 
				EnableWindow(hwndEdit, FALSE);
		}
			initDialog(hwnd);

		}
	

	}



	
	
	



	if(FirstTime1==TRUE) //checks to see if this is the first time its run, if it is continue
	{
		
		ReadProcessMemory(hand, (void*) 0x123456 , &original_code, 2, &bytes); // reads the bytes at address 12346 and stores them


		
																				// in our variable

		FirstTime1=FALSE;
	}
	// What we are doing here is reading 2 bytes of the games code and storing them in a variable called "original_code"
	//	The number in sqaure brackets is the number of bytes, this has to match the number after our variable
	// "original_code" in the ReadProcessMemory line.

	// You can add more addresses in, just be sure to have unique varible names and specify the right number of bytes.


///////////////////////////////////////////////////////////////////////////
/////Start Hotkey Functions Below

		/* --Example Function----------------------------------------- */

	if(GetAsyncKeyState(VK_NUMPAD1)) // User Pressed the NumPad1 to switch on code
	{			
		if(IsHack1On==FALSE) //if this hack is not on do this........
			{  
				/*
				WriteProcessMemory(hand, (void*)+skyposition, &skyx_s,4, &bytes); // Write the original code into memory
				WriteProcessMemory(hand, (void*)+(skyposition+(int)4), &skyy_s,4, &bytes); // Write the original code into memory
				WriteProcessMemory(hand, (void*)+(skyposition+(int)8), &skyz_s,4, &bytes); // Write the original code into memory
				skyx_s=skyx_s-steps;
				*/
				IsHack1On=TRUE; //Sets our "Is On" flag to "on"
			}
			
			{
				/*
				WriteProcessMemory(hand, (void*)+skyposition, &skyx_s,4, &bytes); // Write the original code into memory
				WriteProcessMemory(hand, (void*)+(skyposition+(int)4), &skyy_s,4, &bytes); // Write the original code into memory
				WriteProcessMemory(hand, (void*)+(skyposition+(int)8), &skyz_s,4, &bytes); // Write the original code into memory
				
				skyx_s=skyx_s-steps;

				*/
				
				IsHack1On=FALSE; //Sets our "Is On" flag to "off"
			}
	

		}
	//The function above will toggle between hack on and hack off status. For a list of virtual keys please visit:
	// http://msdn.microsoft.com/library/default.asp?url=/library/en-us/winui/winui/WindowsUserInterface/UserInput/VirtualKeyCodes.asp
     		
	/* --Example Function --END------------------------------------ */



	/** End **/
}


BOOL GetProcessList( )
{
  HANDLE hProcessSnap;
  HANDLE hProcess;
  PROCESSENTRY32 pe32;
  DWORD dwPriorityClass;
  int PidTest;
  GameRunning=FALSE;
 
  
  // Take a snapshot of all processes in the system.
  hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
  if( hProcessSnap == INVALID_HANDLE_VALUE ) return( FALSE );
  

  // Set the size of the structure before using it.
  pe32.dwSize = sizeof( PROCESSENTRY32 );

  // Retrieve information about the first process,
  // and exit if unsuccessful
  if( !Process32First( hProcessSnap, &pe32 ) )
  {
    CloseHandle( hProcessSnap );     // Must clean up the snapshot object!
    return( FALSE );
  }

  // Now walk the snapshot of processes, and
  // display information about each process in turn
  
  do
  {
    // Retrieve the priority class.
    dwPriorityClass = 0;
    hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID );
    if( hProcess != NULL )
    {
      dwPriorityClass = GetPriorityClass( hProcess );
      if( !dwPriorityClass )
        
      CloseHandle( hProcess );
    }

    PidTest=strcmp(gameWindow, pe32.szExeFile);
	if(PidTest==0){ pid=pe32.th32ProcessID; GameRunning=TRUE;}

  } while( Process32Next( hProcessSnap, &pe32 ) );

  // Don't forget to clean up the snapshot object!
  CloseHandle( hProcessSnap );
  return( TRUE );
}

BOOL CALLBACK DialogProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    switch (message)
	{
		case WM_INITDIALOG:
			Initialize(hwnd,wParam,lParam);

//			MessageBox(NULL, "TRAINER LOADED", "Error", MB_OK + MB_ICONWARNING);

			return TRUE;

		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case IDC_ABOUT:
					aboutButton(hwnd);
					return TRUE;

				case IDC_EXIT:
					EndDialog (hwnd, 0);
					return TRUE;
				case IDC_SET:
					setDialog(hwnd);
					return TRUE;

				case IDC_HKL:
						hkl(hwnd);
					return TRUE;

				case IDC_DEMOFIX:
					GetProcessList();
					if(GameRunning==TRUE)
						{
							fixdemo(hwnd);
						}
					return TRUE;

				case IDC_UNLOCK:
					GetProcessList();
					if(GameRunning==TRUE)
						{
							unlockconsole(hwnd);
						}
					return TRUE;

				case IDC_DEMOSKIP:
					GetProcessList();
					if(GameRunning==TRUE)
						{
							demoskip(hwnd);
						}
					return TRUE;

				case IDC_DEMOJUMPER:
					GetProcessList();
					if(GameRunning==TRUE)
						{
							demojump(hwnd);
						}
					return TRUE;

				case IDC_INIT:
					//initDialog(hwnd);
					GetProcessList();
					if(GameRunning==TRUE)
						{
							Initialize(hwnd,wParam,lParam);
						} 
						else 
						{ //Error message for when game not found in process list

							MessageBox(NULL, "BF2142 not detected, please run the game before running the trainer", "Error", MB_OK + MB_ICONWARNING);
						}
					return TRUE;
				case IDC_ANIMATION_START:
					animate(hwnd);
					return TRUE;
				case IDC_ANIMATION_STOP:
					animated=0;
					testzaehler=0;
					hwndEdit = GetDlgItem(hwnd, IDC_ANIMATION_STOP); 
					EnableWindow(hwndEdit, FALSE);
					return TRUE;
					
			}

		return TRUE;

		case WM_DESTROY:
			CloseHandle(pFile);
			PostQuitMessage(0);
			return TRUE;

		case WM_CLOSE:
			PostQuitMessage(0);
			return TRUE;
		case WM_TIMER:
			timerCall(hwnd);
			return TRUE;
    }
    return FALSE;
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{


	DialogBox(hInstance,MAKEINTRESOURCE(IDD_MAINDLG), NULL,DialogProc);
	return 0;
}
























/*do
		{
			switch( char_adress[x] )
				 {case 'a' : printf( "Excellent" );
					case 'b' : printf( "Good" );
					case 'c' : printf( "OK" );
					case 'd' : printf( "Mmmmm-.." );
					case 'e' : printf( "You must do better than this" );    
					case 'f' : printf( "You must do better than this" );    
					default  : printf( "What is your grade anyway?" );
				 }
				MessageBox(hwnd,char_adress,"About",MB_ICONINFORMATION);
				x++;
		}while (char_adress[x]!='\0');*/



			


	
	//MessageBox(hwnd,buf,"About",MB_ICONINFORMATION);




	//skyposition = atoi(char_adress);
	//MessageBox(hwnd,zahl,"About",MB_ICONINFORMATION);

	/*MessageBox(hwnd,char_adress,"About",MB_ICONINFORMATION);
	MessageBox(hwnd,char_skyx_start,"About",MB_ICONINFORMATION);
	MessageBox(hwnd,char_skyx_end,"About",MB_ICONINFORMATION);
	MessageBox(hwnd,char_skyy_start,"About",MB_ICONINFORMATION);
	MessageBox(hwnd,char_skyy_end,"About",MB_ICONINFORMATION);
	MessageBox(hwnd,char_skyz_start,"About",MB_ICONINFORMATION);
	MessageBox(hwnd,char_skyz_end,"About",MB_ICONINFORMATION);
	MessageBox(hwnd,char_timer,"About",MB_ICONINFORMATION);
	MessageBox(hwnd,skyposition,"SKY",MB_ICONINFORMATION);
	*/