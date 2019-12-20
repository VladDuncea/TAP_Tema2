#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <ctime>

using namespace std;

int interv_min, interv_max;
double coeficient;

//-x^2 + x + 2
double fitness(double val)
{
	//return -(val * val) + val + 2;
	return 5 * (val * val * val) - 7 * (val * val) - 6 * val + 8;
}

class Cromozom
{
public:
	vector<bool> data;
	double value,fitness,prob_select;

	Cromozom(int size)
	{
		data.resize(size);
		for (int i = 0; i < size; i++)
		{
			data[i] = rand() % 2;
		}
		value = fitness = prob_select = 0;
	}

	void compute_val_and_fitness(int min, double coef, double fitness(double))
	{
		//compute value
		long long dataval=0;
		for (int i = 0; i < data.size(); i++)
		{
			if (data[i])
				dataval += pow(2, data.size() - (i + 1));
		}
		value = coef * (double)dataval + min;

		//compute fitness
		this->fitness = fitness(value);
	}

	void compute_prob_selection(int sum_fitness)
	{
		prob_select = fitness / sum_fitness;
	}

	bool operator!=(const Cromozom& other)
	{
		for (int i=0; i < data.size(); i++)
		{
			if (data[i] != other.data[i])
				return true;
		}
		return false;
	}

	void print_data() const
	{
		for (bool b : data)
			printf("%d", b);
		printf(" ");
	}
};

void print_cromozomes(vector<Cromozom>& cromozomes)
{
	for(int i=0;i<cromozomes.size();i++)
	{
		//print id
		printf("%*d: ",3,i+1);
		//print binary value
		for (bool b : cromozomes[i].data)
			printf("%d", b);
		//print actual value
		printf(" x=%10.6lf", cromozomes[i].value);
		//print fitness
		printf(" f= %.10lf\n", cromozomes[i].fitness);
	}
}
double find_max(vector<Cromozom> cromozomes)
{
	double maxx = cromozomes[0].fitness;

	for (int i = 1; i < cromozomes.size(); i++)
	{
		if (cromozomes[i].fitness > maxx)
			maxx = cromozomes[i].fitness;
	}

	return maxx;
}
double find_avg(vector<Cromozom> cromozomes)
{
	double sum = 0;

	for (int i = 0; i < cromozomes.size(); i++)
	{
		sum += cromozomes[i].fitness;
	}

	return sum / cromozomes.size();
}

vector<Cromozom> evolve(vector<Cromozom> &last_gen,double prob_crossover,double prob_mutate)
{
	std::default_random_engine generator;
	generator.seed(time(NULL));
	std::uniform_real_distribution<double> distribution(0.0, 1.0);

	int pop_size = last_gen.size();
	double total_fitness = 0;

	//COMPUTE PROB OF SELECTION
	for (int i = 0; i < pop_size; i++)
	{
		total_fitness += last_gen[i].fitness;
	}
	for (int i = 0; i < pop_size; i++)
	{
		last_gen[i].compute_prob_selection(total_fitness);
	}


	//CALCULATE ROULETTE INTERVALS
	vector<double> selection_interv;
	selection_interv.push_back(last_gen[0].prob_select);

	for (size_t i = 1; i < pop_size; ++i) 
	{
		selection_interv.push_back(selection_interv[i - 1] + last_gen[i].prob_select);
	}

	//CREATE NEW GENERATION
	vector<Cromozom> next_gen;

	//SELECTION
	for (int i = 0; i < pop_size; ++i) {
		double number = distribution(generator);

		int index = lower_bound(selection_interv.begin(), selection_interv.end(), number) - selection_interv.begin();
		next_gen.push_back(last_gen[index]);
	}


	//CROSSOVER
	vector<bool> participate(pop_size, false);

	for (int i = 0; i < pop_size; ++i) {
		double number = distribution(generator);

		if (number < prob_crossover)
		{
			participate[i] = true;
		}
	}

	//compute crossover
	for (int i = 0; i < pop_size; i++)
	{
		if (participate[i])
		{
			for (int j = i + 1; j < pop_size; j++)
			{
				if (participate[j] && next_gen[i] != next_gen[j])
				{
					int split = rand() % next_gen[0].data.size();
					bool aux;

					for (int k = 0; k <= split; k++)
					{
						aux = next_gen[i].data[k];
						next_gen[i].data[k] = next_gen[j].data[k];
						next_gen[j].data[k] = aux;
					}

					participate[i] = participate[j] = false;
					next_gen[i].compute_val_and_fitness(interv_min, coeficient, fitness);
					next_gen[j].compute_val_and_fitness(interv_min, coeficient, fitness);
					break;
				}
			}
		}
	}

	//MUTATION
	for (int i = 0; i < pop_size; i++)
	{
		bool mutated = false;
		for (int j = 0; j < next_gen[i].data.size(); j++)
		{
			double number = distribution(generator);
			if (number < prob_mutate)
			{
				mutated = true;
				next_gen[i].data[j] = !next_gen[i].data[j];
			}
		}
		if (mutated)
		{
			next_gen[i].compute_val_and_fitness(interv_min, coeficient, fitness);
		}

	}

	return next_gen;
}

int main()
{
	//input
	int pop_size, precizion,max_generations;
	double prob_crossover, prob_mutate;
	std::default_random_engine generator;
	generator.seed(time(NULL));
	std::uniform_real_distribution<double> distribution(0.0, 1.0);
	srand(time(NULL));
	//read input data
	ifstream fin("data.in");
	fin >> pop_size >> interv_min >> interv_max >>precizion;
	fin >> prob_crossover >> prob_mutate >> max_generations;

	//calculate binary positions needed
	int bin_size = log2((interv_max - interv_min) * pow(10, precizion)) + 1;


	//calculate coeficient
	coeficient = (double)interv_max - interv_min;
	coeficient = coeficient / (pow(2, bin_size) - 1);

	//cromozomes
	vector<Cromozom> cromozomes;
	double total_fitness = 0;
	for (int i = 0;i < pop_size; i++)
	{
		Cromozom cr(bin_size);
		cr.compute_val_and_fitness(interv_min,coeficient,fitness);
		total_fitness += cr.fitness;
		cromozomes.push_back(cr);
	}
	for (int i = 0; i < pop_size; i++)
	{
		cromozomes[i].compute_prob_selection(total_fitness);
	}

	//print initial cromozomes
	cout <<endl<< "Cromozomi initiali" << endl;
	print_cromozomes(cromozomes);

	//print initial selection probabilities
	cout << endl << "Probabilitati de selectie" << endl;
	for (int i = 0; i < pop_size; i++)
	{
		printf("cromozom %3d prob: %.10lf\n", i +1, cromozomes[i].prob_select);
	}

	//calculate intervals
	vector<double> selection_interv;
	selection_interv.push_back(cromozomes[0].prob_select);

	cout <<endl<< "Intervale probabilitati de selectie"<<endl;
	cout << "0 " << selection_interv[0]<<" ";
	for (size_t i = 1; i < pop_size; ++i) {
		selection_interv.push_back(selection_interv[i - 1] + cromozomes[i].prob_select);
		cout << selection_interv[i] << " ";
	}

	//start working on next generation
	vector<Cromozom> next_gen;

	//SELECTION
	cout << endl << "Selectie" << endl;
	for (int i = 0; i < pop_size; ++i) {
		double number = distribution(generator);
		
		int index = lower_bound(selection_interv.begin(), selection_interv.end(), number) - selection_interv.begin();
		next_gen.push_back(cromozomes[index]);
		printf("u=%.10lf selectam cromozomul %d\n", number, index + 1);
	}

	printf("\nDupa selectie:\n");
	print_cromozomes(next_gen);
	
	//CROSSOVER

	//vector de marcare
	vector<bool> participate(pop_size, false);

	for (int i = 0; i < pop_size; ++i) {
		double number = distribution(generator);

		if (number < prob_crossover)
		{
			participate[i] = true;
			printf("%3d: cromozomul    participa u=%.10lf < %.2lf\n", i + 1, number, prob_crossover);
		}
		else
		{
			printf("%3d: cromozomul nu participa u=%.10lf\n", i + 1, number);
		}
	}

	
	//compute crossover
	for (int i = 0; i < pop_size; i++)
	{
		if (participate[i])
		{
			for (int j = i+1; j < pop_size; j++)
			{
				if (participate[j] && next_gen[i] != next_gen[j])
				{
					printf("\nRecombinare dintre cromozomul %d cu cromozomul %d:\n", i + 1, j + 1);
					int split = rand() % next_gen[0].data.size();
					bool aux;

					next_gen[i].print_data();
					next_gen[j].print_data();
					printf("punct %d\n", split);
					for (int k = 0; k <= split; k++)
					{
						aux = next_gen[i].data[k];
						next_gen[i].data[k] = next_gen[j].data[k];
						next_gen[j].data[k] = aux;
					}
					participate[i] = participate[j] = false;
					next_gen[i].compute_val_and_fitness(interv_min,coeficient,fitness);
					next_gen[j].compute_val_and_fitness(interv_min, coeficient, fitness);
					printf("Rezultat ");
					next_gen[i].print_data();
					next_gen[j].print_data();
					break;
				}
			}
		}
	}

	printf("\nDupa recombinare:\n");
	print_cromozomes(next_gen);

	//MUTATION
	printf("Probabilitate de mutatie pentru fiecare gena %.2lf\n", prob_mutate);
	printf("Au fost modificati cromozomii: ");
	for (int i = 0; i < pop_size; i++)
	{
		bool mutated = false;
		for (int j = 0; j < next_gen[i].data.size(); j++)
		{
			double number = distribution(generator);
			if (number < prob_mutate)
			{
				mutated = true;
				next_gen[i].data[j] = !next_gen[i].data[j];
			}
		}
		if (mutated)
		{
			printf("%d ", i + 1);
			next_gen[i].compute_val_and_fitness(interv_min, coeficient, fitness);
		}
			
	}
	printf("\nDupa mutatie:\n");
	print_cromozomes(next_gen);

	printf("Valori maxime/valori medii: \n");
	printf("%3d: %.10lf/%.10lf\n", 0,find_max(cromozomes),find_avg(cromozomes));
	printf("%3d: %.10lf/%.10lf\n", 1,find_max(next_gen), find_avg(next_gen));

	for (int i = 2; i < max_generations; i++)
	{
		next_gen = evolve(next_gen, prob_crossover, prob_mutate);

		printf("%3d: %.10lf/%.10lf\n", i+1, find_max(next_gen), find_avg(next_gen));
	}

	return 0;
}