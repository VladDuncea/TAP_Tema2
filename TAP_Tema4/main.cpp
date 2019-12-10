#include <iostream>
#include <fstream>
#include <vector>
#include <list>

using namespace std;

class var
{
public:
	bool used, negated;

	var(bool used = false, bool negated = false)
	{
		this->used = used;
		this->negated = negated;
	}
};

class clauza
{
public:
	vector<var> variables;
	int used_vars;
	clauza(int n)
	{
		var v;
		variables.resize(n, 0);
		used_vars = 0;
	}
};

void sat_solve(int k, list<clauza>& clauze, int n, vector<int>& sol)
{
	//found solution
	if (clauze.empty())
	{
		cout << "Solutie: " << endl;
		for (int i = 0; i < k - 1; i++)
		{
			cout << sol[i] << " ";
		}
		exit(0);
	}

	//there are no more variables
	if (k > n)
		return;

	bool valid;
	
	//salvam starea curenta
	list<clauza> copy_clauza = clauze;

	//iterator
	list<clauza>::iterator it;

	//testam pentru fals/adevarat
	for (int i = 0; i < 2; i++)
	{
		valid = true;
		it = clauze.begin();

		//memorize solution
		sol[k-1] = i;

		while (it != clauze.end())
		{
			bool erased = false;
			if ((*it).variables[k - 1].used)
			{
				if ((*it).variables[k - 1].negated != i)
				{
					//var is true so clause is true and we can remove it
					it = clauze.erase(it);
					erased = true;
				}
				else
				{
					//var is false so it is useless so we remove it
					if ((*it).used_vars > 1)
					{
						(*it).variables[k - 1].used = false;
						(*it).used_vars--;
					}
					else
					{
						//empty clause so insatisfialble solution
						valid = false;
						break;
					}
				}
			}

			//dont iterate if we just removed something
			if (!erased)
				it++;
		}

		//advance
		if (valid)
			sat_solve(k + 1, clauze, n, sol);

		//reset
		clauze = copy_clauza;
	}
}

int main()
{
	int n, m;
	fstream fin("date.in");

	//citire nr maxim de variabile si nr de clauze
	fin >> n >> m;
	//constructie lista clauze
	list<clauza> clauze;
	for (int i = 0; i < m; i++)
	{
		int x;
		clauza cl(n);

		//read nr of clauses
		fin >> x;

		//read variables of clause
		for (int j = 0; j < x; j++)
		{
			int k;
			fin >> k;
			if (k < 0)
			{
				cl.variables[-k - 1].used = true;
				cl.variables[-k - 1].negated = true;
			}
			else
			{
				cl.variables[k - 1].used = true;
			}
			cl.used_vars++;
		}
		clauze.push_back(cl);
	}
	vector<int> sol(n, 0);
	//solve with backtracking
	sat_solve(1,clauze,n,sol);

	return 0;
}
