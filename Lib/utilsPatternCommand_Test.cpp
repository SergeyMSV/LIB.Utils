#include "utilsPatternCommand.h"

#include "utilsTest.h"

#include <algorithm>

#include <iostream>

#include <string>
#include <vector>

namespace utils
{
	namespace unit_test
	{

void UnitTest_PatternCommmand()
{
	std::cout << "\n""utils::PatternCommmand" << std::endl;
	
	{
		class tSomeDevice : public pattern_Command::tObjectCommand
		{
			using tCommandBase = pattern_Command::tObjectCommand::tCommand;

			class tCommandA : public tCommandBase
			{
				std::string m_Data;

			public:
				tCommandA(tSomeDevice* obj, const std::string& data)
					:tCommandBase(obj), m_Data(data)
				{}

				virtual void operator()()
				{
					std::cout << m_Data << " CommandA\n";

					GetObject<tSomeDevice>()->OnResult(2);
				}
			};

			class tCommandB : public tCommandBase
			{
				std::string m_Data;

			public:
				tCommandB(tSomeDevice* obj, const std::string& data)
					:tCommandBase(obj), m_Data(data)
				{}

				virtual void operator()()
				{
					std::cout << m_Data << " CommandB\n";

					GetObject<tSomeDevice>()->OnResultLALALA("adsfasfasfaf");
				}
			};

		public:
			void CommandA()
			{
				PutCommand(new tCommandA(this, "Command A"));
			}

			void CommandA1()
			{
				PutCommand(new tCommandA(this, "Command A1"));
			}

			void CommandB()
			{
				PutCommand(new tCommandB(this, "Command B"));
			}

			void operator()()
			{
				tCommand* Cmd = GetCommand();

				while (Cmd != nullptr)
				{
					(*Cmd)();

					Cmd = SetCommandNext();
				}
			}

			void OnResult(unsigned int cerr)
			{
				std::cout << "OnResult: " << cerr << '\n';
			}

			void OnResultLALALA(const std::string& msg)
			{
				std::cout << "OnResultLALALA: " << msg << '\n';
			}
		};

		tSomeDevice A;

		A.CommandA();
		A.CommandA1();
		A.CommandB();

		A();

		A.CommandA();
		A.CommandA1();
		A.CommandB();
	}


	std::cout << std::endl;
}

	}
}
