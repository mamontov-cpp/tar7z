#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include "reader.h"
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)

#include <string>
#include <fstream>
#include <streambuf>

/*!
 * Tests sad::Controls callbacks working and main workflow
 */
struct ReaderTest : tpunit::TestFixture
{
 public:
   ReaderTest() : tpunit::TestFixture(
           TEST(ReaderTest::test_empty),
           TEST(ReaderTest::test_one_simple_file),
           TEST(ReaderTest::test_four_files_with_short_names),
           TEST(ReaderTest::test_longname_in_beginning),
           TEST(ReaderTest::test_longname_in_center),
           TEST(ReaderTest::test_longname_in_end),
           TEST(ReaderTest::test_7z),
           TEST(ReaderTest::test_not_exists),
           TEST(ReaderTest::test_invalid_file)
   ) {}

   void test_empty()
   {
      tar7z::Archive ar;
      tar7z::Reader reader;
      tar7z::Error error = reader.read("empty_ethalon.tar", ar);
      ASSERT_TRUE(error == tar7z::T7ZE_OK);
      ASSERT_TRUE(ar.count() == 0);
   }

   void test_one_simple_file()
   {
       tar7z::Archive ar;
       tar7z::Reader reader;
       tar7z::Error error = reader.read("simple_ethalon.tar", ar);
       ASSERT_TRUE(error == tar7z::T7ZE_OK);
       ASSERT_TRUE(ar.count() == 1);
       char* buf = ar.file("nicesmallname.txt")->contents();
       std::string ethalon("this is a nice block content!");
       ASSERT_TRUE(memcmp(buf, &(ethalon[0]), ethalon.size()) == 0);
   }

   void test_four_files_with_short_names()
   {
       tar7z::Archive ar;
       tar7z::Reader reader;
       tar7z::Error error = reader.read("shortnames_ethalon.tar", ar);
       ASSERT_TRUE(error == tar7z::T7ZE_OK);
       ASSERT_TRUE(ar.count() == 4);

       {
           char* buf = ar.file("nicesmallname.txt")->contents();
           std::string ethalon("this is a nice block content!");
           ASSERT_TRUE(memcmp(buf, &(ethalon[0]), ethalon.size()) == 0);
       }

       {
           char* buf = ar.file("thisfileisintentially100characterslongandweshouldcareaboutitbecausewemustalsohandlelinksandallstuff!")->contents();
           std::string ethalon(512, 'm');
           ASSERT_TRUE(memcmp(buf, &(ethalon[0]), ethalon.size()) == 0);
       }

       {
           tar7z::Entry* e = ar.file("ignoreme.txt");
           ASSERT_TRUE(e->Size == 0);
       }

       {
           tar7z::Entry* e = ar.file("ignoreme2.txt");
           ASSERT_TRUE(e->Size == 0);
       }
   }

   void test_longname_in_beginning()
   {
       tar7z::Archive ar;
       tar7z::Reader reader;
       tar7z::Error error = reader.read("longnameinbeginning_ethalon.tar", ar);
       ASSERT_TRUE(error == tar7z::T7ZE_OK);
       ASSERT_TRUE(ar.count() == 3);

       {
           char* buf = ar.file("this file is intentionally longer than one hundred of characters because we need to test long names no part should be changed.txt")->contents();
           std::string ethalon("this is content of file with long name");
           ASSERT_TRUE(memcmp(buf, &(ethalon[0]), ethalon.size()) == 0);
       }

       {
           char* buf = ar.file("testfile.txt")->contents();
           std::string ethalon("short content");
           ASSERT_TRUE(memcmp(buf, &(ethalon[0]), ethalon.size()) == 0);
       }

       {
           char* buf = ar.file("testfile1.txt")->contents();
           std::string ethalon("very short content");
           ASSERT_TRUE(memcmp(buf, &(ethalon[0]), ethalon.size()) == 0);
       }
   }

   void test_longname_in_center()
   {
       tar7z::Archive ar;
       tar7z::Reader reader;
       tar7z::Error error = reader.read("longnameincenter_ethalon.tar", ar);
       ASSERT_TRUE(error == tar7z::T7ZE_OK);
       ASSERT_TRUE(ar.count() == 4);

       {
           char* buf = ar.file("this file is intentionally longer than one hundred of characters because we need to test long names no part should be changed.txt")->contents();
           std::string ethalon("this is content of file with long name");
           ASSERT_TRUE(memcmp(buf, &(ethalon[0]), ethalon.size()) == 0);
       }

       {
           char* buf = ar.file("this is copy of file is intentionally longer than one hundred of characters because we need to test long names no part should be changed.txt")->contents();
           std::string ethalon("this is content of second file with long name");
           ASSERT_TRUE(memcmp(buf, &(ethalon[0]), ethalon.size()) == 0);
       }


       {
           char* buf = ar.file("testfile.txt")->contents();
           std::string ethalon("short content");
           ASSERT_TRUE(memcmp(buf, &(ethalon[0]), ethalon.size()) == 0);
       }

       {
           char* buf = ar.file("testfile1.txt")->contents();
           std::string ethalon("very short content");
           ASSERT_TRUE(memcmp(buf, &(ethalon[0]), ethalon.size()) == 0);
       }
   }

   void test_longname_in_end()
   {
       tar7z::Archive ar;
       tar7z::Reader reader;
       tar7z::Error error = reader.read("longnameinend_ethalon.tar", ar);
       ASSERT_TRUE(error == tar7z::T7ZE_OK);
       ASSERT_TRUE(ar.count() == 4);

       {
           char* buf = ar.file("this file is intentionally longer than one hundred of characters because we need to test long names no part should be changed.txt")->contents();
           std::string ethalon("this is content of file with long name");
           ASSERT_TRUE(memcmp(buf, &(ethalon[0]), ethalon.size()) == 0);
       }

       {
           char* buf = ar.file("this is copy of file is intentionally longer than one hundred of characters because we need to test long names no part should be changed.txt")->contents();
           std::string ethalon("this is content of second file with long name");
           ASSERT_TRUE(memcmp(buf, &(ethalon[0]), ethalon.size()) == 0);
       }


       {
           char* buf = ar.file("testfile.txt")->contents();
           std::string ethalon("short content");
           ASSERT_TRUE(memcmp(buf, &(ethalon[0]), ethalon.size()) == 0);
       }

       {
           char* buf = ar.file("testfile1.txt")->contents();
           std::string ethalon("very short content");
           ASSERT_TRUE(memcmp(buf, &(ethalon[0]), ethalon.size()) == 0);
       }
   }

   void test_7z()
   {
       tar7z::Archive ar;
       tar7z::Reader reader;
       tar7z::Error error = reader.read("7z.tar", ar);
       ASSERT_TRUE(error == tar7z::T7ZE_OK);
       ASSERT_TRUE(ar.count() == 4);
   }

   void test_not_exists()
   {
       tar7z::Archive ar;
       tar7z::Reader reader;
       tar7z::Error error = reader.read("notexists.tar", ar);
       ASSERT_TRUE(error == tar7z::T7ZE_CANNOT_OPEN_FILE);
   }

   void test_invalid_file()
   {
       tar7z::Archive ar;
       tar7z::Reader reader;
       tar7z::Error error = reader.read("invalid.tar", ar);
       ASSERT_TRUE(error == tar7z::T7ZE_INVALID_CHECKSUM);

       error = reader.read("tooempty.tar", ar);
       ASSERT_TRUE(error == tar7z::T7ZE_NO_TRAILING_ZEROES);
   }

} reader_test;
