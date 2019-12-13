#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <queue>

using namespace std;

#pragma region SAT_SOLVER

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
		std::cout << "Solutie: " << endl;
		for (int i = 0; i < k - 1; i++)
		{
			std::cout << sol[i] << " ";
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
		sol[k - 1] = i;

		while (it != clauze.end())
		{
			bool erased = false;
			if ((*it).variables[k - 1].used)
			{
				if ((*it).variables[k - 1].negated != (bool)i)
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

void problema1()
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
	sat_solve(1, clauze, n, sol);

	cout << "Expresia este nesatisfiabila!"<<endl;
}

#pragma endregion

#pragma region BranchAndBound_HamiltonianCircuit

class state
{
public:
	int cost,level,last_node;
	vector<bool> used;
	state* father;
	vector<vector<int>> matrix;

	state(vector<bool> &used, int cost = 0,state* father = NULL)
	{
		this->cost = cost;
		this->father = father;
		this->used = used;
		last_node = 0;
		level = 1;
	}

};

struct customCmp
{
	bool operator()(const state* a, const state* b)
	{
		return a->cost > b->cost;
	}
};


void print_sol(state* st)
{
	if (st == NULL)
		return;

	print_sol(st->father);
	std::cout <<st->last_node + 1 << " ";
}

int reduction(vector<vector<int>>& matrix)
{
	int n = matrix.size(),red = 0;
	for (int i = 0; i < n; i++)
	{
		int min = -1;
		for (int j = 0; j < n; j++)
		{
			if (matrix[i][j] != -1 && (matrix[i][j] < min || min == -1))
			{
				min = matrix[i][j];
			}
		}

		//nothing to do
		if (min <= 0)
			continue;

		red += min;
		for (int j = 0; j < n; j++)
		{
			if (matrix[i][j] != -1)
				matrix[i][j] -= min;
		}
	}

	for (int i = 0; i < n; i++)
	{
		int min = -1;
		for (int j = 0; j < n; j++)
		{
			if (matrix[j][i] != -1 && (matrix[j][i] < min || min == -1))
			{
				min = matrix[j][i];
			}
		}

		//nothing to do
		if (min <= 0)
			continue;

		red += min;
		for (int j = 0; j < n; j++)
		{
			if (matrix[j][i] != -1)
				matrix[j][i] -= min;
		}
	}

	return red;
}

void problema4()
{
	priority_queue<state*,vector<state*>, customCmp> min_heap;
	ifstream fin("date4.in");
	vector<vector<int>> matrix;
	int n, red;
	fin >> n;
	matrix.resize(n);

	//read adj matrix
	for (int i = 0; i < n; i++)
	{
		matrix[i].resize(n);

		for (int j = 0; j < n; j++)
		{
			int x;
			if (i == j)
				matrix[i][j] = -1;
			else
			{
				fin >> x;
				matrix[i][j] = x;
			}
		}
	}

	//reduction
	red = reduction(matrix);

	vector<bool> used(n, 0);
	used[0] = 1;
	//add initial state to min_heap
	state* initial_state = new state(used);
	initial_state->matrix = matrix;
	initial_state->cost = red;

	min_heap.push(initial_state);

	//to be used after finding solutions
	int best_sol = -1;
	state* best_state = NULL;

	//Extract states one by one
	while (!min_heap.empty())
	{
		//take best state out
		state* selected_state = min_heap.top();
		min_heap.pop();

		//poped bad node
		if (selected_state->cost > best_sol&& best_sol != -1)
		{
			delete(selected_state);
			continue;
		}

		//compute all of its children that can yield solutions and add them to min_heap
		for (int i = 1; i < n; i++)
		{
			//jump over used nodes
			if (selected_state->used[i])
				continue;

			state* child = new state(selected_state->used, selected_state->cost, selected_state);
			child->level = selected_state->level + 1;
			child->matrix = selected_state->matrix;
			child->last_node = i;

			//last level node, we can compute real cost and add as solution
			if (selected_state->level == n - 1)
			{
				child->cost += child->matrix[selected_state->last_node][i] + child->matrix[i][0];
				
				if (child->cost < best_sol || best_sol == -1)
				{
					best_sol = child->cost;
					best_state = child;
				}
			}
			else
			{
				//line i is -1
				for (int j = 0; j < n; j++)
					child->matrix[selected_state->last_node][j] = -1;

				//column j is -1
				for (int j = 0; j < n; j++)
					child->matrix[j][i] = -1;

				//dont go back
				child->matrix[i][0] = -1;

				//reduce matrix
				child->cost += reduction(child->matrix);

				//not last level node, we compute cost an insert it in min_heap
				child->cost += selected_state->matrix[selected_state->last_node][i];
				
				//check if child can make a better solution
				if (child->cost >= best_sol && best_sol!=-1)
				{
					delete(child);
					continue;
				}
				
				child->used[i] = true;
				min_heap.push(child);
			}
		}

	}

	//print solution
	cout <<"Cost minim:"<< best_sol << endl;
	print_sol(best_state);
	cout << "1"<<endl;
	
}

#pragma endregion


int main()
{
	problema4();
	return 0;
}
