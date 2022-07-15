#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

class convol_layer
{
public:
	struct feature_map {	
		float **x;
		float **e;
		float **e2;
		float ***hkk;
		float ***hee;
		float ***w;
		float bias;
		float biashkk;
		float biashee;
	} *plane;			//priznakove mapy
	int sizeM;			//rozmer mapy (matica m*n)
	int sizeN;
	int receptiveM;		//rozmer receptivneho pola (matica m*n)
	int receptiveN;
	int this_planes;	//pocet priznakovych map v tejto vrstve 
	int prev_planes;	//pocet priznakovych map v predchadzajucej vrstve
	short **connection_field;	//tabulka spojeni s predchadzajucou vrstvou /* ZMENI SA ZATIAL NEVIEM AKO */

	//sizeM, sizeN, receptiveM, receptiveN, this_planes, prev_planes, conection     
	convol_layer(int, int, int, int, int, int, int**);
	~convol_layer(void);
};
