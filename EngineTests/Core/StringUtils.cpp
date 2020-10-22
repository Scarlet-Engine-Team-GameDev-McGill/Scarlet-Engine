#include <gtest/gtest.h>
#include <Utils/StringUtils.h>

using namespace ScarletEngine;

TEST(StringUtils, BytesString)
{
	size_t B = 1; // 2^0
	String BString = StringUtils::CreateBytesString(B);
	EXPECT_STREQ(BString.c_str(), "1.00 B");

	size_t KB = 1024; // 2^10
	String KBString = StringUtils::CreateBytesString(KB);
	EXPECT_STREQ(KBString.c_str(), "1.00 kB");

	size_t MB = 1048576; // 2^20
	String MBString = StringUtils::CreateBytesString(MB);
	EXPECT_STREQ(MBString.c_str(), "1.00 MB");

	size_t GB = 1073741824; // 2^30
	String GBString = StringUtils::CreateBytesString(GB);
	EXPECT_STREQ(GBString.c_str(), "1.00 GB");

	size_t TB = 1099511627776; // 2^40
	String TBString = StringUtils::CreateBytesString(TB);
	EXPECT_STREQ(TBString.c_str(), "1.00 TB");

	size_t TestSize = 1572864; // 2^20 + (2^20)/2
	String TestSizeString = StringUtils::CreateBytesString(TestSize);
	EXPECT_STREQ(TestSizeString.c_str(), "1.50 MB"); // 1.50 = 1 + 1/2

	TestSize = 1310720; // 2^20 + (2^20)/4
	TestSizeString = StringUtils::CreateBytesString(TestSize);
	EXPECT_STREQ(TestSizeString.c_str(), "1.25 MB"); // 1.13 = 1 + 1/4 (rounded to two decimal)
}