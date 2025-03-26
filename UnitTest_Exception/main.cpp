#include <utilsException.h>

#include <iostream>
//namespace utils
//{
//	void UnitTest_Exception();
//}

int main()
{
	try
	{
		THROW_INVALID_ARGUMENT("some error");
	}
	catch (std::exception ex)
	{
		std::cout << ex.what() << '\n';
	}
	//utils::UnitTest_Exception();

	return 0;
}