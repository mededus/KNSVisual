#include "StdAfx.h"
#include "convol_layer.h"

convol_layer::convol_layer(int t11, int t12, int t21, int t22, int t3, int t4, int **temp)
{
	sizeM = t11;		//DVA PARAMETRE pre konvolucnu vrstvu (m*n)
	sizeN = t12;
	receptiveM = t21;	//DVA PARAMETRE pre receptivne polia (m*n)
	receptiveN = t22;
	this_planes = t3; 
	prev_planes = t4; 
	
	connection_field = new short*[this_planes];                
	for (int i = 0 ; i < this_planes ; i++)
		connection_field[i] = new short[prev_planes];
	//tabulka spojeni medzi vrstvami S2 aC3
	for (int i = 0 ; i < this_planes ; i++)
		for (int j = 0 ; j < prev_planes ; j++)
			connection_field[i][j] = temp[i][j];
	
	//nastavenie pociatocnych hodnot vah
	plane = new feature_map[this_planes];
	//inicializacia mapy
	for ( int a = 0 ; a < this_planes ; a++) {
		plane[a].w = new float**[prev_planes];
		plane[a].hkk = new float**[prev_planes];
		plane[a].hee = new float**[prev_planes];
		plane[a].x = new float*[sizeM];
		plane[a].e = new float*[sizeM];
		plane[a].e2 = new float*[sizeM];
		for ( int b = 0 ; b < prev_planes ; b++) {
			plane[a].w[b] = new float*[receptiveM];
			plane[a].hkk[b] = new float*[receptiveM];
			plane[a].hee[b] = new float*[receptiveM];
			for ( int c = 0 ; c < receptiveM ; c++) {
				plane[a].w[b][c] = new float[receptiveN];
				plane[a].hkk[b][c] = new float[receptiveN];
				plane[a].hee[b][c] = new float[receptiveN];
			}
		}	
		for ( int d = 0 ; d < sizeM ; d++) {
			plane[a].x[d] = new float[sizeN];
			plane[a].e[d] = new float[sizeN];
			plane[a].e2[d] = new float[sizeN];
		}
	}
	//nastavenie pociatocnych hodnot vah
	for ( int k = 0 ; k < this_planes ; k++) {
		int con = 0; //kolko planov z S2 je spojenych s k-tym planom podla toho je inicializovany interval vah
		for ( int m = 0 ; m < prev_planes ; m++) {
			if ( temp[k][m] == 1 )
				con++;
		}
		for ( int i = 0 ; i < receptiveM ; i++) {
			for ( int j = 0 ; j < receptiveN ; j++) {
				for (int h = 0 ; h < prev_planes ; h++)	{
						float weight = connection_field[k][h] * ((rand()%(int)(((10000) + 1)-(-10000)))+ (-10000));
						plane[k].w[h][i][j] = weight / (10000 / (2.4 / (receptiveM*receptiveN*con)));
						plane[k].hkk[h][i][j] = 0;
						plane[k].hee[h][i][j] = 0;
					}
			}
		}
	//nastavenie 0 hodnot biasu a vystupov z neuronov
		
		plane[k].bias = 0;
		plane[k].biashkk = 0;
		plane[k].biashee = 0;
		for ( int i = 0 ; i < sizeM ; i++) {
			for ( int j = 0 ; j < sizeN ; j++) {
				plane[k].x[i][j] = 0;
				plane[k].e[i][j] = 0;
				plane[k].e2[i][j] = 0;
			}
		}
	}
}

convol_layer::~convol_layer(void)
{
}
