#include <iostream>
#include <cppunit/TestFixture.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCase.h>
#include <cppunit/ui/text/TestRunner.h>
#include "drzewo.hpp"

class MustHaveTests : public CppUnit::TestFixture
{
public:
	static CppUnit::Test *suite()
	{
		CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("MustHaveTests");
		suiteOfTests->addTest( new CppUnit::TestCaller<MustHaveTests>("constr_destr_test_empty", &MustHaveTests::constr_destr_test_empty) );
		suiteOfTests->addTest( new CppUnit::TestCaller<MustHaveTests>("constr_destr__test", &MustHaveTests::constr_destr_test) );
		suiteOfTests->addTest( new CppUnit::TestCaller<MustHaveTests>("constr_size_test_empty", &MustHaveTests::constr_size_test_empty) );
		return suiteOfTests;
	}
	
	void constr_destr_test_empty()
	{
		Drzewo<int>* temp1;
		Drzewo<char>* temp2;
		Drzewo<double>* temp3;
		for(int i=0; i<10; i++)
		{
			temp1 = new Drzewo<int>();
			temp2 = new Drzewo<char>();
			temp3 = new Drzewo<double>();
			delete temp1;
			delete temp2;
			delete temp3;
		}
		CPPUNIT_ASSERT(true);
	}
	
	void constr_destr_test()
	{
		Drzewo<int>* temp1;
		Drzewo<char>* temp2;
		Drzewo<double>* temp3;
		for(int i=0; i<10; i++)
		{
			temp1 = new Drzewo<int>(2017);
			temp2 = new Drzewo<char>('t');
			temp3 = new Drzewo<double>(19.95);
			delete temp1;
			delete temp2;
			delete temp3;
		}
		CPPUNIT_ASSERT(true);
	}
	
	void constr_size_test_empty()
	{
		Drzewo<int>* drz1 = new Drzewo<int>();
		Drzewo<int>* drz2 = new Drzewo<int>(22);
		CPPUNIT_ASSERT_EQUAL(true, drz1->size() == 0);
		CPPUNIT_ASSERT_EQUAL(true, drz1->empty());
		CPPUNIT_ASSERT_EQUAL(true, drz2->size() == 1);
		CPPUNIT_ASSERT_EQUAL(false, drz2->empty());
	}
};

class ConstructorTests : public CppUnit::TestFixture
{
	Drzewo<int>* t1;
	Drzewo<double>* t2;
	Drzewo<char>* t3;
public:
	static CppUnit::Test *suite()
	{
		CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("ConstructorTests");
		suiteOfTests->addTest( new CppUnit::TestCaller<ConstructorTests>("constr_test", &ConstructorTests::constr_test ) );
		return suiteOfTests;
	}

	void setUp() 
	{
		t1 = new Drzewo<int>();
		t2 = new Drzewo<double>();
		t3 = new Drzewo<char>();
	}

	void tearDown()
	{
		delete t1;
		delete t2;
		delete t3;
	}		 

	void constr_test()
	{
	
	}
};

int main()
{
	CppUnit::TextUi::TestRunner runner;
	runner.addTest(MustHaveTests::suite());
	//runner.addTest(ConstructorTests::suite());
	runner.run();
}
