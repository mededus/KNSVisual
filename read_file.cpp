//********************************************************
//read_file.cpp
//
//Metody triedy pre uciace(trenovacie) vzorky

//********************************************************
#include "StdAfx.h"
#include "read_file.h"

read_file::read_file(void)
{
	magic_number = 0;
	patterns = 0;
	x_size = 0;
	y_size = 0;	
}

read_file::~read_file(void)
{
	delete[] pattern;
}

bool read_file::open_file(CString test_name, CString membership_name)
{
	if ( test_name != "" && membership_name != "" ) {
		f = fopen(test_name,"rb");
		g = fopen(membership_name,"rb");
		return(true);
	}
	else
		return(false);
}

void read_file::read_header(void)
{
	long temp[4];
	fseek(f,0,0); //magic_number na prvych 4 bajtoch
	for ( int i = 0 ; i < 4 ; i++) temp[i] = fgetc(f);
	magic_number = temp[0]*pow(16,6)+temp[1]*pow(16,4)+temp[2]*pow(16,2)+temp[3];
	//fseek(f,4,0); //pocet vzoriek na druhych 4 bajtoch
	for ( int i = 0 ; i < 4 ; i++) temp[i] = fgetc(f);
	patterns = temp[0]*pow(16,6)+temp[1]*pow(16,4)+temp[2]*pow(16,2)+temp[3];
	//fseek(f,8,0); //rozmer vzorky na tretich 4 bajtoch
	for ( int i = 0 ; i < 4 ; i++) temp[i] = fgetc(f);
	x_size = temp[0]*pow(16,6)+temp[1]*pow(16,4)+temp[2]*pow(16,2)+temp[3];
	//fseek(f,12,0); //rozmer vzorky na stvrtych 4 bajtoch
	for ( int i = 0 ; i < 4 ; i++) temp[i] = fgetc(f);
	y_size = temp[0]*pow(16,6)+temp[1]*pow(16,4)+temp[2]*pow(16,2)+temp[3];
}

void read_file::read_patterns(void)
{
	pattern = new input[patterns];
	fseek(f,16,0);	//nastavenie na prvu obrazovu info v f
	/*
	for (int k = 0 ; k < patterns ; k++) 
		for (int i = 0 ; i < x_size ; i++) 
			for (int j = 0 ; j < y_size ; j++) {
				// mnist cisla
				//pattern[k].pixel[j][i] = fgetc(f);		
				// chromozomy
				pattern[k].pixel[i][j] = fgetc(f);	
			}
	*/
	for (int k = 0 ; k < patterns ; k++) 
		for (int i = 0 ; i < y_size ; i++) 
			for (int j = 0 ; j < x_size ; j++) {
				// mnist cisla
				//pattern[k].pixel[j][i] = fgetc(f);		
				// chromozomy
				pattern[k].pixel[j][i] = fgetc(f);	
			}


	fseek(g,8,0);	//nastavenie na prvu obrazovu info v g
	for (int k = 0 ; k < patterns ; k++) 
		pattern[k].membership = fgetc(g);

	fclose(f);
	fclose(g);
}

