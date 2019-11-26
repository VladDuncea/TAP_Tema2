#include <iostream> 
#include <fstream>
#include <algorithm> 
#include <vector>
#include <string>
#include <sstream>

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

//find last(by end time) job that doesn't confilict with current job
int lastNonConflict(vector<Job> jobs, int poz)
{
	// Initialize 'lo' and 'hi'
	int lo = 0, hi = poz - 1;

	// basic binary search
	while (lo <= hi)
	{
		int mid = (lo + hi) / 2;
		if (jobs[mid].finish <= jobs[poz].start)
		{
			if (jobs[mid + 1].finish <= jobs[poz].start)
				lo = mid + 1;
			else
				return mid;
		}
		else
			hi = mid - 1;
	}

	//not found
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

#pragma region Ex5_Var2

//job data structure
struct Job2
{
	int profit, deadline, duration,id;
};

// sort acordingly to deadline
bool cmp(Job2 s1, Job2 s2)
{
	return (s1.deadline < s2.deadline);
}

void printData(vector<Job2> &jobs,vector<vector<int>> &data, int i, int t)
{
	if (i == 0)
		return;
	if (data[i][t] == data[i - 1][t])
		printData(jobs,data,i - 1, t);
	else
	{
		int min_t = min(t, jobs[i-1].deadline) - jobs[i-1].duration;
		printData(jobs, data, i - 1, min_t);
		cout << jobs[i-1].id << " ";
	}
		
}	

void findBestSchedule(vector<Job2> jobs, int nr)
{
	//get max deadline
	int max_deadline = jobs[nr - 1].deadline;

	//Place to store dinamic data
	vector<vector<int>> data;
	data.resize(nr + 1);
	for (int i = 0; i <= nr; i++)
		data[i].resize(max_deadline + 1);

	//Initialize dinamic data
	for (int i = 0; i <= max_deadline; i++)
		data[0][i] = 0;

	for (int i = 1; i <= nr; i++)
	{
		for (int t = 0; t <= max_deadline; t++)
		{
			//time at which time the job has to end 
			int min_t = min(t, jobs[i-1].deadline) - jobs[i-1].duration;

			//not enough time
			if (min_t < 0)
			{
				data[i][t] = data[i - 1][t];
			}
			else
			{
				data[i][t] = max(data[i - 1][t], data[i - 1][min_t] + jobs[i - 1].profit);
			}
		}
	}

	cout << data[nr][max_deadline] << endl;
	printData(jobs, data, nr, max_deadline);
	

}

int problema5_var2()
{
	//open data file
	ifstream fin("date_5_2.txt");

	//Variables
	int nr;
	vector<Job2> jobs;

	//read input
	fin >> nr;
	for (int i = 0; i < nr; i++)
	{
		Job2 read;
		fin >> read.profit >> read.deadline >> read.duration;
		read.id = i + 1;
		jobs.push_back(read);
	}

	//Sort jobs after deadline
	sort(jobs.begin(), jobs.end(), cmp);

	//PD the shit out of the problem
	findBestSchedule(jobs, nr);
	return 0;
}


#pragma endregion

#pragma region Ex4_Var1

void printSol(vector<vector<int>>& data, int nr, int x)
{
	//finished
	if (x == 0)
		return;

	printSol(data, nr - 1, x - data[nr][x - 1]);
	cout << data[nr][x - 1] << " ";
}

int problema4_var1()
{
	ifstream fin("date4_1.txt");

	//reading data
	int n, k;
	fin >> n >> k;
	fin.get();
	//create dinamic space for solving
	vector<vector<int>> data(n);
	for (int i = 0; i < n; i++)
		data[i].resize(k, 0);

	int x;
	string line;
	getline(fin, line);
	//fin.get();
	std::istringstream iss(line);

	//read first vector
	while (iss >> x)
	{
		if (x < k)
			data[0][x-1] = x;
	}

	//go over vectors and compute data
	for (int i = 1; i < n; i++)
	{
		getline(fin, line);
		//fin.get();
		std::istringstream iss(line);
		//read until 0
		while (iss >> x)
		{
			for (int j = 0; j < k; j++)
			{
				int a = 0;
				if (data[i - 1][j] != 0)
				{
					a= j + 1 + x;
					if (a <= k)
						data[i][a - 1] = x;
				}
			}
		}
	}

	//no solution found
	if (data[n-1][k-1] == 0)
	{
		cout << "Nu se poate calcula!" << endl;
		return 0;
	}
	printSol(data, n - 1, k);
	
	return 0;
}

#pragma endregion


int main()
{
	problema4_var1();
	return 0;
}
