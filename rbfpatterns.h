#pragma once

class rbfpatterns
{
public:
	FILE *f;
	int size_x;
	int size_y;
	int number_of_pat;

	struct patterns {
		int **pixel;
		int code;
	} *pattern;  
	
	rbfpatterns(void);
	~rbfpatterns(void);
	void print_pattern(int);
	bool open_file(char *);
	void read_patterns(short act_function);
};