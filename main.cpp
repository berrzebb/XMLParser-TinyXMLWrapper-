#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)

#include "gtest/gtest.h"

GTEST_API_ void main(int argc, char** argv)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(1483673);

	testing::InitGoogleTest(&argc, argv);	

	RUN_ALL_TESTS();

	system("pause");
}