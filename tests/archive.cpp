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
           TEST(ArchiveTest::test_add),
           TEST(ArchiveTest::test_remove),
           TEST(ArchiveTest::test_add_replace),
           TEST(ArchiveTest::test_file)
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

   void test_remove()
   {
       {
           tar7z::Archive ar;
           ar.add("testname.txt", this->stringToUcharVector("test file"), true);
           ar.add("testname2.txt", this->stringToUcharVector("test file 2"), true);
           ar.add("testname3.txt", this->stringToUcharVector("test file 3"), true);

           ar.remove("nonexistingfile");
           ar.remove("testname2.txt");

           tar7z::Writer writer;
           ASSERT_TRUE(writer.write("remove1.tar", ar) == tar7z::Error::T7ZE_OK);
           ASSERT_TRUE(this->filesAreEqual("remove1.tar", "remove1_ethalon.tar"));
       }

       {
           tar7z::Archive ar;
           ar.add("testname.txt", this->stringToUcharVector("test file"), true);
           ar.add("testname2.txt", this->stringToUcharVector("test file 2"), true);
           ar.add("testname3.txt", this->stringToUcharVector("test file 3"), true);

           ar.remove("testname.txt");

           tar7z::Writer writer;
           ASSERT_TRUE(writer.write("remove2.tar", ar) == tar7z::Error::T7ZE_OK);
           ASSERT_TRUE(this->filesAreEqual("remove2.tar", "remove2_ethalon.tar"));
       }

       {
           tar7z::Archive ar;
           ar.add("testname.txt", this->stringToUcharVector("test file"), true);
           ar.add("testname2.txt", this->stringToUcharVector("test file 2"), true);
           ar.add("testname3.txt", this->stringToUcharVector("test file 3"), true);

           ar.remove("testname3.txt");

           tar7z::Writer writer;
           ASSERT_TRUE(writer.write("remove3.tar", ar) == tar7z::Error::T7ZE_OK);
           ASSERT_TRUE(this->filesAreEqual("remove3.tar", "remove3_ethalon.tar"));
       }

       {
           tar7z::Archive ar;
           std::string longname = "this file is intentionally longer than one hundred of characters because we need to test long names no part should be changed.txt";
           ar.add(longname, this->stringToUcharVector("test file"), true);
           ar.add("testname2.txt", this->stringToUcharVector("test file 2"), true);
           ar.add("testname3.txt", this->stringToUcharVector("test file 3"), true);

           ar.remove(longname);

           tar7z::Writer writer;
           ASSERT_TRUE(writer.write("remove4.tar", ar) == tar7z::Error::T7ZE_OK);
           ASSERT_TRUE(this->filesAreEqual("remove4.tar", "remove4_ethalon.tar"));
       }

       {
           tar7z::Archive ar;
           std::string longname = "this file is intentionally longer than one hundred of characters because we need to test long names no part should be changed.txt";
           ar.add("testname2.txt", this->stringToUcharVector("test file 2"), true);
           ar.add(longname, this->stringToUcharVector("test file"), true);
           ar.add("testname3.txt", this->stringToUcharVector("test file 3"), true);

           ar.remove(longname);

           tar7z::Writer writer;
           ASSERT_TRUE(writer.write("remove5.tar", ar) == tar7z::Error::T7ZE_OK);
           ASSERT_TRUE(this->filesAreEqual("remove5.tar", "remove5_ethalon.tar"));
       }

       {
           tar7z::Archive ar;
           std::string longname = "this file is intentionally longer than one hundred of characters because we need to test long names no part should be changed.txt";
           ar.add("testname2.txt", this->stringToUcharVector("test file 2"), true);
           ar.add("testname3.txt", this->stringToUcharVector("test file 3"), true);
           ar.add(longname, this->stringToUcharVector("test file"), true);

           ar.remove(longname);

           tar7z::Writer writer;
           ASSERT_TRUE(writer.write("remove6.tar", ar) == tar7z::Error::T7ZE_OK);
           ASSERT_TRUE(this->filesAreEqual("remove6.tar", "remove6_ethalon.tar"));
       }
   }

   void test_add_replace()
   {
       {
           tar7z::Archive ar;
           ar.add("testname2.txt", this->stringToUcharVector("test file 2"), true);
           ar.add("file.txt", this->stringToUcharVector("file source content"), true);
           ar.add("testname3.txt", this->stringToUcharVector("test file 3"), true);

           ar.add("file.txt", this->stringToUcharVector(std::string(680, '.')), true);

           tar7z::Writer writer;
           ASSERT_TRUE(writer.write("replace1.tar", ar) == tar7z::Error::T7ZE_OK);
           ASSERT_TRUE(this->filesAreEqual("replace1.tar", "replace1_ethalon.tar"));
       }

       {
           tar7z::Archive ar;
           std::string longname = "this file is intentionally longer than one hundred of characters because we need to test long names no part should be changed.txt";

           ar.add("testname2.txt", this->stringToUcharVector("test file 2"), true);
           ar.add(longname, this->stringToUcharVector("file source content"), true);
           ar.add("testname3.txt", this->stringToUcharVector("test file 3"), true);

           ar.add(longname, this->stringToUcharVector(std::string(680, '.')), true);

           tar7z::Writer writer;
           ASSERT_TRUE(writer.write("replace2.tar", ar) == tar7z::Error::T7ZE_OK);
           ASSERT_TRUE(this->filesAreEqual("replace2.tar", "replace2_ethalon.tar"));
       }
   }


   void test_file()
   {
       tar7z::Archive ar;
       std::string testcontents = "this is another data";
       ar.add("testname", this->stringToUcharVector("this is useless data, that will be overwritten anyway"));
       ar.add("testname", this->stringToUcharVector(testcontents));
       ASSERT_TRUE(ar.file("testname"));
       ASSERT_TRUE( memcmp(ar.file("testname")->contents(), &(testcontents[0]), testcontents.size()) == 0);
       ASSERT_TRUE(ar.file("dontcare") == NULL);
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
} _archive_test;
