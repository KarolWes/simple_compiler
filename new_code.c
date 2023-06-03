	/* Global variables */
int a;
int b;
int i;
int n;
float x [100];

int gcd(int a, int b)
{
	int result;
	while (a * b != 0)
		if (a > b)
			a = a - b;
		else
			b = b - a;
	if (a == 0)
		result = b;
	else
		result = a;
	return result;
}

int factorial(int a)
{
	int k;
	int fact;
	int result;
	fact = 1;
	k = 2;
	while (k <= a)
	{
		fact = fact * k;
		k = k + 1;
	}
	result = fact;
	return result;
}

int sum(int n)
{
	int s;
	int k;
	int result;
	s = 0;
	k = 1;
	while (k <= n)
	{
		result = result + k;
		k = k + 1;
	}
	if (s != n * (n + 1) * 1.0 / 2)
		result = -1;
	else
		result = s;
	return result;
}

bool is_prime(int n)
{
	int k;
	bool b;
	bool result;
	k = 2;
	b = true;
	while ((k <= i / 2) && b)
	{
		if (i / k == i * 1.0 / k)
			b = false;
		k = k + 1;
	}
	result = b;
	return result;
}

void bubble_sort(float a[100], int n)
{
	int i;
	bool k;
	float t;
	k = true;
	while (k)
	{
		i = 1;
		k = false;
		while (i < n)
		{
			if (a[i] > a[i + 1])
			{
				t = a[i];
				a[i] = a[i + 1];
				a[i + 1] = t;
				k = true;
			}
			i = i + 1;
		}
	}
}


	/* program: 'minipas' */


int main(int argc, char *argv[])
{
	a = 48;
	b = 84;
	a = gcd(a, b);
	b = factorial(a + b);
	b = b - sum(a * b);
	a = 1;
	i = 2;
	while (i <= b)
	{
		if (is_prime(i))
			a = a + 1;
		i = i + 1;
	}
	i = 1;
	while (i < b)
	{
		x[i] = (n - i + 1) * 3.140000;
		i = i + 1;
	}
	bubble_sort(x, 1);
	return 0;
}


	 /* Created using parser by Karol Wesolowski */