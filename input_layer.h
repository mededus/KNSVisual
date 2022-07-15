#pragma once

class input_layer
{
public:
	float **x;		//pole hodnot neuronov
	int sizeM;		//rozmer M
	int sizeN;		//rozmer N

	input_layer(int, int);
	~input_layer(void);
};
