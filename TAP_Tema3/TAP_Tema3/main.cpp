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


int main()
{
	problema5_var2();
	return 0;
}
