// btlmt_channelpoolchannel.t.cpp                                     -*-C++-*-

#include <btlmt_channelpoolchannel.h>

#include <btlmt_asyncchannel.h>
#include <btlmt_channelpool.h>
#include <btlmt_session.h>

#include <btlb_blob.h>
#include <btlb_blobutil.h>
#include <bslma_testallocator.h>
#include <bdlqq_mutex.h>
#include <bdlqq_threadutil.h>
#include <bdlqq_barrier.h>

#include <bdlf_function.h>
#include <bdlf_placeholder.h>
#include <bdlf_memfn.h>
#include <bdlf_bind.h>

#include <bslma_defaultallocatorguard.h>
#include <bslma_allocator.h>
#include <bslma_default.h>

#include <bslx_marshallingutil.h>

#include <btlso_ipv4address.h>
#include <btlso_inetstreamsocketfactory.h>
#include <btlso_streamsocket.h>
#include <btlsos_tcpconnector.h>

#include <bsl_c_stdlib.h>     // atoi()
#include <bsl_iostream.h>

using namespace BloombergLP;
using namespace bsl;  // automatically added by script

//=============================================================================
//                             TEST PLAN
// Create concrete implementations of the btlmt::Session, and
// btlmt::SessionFactory protocols and instantiate them.
//
//-----------------------------------------------------------------------------
//                              Overview
//                              --------
// This test driver simply verifies that a concrete instance of btlmt::Session
// and btlmt::SessionFactory can be implement and instantiated.
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// [1 ] BREATHING TEST
//=============================================================================
//                      STANDARD BDE ASSERT TEST MACRO
//-----------------------------------------------------------------------------
static int testStatus = 0;

static void aSsErT(int c, const char *s, int i)
{
    if (c) {
        cout << "Error " << __FILE__ << "(" << i << "): " << s
             << "    (failed)" << endl;
        if (0 <= testStatus && testStatus <= 100) ++testStatus;
    }
}

#define ASSERT(X) { aSsErT(!(X), #X, __LINE__); }

//=============================================================================
//                  STANDARD BDE LOOP-ASSERT TEST MACROS
//-----------------------------------------------------------------------------
#define LOOP_ASSERT(I,X) { \
   if (!(X)) { cout << #I << ": " << I << "\n"; aSsErT(1, #X, __LINE__); }}

#define LOOP2_ASSERT(I,J,X) { \
   if (!(X)) { cout << #I << ": " << I << "\t" << #J << ": " \
              << J << "\n"; aSsErT(1, #X, __LINE__); } }

#define LOOP3_ASSERT(I,J,K,X) { \
   if (!(X)) { cout << #I << ": " << I << "\t" << #J << ": " << J << "\t" \
              << #K << ": " << K << "\n"; aSsErT(1, #X, __LINE__); } }

#define LOOP4_ASSERT(I,J,K,L,X) { \
   if (!(X)) { cout << #I << ": " << I << "\t" << #J << ": " << J << "\t" << \
       #K << ": " << K << "\t" << #L << ": " << L << "\n"; \
       aSsErT(1, #X, __LINE__); } }

#define LOOP5_ASSERT(I,J,K,L,M,X) { \
   if (!(X)) { cout << #I << ": " << I << "\t" << #J << ": " << J << "\t" << \
       #K << ": " << K << "\t" << #L << ": " << L << "\t" << \
       #M << ": " << M << "\n"; \
       aSsErT(1, #X, __LINE__); } }

#define LOOP6_ASSERT(I,J,K,L,M,N,X) { \
   if (!(X)) { cout << #I << ": " << I << "\t" << #J << ": " << J << "\t" << \
       #K << ": " << K << "\t" << #L << ": " << L << "\t" << \
       #M << ": " << M << "\t" << #N << ": " << N << "\n"; \
       aSsErT(1, #X, __LINE__); } }

//=============================================================================
//                  SEMI-STANDARD TEST OUTPUT MACROS
//-----------------------------------------------------------------------------
#define P(X) cout << #X " = " << (X) << endl; // Print identifier and value.
#define Q(X) cout << "<| " #X " |>" << endl;  // Quote identifier literally.
#define NL() cout << endl;                    // End of line
#define P_(X) cout << #X " = " << (X) << ", "<< flush; // P(X) without '\n'
#define T_()  cout << '\t' << flush;          // Print tab w/o newline
#define L_ __LINE__                           // current Line number

//=============================================================================
//                  SUPPORT CLASSES AND FUNCTIONS USED FOR TESTING
//-----------------------------------------------------------------------------

//=============================================================================
//          USAGE example from header(with assert replaced with ASSERT)
//-----------------------------------------------------------------------------
//

//=============================================================================
//                  GLOBAL TYPEDEFS/CONSTANTS FOR TESTING
//-----------------------------------------------------------------------------
typedef btlmt::ChannelPoolChannel Obj;
typedef btlmt::AsyncChannel       AsyncChannel;

static int verbose = 0;
static int veryVerbose = 0;
static int veryVeryVerbose = 0;

static bdlqq::Mutex  coutMutex;

#define MTCOUT   { coutMutex.lock(); cout \
                                        << bdlqq::ThreadUtil::selfIdAsInt()   \
                                        << ": "
#define MTENDL   endl << bsl::flush ;  coutMutex.unlock(); }
#define MTFLUSH  bsl::flush; } coutMutex.unlock()

#define PT(X) { MTCOUT << #X " = " << (X) << MTENDL; }
#define PT2(X,Y) { MTCOUT << #X " = " << (X) << ", " \
                          << #Y " = " << (Y) << MTENDL; }
#define PT3(X,Y,Z) { MTCOUT << #X " = " << (X) << ", " \
                            << #Y " = " << (Y) << ", " \
                            << #Z " = " << (Z) << MTENDL; }
                                                 // Print identifier and value.

// The following macro facilitates outputting a vector of events.

// template <class TYPE>
// void printVector(const bsl::vector<TYPE>& vec, const char *name)
// {
//     cout << name << " = {";
//     copy(vec.begin(), vec.end(), ostream_iterator<TYPE>(cout, "\n\t"));
//     cout << "\n}" << endl;
// }

// #define PV(X) printVector( (X), #X);

#define MTASSERT(X) { \
  if (!(X)) { MTCOUT; aSsErT(1, #X, __LINE__); cout << MTFLUSH; } }

#define MTLOOP_ASSERT(I,X) { \
  if (!(X)) { MTCOUT; P(I); aSsErT(1, #X, __LINE__); cout << MTFLUSH; }}

#define MTLOOP2_ASSERT(I,J,X) { \
  if (!(X)) { MTCOUT; P_(I); P(J); aSsErT(1, #X, __LINE__); cout << MTFLUSH; }}

void readCb(int            result,
            int           *numNeeded,
            btlb::Blob  *blob,
            int            channelId,
            const string&  s)
{
    if (veryVerbose) {
        PT3(result, *numNeeded, channelId);
        PT2(blob->length(), s);
    }
}

namespace CASE1 {

                    // ================
                    // class DataReader
                    // ================

class DataReader {
    // This class implements a data reader whose callback functions are
    // invoked to read the data from a btlmt::ChannelPoolChannel object.

    int                  d_msgId;           // message id
    int                  d_msgLength;       // message length
    bsl::string          d_data;            // actual data
    mutable bdlqq::Mutex d_mutex;           // mutex for data

  public:
    // CREATORS
    DataReader();
        // Construct a default reader.

    ~DataReader();
        // Destroy this reader.

    // MANIPULATORS
    void blobBasedReadCb(int         state,
                         int        *numNeeded,
                         btlb::Blob *msg,
                         int         channelId);
        // Blob based read callback.

    // ACCESSORS
    const bsl::string& data() const;
        // Return the data stored in this reader.

    int msgId() const;
        // Return the message id of the data in this reader.

    int msgLength() const;
        // Return the message length of data expected by this reader.

    bool done() const;
        // Return 'true' if this reader has read the expected number of
        // data bytes, and 'false' otherwise.
};

                    // ===============
                    // class my_Server
                    // ===============

class my_Server {
    // This class implements a multi-user multi-threaded server.

    // DATA
    typedef bsl::map<int, Obj *>    ChannelMap;           // Channel Map
                                                          // typedef

    btlmt::ChannelPoolConfiguration  d_config;             // pool
                                                           // configuration

    bdlqq::Mutex                     d_mapMutex;           // map lock

    mutable bdlqq::Mutex             d_idxMutex;           // idx lock

    ChannelMap                       d_channelMap;         // channel map

    bsl::vector<int>                 d_channelIdxMap;      // channel idx
                                                           // map

    btlmt::ChannelPool              *d_channelPool_p;      // channel pool

    int                              d_portNumber;         // port on which
                                                           // this server
                                                           // is listening

    btlb::PooledBlobBufferFactory  d_blobBufferFactory;  // blob buffer
                                                           // factory

    bdlma::ConcurrentPoolAllocator   d_spAllocator;        // smart pointers
                                                           // allocators

    bslma::Allocator                *d_allocator_p;        // memory
                                                           // allocator
                                                           // (held)

    // PRIVATE MANIPULATORS
    void channelStateCb(int   channelId,
                        int   sourceId,
                        int   state,
                        void *userData);
        // Wrapper for channel pool's channel callback state callback.

    void blobBasedReadCb(int        *numNeeded,
                         btlb::Blob *msg,
                         int         channelId,
                         void       *userData);
        // Channel pool's blob based read callback.

    void poolStateCb(int reason, int source, int severity);
        // Indicates the status of the whole pool.

  private:
    // NOT IMPLEMENTED
    my_Server(const my_Server& original);
    my_Server& operator=(const my_Server& rhs);

  public:
    // TRAITS
    BSLALG_DECLARE_NESTED_TRAITS(my_Server,
                                 bslalg::TypeTraitUsesBslmaAllocator);

    // CREATORS
    my_Server(const btlmt::ChannelPoolConfiguration&  config,
              bslma::Allocator                       *basicAllocator = 0);
        // Construct this server configured by the specified 'config' using
        // 'btlb::Blob' for data reads.  Optionally specify 'basicAllocator'
        // used to allocate memory.

    ~my_Server();
        // Destroy this server.

    // MANIPULATORS
    int portNumber() const;
        // Return the actual port number on which this server is
        // listening.

    Obj *channel(int index);
        // Return the channel corresponding to the specified 'index'.
};

                    // =====================
                    // class CallbackDetails
                    // =====================

struct CallbackDetails {
    // This 'struct' represents a POD type specifying the details of the
    // read callback.

    int  d_numBytes;    // number of bytes to be read
    bool d_blobBased;   // use the blob based read callback
};

                    // ==============
                    // class TestData
                    // ==============

struct TestData {
    // This 'struct' represents the data passed the testing thread.

    int                    d_threadId;        // thread id
    DataReader             d_reader;          // data reader
    my_Server             *d_server_p;        // server to read from
    const CallbackDetails *d_callbacks;       // callbacks
    int                    d_numCbs;          // num callbacks

    void run();
        // Run the test function.
};

                        // ----------------
                        // class DataReader
                        // ----------------

// CREATORS
DataReader::DataReader()
: d_msgId(-1)
, d_msgLength(-1)
{
}

DataReader::~DataReader()
{
}

// MANIPULATORS
void DataReader::blobBasedReadCb(int         state,
                                 int        *numNeeded,
                                 btlb::Blob *msg,
                                 int         channelId)
{
    if (veryVerbose) {
        MTCOUT << "Blob based callback called with "
               << "for Channel Id: " << channelId
               << " with state: " << state << MTENDL;
    }

    if (btlmt::AsyncChannel::e_SUCCESS != state) {
        return;                                                       // RETURN
    }

    if (veryVerbose) {
        MTCOUT << "Blob based callback called with "
               << "for Channel Id: " << channelId
               << " with data of length: " << msg->length()
               << MTENDL;
    }

    bdlqq::LockGuard<bdlqq::Mutex> guard(&d_mutex);

    const int INT_SIZE = static_cast<int>(sizeof(int));
    if (-1 == d_msgId) {
        if (msg->length() < INT_SIZE) {
            *numNeeded = INT_SIZE;
            return;                                                   // RETURN
        }

        char *dataPtr = msg->buffer(0).data();
        if (msg->buffer(0).size() < INT_SIZE) {
            char tmp[INT_SIZE];
            const int numBytes = msg->buffer(0).size();
            memcpy(tmp, msg->buffer(0).data(), numBytes);
            ASSERT(msg->buffer(1).size() <= INT_SIZE - numBytes);
            memcpy(tmp + numBytes,
                   msg->buffer(1).data(),
                   INT_SIZE - numBytes);
            dataPtr = tmp;
        }

        bslx::MarshallingUtil::getInt32(&d_msgId, dataPtr);
        ASSERT(0 <= d_msgId);
        btlb::BlobUtil::erase(msg, 0, INT_SIZE);
        if (0 == msg->length()) {
            *numNeeded = INT_SIZE;
            return;                                                   // RETURN
        }
    }

    if (-1 == d_msgLength) {
        if (msg->length() < INT_SIZE) {
            *numNeeded = INT_SIZE;
            return;                                                   // RETURN
        }

        char *dataPtr = msg->buffer(0).data();
        if (msg->buffer(0).size() < INT_SIZE) {
            char tmp[INT_SIZE];
            const int numBytes = msg->buffer(0).size();
            memcpy(tmp, msg->buffer(0).data(), numBytes);
            ASSERT(msg->buffer(1).size() >= INT_SIZE - numBytes);
            memcpy(tmp + numBytes,
                   msg->buffer(1).data(),
                   INT_SIZE - numBytes);
            dataPtr = tmp;
        }

        bslx::MarshallingUtil::getInt32(&d_msgLength, dataPtr);
        ASSERT(0 <= d_msgLength);
        btlb::BlobUtil::erase(msg, 0, INT_SIZE);

        if (0 == msg->length()) {
            *numNeeded = d_msgLength;
            return;                                                   // RETURN
        }
    }

    bsl::string msgData;

    const int numDataBufs = msg->numDataBuffers();
    for (int i = 0; i < numDataBufs - 1; ++i) {
        msgData.append(msg->buffer(i).data(), msg->buffer(i).size());
    }
    msgData.append(msg->buffer(numDataBufs - 1).data(),
                   msg->lastDataBufferLength());

    btlb::BlobUtil::erase(msg, 0, msg->length());
    d_data.append(msgData);
    const int numRemaining = d_msgLength - d_data.size();
    *numNeeded = numRemaining <= 0 ? 0 : numRemaining;
}

// ACCESSORS
const bsl::string& DataReader::data() const
{
    bdlqq::LockGuard<bdlqq::Mutex> guard(&d_mutex);
    return d_data;
}

int DataReader::msgId() const
{
    bdlqq::LockGuard<bdlqq::Mutex> guard(&d_mutex);
    return d_msgId;
}

int DataReader::msgLength() const
{
    bdlqq::LockGuard<bdlqq::Mutex> guard(&d_mutex);
    return d_msgLength;
}

bool DataReader::done() const
{
    bdlqq::LockGuard<bdlqq::Mutex> guard(&d_mutex);
    return (int) d_data.size() == d_msgLength;
}

                        // ---------------
                        // class my_Server
                        // ---------------

// PRIVATE MANIPULATORS
void my_Server::channelStateCb(int   channelId,
                               int   sourceId,
                               int   state,
                               void *)
{
    if (veryVerbose) {
        MTCOUT << "Channel state callback called with"
               << " Channel Id: " << channelId
               << " Source Id: "  << sourceId
               << " State: " << state << MTENDL;
    }

    bdlqq::LockGuard<bdlqq::Mutex> guard(&d_mapMutex);
    ChannelMap::iterator iter = d_channelMap.find(channelId);

    switch (state) {
      case btlmt::ChannelPool::e_CHANNEL_DOWN: {
        if (iter == d_channelMap.end()) {
            if (veryVerbose) {
                MTCOUT << "Channel Id: " << channelId
                       << " not found" << MTENDL;
            }
            return;                                                   // RETURN
        }
      } break;

      case btlmt::ChannelPool::e_CHANNEL_UP: {
        if (iter != d_channelMap.end()) {
            if (veryVerbose) {
                MTCOUT << "Channel Id: " << channelId
                       << " already exists" << MTENDL;
            }
            d_channelPool_p->shutdown(channelId,
                                      btlmt::ChannelPool::e_IMMEDIATE);
            return;                                                   // RETURN
        }

        Obj *channel;
        channel = new (*d_allocator_p) btlmt::ChannelPoolChannel(
                                                          channelId,
                                                          d_channelPool_p,
                                                          &d_blobBufferFactory,
                                                          &d_spAllocator,
                                                          d_allocator_p);
        d_channelMap[channelId] = channel;
        d_idxMutex.lock();
        d_channelIdxMap.push_back(channelId);
        d_idxMutex.unlock();
      } break;
      default: {
      } break;
    }
}

void my_Server::poolStateCb(int reason, int source, int)
{
    if (veryVerbose) {
        MTCOUT << "Pool state changed: (" << reason << ", " << source
               << ") " << MTENDL;
    }
}

void my_Server::blobBasedReadCb(int        *numNeeded,
                                btlb::Blob *msg,
                                int         channelId,
                                void       *userData)
{
    bdlqq::LockGuard<bdlqq::Mutex> guard(&d_mapMutex);
    ChannelMap::iterator iter = d_channelMap.find(channelId);
    if (iter == d_channelMap.end()) {
        if (veryVerbose) {
            MTCOUT << "Channel Id: " << channelId
                   << " not found" << MTENDL;
        }
        return;                                                       // RETURN
    }

    Obj *channel = iter->second;
    guard.release()->unlock();
    channel->blobBasedDataCb(numNeeded, msg);
}

// CREATORS
my_Server::my_Server(
                const btlmt::ChannelPoolConfiguration&  config,
                bslma::Allocator                       *basicAllocator)
: d_config(config)
, d_channelMap(basicAllocator)
, d_channelPool_p(0)
, d_portNumber(0)
, d_blobBufferFactory(config.maxIncomingMessageSize(), basicAllocator)
, d_spAllocator(basicAllocator)
, d_allocator_p(bslma::Default::allocator(basicAllocator))
{
    btlmt::ChannelPool::ChannelStateChangeCallback channelStateFunctor(
            bdlf::MemFnUtil::memFn(&my_Server::channelStateCb, this)
          , d_allocator_p);

    btlmt::ChannelPool::PoolStateChangeCallback poolStateFunctor(
            bdlf::MemFnUtil::memFn(&my_Server::poolStateCb, this)
          , d_allocator_p);

    btlmt::ChannelPool::BlobBasedReadCallback dataFunctor =
            bdlf::MemFnUtil::memFn(&my_Server::blobBasedReadCb,
                                  this);

    d_channelPool_p = new (*d_allocator_p)
                                     btlmt::ChannelPool(channelStateFunctor,
                                                        dataFunctor,
                                                        poolStateFunctor,
                                                        d_config,
                                                        d_allocator_p);

    d_channelPool_p->start();

    btlso::IPv4Address endpoint;
    endpoint.setPortNumber(d_portNumber);
    d_channelPool_p->listen(endpoint, 5, 1);
    const btlso::IPv4Address *address = d_channelPool_p->serverAddress(1);
    if (address) {
        d_portNumber = address->portNumber();
    }
}

my_Server::~my_Server()
{
    for (ChannelMap::iterator iter = d_channelMap.begin();
         iter != d_channelMap.end(); ++iter) {
        d_allocator_p->deleteObject(iter->second);
    }
    d_allocator_p->deleteObject(d_channelPool_p);
}

// ACCESSORS
Obj *my_Server::channel(int index)
{
    bdlqq::LockGuard<bdlqq::Mutex> guard1(&d_idxMutex);
    if (index >= (int) d_channelIdxMap.size()) {
        return 0;                                                     // RETURN
    }
    int channelId = d_channelIdxMap[index];
    guard1.release()->unlock();

    bdlqq::LockGuard<bdlqq::Mutex> guard2(&d_mapMutex);
    ChannelMap::iterator iter = d_channelMap.find(channelId);
    if (iter != d_channelMap.end()) {
        return iter->second;                                          // RETURN
    }
    return 0;
}

int my_Server::portNumber() const
{
    return d_portNumber;
}

extern "C" void* threadFunction(void *testData)
{
    ((TestData *) testData)->run();
    return 0;
}

                        // --------------
                        // class TestData
                        // --------------

void TestData::run()
{
    Obj *channel = 0;
    while (!channel) {
        channel = d_server_p->channel(d_threadId);
        bdlqq::ThreadUtil::yield();
    }

    btlmt::AsyncChannel::BlobBasedReadCallback blobBasedCb =
                        bdlf::MemFnUtil::memFn(&DataReader::blobBasedReadCb,
                                              &d_reader);

    for (int i = 0; i < d_numCbs; ++i) {
        const CallbackDetails cbDetails = d_callbacks[i];
        const int numBytes  = cbDetails.d_numBytes;

        channel->read(numBytes, blobBasedCb);
    }

    while (!d_reader.done()) {
        bdlqq::ThreadUtil::yield();
    }
}

}  // close namespace CASE1

//=============================================================================
//                              MAIN PROGRAM
//-----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    int test = argc > 1 ? atoi(argv[1]) : 0;
    verbose = argc > 2;
    veryVerbose = argc > 3;
    veryVeryVerbose = argc > 4;

    cout << "TEST " << __FILE__ << " CASE " << test << endl;;

    switch (test) { case 0:  // Zero is always the leading case.
      case 3: {
        // --------------------------------------------------------------------
        // TEST USAGE EXAMPLE
        //   The usage example from the header has been incorporated into this
        //   test driver.  All references to 'assert' have been replaced with
        //   'ASSERT'.  Call the test example function and assert that it works
        //   as expected.
        //
        // Plan:
        // Testing:
        // --------------------------------------------------------------------

      }break;
      case 2: {
        // --------------------------------------------------------------------
        // TESTING 'read' destroys the passed callback - DRQS 32546982
        //
        // Plan:
        //
        // Testing:
        // --------------------------------------------------------------------

        {
            btlmt::ChannelPoolConfiguration config;
            config.setMaxThreads(1);
            config.setMetricsInterval(10.0);

            btlmt::ChannelPool::ChannelStateChangeCallback channelCb;
            btlmt::ChannelPool::PoolStateChangeCallback    poolCb;
            btlmt::ChannelPool::BlobBasedReadCallback      dataCb;

            btlb::PooledBlobBufferFactory  pbbf(1024);

            bslma::TestAllocator ca;
            bdlma::ConcurrentPoolAllocator pa(&ca);

            btlmt::ChannelPool cp(channelCb, dataCb, poolCb, config, &ca);

            bslma::TestAllocator ta(veryVeryVerbose);
            bslma::DefaultAllocatorGuard dag(&ta);

            Obj mX(0, &cp, &pbbf, &pa, &ca);

            string logString(&ta);
            logString = "12345678901234567890123456789012" \
                        "123456789012345678901234567890123";

            AsyncChannel::BlobBasedReadCallback cb =
                bdlf::BindUtil::bindA(&ta,
                                     &readCb,
                                     bdlf::PlaceHolders::_1,
                                     bdlf::PlaceHolders::_2,
                                     bdlf::PlaceHolders::_3,
                                     bdlf::PlaceHolders::_4,
                                     logString);

            int rc = mX.read(5, cb);
            ASSERT(!rc);
        }
      } break;
      case 1: {
        // --------------------------------------------------------------------
        // BREATHING TEST:
        //
        // Plan:
        //
        // Testing:
        //   This "test" *exercises* basic functionality, but *tests* nothing.
        // --------------------------------------------------------------------

        using namespace CASE1;

        const int MAX_NUM_CBS = 100;
        struct {
            const int             d_line;
            const bsl::string     d_expData;
            const int             d_numCbs;
            const CallbackDetails d_cbDetails[MAX_NUM_CBS];
        } DATA[] = {
            {
                L_,
                "Hello World!",
                7,
                {
                    { 4, true },
                    { 4, true },
                    { 1, true },
                    { 2, true },
                    { 3, true },
                    { 4, true },
                    { 5, true },
                },
            },
            {
                L_,
                "Cruel Life",
                7,
                {
                    { 4, false },
                    { 4, false },
                    { 1, false },
                    { 2, false },
                    { 3, false },
                    { 4, false },
                    { 5, false },
                }
            },
            {
                L_,
                "Testing this component requires some code",
                7,
                {
                    { 4, true  },
                    { 4, false },
                    { 1, true  },
                    { 2, false },
                    { 3, true  },
                    { 4, false },
                    { 5, true  },
                }
            },
            {
                L_,
                "Another short message",
                7,
                {
                    { 4, false },
                    { 4, true  },
                    { 1, false },
                    { 2, true  },
                    { 3, false },
                    { 4, true  },
                    { 5, false },
                }
            },
            {
                L_,
                "A long message that should invoke quite a few callbacks and "
                "give us some confidence that this test case works.",
                17,
                {
                    {  4, true },
                    {  4, true },
                    {  1, true },
                    {  2, true },
                    {  3, true },
                    {  4, true },
                    {  5, true },
                    {  6, true },
                    {  7, true },
                    {  8, true },
                    {  9, true },
                    { 10, true },
                    { 11, true },
                    { 12, true },
                    { 13, true },
                    { 14, true },
                    { 15, true },
                }
            },
            {
                L_,
                "A second long message that should invoke quite a few "
                "callbacks and give us some confidence that this works.",
                17,
                {
                    {  4, false },
                    {  4, false },
                    {  1, false },
                    {  2, false },
                    {  3, false },
                    {  4, false },
                    {  5, false },
                    {  6, false },
                    {  7, false },
                    {  8, false },
                    {  9, false },
                    { 10, false },
                    { 11, false },
                    { 12, false },
                    { 13, false },
                    { 14, false },
                    { 15, false },
                }
            },
            {
                L_,
                "To be or not to be that is the question."
                "To be or not to be that is the question."
                "To be or not to be that is the question.",
                17,
                {
                    {  4, true  },
                    {  4, false },
                    {  1, true  },
                    {  2, false },
                    {  3, true  },
                    {  4, false },
                    {  5, true  },
                    {  6, false },
                    {  7, true  },
                    {  8, false },
                    {  9, true  },
                    { 10, false },
                    { 11, true  },
                    { 12, false },
                    { 13, true  },
                    { 14, false },
                    { 15, true  },
                }
            },
            {
                L_,
                "A long message that should invoke quite a few callbacks and "
                "give us some confidence that this test case works as we want",
                17,
                {
                    {  4, false },
                    {  4, true  },
                    {  1, false },
                    {  2, true  },
                    {  3, false },
                    {  4, true  },
                    {  5, false },
                    {  6, true  },
                    {  7, false },
                    {  8, true  },
                    {  9, false },
                    { 10, true  },
                    { 11, false },
                    { 12, true  },
                    { 13, false },
                    { 14, true  },
                    { 15, false },
                }
            },
        };
        const int NUM_DATA = sizeof DATA / sizeof *DATA;

        btlmt::ChannelPoolConfiguration config;
        config.setMaxThreads(NUM_DATA);           // I/O threads
        config.setReadTimeout(1.0);               // in seconds
        config.setMetricsInterval(10.0);          // seconds
        config.setIncomingMessageSizes(sizeof(int), 10, 1024);

        typedef btlso::InetStreamSocketFactory<btlso::IPv4Address> IPv4Factory;
        typedef btlso::StreamSocket<btlso::IPv4Address>            Socket;

        bslma::TestAllocator     ta(veryVeryVerbose);
        my_Server                server(config, &ta);
        const btlso::IPv4Address ADDRESS("127.0.0.1", server.portNumber());

        IPv4Factory              factory(&ta);
        bsl::vector<TestData>    tests(NUM_DATA);
        bdlqq::ThreadUtil::Handle  handles[NUM_DATA];
        bsl::vector<bsl::string> expData(NUM_DATA);
        bsl::vector<Socket *>    sockets(NUM_DATA);

        for (int i = 0; i < NUM_DATA; ++i) {
            const bsl::string EXP_DATA = DATA[i].d_expData;
            const int         NUM_CBS  = DATA[i].d_numCbs;
            TestData&         test     = tests[i];

            Socket *socket   = factory.allocate();
            test.d_threadId  = i;
            test.d_server_p  = &server;
            test.d_numCbs    = NUM_CBS;
            test.d_callbacks = DATA[i].d_cbDetails;
            bdlqq::ThreadUtil::create(&handles[i],  threadFunction, &test);

            sockets[i] = socket;
            expData[i] = EXP_DATA;
            ASSERT(0 == socket->connect(ADDRESS));
        }

        const int MAX_DATA_SIZE = 1024;
        for (int i = 0; i < NUM_DATA; ++i) {
            Socket             *socket    = sockets[i];
            const bsl::string&  writeData = expData[i];
            const int           DATA_SIZE = writeData.size();

            char data[MAX_DATA_SIZE];
            bsl::memset(data, 0, MAX_DATA_SIZE);
            ASSERT(DATA_SIZE <= MAX_DATA_SIZE);

            int offset = 0;
            bslx::MarshallingUtil::putInt32(data, i);
            offset += sizeof(int);

            bslx::MarshallingUtil::putInt32(data + offset, DATA_SIZE);
            offset += sizeof(int);

            const int TOTAL_SIZE = DATA_SIZE + offset;

            bsl::memcpy(data + offset, writeData.data(), DATA_SIZE);

            offset = 0;
            int incr = 1, remaining = TOTAL_SIZE;
            while (remaining > 0) {
                ASSERT(incr == socket->write(data + offset, incr));
                remaining -= incr;
                offset += incr;
                if (remaining <= incr) {
                    incr = remaining;
                }
                else {
                    ++incr;
                }
                bdlqq::ThreadUtil::microSleep(100);
            }
        }

        for (int i = 0; i < NUM_DATA; ++i) {
            ASSERT(0 == bdlqq::ThreadUtil::join(handles[i]));
        }

        for (int i = 0; i < NUM_DATA; ++i) {
            const DataReader& reader = tests[i].d_reader;
            LOOP_ASSERT(i, reader.data() == expData[reader.msgId()]);
            factory.deallocate(sockets[i], true);
        }
      } break;
      default: {
        cerr << "WARNING: CASE `" << test << "' NOT FOUND." << endl;
        testStatus = -1;
      }
    }

    if (testStatus > 0) {
        cerr << "Error, non-zero test status = " << testStatus << "." << endl;
    }
    return testStatus;
}

// ----------------------------------------------------------------------------
// Copyright 2015 Bloomberg Finance L.P.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ----------------------------- END-OF-FILE ----------------------------------
