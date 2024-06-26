#include "catch.hpp"

#include <story.h>
#include <runner.h>
#include <globals.h>
#include <compiler.h>
#include <choice.h>

#include <string.h>

using namespace ink::runtime;

SCENARIO("List logic operations", "[lists]")
{
	GIVEN("a demo story")
	{
		auto   ink    = story::from_file(INK_TEST_RESOURCE_DIR "ListLogicStory.bin");
		runner thread = ink->new_runner();
		WHEN("just run")
		{
			std::string out = thread->getall();

			REQUIRE(out == R"==(A, C
yes
false
true
true
true
true
A
B
>B
>
> Z, A, B, C
>
>
B, C
>C > > >
>A, C >B >
>B
>
> >Z >A >
Hey
)==");
		}
	}
}
SCENARIO("run a story with lists", "[lists]")
{
	GIVEN("a story with multi lists")
	{
		auto    ink     = story::from_file(INK_TEST_RESOURCE_DIR "ListStory.bin");
		globals globals = ink->new_globals();
		runner thread = ink->new_runner(globals);

		WHEN("just run")
		{
			std::string out = thread->getall();
			std::string choice1 = thread->get_choice(0)->text();
			thread->choose(0);
			std::string out2 = thread->getall();
			THEN("should output expected")
			{
				REQUIRE(out == "cat, snake\n");
				REQUIRE(choice1 == "list: bird, red, yellow");
				REQUIRE(out2 == "list: bird, red, yellow\ncat, snake\nbird, red, yellow\n");
			}
		}

		WHEN("modify with full flag name")
		{
			std::string out     = thread->getall();
			std::string choice1 = thread->get_choice(0)->text();
			thread->choose(0);

			list l1 = *globals->get<list>("list");
			l1->add("animals.dog");
			l1->remove("colors.red");
			REQUIRE(globals->set<list>("list", l1));

			std::string out2 = thread->getall();

			THEN("Output should be changed")
			{
				REQUIRE(out2 == "list: bird, dog, yellow\ncat, snake\nbird, dog, yellow\n");
			}
		}

		WHEN("modify")
		{
			std::string out = thread->getall();
			std::string choice1 = thread->get_choice(0)->text();
			thread->choose(0);

			list l1 = *globals->get<list>("list");
			l1->add("dog");
			l1->remove("red");
			REQUIRE(globals->set<list>("list", l1));

			l1 = *globals->get<list>("animals");
			l1->add("bird");
			l1->add("dog");
			l1->remove("snake");
			l1->remove("cat");
			l1->add("cat");
			REQUIRE(globals->set<list>("animals", l1));

			std::string out2 = thread->getall();
			THEN("should output change")
			{
				REQUIRE(out == "cat, snake\n");
				REQUIRE(choice1 == "list: bird, red, yellow");
				// changing the list will also change the text of the repeated choice
				REQUIRE(out2 == "list: bird, dog, yellow\nbird, cat, dog\nbird, dog, yellow\n");
			}

			THEN("list should contain things")
			{
				l1 = *globals->get<list>("list");
				REQUIRE(l1->contains("bird"));
				REQUIRE(l1->contains("dog"));
				REQUIRE(l1->contains("yellow"));

				REQUIRE_FALSE(l1->contains("cat"));
				REQUIRE_FALSE(l1->contains("snake"));
				REQUIRE_FALSE(l1->contains("blue"));
				REQUIRE_FALSE(l1->contains("white"));
			}

			THEN("should iterate all contained flags")
			{
				l1 = *globals->get<list>("list");
				for(auto flag : *l1) {
					INFO(flag);
					REQUIRE((strcmp(flag.list_name, "colors")==0 || strcmp(flag.list_name, "animals") == 0));
					REQUIRE((strcmp(flag.flag_name, "bird") == 0
						|| strcmp(flag.flag_name, "dog") == 0
						|| strcmp(flag.flag_name, "yellow") == 0
					));
				}
			}
		}
	}
}
