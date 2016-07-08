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
struct WriterTest : tpunit::TestFixture
{
 public:
   WriterTest() : tpunit::TestFixture(
           TEST(WriterTest::test_empty),
           TEST(WriterTest::test_one_simple_file),
           TEST(WriterTest::test_four_files_with_short_names)
   ) {}

   void test_empty()
   {
      tar7z::Archive ar;
      tar7z::Writer writer;
      ASSERT_TRUE(writer.write("empty.tar", ar) == tar7z::T7ZE_OK);
      ASSERT_TRUE(this->compareFiles("empty.tar", "empty_ethalon.tar"));
   }


   void test_one_simple_file()
   {
       tar7z::Archive ar;

       ar.add("nicesmallname.txt", this->stringToUcharVector("this is a nice block content!"));

       tar7z::Writer writer;
       ASSERT_TRUE(writer.write("simple.tar", ar) == tar7z::T7ZE_OK);
   }

   void test_four_files_with_short_names()
   {
       tar7z::Archive ar;

       ar.add("nicesmallname.txt", this->stringToUcharVector("this is a nice block content!"));
       ar.add("thisfileisintentially100characterslongandweshouldcareaboutitbecausewemustalsohandlelinksandallstuff!", this->stringToUcharVector(std::string(512, 'm')));
       ar.add("ignoreme.txt", std::vector<char>());
       ar.add("ignoreme2.txt", std::vector<char>());

       tar7z::Writer writer;
       ASSERT_TRUE(writer.write("shortnames.tar", ar) == tar7z::T7ZE_OK);
   }

   /*! Compares two files
       \param[in] f1 first file
       \param[in] f2 second file
       \return whether they are equal
    */
   bool compareFiles(const std::string& f1, const std::string& f2)
   {
       std::ifstream t1(f1.c_str());
       std::vector<char> v1((std::istreambuf_iterator<char>(t1)),
                             std::istreambuf_iterator<char>());

       std::ifstream t2(f2.c_str());
       std::vector<char> v2((std::istreambuf_iterator<char>(t2)),
                             std::istreambuf_iterator<char>());
       return v1 == v2;
   }

   /**
    * Returns string  as unsigned char vector
    * @param[in] s string
    * @return vector
    */
   std::vector<char> stringToUcharVector(const std::string& s)
   {
       std::vector<char> v;
       v.insert(v.end(), s.begin(), s.end());
       return v;
   }

} writer_test;
