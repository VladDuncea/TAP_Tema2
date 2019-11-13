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

vector<int> arr;

int inv_count(int left, int right)
{
	if (right == left)
		return 0;

	int count = 0;

	//Get the middle
	int middle = (left + right) / 2;

	//Sort the two subvectors
	count += inv_count(left, middle);
	count += inv_count(middle + 1, right);

	vector<int> sorted;

	int i = left, j = middle + 1;

	//count significant inversions
	while (i <= middle && j <= right)
	{
		if (arr[i] > arr[j] * 2)
		{
			//increase inversion count
			count += middle - i + 1;
			j++;
		}
		else
			i++;
	}

	i = left;
	j = middle + 1;
	//merge data
	while (i <= middle && j <= right)
	{
		if (arr[i] > arr[j])
		{
			sorted.push_back(arr[j++]);
		}
		else
			sorted.push_back(arr[i++]);
	}

	//what remains
	while (i <= middle)
	{
		sorted.push_back(arr[i++]);
	}

	while (j <= right)
	{
		sorted.push_back(arr[j++]);
	}

	int z = 0;
	for (int k = left; k <= right; k++)
		arr[k] = sorted[z++];

	return count;
}

void problema3_var3()
{
	ifstream fin("date_2.in");
	int n;
	//Read vect size
	
	fin >> n;
	
	for (int i = 0; i < n; ++i)
	{
		int x;
		fin >> x;
		arr.push_back(x);
	}

	cout << inv_count(0, arr.size() - 1);
}
#pragma endregion

#pragma region Problema 4 var 1

struct Point
{
	long long x, y;
};

bool cmp(Point p1, Point p2) {
	if(p1.x == p2.x)
		return p1.y < p2.y;
	return p1.x < p2.x;

}

long long distance(Point a, Point b)
{
	return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

void merge_function(int st, int m, int dr, vector <Point>& v)
{
	int i = st, j = m + 1;
	vector <Point> aux;

	while (i <= m && j <= dr)
	{
		if (v[i].y < v[j].y)
			aux.push_back(v[i++]);
		else aux.push_back(v[j++]);
	}

	while (i <= m)
		aux.push_back(v[i++]);
	while (j <= dr)
		aux.push_back(v[j++]);

	int index = st;
	for (Point a : aux)
	{
		v[index] = a;
		index++;
	}
}

long long divide(int st, int dr, vector <Point>& v)
{
	if (dr - st == 1)
	{
		long long minim = distance(v[st], v[dr]);
		if (v[st].y > v[dr].y)   //sortam dupa ordonata
			swap(v[st], v[dr]);

		return minim;

	}
	else if (dr - st == 2)
	{
		//minimul in combinatii de cate 2
		long long minim = min(distance(v[st], v[st + 1]), min(distance(v[st], v[dr]), distance(v[dr], v[st + 1])));

		//sortare cele 3 puncte
		if (v[st].y > v[st + 1].y)
			swap(v[st], v[st + 1]);
		merge_function(st, st + 1, dr, v);

		return minim;
	}
	else
	{
		int m = (st + dr) / 2; //mijloc
		long long mst = divide(st, m, v); //minim stanga
		long long mdr = divide(m + 1, dr, v);	//minim dreapta

		long long minim = min(mst, mdr);	//minim global
		merge_function(st, m, dr, v);   //se sorteaza partea stanga si dreapta dupa ordonata

		vector <Point> aux;
		for (int i = st; i <= dr; i++)
			if (abs(v[i].x - v[m].x) <= minim)   //punem intr-un vector punctele care se afla la distanta minim fata de dreapta imaginara verticala ce imparte partea stanga de cea dreapta
				aux.push_back(v[i]);

		int siz = aux.size();

		for (int i = 0; i < siz - 1; i++)   //vedem daca exista o distanta intre doua pct mai mica decat minim cu un pct din dreapta si unul din stanga
			for (int j = i + 1; j < siz && j <= i + 7; j++)	//ze 8 puncte magige idk some theorem
				minim = min(minim, distance(aux[i], aux[j]));

		return minim;

	}
}

void problema4()
{
	ifstream fin("date_3.in");
	
	vector <Point> v;
	int n;
	fin >> n;
	v.resize(n);

	//citire vector de puncte
	for (int i = 0; i < n; i++)
		fin >> v[i].x >> v[i].y;
	
	//sortare vector
	sort(v.begin(), v.end(), cmp);

	cout << sqrt(divide(0, n - 1, v));
	fin.close();
}

#pragma endregion


int main()
{
	problema4();
	return 0;
}