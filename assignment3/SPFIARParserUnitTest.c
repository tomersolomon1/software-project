#include "unit_test_util.h"
#include "SPFIARParser.h"
#include <stddef.h>

static bool spParserCheckIsInt() {
	ASSERT_TRUE(spParserIsInt("1"));
	ASSERT_TRUE(spParserIsInt("11"));
	ASSERT_TRUE(spParserIsInt("111"));
	ASSERT_TRUE(spParserIsInt("-1"));
	ASSERT_TRUE(spParserIsInt("-11"));
	ASSERT_TRUE(spParserIsInt("-111"));
	ASSERT_TRUE(spParserIsInt("01"));
	ASSERT_TRUE(spParserIsInt("011"));
	ASSERT_TRUE(spParserIsInt("0111"));
	ASSERT_TRUE(spParserIsInt("-01"));
	ASSERT_TRUE(spParserIsInt("-011"));
	ASSERT_TRUE(spParserIsInt("-0111"));
	return true;
}
static bool spParserCheckParseLine() {
	SPCommand cmd;
	cmd = spParserPraseLine("add_disc 17");
	ASSERT_TRUE(cmd.cmd == SP_ADD_DISC && cmd.validArg && cmd.arg == 17);
	cmd = spParserPraseLine("add_disc	17\n");
	ASSERT_TRUE(cmd.cmd == SP_ADD_DISC && cmd.validArg && cmd.arg == 17);
	cmd = spParserPraseLine("	add_disc 17\n");
	ASSERT_TRUE(cmd.cmd == SP_ADD_DISC && cmd.validArg && cmd.arg == 17);
	cmd = spParserPraseLine("aDd_disc 17");
	ASSERT_TRUE(cmd.cmd == SP_INVALID_LINE && !cmd.validArg);
	return true;
}
int main450() {
	RUN_TEST(spParserCheckIsInt);
	RUN_TEST(spParserCheckParseLine);
	bool a = spParserIsInt("4r");
	int i = atoi("4r");
	if(a){
		printf("4r is int %d", i);
		fflush(stdout);
	}
	else{
		printf("4r is NOT int");
		fflush(stdout);
	}
	return 0;
}
