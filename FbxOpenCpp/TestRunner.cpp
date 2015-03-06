﻿
#include "common.h"
#include <vector>

using namespace std;

class ScopedTestRun
{
public:
    ScopedTestRun(TestFixture* _test, TestCase* _testCase, std::string** _exception)
        : test(_test), testCase(_testCase), exception(_exception)
    {
        if (exception != NULL)
        {
            *exception = NULL;
        }
    }

    TestFixture* test;
    TestCase* testCase;
    std::string** exception;

    ~ScopedTestRun()
    {
        try
        {
            test->TearDown();
        }
        catch (std::string* e)
        {
            cout << "Error in tear down: " << *e << endl;
            if (exception != NULL)
            {
                *exception = e;
            }
            else
            {
                delete e;
            }
        }
        catch (char* e)
        {
            cout << "Error in tear down: " << e << endl;
            if (exception != NULL)
            {
                *exception = new string(e);
            }
        }
    }

    void RunTest()
    {
        cout << "    Running test " << testCase->Name << "... ";

        try
        {
            test->Setup();
        }
        catch (std::string* e)
        {
            cout << "Error in setup: " << *e << endl;
            if (exception != NULL)
            {
                *exception = e;
            }
            else
            {
                delete e;
            }
            return;
        }
        catch (char* e)
        {
            cout << "Error in setup: " << e << endl;
            if (exception != NULL)
            {
                *exception = new string(e);
            }
        }

        try
        {
            testCase->Function();
        }
        catch (std::string* e)
        {
            cout << *e << endl;
            if (exception != NULL)
            {
                *exception = e;
            }
            else
            {
                delete e;
            }
        }
        catch (char* e)
        {
            cout << e << endl;
            if (exception != NULL)
            {
                *exception = new string(e);
            }
        }
    }
};

void RunTests()
{
    vector<TestFixture*> tests;

    tests.push_back(new NodeTest());

    cout << "Running tests..." << endl;

    int i;
    for (i = 0; i < tests.size(); i++)
    {
        TestFixture* test = tests[i];

        cout << "  Running test class " << test->Name << endl;

        test->SetupFixture();

        int j;
        for (j = 0; j < test->TestCases.size(); j++)
        {
            TestCase* testCase = &test->TestCases[j];
            string* exception;
            {
                ScopedTestRun testRun(test, testCase, &exception);
                testRun.RunTest();
            }
            if (exception == NULL)
            {
                cout << "pass" << endl;
            }
        }

        test->TearDownFixture();
    }
}
