#include "catch.hpp"

#include <story.h>
#include <globals.h>
#include <runner.h>
#include <compiler.h>

using namespace ink::runtime;

SCENARIO("a story has the proper line breaks", "[lines]")
{
	GIVEN("a story with line breaks")
	{
		auto   ink    = story::from_file(INK_TEST_RESOURCE_DIR "LinesStory.bin");
		runner thread = ink->new_runner();
		WHEN("start thread")
		{
			THEN("thread can continue") { REQUIRE(thread->can_continue()); }
			WHEN("consume lines")
			{
				std::string line1 = thread->getline();
				std::string line2 = thread->getline();
				std::string line3 = thread->getline();
				std::string line4 = thread->getline();
				THEN("lines are correct")
				{
					REQUIRE(line1 == "Line 1\n");
					REQUIRE(line2 == "Line 2\n");
					REQUIRE(line3 == "Line 3\n");
					REQUIRE(line4 == "Line 4\n");
				}
			}
			WHEN("consume lines with functions")
			{
				thread->move_to(ink::hash_string("Functions"));
				std::string line1 = thread->getline();
				std::string line2 = thread->getline();

				THEN("function lines are correct")
				{
					REQUIRE(line1 == "Function Line\n");
					REQUIRE(line2 == "Function Result\n");
				}
			}
			WHEN("consume lines with tunnels")
			{
				thread->move_to(ink::hash_string("Tunnels"));
				std::string line1 = thread->getline();
				std::string line2 = thread->getline();

				THEN("tunnel lines are correct")
				{
					REQUIRE(line1 == "Tunnel Line\n");
					REQUIRE(line2 == "Tunnel Result\n");
				}

				THEN("thread cannot continue") { REQUIRE(! thread->can_continue()); }
			}
		}
	}
	GIVEN("a complex story")
	{
		auto   ink    = story::from_file(INK_TEST_RESOURCE_DIR "TheIntercept.bin");
		runner thread = ink->new_runner();
		// based on issue #82
		WHEN("run sequence 1 3 3 3 2 3")
		{
			for (int i : {1, 3, 3, 3, 2, 3}) {
				thread->getall();
				thread->choose(i - 1);
			}
			std::string text = thread->getall();
			THEN("no newline before dot") { REQUIRE(text == "\"I don't see why,\" I reply.\n"); }
		}
	}
}
