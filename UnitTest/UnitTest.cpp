
#include "CppUnitTest.h"

//#include <Vector.h>

import <format>;
import CLR.Utils;
import CLR.Math.Vector;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// https://learn.microsoft.com/en-us/visualstudio/test/how-to-use-microsoft-test-framework-for-cpp?view=vs-2022

namespace UnitTest
{
	using namespace CLR;

	TEST_CLASS(UnitTest)
	{
	public:
		
		TEST_METHOD(Vector)
		{
			Math::Vector3f vec3_a;
			Math::Vector3f vec3_b(1.f, 0.f, -1.f);

			UNUSED_PARAMS(vec3_a);
			UNUSED_PARAMS(vec3_b);

			Logger::WriteMessage("Hello CLR Unit Test.");

			char vec_str[256]{};
			sprintf_s(vec_str, "vec.x = %f, vec.y = %f, vec.z = %f", vec3_b[0], vec3_b[1], vec3_b[2]);
			Logger::WriteMessage(vec_str);
		}

		TEST_METHOD(CppFormat)
		{
			std::string str = std::format("{} {}!\n", "Hello", "world", "something");
			Logger::WriteMessage(str.c_str());

			struct TestArrayType
			{
				int a;
				float b;
			};
			TestArrayType testArray[] = { {1, 1.f}, {3, 1.f}, {0, 4.f} };
			str = std::format("GetArrayLength = {}\n", GetArrayLength(testArray));
			Logger::WriteMessage(str.c_str());

			str = std::format("std::size = {}\n", std::size(testArray));
			Logger::WriteMessage(str.c_str());
		}
	};
}
