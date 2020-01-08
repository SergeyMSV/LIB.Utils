#include "utilsPatternState.h"

#include "utilsTest.h"

#include <algorithm>

#include <iostream>

#include <string>
#include <vector>

//#define DEBUG_SHOW_DOES_NOT_WORK

namespace utils
{
	namespace unit_test
	{

void UnitTest_PatternState()
{
	std::cout << "\n""utils::PatternState" << std::endl;

	//////////////////////////////////////////////////////////////////////////////////////////////////
	{
		class tDevice :public pattern_State::tObjectState
		{
			class tState :public pattern_State::tObjectState::tStateBase
			{
			public:
				tState(tObjectState* obj) :tStateBase(obj) {}

				virtual void DoSomeWork()
				{
					GetObject<tDevice>()->DoSomeWork_Main();
				}
			};

			class tStateStop;

			class tStateStart :public tState
			{
			public:
				tStateStart(tObjectState* obj, const std::string& value) :tState(obj)
				{
					std::cout << "tState-Start: " << value << '\n';
				}

				void DoSomeWork()
				{
					ChangeState(new tStateStop(GetObject<pattern_State::tObjectState>(), "lalala"));
				}
			};

			class tStateStop :public tState
			{
			public:
				tStateStop(tObjectState* obj, const std::string& value) :tState(obj)
				{
					std::cout << "tState-Stop:  " << value << '\n';
				}
			};

		public:
			tDevice()
			{
				ChangeState(new tStateStart(this, "the very start"));
			}

			void operator()()
			{
				GetState<tState>()->DoSomeWork();

				class tHren
				{
				public:
					virtual void DoSomeWork()
					{
					}
				};
				tHren* State = GetState<tHren>();
				if (State)
					State->DoSomeWork();
			}

			void DoSomeWork_Main()
			{
				std::cout << "DoSomeWork_Main\n";
			}
		};

		tDevice A;

		for (int i = 0; i < 10; ++i)
		{
			std::cout << i << " ";
			A();
		}
	}


	std::cout << std::endl;
}

	}
}
