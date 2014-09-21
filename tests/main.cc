#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TextTestRunner.h>
#include <cppunit/TestResult.h>
#include <cppunit/BriefTestProgressListener.h>

int main()
{
	CppUnit::TestResult controller;
	CppUnit::BriefTestProgressListener progressListener;
	controller.addListener(&progressListener);

	CppUnit::TextTestRunner runner;
	CppUnit::TestFactoryRegistry & registry(
		CppUnit::TestFactoryRegistry::getRegistry());

	CppUnit::Test *test(registry.makeTest());

	runner.addTest(test);
	runner.run(controller);
	return 0;
}
