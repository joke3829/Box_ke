#include "stdafx.h"

int Align(int size, int multiple)
{
	return (size + (multiple - 1)) & ~(multiple - 1);
}