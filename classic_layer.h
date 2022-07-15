#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

class classic_layer
{
public:
	struct unit {	
		float x;
		float e;
		float e2;
		float *hkk;
		float *hee;
		float *w;
		float bias;
		float biashkk;
		float biashee;
		int membership;
	} *plane;			//neurony
	int size;			//rozmer mapy = 1
	int connections;	//pocet spojeni s predch vrstvou
	int this_planes;		//pocet priznakovych map (neuronov)

	classic_layer(int, int, int);
	~classic_layer(void);
};
