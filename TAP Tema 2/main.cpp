#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

#pragma region Problema 3 var 1

void problema3_var1()
{
	ifstream fin("date_1.in");

	int size_a, size_b;
	vector<int> a, b;
	
	//read first vect
	fin >> size_a;
	for (int i = 0; i < size_a; i++)
	{
		int x;
		fin >> x;
		a.push_back(x);
	}

	//read second vect
	fin >> size_b;
	for (int i = 0; i < size_b; i++)
	{
		int x;
		fin >> x;
		b.push_back(x);
	}

	//Swap vectors if the first one is bigger
	if (size_a > size_b)
	{
		swap(a, b);
		swap(size_a, size_b);
	}

	//Get the size to the median
	int size_to_median = (size_a + size_b + 1) / 2;

	//Set maximum and minimum contribution of a
	int min_count = 0;
	int max_count = size_a;

	while (min_count <= max_count)
	{
		//Calculate "guessed" contribution of a and b
		int contribution_a = min_count + ((max_count - min_count) / 2);
		int contribution_b = size_to_median - contribution_a;

		//find where the median lies
		if (contribution_a > 0 && a[contribution_a - 1] > b[contribution_b])
		{
			//the median is on the left
			max_count = contribution_a - 1;
		}
		else if(contribution_a < size_a && b[contribution_b - 1] > a[contribution_a])
		{
			//the median is on the right
			min_count = contribution_a + 1;
		}
		else
		{
			//this is the right size for a
			int left_val = 0;
			if (contribution_a == 0)
				left_val = b[contribution_b - 1];
			else if (contribution_b == 0)
				left_val = a[contribution_a - 1];
			else
				left_val = max(a[contribution_a - 1], b[contribution_b - 1]);

			if ((contribution_a + contribution_b) % 2 != 0)
			{
				cout << left_val;
				return;
			}
				
			
			int right_val = (contribution_a == size_a) ? b[contribution_b] : (contribution_b == size_b) ? a[contribution_a] : min(a[contribution_a], b[contribution_b]);
			cout << (left_val + right_val) / 2;
			return;
		}
	}

}

#pragma endregion

#pragma region Problema 3 var 3

void problema3_var3()
{
	ifstream fin("date_2.in");


}

#pragma endregion



int main()
{
	problema3_var3();
	return 0;
}