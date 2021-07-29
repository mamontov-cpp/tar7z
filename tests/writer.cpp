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
           TEST(WriterTest::test_four_files_with_short_names),
           TEST(WriterTest::test_uchar_vector),
           TEST(WriterTest::test_longname_in_beginning),
           TEST(WriterTest::test_longname_in_center),
           TEST(WriterTest::test_longname_in_end)
   ) {}

   void test_empty()
   {
      tar7z::Archive ar;
      tar7z::Writer writer;
      ASSERT_TRUE(writer.write("empty.tar", ar) == tar7z::Error::T7ZE_OK);
      ASSERT_TRUE(this->filesAreEqual("empty.tar", "empty_ethalon.tar"));
   }


   void test_one_simple_file()
   {
       tar7z::Archive ar;

       ar.add("nicesmallname.txt", this->stringToUcharVector("this is a nice block content!"), true);

       tar7z::Writer writer;
       ASSERT_TRUE(writer.write("simple.tar", ar) == tar7z::Error::T7ZE_OK);
       ASSERT_TRUE(this->filesAreEqual("simple.tar", "simple_ethalon.tar"));
   }

   void test_four_files_with_short_names()
   {
       tar7z::Archive ar;

       ar.add("nicesmallname.txt", this->stringToUcharVector("this is a nice block content!"), true);
       ar.add("thisfileisintentially100characterslongandweshouldcareaboutitbecausewemustalsohandlelinksandallstuff!", this->stringToUcharVector(std::string(512, 'm')), true);
       ar.add("ignoreme.txt", std::vector<char>(), true);
       ar.add("ignoreme2.txt", std::vector<char>(), true);

       tar7z::Writer writer;
       ASSERT_TRUE(writer.write("shortnames.tar", ar) == tar7z::Error::T7ZE_OK);
       ASSERT_TRUE(this->filesAreEqual("shortnames.tar", "shortnames_ethalon.tar"));
   }

   void test_uchar_vector()
   {
       tar7z::Archive ar;
       std::vector<unsigned char> ucharvector;
       std::string buffer(613, '!');
       ucharvector.insert(ucharvector.end(), buffer.begin(), buffer.end());
       ar.add("test.txt", ucharvector, true);
       ar.add("ignoreme.txt", std::vector<char>(), true);
       ar.add("ignoreme2.txt", std::vector<char>(), true);

       tar7z::Writer writer;
       ASSERT_TRUE(writer.write("ucharvector.tar", ar) == tar7z::Error::T7ZE_OK);
       ASSERT_TRUE(this->filesAreEqual("ucharvector.tar", "ucharvector_ethalon.tar"));
   }

   void test_longname_in_beginning()
   {
       tar7z::Archive ar;
       ar.add("this file is intentionally longer than one hundred of characters because we need to test long names no part should be changed.txt", this->stringToUcharVector("this is content of file with long name"), true);
       ar.add("testfile.txt", this->stringToUcharVector("short content"), true);
       ar.add("testfile1.txt", this->stringToUcharVector("very short content"), true);

       tar7z::Writer writer;
       ASSERT_TRUE(writer.write("longnameinbeginning.tar", ar) == tar7z::Error::T7ZE_OK);
       ASSERT_TRUE(this->filesAreEqual("longnameinbeginning.tar", "longnameinbeginning_ethalon.tar"));
   }

   void test_longname_in_center()
   {
       tar7z::Archive ar;
       ar.add("testfile.txt", this->stringToUcharVector("short content"), true);
       ar.add("this file is intentionally longer than one hundred of characters because we need to test long names no part should be changed.txt", this->stringToUcharVector("this is content of file with long name"), true);
       ar.add("this is copy of file is intentionally longer than one hundred of characters because we need to test long names no part should be changed.txt", this->stringToUcharVector("this is content of second file with long name"), true);
       ar.add("testfile1.txt", this->stringToUcharVector("very short content"), true);

       tar7z::Writer writer;
       ASSERT_TRUE(writer.write("longnameincenter.tar", ar) == tar7z::Error::T7ZE_OK);
       ASSERT_TRUE(this->filesAreEqual("longnameincenter.tar", "longnameincenter_ethalon.tar"));
   }

   void test_longname_in_end()
   {
       tar7z::Archive ar;
       ar.add("testfile.txt", this->stringToUcharVector("short content"), true);
       ar.add("testfile1.txt", this->stringToUcharVector("very short content"), true);
       ar.add("this file is intentionally longer than one hundred of characters because we need to test long names no part should be changed.txt", this->stringToUcharVector("this is content of file with long name"), true);
       ar.add("this is copy of file is intentionally longer than one hundred of characters because we need to test long names no part should be changed.txt", this->stringToUcharVector("this is content of second file with long name"), true);

       tar7z::Writer writer;
       ASSERT_TRUE(writer.write("longnameinend.tar", ar) == tar7z::Error::T7ZE_OK);
       ASSERT_TRUE(this->filesAreEqual("longnameinend.tar", "longnameinend_ethalon.tar"));
   }

   /*! Compares two files and returns true if they are equal
       \param[in] f1 first file
       \param[in] f2 second file
       \return whether they are equal
    */
   bool filesAreEqual(const std::string& f1, const std::string& f2)
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
