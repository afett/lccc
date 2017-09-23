#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include <lccc/cpp.h>
#include <lccc/header.h>

namespace unittests {
namespace cpp {

class test : public CppUnit::TestCase {
public:
	test();
	void setUp();
	void tearDown();

private:
	void test_define();
	void test_include();
	void test_ifdef();
	void test_ifndef();
	void test_guard();
	void test_header();

	CPPUNIT_TEST_SUITE(test);
	CPPUNIT_TEST(test_define);
	CPPUNIT_TEST(test_include);
	CPPUNIT_TEST(test_ifdef);
	CPPUNIT_TEST(test_ifndef);
	CPPUNIT_TEST(test_guard);
	CPPUNIT_TEST(test_header);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(test);

test::test()
{ }

void test::setUp()
{ }

void test::tearDown()
{ }

void test::test_define()
{
	auto def(lccc::cpp_define::make("FOO"));
	std::stringstream out;
	def->print(out);
	std::string expected("#define FOO\n");
	CPPUNIT_ASSERT_EQUAL(expected, out.str());
}

void test::test_include()
{
	auto def(lccc::cpp_include::make("foo.h"));
	std::stringstream out;
	def->print(out);
	std::string expected("#include<foo.h>\n");
	CPPUNIT_ASSERT_EQUAL(expected, out.str());
}

void test::test_ifdef()
{
	auto def(lccc::cpp_ifdef::make("FOOBAR"));
	def->add(lccc::cpp_define::make("FOO"));
	std::stringstream out;
	def->print(out);
	std::string expected(
		"#ifdef FOOBAR\n"
		"#define FOO\n"
		"#endif\n"
	);
	CPPUNIT_ASSERT_EQUAL(expected, out.str());
}

void test::test_ifndef()
{
	auto def(lccc::cpp_ifndef::make("FOOBAR"));
	def->add(lccc::cpp_define::make("FOO"));
	std::stringstream out;
	def->print(out);
	std::string expected(
		"#ifndef FOOBAR\n"
		"#define FOO\n"
		"#endif\n"
	);
	CPPUNIT_ASSERT_EQUAL(expected, out.str());
}

void test::test_guard()
{
	auto def(lccc::cpp_guard::make("FOO_H"));
	def->add(lccc::cpp_define::make("FOO"));
	std::stringstream out;
	def->print(out);
	std::string expected(
		"#ifndef FOO_H\n"
		"#define FOO_H\n"
		"#define FOO\n"
		"#endif\n"
	);
	CPPUNIT_ASSERT_EQUAL(expected, out.str());
}

void test::test_header()
{
	auto def(lccc::header::make("lccc/cc_src.h"));
	def->add(lccc::cpp_define::make("FOO"));
	std::stringstream out;
	def->print(out);
	std::string expected(
		"#ifndef LCCC_CC_SRC_H\n"
		"#define LCCC_CC_SRC_H\n"
		"#define FOO\n"
		"#endif\n"
	);
	CPPUNIT_ASSERT_EQUAL(expected, out.str());
}

}}

