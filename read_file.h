//********************************************************
//read_file.h
//
//Trieda pre uciace(trenovacie) vzorky

//********************************************************
#pragma once
#include <math.h>

class read_file
{
public:
	FILE *f;
	FILE *g;
	long magic_number;
	long patterns;
	int x_size;	
	int y_size;
	struct input {
		unsigned char pixel[150][150]; 
		unsigned char membership; 
	} *pattern;		//pole pixelov a k nemu trieda

	read_file(void);
	~read_file(void);
	bool open_file(CString, CString);
	void read_header(void);
	void read_patterns(void);
};