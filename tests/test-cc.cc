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
	void test_class_inheritance();
	void test_visibility();
	void test_constructor();
	void test_constructor_initializer();
	void test_destructor();
	void test_virtual_destructor();
	void test_member();

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
	CPPUNIT_TEST(test_class_inheritance);
	CPPUNIT_TEST(test_visibility);
	CPPUNIT_TEST(test_constructor);
	CPPUNIT_TEST(test_constructor_initializer);
	CPPUNIT_TEST(test_destructor);
	CPPUNIT_TEST(test_virtual_destructor);
	CPPUNIT_TEST(test_member);
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
	auto ns1(lccc::cc_namespace::make("foo"));
	auto ns2(lccc::cc_namespace::make("bar"));
	auto ns3(lccc::cc_namespace::make("baz"));
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
	auto src(lccc::cc_block::make());
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
	auto src(lccc::cc_method::make("int", "foo"));
	std::stringstream out;
	src->print(out);
	std::string expected("int foo();\n\n");
	CPPUNIT_ASSERT_EQUAL(expected, out.str());
}

void test::test_method_virtual()
{
	auto src(lccc::cc_method::make("int", "foo"));
	src->make_virtual();
	std::stringstream out;
	src->print(out);
	std::string expected("virtual int foo();\n\n");
	CPPUNIT_ASSERT_EQUAL(expected, out.str());
}

void test::test_method_abstract()
{
	auto src(lccc::cc_method::make("int", "foo"));
	src->make_abstract();
	std::stringstream out;
	src->print(out);
	std::string expected("virtual int foo() = 0;\n\n");
	CPPUNIT_ASSERT_EQUAL(expected, out.str());
}

void test::test_method_const()
{
	auto src(lccc::cc_method::make("int", "foo"));
	src->make_const();
	std::stringstream out;
	src->print(out);
	std::string expected("int foo() const;\n\n");
	CPPUNIT_ASSERT_EQUAL(expected, out.str());
}

void test::test_method_args()
{
	auto src(lccc::cc_method::make("void", "foo"));
	src->add_arg("int", "f1");
	src->add_arg("std::string const&", "f2");
	std::stringstream out;
	src->print(out);
	std::string expected("void foo(int, std::string const&);\n\n");
	CPPUNIT_ASSERT_EQUAL(expected, out.str());
}

void test::test_method_definition()
{
	auto src(lccc::cc_method::make("void", "foo"));
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
	auto src(lccc::cc_base_class::make("foo"));

	{
		std::stringstream out;
		src->print(out);
		std::string expected("foo");
		CPPUNIT_ASSERT_EQUAL(expected, out.str());
	}

	{
		std::stringstream out;
		auto init(src->make_initializer("42"));
		init->print(out);
		std::string expected("foo(42)");
		CPPUNIT_ASSERT_EQUAL(expected, out.str());
	}
}

void test::test_class()
{
	auto src(lccc::cc_class::make("foo"));

	std::stringstream out;
	src->print(out);
	std::string expected(
		"class foo {\n"
		"};\n"
	);
	CPPUNIT_ASSERT_EQUAL(expected, out.str());
}

void test::test_class_inheritance()
{
	auto src(lccc::cc_class::make("foo"));
	auto base1(lccc::cc_base_class::make("bar"));
	auto base2(lccc::cc_base_class::make("baz"));
	src->add(base1);
	src->add(base2);

	std::stringstream out;
	src->print(out);
	std::string expected(
		"class foo :\n"
		"\tpublic bar,\n"
		"\tpublic baz\n"
		"{\n"
		"};\n"
	);
	CPPUNIT_ASSERT_EQUAL(expected, out.str());
}

void test::test_visibility()
{
	auto src(lccc::cc_class::make("foo"));
	src->vpublic()->add(lccc::cc_method::make("void", "foo_public"));
	src->vprivate()->add(lccc::cc_method::make("void", "foo_private"));
	src->vprotected()->add(lccc::cc_method::make("void", "foo_protected"));

	std::stringstream out;
	src->print(out);
	std::string expected(
		"class foo {\n"
		"public:\n"
		"\tvoid foo_public();\n"
		"\n"
		"protected:\n"
		"\tvoid foo_protected();\n"
		"\n"
		"private:\n"
		"\tvoid foo_private();\n"
		"\n"
		"};\n"
	);
	CPPUNIT_ASSERT_EQUAL(expected, out.str());
}

void test::test_constructor()
{
	auto src(lccc::cc_class::make("foo"));
	auto ctor(src->make_constructor());
	ctor->add_arg("std::string const&", "str");
	auto bl(lccc::cc_block::make());
	bl->src() << "do_foo(str);\n";
	ctor->define(bl);

	std::stringstream out;
	ctor->print(out);
	std::string expected(
		"foo(std::string const& str)\n"
		"{\n"
		"\tdo_foo(str);\n"
		"}\n\n"
	);
	CPPUNIT_ASSERT_EQUAL(expected, out.str());
}

void test::test_constructor_initializer()
{
	auto src(lccc::cc_class::make("foo"));
	auto ctor(src->make_constructor());

	auto base1(lccc::cc_base_class::make("bar"));
	auto base2(lccc::cc_base_class::make("baz"));

	ctor->add(base1->make_initializer("42"));
	ctor->add(base2->make_initializer("\"hello\""));
	auto bl(lccc::cc_block::make());
	ctor->define(bl);

	std::stringstream out;
	ctor->print(out);
	std::string expected(
		"foo()\n"
		":\n"
		"\tbar(42),\n"
		"\tbaz(\"hello\")\n"
		"{\n"
		"}\n\n"
	);
	CPPUNIT_ASSERT_EQUAL(expected, out.str());
}

void test::test_destructor()
{
	auto src(lccc::cc_class::make("foo"));
	auto dtor(src->make_destructor());
	auto bl(lccc::cc_block::make());
	bl->src() << "do_foo(str);\n";
	dtor->define(bl);

	std::stringstream out;
	dtor->print(out);
	std::string expected(
		"~foo()\n"
		"{\n"
		"\tdo_foo(str);\n"
		"}\n\n"
	);
	CPPUNIT_ASSERT_EQUAL(expected, out.str());
}

void test::test_virtual_destructor()
{
	auto src(lccc::cc_class::make("foo"));
	auto dtor(src->make_destructor());
	dtor->make_virtual();
	auto bl(lccc::cc_block::make());
	bl->src() << "do_foo(str);\n";
	dtor->define(bl);

	std::stringstream out;
	dtor->print(out);
	std::string expected(
		"virtual ~foo()\n"
		"{\n"
		"\tdo_foo(str);\n"
		"}\n\n"
	);
	CPPUNIT_ASSERT_EQUAL(expected, out.str());
}

void test::test_member()
{
	auto src(lccc::cc_member::make("void *", "bar_"));

	std::stringstream out;
	src->print(out);
	std::string expected("void * bar_;\n");
	CPPUNIT_ASSERT_EQUAL(expected, out.str());
}

}}
