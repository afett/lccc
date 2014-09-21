#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/TestResult.h>
#include <cppunit/TextTestRunner.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

int main()
{
	CppUnit::TextTestRunner runner;
	CppUnit::TestFactoryRegistry & registry(
		CppUnit::TestFactoryRegistry::getRegistry());
	CppUnit::Test *test(registry.makeTest());
	runner.addTest(test);

	CppUnit::BriefTestProgressListener progress;
	runner.eventManager().addListener(&progress);

	return runner.run() ? 0 : 1;
}
