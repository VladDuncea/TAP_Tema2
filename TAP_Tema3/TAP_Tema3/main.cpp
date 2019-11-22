#include <iostream> 
#include <fstream>
#include <algorithm> 
#include <vector>

using namespace std;


#pragma region Ex5_Var1

//job data structure
struct Job
{
	int start, finish, profit;
};

//sort acordingly to finish time
bool jobComparataor(Job s1, Job s2)
{
	return (s1.finish < s2.finish);
}

//find last job that doesn't confilict with current job
//TODO: this has to be logn
int lastNonConflict(vector<Job> jobs, int i)
{
	for (int j = i - 1; j >= 0; j--)
	{
		if (jobs[j].finish <= jobs[i].start)
			return j;
	}
	return -1;
}

int findMaxProfit(vector<Job> jobs, int nr)
{
	// Sort jobs according to finish time 
	sort(jobs.begin(), jobs.end(), jobComparataor);

	//place to dinamically save subproblems
	vector<int> table;
	table.resize(nr);

	//initial data
	table[0] = jobs[0].profit;

	// Fill entries in table using recursive property 
	for (int i = 1; i < nr; i++)
	{
		// Find profit including the current job 
		int inclProf = jobs[i].profit;
		int lastProf = lastNonConflict(jobs, i);
		if (lastProf != -1)
			inclProf += table[lastProf];

		// Store maximum of including and excluding 
		table[i] = max(inclProf, table[i - 1]);

	}

	// Store result and free dynamic memory allocated for table[] 
	int result = table[nr - 1];


	//Print esult
	cout << result << endl;

	for (int i = nr - 1; i > 0; i--)
	{
		if (result == 0)
			return 0;
		if (table[i - 1] < table[i] && table[i] == result)
		{
			cout << jobs[i].start << " " << jobs[i].finish << endl;
			result -= jobs[i].profit;
		}
	}

	if (result != 0)
		cout << jobs[0].start << " " << jobs[0].finish << endl;

	return 0;
}

// Driver program 
int problema5_var1()
{
	//open data file
	ifstream fin("date_5_1.txt");

	//variables
	vector<Job> jobs;
	int nr;

	//read input
	fin >> nr;
	for (int i = 0; i < nr; i++)
	{
		Job read;
		fin >> read.start >> read.finish >> read.profit;
		jobs.push_back(read);
	}

	findMaxProfit(jobs, nr);
	return 0;
}

#pragma endregion


int main()
{
	problema5_var1();
	return 0;
}
