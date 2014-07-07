int int2fixed_point (int n)
{
  return n * F;
}

int fixed_point2int_round (int x)
{
  return x >= 0 ? (x + F / 2) / F : (x - F / 2) / F;
}

int fixed_point2int (int x)
{
  return x / F;
}

int fixed_point_add (int x, int y)
{
  return x + y;
}

int fixed_point_add_mixed (int x, int n)
{
  return fixed_point_add (x, int2fixed_point (n));
}

int fixed_point_sub (int x, int y)
{
  return x - y;
}

int fixed_point_sub_mixed (int x, int n)
{
  return fixed_point_sub (x, int2fixed_point (n));
}

int fixed_point_multiply (int x, int y)
{
  return (int64_t)x * y / F;
}

int fixed_point_multiply_mixed (int x, int n)
{
  return x * n;
}

int fixed_point_divide (int x, int y)
{
  return (int64_t)x * F / y;
}

int fixed_point_divide_mixed (int x, int n)
{
  return x / n;
}
