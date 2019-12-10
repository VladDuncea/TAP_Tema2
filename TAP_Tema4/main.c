#include <stdio.h>
#include <math.h>

//scanf("%d", &n);
//printf("%d ", a[i*10 +j]);

#pragma warning(disable:4996)

int main()
{
	double a, b, c, delta;
	double x1, x2;
	scanf("%lf %lf %lf", &a, &b, &c);
	delta = b * b - 4 * a * c;
	if (delta < 0)
	{
		printf("Nu avem solutii reale.\n");
		return 0;
	}
	else if (delta == 0)
	{
			x1 = ((-1) * b + sqrt(delta)) / (2 * a);
			x2 = x1;
			printf("Solutiile sunt egale: %d %d\n", x1, x2);
	}
	else if (delta > 0)
	{
			x1 = ((-1) * b + sqrt(delta)) / (2 * a);
			x2 = ((-1) * b - sqrt(delta)) / (2 * a);
			printf("Solutiile sunt: %lf %lf\n", x1, x2);
	}

	return 0;
}