#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include <lccc/cc.h>

namespace unittests {
namespace cc {

class test : public CppUnit::TestCase {
public:
	test();
	void setUp();
	void tearDown();

private:
	void test_namespace();
	void test_block();
	void test_method();
	void test_method_virtual();
	void test_method_abstract();
	void test_method_const();
	void test_method_args();
	void test_method_definition();
	void test_base_class();
	void test_class();
	void test_visibility();

	CPPUNIT_TEST_SUITE(test);
	CPPUNIT_TEST(test_namespace);
	CPPUNIT_TEST(test_block);
	CPPUNIT_TEST(test_method);
	CPPUNIT_TEST(test_method_virtual);
	CPPUNIT_TEST(test_method_abstract);
	CPPUNIT_TEST(test_method_const);
	CPPUNIT_TEST(test_method_args);
	CPPUNIT_TEST(test_method_definition);
	CPPUNIT_TEST(test_base_class);
	CPPUNIT_TEST(test_class);
	CPPUNIT_TEST(test_visibility);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(test);

test::test()
{ }

void test::setUp()
{ }

void test::tearDown()
{ }

void test::test_namespace()
{
	lccc::cc_namespace::ptr_t ns1(lccc::cc_namespace::make("foo"));
	lccc::cc_namespace::ptr_t ns2(lccc::cc_namespace::make("bar"));
	lccc::cc_namespace::ptr_t ns3(lccc::cc_namespace::make("baz"));
	ns1->add(ns2);
	ns2->add(ns3);
	std::stringstream out;
	ns1->print(out);
	std::string expected(
		"namespace foo {\n"
		"namespace bar {\n"
		"namespace baz {\n"
		"}\n"
		"}\n"
		"}\n"
	);
	CPPUNIT_ASSERT_EQUAL(expected, out.str());
}

void test::test_block()
{
	lccc::cc_block::ptr_t src(lccc::cc_block::make());
	src->src() << "do_foo();\n";
	src->src() << "do_bar();\n";
	src->src() << "do_baz();\n";
	std::stringstream out;
	src->print(out);
	std::string expected(
		"{\n"
		"\tdo_foo();\n"
		"\tdo_bar();\n"
		"\tdo_baz();\n"
		"}\n"
	);
	CPPUNIT_ASSERT_EQUAL(expected, out.str());
}

void test::test_method()
{
	lccc::cc_method::ptr_t src(lccc::cc_method::make("int", "foo"));
	std::stringstream out;
	src->print(out);
	std::string expected("int foo();\n");
	CPPUNIT_ASSERT_EQUAL(expected, out.str());
}

void test::test_method_virtual()
{
	lccc::cc_method::ptr_t src(lccc::cc_method::make("int", "foo"));
	src->make_virtual();
	std::stringstream out;
	src->print(out);
	std::string expected("virtual int foo();\n");
	CPPUNIT_ASSERT_EQUAL(expected, out.str());
}

void test::test_method_abstract()
{
	lccc::cc_method::ptr_t src(lccc::cc_method::make("int", "foo"));
	src->make_abstract();
	std::stringstream out;
	src->print(out);
	std::string expected("virtual int foo() = 0;\n");
	CPPUNIT_ASSERT_EQUAL(expected, out.str());
}

void test::test_method_const()
{
	lccc::cc_method::ptr_t src(lccc::cc_method::make("int", "foo"));
	src->make_const();
	std::stringstream out;
	src->print(out);
	std::string expected("int foo() const;\n");
	CPPUNIT_ASSERT_EQUAL(expected, out.str());
}

void test::test_method_args()
{
	lccc::cc_method::ptr_t src(lccc::cc_method::make("void", "foo"));
	src->add_arg("int", "f1");
	src->add_arg("std::string const&", "f2");
	std::stringstream out;
	src->print(out);
	std::string expected("void foo(int, std::string const&);\n");
	CPPUNIT_ASSERT_EQUAL(expected, out.str());
}

void test::test_method_definition()
{
	lccc::cc_method::ptr_t src(lccc::cc_method::make("void", "foo"));
	src->add_arg("std::string const&", "str");
	lccc::cc_block::ptr_t bl(lccc::cc_block::make());
	bl->src() << "do_foo(str);\n";
	src->define(bl);

	std::stringstream out;
	src->print(out);
	std::string expected(
		"void foo(std::string const& str)\n"
		"{\n"
		"\tdo_foo(str);\n"
		"}\n\n"
	);
	CPPUNIT_ASSERT_EQUAL(expected, out.str());
}

void test::test_base_class()
{
	lccc::cc_base_class::ptr_t src(lccc::cc_base_class::make("foo"));

	{
		std::stringstream out;
		src->print(out);
		std::string expected("foo");
		CPPUNIT_ASSERT_EQUAL(expected, out.str());
	}

	{
		std::stringstream out;
		lccc::cc_base_class::initializer::ptr_t init(src->make_initializer("42"));
		init->print(out);
		std::string expected("foo(42)");
		CPPUNIT_ASSERT_EQUAL(expected, out.str());
	}
}

void test::test_class()
{
	lccc::cc_class::ptr_t src(lccc::cc_class::make("foo"));

	std::stringstream out;
	src->print(out);
	std::string expected(
		"class foo {\n"
		"};\n"
	);
	CPPUNIT_ASSERT_EQUAL(expected, out.str());
}

void test::test_visibility()
{
	lccc::cc_class::ptr_t src(lccc::cc_class::make("foo"));
	src->vpublic()->add(lccc::cc_method::make("void", "foo_public"));
	src->vprivate()->add(lccc::cc_method::make("void", "foo_private"));
	src->vprotected()->add(lccc::cc_method::make("void", "foo_protected"));

	std::stringstream out;
	src->print(out);
	std::string expected(
		"class foo {\n"
		"public:\n"
		"\tvoid foo_public();\n"
		"protected:\n"
		"\tvoid foo_protected();\n"
		"private:\n"
		"\tvoid foo_private();\n"
		"};\n"
	);
	CPPUNIT_ASSERT_EQUAL(expected, out.str());
}

}}
