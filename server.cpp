#include <Ice/Ice.h>
#include<thread>
#include<iostream>
#include<list>
#include <test.h>
 
using namespace std;
using namespace mdds;

 

struct FddsTask {
	FddsTask(const ::mdds::AMD_idds_fddsPtr& cb, ::Ice::Int p): m_cb(cb), m_p(p) {
	}

	FddsTask(const FddsTask& other) {
		m_cb = other.m_cb;
		m_p = other.m_p;
	}
	
	void exec() {
		cout<<"do task impl"<<endl;
		m_cb->ice_response(m_p*2, 2); 
	}
	
	::mdds::AMD_idds_fddsPtr m_cb;
	::Ice::Int m_p;
};


class TestI: virtual public   idds, virtual public IceUtil::Mutex
{
public:
//	::Ice::Int fdds(::Ice::Int __p_a, ::Ice::Int& __p_b, const Ice::Current&);

    virtual void fdds_async(const ::mdds::AMD_idds_fddsPtr&, ::Ice::Int, const ::Ice::Current& ctx = ::Ice::Current());
	
    void  doTask();

private:
   list<FddsTask> m_pList;
	
};

void TestI::fdds_async(const ::mdds::AMD_idds_fddsPtr& amdPtr, ::Ice::Int p, const ::Ice::Current& ctx) 
{
	cout<<"fdds_async invoke param  a :"<<p<<endl;
	IceUtil::Mutex::Lock sync(*this);
	FddsTask task(amdPtr, p);
	m_pList.push_back(task);
//	amdPtr->ice_response(1,2);
	return;
}
 

void TestI::doTask() {
	cout<<"fdds_async invoke for task :"<<endl;
	bool empty = false;
	while(1) {
		{
			IceUtil::Mutex::Lock sync(*this);
			if( (empty = m_pList.empty()) != true) {
				FddsTask task = std::move(m_pList.front());
				m_pList.pop_front();
				task.exec();
			}
		}	
		if(empty) {
			cout<<"sleep 1s"<<endl;
			sleep(1);
		}
	}
	return;
	
}
int
main(int argc, char* argv[])
{
    int status = 0;
    Ice::CommunicatorPtr ic;
    try {
        ic = Ice::initialize(argc, argv);
        Ice::ObjectAdapterPtr adapter =
            ic->createObjectAdapter("SimplePrinterAdapter");
	TestI* testiPtr = new TestI();
        Ice::ObjectPtr object = testiPtr;
	thread t1 ( [&testiPtr](){
		testiPtr->doTask();
	});
	
        adapter->add(object, ic->stringToIdentity("SimplePrinter"));
        adapter->activate();
        ic->waitForShutdown();
    } catch (const Ice::Exception& e) {
        cerr << e << endl;
        status = 1;
    } catch (const char* msg) {
        cerr << msg << endl;
        status = 1;
    }
    if (ic) {
        try {
            ic->destroy();
        } catch (const Ice::Exception& e) {
            cerr << e << endl;
            status = 1;
        }
    }
    return status;
}

