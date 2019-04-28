#include <Ice/Ice.h>
#include<iostream>
#include <test.h>

using namespace std;
using namespace mdds;

class Cookie:public Ice::LocalObject
{
public:
    Cookie(int seqId):m_seqId(seqId) {}
    int getSeqId()
    {
        return m_seqId;
    }
private:
    int m_seqId;
};

typedef IceUtil::Handle<Cookie> CookiePtr;

class fddsCallbackShared : public IceUtil::Shared
{
public:
    void response(::Ice::Int outb, ::Ice::Int resVal, const CookiePtr& reqId)
    {
        cout<<"response cb invoke reqid : "<< reqId->getSeqId()<<" res val :" << resVal <<" outb val: " << outb<<endl;
    }

    void exception(const Ice::Exception& ex, const CookiePtr& reqId)
    {
        cerr << "exception invoke reqId :"<<reqId->getSeqId()<<" call failed:\n" << ex << endl;
    }

    void send(bool xSuccess, const CookiePtr& reqId)
    {
        cout<<"send cb invoke reqid : "<< reqId->getSeqId()<<endl;
    }
};
typedef IceUtil::Handle<fddsCallbackShared> CallbackSharedPtr;


/*
class fddsCallback
{
public:

    void response(::Ice::Int outb, ::Ice::Int resVal, const int& reqId)
    {
    cout<<"response cb invoke reqid : "<< reqId <<" res val :" << resVal <<" outb val: " << outb<<endl;
    }

    void exception(const Ice::Exception& ex, const int& reqId)
    {
        cerr << "exception invoke reqId :"<<reqId<<" call failed:\n" << ex << endl;
    }

    void send(bool xSuccess, const int& reqId){
    cout<<"send cb invoke reqid : "<< reqId <<endl;
    }
};
typedef fddsCallback        Callback;
*/

int
main(int argc, char* argv[])
{

    int status = 0;
    Ice::CommunicatorPtr ic;
    try {
        ic = Ice::initialize(argc, argv);

  //      { 
  //          Ice::ObjectPrx base = ic->stringToProxy("SimplePrinter2");
  //          iddsPrx prx= iddsPrx::checkedCast(base);
  //          if (!prx)
  //              throw "Invalid proxy";
  //          int a = 2, b = 0;
  //          int reqId = 0;
  //          CallbackSharedPtr cbSharedPtr = new fddsCallbackShared();
  //          for( int i = 0; i <1; i++ ) {
  //              CookiePtr cookiePtr = new Cookie(i);
  //              //  Ice::Context ctx;
  //              //  ctx["type"] = i;
  //              prx->begin_fdds(a, newCallback_idds_fdds(cbSharedPtr, &fddsCallbackShared::response, &fddsCallbackShared::exception, &fddsCallbackShared::send), cookiePtr);
  //          }
  //      }

      { 
          Ice::ObjectPrx base = ic->stringToProxy("SimplePrinter");
          iddsPrx prx= iddsPrx::checkedCast(base);
          if (!prx)
              throw "Invalid proxy";
          int a = 2, b = 0;
          int reqId = 0;
          CallbackSharedPtr cbSharedPtr = new fddsCallbackShared();
          for( int i = 0; i <10; i++ ) {
              CookiePtr cookiePtr = new Cookie(i);
              //  Ice::Context ctx;
              //  ctx["type"] = i;
              prx->begin_fdds(a, newCallback_idds_fdds(cbSharedPtr, &fddsCallbackShared::response, &fddsCallbackShared::exception, &fddsCallbackShared::send), cookiePtr);
          }
      }

//        {// for muliti node  test code 
//            Ice::ObjectPrx base = ic->stringToProxy("SimplePrinter@TestServer1.SimplePrinterAdapter");
//            iddsPrx prx= iddsPrx::checkedCast(base);
//            if (!prx)
//                throw "Invalid proxy";
//            int a = 2, b = 0;
//            int reqId = 0;
//            CallbackSharedPtr cbSharedPtr = new fddsCallbackShared();
//            for( int i = 0; i <1; i++ ) {
//                CookiePtr cookiePtr = new Cookie(i);
//                //  Ice::Context ctx;
//                //  ctx["type"] = i;
//                prx->begin_fdds(a, newCallback_idds_fdds(cbSharedPtr, &fddsCallbackShared::response, &fddsCallbackShared::exception, &fddsCallbackShared::send), cookiePtr);
//            }
//        }
//
//        {
//            Ice::ObjectPrx base = ic->stringToProxy("SimplePrinter@TestServer2.SimplePrinterAdapter");
//            iddsPrx prx= iddsPrx::checkedCast(base);
//            if (!prx)
//                throw "Invalid proxy";
//            int a = 2, b = 0;
//            int reqId = 0;
//            CallbackSharedPtr cbSharedPtr = new fddsCallbackShared();
//            for( int i = 0; i <1; i++ ) {
//                CookiePtr cookiePtr = new Cookie(i);
//                //  Ice::Context ctx;
//                //  ctx["type"] = i;
//                prx->begin_fdds(a, newCallback_idds_fdds(cbSharedPtr, &fddsCallbackShared::response, &fddsCallbackShared::exception, &fddsCallbackShared::send), cookiePtr);
//            }
//        }
//
//        {
//            Ice::ObjectPrx base = ic->stringToProxy("SimplePrinter@TestServer3.SimplePrinterAdapter");
//            iddsPrx prx= iddsPrx::checkedCast(base);
//            if (!prx)
//                throw "Invalid proxy";
//            int a = 2, b = 0;
//            int reqId = 0;
////        int c =  prx->fdds(a,b);
////  IceInternal::Handle<Ice::AsyncResult> ret = prx->begin_fdds(a);
////  int resVal =  prx->end_fdds(b, ret);
////       cout<<"this is c res "<< resVal <<" a:" << a <<" b:"<<b<<endl;
////
//            CallbackSharedPtr cbSharedPtr = new fddsCallbackShared();
//            for( int i = 0; i <1; i++ ) {
//                CookiePtr cookiePtr = new Cookie(i);
//                //  Ice::Context ctx;
//                //  ctx["type"] = i;
//                prx->begin_fdds(a, newCallback_idds_fdds(cbSharedPtr, &fddsCallbackShared::response, &fddsCallbackShared::exception, &fddsCallbackShared::send), cookiePtr);
//            }
//        }
        sleep(5);
    } catch (const Ice::Exception& ex) {
        cerr << ex << endl;
        status = 1;
    } catch (const char* msg) {
        cerr << msg << endl;
        status = 1;
    }
    if (ic)
        ic->destroy();
    return status;
}
