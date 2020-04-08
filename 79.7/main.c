#include <stdio.h>
#include <stdbool.h>

static bool output = true;

static bool getOutputConfig()
{
	return output;
}

int main()
{
	if (getOutputConfig())
	{
		printf("The Little Prince\n");
	}
}