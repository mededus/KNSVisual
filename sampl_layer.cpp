#include "StdAfx.h"
#include "sampl_layer.h"

sampl_layer::sampl_layer(int t11, int t12, int t21, int t22, int t3)
{
	sizeM = t11;
	sizeN = t12;
	receptiveM = t21;
	receptiveN = t22;
	this_planes = t3;
	//inicializacia mapy
	plane = new feature_map[this_planes];
	for ( int a = 0 ; a < this_planes ; a++) {
		plane[a].x = new float*[sizeM];
		plane[a].e = new float*[sizeM];
		plane[a].e2 = new float*[sizeM];
		for ( int b = 0 ; b < sizeM ; b++) {
			plane[a].x[b] = new float[sizeN];
			plane[a].e[b] = new float[sizeN];
			plane[a].e2[b] = new float[sizeN];
		}
	}
	//nastavenie pociatocnych hodnot vah
	for ( int k = 0 ; k < this_planes ; k++) {
		float weight = ((rand()%(int)(((10000) + 1)-(-10000)))+ (-10000));
		plane[k].w = weight / (10000 / (2.4 / (receptiveM*receptiveN)));
		
	//nastavenie 0 hodnot biasu a vystupov z neuronov
		plane[k].bias = 0;
		plane[k].biashkk = 0;
		plane[k].biashee = 0;
		plane[k].hkk = 0;
		plane[k].hee = 0;
		for ( int i = 0 ; i < sizeM ; i++) {
			for ( int j = 0 ; j < sizeN ; j++) {
				plane[k].x[i][j] = 0;
				plane[k].e[i][j] = 0;
				plane[k].e2[i][j] = 0;
			}
		}
	}
}

sampl_layer::~sampl_layer(void)
{
}
