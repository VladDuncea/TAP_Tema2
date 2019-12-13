#include <iostream>
#include <fstream>
#include <vector>

using namespace std;


//-x^2 + x + 2
double fitness(double val)
{
	return -(val * val) + val + 2;
}

class Cromozom
{
public:
	vector<bool> data;
	double value,fitness;

	Cromozom(int size)
	{
		data.resize(size);
		for (int i = 0; i < size; i++)
		{
			data[i] = rand() % 2;
		}
		value = fitness = 0;
	}

	void compute(int min,int precision,double fitness(double))
	{
		//compute value
		for (int i = 0; i < data.size(); i++)
		{
			if (data[i])
				value += pow(2, i);
		}
		value /= pow(10, precision);
		value += min;

		//compute fitness
		this->fitness = fitness(value);
	}
};

void print_cromozomes(vector<Cromozom>& cromozomes)
{
	for(int i=0;i<cromozomes.size();i++)
	{
		//print id
		printf("%*d: ",2,i+1);
		//print binary value
		for (bool b : cromozomes[i].data)
			printf("%d", b);
		//print actual value
		printf(" x=%10.6lf", cromozomes[i].value);
		//print fitness
		printf(" f= %lf\n", cromozomes[i].fitness);
	}
}

int main()
{
	//input
	int pop_size, min, max, precizion,max_generations;
	double prob_crossover, prob_mutate;

	//read input data
	ifstream fin("data.in");
	fin >> pop_size >> min >> max>>precizion;
	fin >> prob_crossover >> prob_mutate >> max_generations;

	//calculate binary positions needed
	int bin_size = log2((max - min) * pow(10, precizion));

	//cromozomes
	vector<Cromozom> cromozomes;
	for (int i = 0;i < pop_size; i++)
	{
		Cromozom cr(bin_size);
		cr.compute(min,precizion,fitness);
		cromozomes.push_back(cr);
	}

	//print initial cromozomes
	print_cromozomes(cromozomes);

	return 0;
}