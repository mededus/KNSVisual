#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

class sampl_layer
{
public:
	struct feature_map {	
		float **x;
		float **e;
		float **e2;
		float hkk;
		float hee;
		float bias;
		float biashkk;
		float biashee;
		float w;
	} *plane;			//priznakove mapy
	int sizeM;			//rozmer mapy (matica m*n)
	int sizeN;
	int receptiveM;		//rozmer receptivneho pola (matica m*n)
	int receptiveN;
	int this_planes;	//pocet priznakovych map

	sampl_layer(int, int, int, int, int);
	~sampl_layer(void);
};
