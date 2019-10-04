#include "utilsCryptoRSA32.h"

namespace utils
{
	namespace crypto
	{
		namespace RSA32
		{

int ExtEuclid(int a, int mod)
{
	if (mod == 0)
	{
		return 1;
	}

	int B = mod;

	int X = 0;
	//int Y = 0;

	int X1 = 0;
	//int Y1 = 1;

	int X2 = 1;
	//int Y2 = 0;

	double Q = 0;
	double R = 0;

	while (B > 0)
	{
		Q = a / B;
		R = a % B;//R = a - Q * B;
		
		X = (int)(X2 - Q * X1);
		//Y = (int)(Y2 - Q * Y1);
		
		a = B;
		B = (int)R;
		
		X2 = X1;
		//Y2 = Y1;

		X1 = X;
		//Y1 = Y;
	}

	X = X2;
	//Y = Y2;
	 
	if (X < 0)
	{
		X = X + mod;
	}
	 
	return X;
}

int Qe2(int x1, unsigned int y, int mod)
{
	long long S = 1;

	long long x = x1;

	while (y)
	{
		if (y & 1)
		{
			S = (S * x) % mod;
		}

		y >>= 1;

		x = (x * x) % mod;
	}

	return static_cast<int>(S);
}

int GetGcd(int x, int y)
{
	if (x < 0)
	{
		x = -x;
	}

	if (y < 0)
	{
		y = -y;
	}

	if (x + y == 0)
	{
		return -1;//ERROR
	}

	int G = y;

	while (x > 0)
	{
		G = x;

		x = y % x;

		y = G;
	}

	return G;
}

int GetPrivateKey(int e, int a)
{
	return ExtEuclid(e, a);
}

int GetPrivateKey(int e, int a, int &gcd)
{
	gcd = GetGcd(e, a);

	return ExtEuclid(e, a);
}

	}

std::vector<int> RSA32_Encrypt(std::vector<int>& msg, tKey32 key, tKey32 mod)
{
	std::vector<int> Res;

	size_t Size = msg.size();

	Res.reserve(Size * 2);

	for (size_t i = 0; i < Size; ++i)
	{
		Res.push_back(RSA32::Qe2(((msg[i] >> 16) & 0x0000FFFF), key.Field.A, static_cast<int>(mod.Field.A)));

		Res.push_back(RSA32::Qe2(msg[i] & 0x0000FFFF, key.Field.A, static_cast<int>(mod.Field.A)));
	}

	return Res;
}

std::vector<int> RSA32_Decrypt(std::vector<int>& msg, tKey32 key, tKey32 mod)
{
	std::vector<int> Res;

	size_t Size = msg.size();

	Res.reserve(Size / 2);

	for (size_t i = 0; i < Size; i += 2)
	{	
		int Value = RSA32::Qe2(msg[i], key.Field.A, static_cast<int>(mod.Field.A)) << 16;

		Value |= RSA32::Qe2(msg[i + 1], key.Field.A, static_cast<int>(mod.Field.A));

		Res.push_back(Value);
	}

	return Res;
}

	}
}
