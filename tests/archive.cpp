#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include "writer.h"
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)

#include <string>
#include <fstream>
#include <streambuf>

/*!
 * Tests sad::Controls callbacks working and main workflow
 */
struct ArchiveTest : tpunit::TestFixture
{
 public:
   ArchiveTest() : tpunit::TestFixture(
           TEST(ArchiveTest::test_add)
   ) {}

   void test_add()
   {
       tar7z::Archive ar;
       ASSERT_TRUE(ar.add("1", std::vector<char>()));
       // Windows is not happy to see such files
       ASSERT_TRUE(ar.add("CON", std::vector<char>()) == false);
       ASSERT_TRUE(ar.add("!<>", std::vector<char>()) == false);
       // Empty file names are bad
       ASSERT_TRUE(ar.add("", std::vector<char>()) == false);

       ASSERT_TRUE(ar.add("Just your average file name.txt", std::vector<char>()));
   }
} _archive_test;
