// balb_pipecontrolchannel.t.cpp                                      -*-C++-*-

// ----------------------------------------------------------------------------
//                                   NOTICE
//
// This component is not up to date with current BDE coding standards, and
// should not be used as an example for new development.
// ----------------------------------------------------------------------------

#include <balb_pipecontrolchannel.h>

#include <bslim_testutil.h>

#include <bdlf_bind.h>
#include <bdlf_placeholder.h>

#include <bslmt_barrier.h>
#include <bslmt_condition.h>
#include <bslmt_mutex.h>
#include <bslmt_threadgroup.h>
#include <bslmt_threadutil.h>

#include <bdls_pipeutil.h>

#include <bsl_algorithm.h>
#include <bsl_cstdlib.h>
#include <bsl_fstream.h>
#include <bsl_iostream.h>

#include <bslma_testallocator.h>

#include <bsls_assert.h>
#include <bsls_atomic.h>
#include <bsls_log.h>
#include <bsls_platform.h>
#include <bsls_timeinterval.h>

#ifdef BSLS_PLATFORM_OS_UNIX
#include <bsl_c_signal.h>
#include <bsl_cstdio.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

using namespace BloombergLP;
using namespace bsl;  // automatically added by script

// ============================================================================
//                                 TEST PLAN
// ----------------------------------------------------------------------------
//                                 Overview
//                                 --------
// The component under test is a mechanism that reads data from a named pipe
// and invokes a user-supplied callback function with that data.  This test
// driver verifies the implementation of by ensuring the component may be
// started, stopped, and shutdown, and also by ensuring that any data written
// to the named pipe is supplied as an argument to the user-defined callback
// function.
// ----------------------------------------------------------------------------
// CREATORS
// [ 1] explicit
//      balb::PipeControlChannel(const ControlCallback&  callback,
//                               bslma::Allocator       *basicAllocator = 0);
//
// [ 4] ~balb::PipeControlChannel();
//
// MANIPULATORS
// [ 2] int start(const bsl::string& pipeName);
// [12] int start(const bsl::string&, const bslmt::ThreadAttributes&);
// [ 2] void shutdown();
// [ 2] void stop();
//
// ACCESSORS
// [ 1] const bsl::string& pipeName() const;
// ----------------------------------------------------------------------------
// [ 1] BREATHING TEST
// [ 2] TESTING CONCERN: THE COMPONENT MAY BE RESTARTED
// [ 3] TESTING CONCERN: 'shutdown' IS IDEMPOTENT
// [ 4] TESTING CONCERN: THE DESTRUCTOR INVOKES 'shutdown'
// [ 5] TESTING CONCERN: DATA SENT BY A CLIENT IS READ FROM THE PIPE
// [ 6] TESTING CONCERN: CONCURRENT WRITES
// [ 7] TESTING USAGE EXAMPLE

// ============================================================================
//                     STANDARD BDE ASSERT TEST FUNCTION
// ----------------------------------------------------------------------------

namespace {

int testStatus = 0;

void aSsErT(bool condition, const char *message, int line)
{
    if (condition) {
        cout << "Error " __FILE__ "(" << line << "): " << message
             << "    (failed)" << endl;

        if (0 <= testStatus && testStatus <= 100) {
            ++testStatus;
        }
    }
}

}  // close unnamed namespace

// ============================================================================
//               STANDARD BDE TEST DRIVER MACRO ABBREVIATIONS
// ----------------------------------------------------------------------------

#define ASSERT       BSLIM_TESTUTIL_ASSERT
#define ASSERTV      BSLIM_TESTUTIL_ASSERTV

#define LOOP_ASSERT  BSLIM_TESTUTIL_LOOP_ASSERT
#define LOOP0_ASSERT BSLIM_TESTUTIL_LOOP0_ASSERT
#define LOOP1_ASSERT BSLIM_TESTUTIL_LOOP1_ASSERT
#define LOOP2_ASSERT BSLIM_TESTUTIL_LOOP2_ASSERT
#define LOOP3_ASSERT BSLIM_TESTUTIL_LOOP3_ASSERT
#define LOOP4_ASSERT BSLIM_TESTUTIL_LOOP4_ASSERT
#define LOOP5_ASSERT BSLIM_TESTUTIL_LOOP5_ASSERT
#define LOOP6_ASSERT BSLIM_TESTUTIL_LOOP6_ASSERT

#define Q            BSLIM_TESTUTIL_Q   // Quote identifier literally.
#define P            BSLIM_TESTUTIL_P   // Print identifier and value.
#define P_           BSLIM_TESTUTIL_P_  // P(X) without '\n'.
#define T_           BSLIM_TESTUTIL_T_  // Print a tab (w/o newline).
#define L_           BSLIM_TESTUTIL_L_  // current Line number

// ============================================================================
//            GLOBAL TYPES, CONSTANTS, AND VARIABLES FOR TESTING
// ----------------------------------------------------------------------------
static int verbose = 0;
static int veryVerbose = 0;
static int veryVeryVerbose = 0;
static int veryVeryVeryVerbose = 0;

extern "C"
{
void noopHandler (int)
{
}
}

namespace {

                            // ===================
                            // class ControlServer
                            // ===================

class ControlServer {

    // DATA
    balb::PipeControlChannel d_channel;
    bsl::vector<bsl::string> d_messages;

    // PRIVATE MANIPULATORS
    void onMessage(const bslstl::StringRef& message)
    {
        if ("EXIT" != message) {
            d_messages.push_back(message);
        }
        else {
            shutdown();
        }
    }

    // UNIMPLEMENTED
    ControlServer(const ControlServer&);             // = deleted
    ControlServer& operator=(const ControlServer&);  // = deleted

  public:
    // CREATORS
    explicit ControlServer(bslma::Allocator *basicAllocator = 0)
    : d_channel(bdlf::BindUtil::bind(&ControlServer::onMessage,
                                     this,
                                     bdlf::PlaceHolders::_1),
                basicAllocator)
    , d_messages(basicAllocator)
    {}

    // MANIPULATORS
    int start(const bslstl::StringRef &pipeName) {
        return d_channel.start(pipeName);
    }

    void shutdown() {
        d_channel.shutdown();
    }

    void stop() {
        d_channel.stop();
    }

    // ACCESSORS
    bsl::size_t numMessages() const {
        return d_messages.size();
    }

    const bsl::string& message(int index) const {
        return d_messages[index];
    }
};

static void threadSend(const bsl::string& pipeName,
                       const bsl::string& message,
                       int                numIterations)
{
    for (int i = 0; i < numIterations; ++i) {
        bdls::PipeUtil::send(pipeName.c_str(), message);
    }
}

void noop(const bslstl::StringRef&)
{
}

void verifyPayload(const bsl::string&        expected,
                   const bslstl::StringRef&  found,
                   bslmt::Barrier           *barrier)
{
    // Verify that the specified 'found' payload has the same value as the
    // specified 'expected' payload.

    ASSERT(expected == found);
    barrier->wait();
}

void loadData(bsl::string *result, int length)
{
    const char DATA[] = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
                        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
                        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
                        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
                        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
                        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
                        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
                        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
                        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
                        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
                        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
                        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
                        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
                        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
                        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
                        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
                        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
                        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
                        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
                        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
    enum { DATA_SIZE = sizeof DATA - 1 };

    int capacity = length;
    result->clear();
    result->reserve(capacity);
    do {
        int nbytes = (capacity >= DATA_SIZE) ? DATA_SIZE : capacity;
        result->append(DATA, nbytes);
        capacity -= nbytes;
    } while (capacity > 0);
}

}  // close unnamed namespace

extern "C"
void onSigPipe(int) {
    BSLS_LOG_WARN("SIGPIPE received");
}

void myCallback(bslmt::Condition         *condition,
                bsls::AtomicBool         *conditionFlag,
                const bslstl::StringRef&  msg)
{
    if (verbose) {
        bsl::cout << "Received message: " << msg << bsl::endl;
    }
    *conditionFlag = true;
    condition->signal();
    bslmt::ThreadUtil::sleep(bsls::TimeInterval(0, 200000000));  // 0.2 seconds
}

void attributesTestCallback(bsl::string              *threadName,
                            bsl::string              *messageStorage,
                            const bslstl::StringRef&  message,
                            bslmt::Barrier           *barrier)
    // Store the specified 'message' into the specified 'messageStorage' and
    // the current tread name into the specified 'threadName' on invocation
    // using the specified 'barrier' for synchronisation.  This function is
    // passed as a callback function to the pipe control mechanism on creation
    // and used to test the ability of adjustment of background processing
    // thread of 'balb::PipeControlChannel' object.  The overload of the
    // 'balb::PipeControlChannel::start' method accepts the desired
    // configuration of such thread.  But public interface of the
    // 'balb::PipeControlChannel' class does not grant access to appropriate
    // private field.  However, the callback function passed to the pipe
    // control mechanism on creation should be called in the context of this
    // thread.  To check that attributes are passed correctly,
    // 'attributesTestCallback' stores the name of the current thread on
    // invocation.  As all atributes are arranged in a single object we need to
    // check just one field's value to be sure that the whole object is passed
    // correctly.  Note that this approach may not work for Windows, since it's
    // early versions don't support thread names.
{
    bslmt::ThreadUtil::getThreadName(threadName);
    *messageStorage = message;

    // Wait for the main thread.
    barrier->wait();
}

void sendHello(const char *pipeName)
{
    // Allow time for the condition.wait() to be reached (see 'main')
    bslmt::ThreadUtil::sleep(bsls::TimeInterval(2, 0));

    bsl::ofstream pipe(pipeName);
    ASSERT(pipe.is_open());
    if (!pipe.is_open()) { // Avoiding waiting forever
        abort();                                                       // ABORT
    }

    pipe << "Hello pipe" << endl;
}

// ============================================================================
//                               MAIN PROGRAM
// ----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    int test = (argc > 1) ? bsl::atoi(argv[1]) : 1;

    verbose = (argc > 2);
    veryVerbose = (argc > 3);
    veryVeryVerbose = (argc > 4);
    veryVeryVeryVerbose = (argc > 5);

#ifdef BSLS_PLATFORM_OS_UNIX
    ofstream devnull;
    if (9 == test) {
        devnull.open("/dev/null");
    }
#endif
    if (test >= 0) {
        cout << "TEST " << __FILE__ << " CASE " << test << endl;
    }

#ifndef BSLS_PLATFORM_OS_WINDOWS
    sigset(SIGPIPE, onSigPipe);
#endif

    switch (test) { case 0:
      case 12: {
        // --------------------------------------------------------------------
        // TESTING THREAD ATTRIBUTES PASSING
        //
        // Concerns:
        //: 1 The 'start' method correctly passes required thread attributes to
        //:   the appropriate thread creation function.
        //
        // Plan:
        //: 1 Using the special callback function (see 'attributesTestCallback'
        //:   documentation) create and start a pipe control mechanism, passing
        //:   the desired thread configuration.  Pass some messages using that
        //:   mechanism and verify, that created tread has the expected
        //:   attributes.
        //
        // Testing
        //   int start(const bsl::string&, const bslmt::ThreadAttributes&);
        //---------------------------------------------------------------------

        if (verbose) {
            cout << "TESTING THREAD ATTRIBUTES PASSING" << endl
                 << "=================================" << endl;
        }

        bsl::string pipeName;

        ASSERT(0 == bdls::PipeUtil::makeCanonicalName(
                                                     &pipeName,
                                                     "ctrl.attributestest12"));

        const bsl::string EXPECTED_THREAD_NAME("ctrl.attrtest12"         );
        const bsl::string MSG                 ("this is the test message");
        bsl::string       threadName;
        bsl::string       message;
        bslmt::Barrier    barrier(2);

        balb::PipeControlChannel pipeChannel(
                                  bdlf::BindUtil::bind(&attributesTestCallback,
                                                      &threadName,
                                                      &message,
                                                      bdlf::PlaceHolders::_1,
                                                      &barrier));

        bslmt::ThreadAttributes attributes;
        attributes.setThreadName(EXPECTED_THREAD_NAME);

        int rc = pipeChannel.start(pipeName, attributes);

        if (0 == rc) {
            rc = bdls::PipeUtil::send(pipeName, MSG + "\n");
            ASSERTV(rc, 0 == rc);
        }

        // Wait for background processing thread.
        barrier.wait();

        pipeChannel.shutdown();
        pipeChannel.stop();

        ASSERTV(message, MSG == message);

#if   defined(BSLS_PLATFORM_OS_LINUX) || defined(BSLS_PLATFORM_OS_DARWIN) ||  \
      defined(BSLS_PLATFORM_OS_SOLARIS)
        ASSERTV(threadName, EXPECTED_THREAD_NAME == threadName);
#elif defined(BSLS_PLATFORM_OS_WINDOWS)
        // The threadname will only be visible if we're running on Windows 10,
        // version 1607 or later, otherwise it will be empty.  It breaks the
        // test strategy, since we can not to rely on the thread name value.
        // But since the behavior being tested is common for all platforms, we
        // dare to say that it is correct on Windows if the tests pass
        // successfully for other platforms.

        ASSERTV(threadName, EXPECTED_THREAD_NAME == threadName
                         || true                 == threadName.empty());
#else
        // Thread names are not supported for other platforms.
        ASSERTV(threadName, true                 == threadName.empty());
#endif
      } break;
      case 11: {
        // --------------------------------------------------------------------
        // 'shutdown' DOES NOT DEADLOCK ON WINDOWS
        //
        // Concerns:
        //   The 'sendEmptyMessage' function does not deadlock during shutdown.
        //
        // Plan:
        //: Create a 'balb_PipeControlChannel' with a callback that triggers a
        //: condition variable then waits 200 milliseconds.  Next create a
        //: thread that (after a 2 seconds) wait will send data to the pipe.
        //: Then wait on the condition variable.  Once the condition is
        //: triggered the main thread is going to destroy the
        //: 'balb_PipeControlChannel', which opens up a window of opportunity
        //: to the tested (regression) race condition to occur.  The test case
        //: has been tested using the unfixed code (on Windows) and the
        //: deadlock occurs.
        //:
        //: The tested regression: The 'shutdown' function sets the
        //: 'd_isRunningFlag' to 'false' while the background thread is still
        //: (in our test case sleeping) in the user callback.  Once the
        //: background thread returns from the user callback it can "see" that
        //: that the 'd_isRunningFlag' is 'false' and stops reading from the
        //: pipe.  This causes the send function to block forever, waiting for
        //: a reader to show up.
        //:
        //: The fix: The code was fixed by introducing a critical section (in
        //: the 'readNamedPipe' function) around the code that reads from the
        //: pipe and runs the user callback, using the already existing
        //: 'd_mutex' member.  A mutex lock is also introduced into the
        //: 'backgroundProcessor' function to protect the reading of the
        //: 'd_isRunningFlag', as it may be written to (from within 'shutdown')
        //: from another thread, therefore reading it without synchronization
        //: is undefined behavior.
        //
        // Testing:
        //   shutdown
        // --------------------------------------------------------------------

        if (verbose) {
            cout << "'shutdown' DOES NOT DEADLOCK ON WINDOWS" << endl
                 << "=======================================" << endl;
        }

#ifndef BSLS_PLATFORM_OS_WINDOWS
        if (verbose) {
            cout << "Skipping test case 11 on non-Windows OS..." << endl;
        }
#else
        const char PIPE_NAME[] = "\\\\.\\pipe\\ctrl.balb.pcctest11";
        bslmt::Mutex mutex;
        bslmt::Condition condition;
        bsls::AtomicBool conditionFlag(false);
        balb::PipeControlChannel pipeChannel(
                                bdlf::BindUtil::bind(&myCallback,
                                                    &condition,
                                                    &conditionFlag,
                                                    bdlf::PlaceHolders::_1));

        pipeChannel.start(PIPE_NAME);
        bslmt::ThreadUtil::Handle thread;
        int rc = bslmt::ThreadUtil::create(&thread, bdlf::BindUtil::bind(
                                                      &sendHello, PIPE_NAME));
        ASSERT(rc == 0);
        if (rc != 0) {
            break;                                                     // BREAK
        }
        condition.wait(&mutex);
#endif
      } break;
      case -10: {
        // --------------------------------------------------------------------
        // Child process for case 10
        // --------------------------------------------------------------------

#if defined(BSLS_PLATFORM_OS_WINDOWS) || defined(BSLS_PLATFORM_OS_CYGWIN)
        if (verbose) {
            cout << "Skipping case -10 on Windows and Cygwin..." << endl;
        }
#else
        if (argc != 5 ||
            (argv[2][0] != 'Y' &&
             argv[2][0] != 'Z' &&
             argv[2][0] != 'A'))
        {
            cout << "Case -10 is for internal use only.  Go away!" << endl;
            exit(99);
        }
        veryVeryVerbose = argv[2][0] == 'A';
        verbose = veryVeryVerbose || argv[2][0] == 'Z';

        cout << "P" << getpid() << "\n" << flush;

        signal(SIGINT, &noopHandler);

        if (verbose) {
            cerr << "Case 10 client process starting" << endl;
        }
        unlink(argv[3]);

        bslmt::Barrier barrier(2);
        balb::PipeControlChannel channel(bdlf::BindUtil::bind(
                                                        &verifyPayload,
                                                        bsl::string(argv[4]),
                                                        bdlf::PlaceHolders::_1,
                                                        &barrier));

        int rc = channel.start(argv[3]);
        ASSERT(0 == rc);

        sigset_t intset;
        sigemptyset(&intset);
        sigaddset(&intset, SIGINT);
        pthread_sigmask(SIG_BLOCK, &intset, 0);

        barrier.wait();

        if (verbose) {
            cerr << "Case 10 client process stopping" << endl;
        }

        channel.shutdown();
        channel.stop();
#endif
      } break;
      case 10: {
#if defined(BSLS_PLATFORM_OS_WINDOWS) || defined(BSLS_PLATFORM_OS_CYGWIN)
        if (verbose) {
            cout << "Skipping case 10 on Windows and Cygwin..." << endl;
        }
#else
        if (verbose) {
            cout << "EINTR test" << endl
                 << "==========" << endl;
        }

        bsl::string pipeName;

        ASSERT(0 == bdls::PipeUtil::makeCanonicalName
               (&pipeName, "ctrl.pcctest10"));

        char buffer[512];
        char verb = veryVeryVerbose ? 'A' :
                          verbose ? 'Z' : 'Y';

        snprintf(buffer, 512, "%s -10 %c %s QUICKBROWNFOX_CASE10",
                 argv[0], verb, pipeName.c_str());

        FILE* childFD = popen(buffer, "r");
        BSLS_ASSERT(0 != childFD); // test invariant

        do {
            fscanf(childFD, "%s", buffer);
        } while (buffer[0] == '\n');

        int childPid = 0;

        if (buffer[0] == 'P') {
            childPid = atoi(&buffer[1]);
        }
        if (0 == childPid) {
            cout << "Communication error with child process!!" << endl;
            cout << "...(received \"" << buffer << "\"" << endl;
            exit(9);
        }
        else if (verbose) {
            cout << "Child pid: " << childPid << endl;
        }

        bslmt::ThreadUtil::microSleep(10000);
        kill(childPid, SIGINT);
        bslmt::ThreadUtil::microSleep(100000);

        if (verbose) {
            cout << "Sending QUICKBROWNFOX_CASE10 to " << pipeName
                 << endl;
        }

        ASSERT(0 == bdls::PipeUtil::send(pipeName, "QUICKBROWNFOX_CASE10\n"));
        ASSERT(0 == pclose(childFD));

#endif
      } break;

      case -9: {
        enum { NUM_CLIENTS = 2, NUM_ITERATIONS = 500 };

        if (argc != 4 ||
            (argv[2][0] != 'Y' &&
             argv[2][0] != 'Z'))
        {
            cout << "Case -9 is for internal use only.  Go away!" << endl;
            exit(99);
        }
        verbose = argv[2][0] == 'Z';

        if (verbose) {
            cerr << "Case 9 client process starting" << endl;
        }

#ifdef BSLS_PLATFORM_OS_UNIX
        unlink(argv[3]);
#endif

        const char MESSAGE[]
             = "Hello, world! The sick cat couldn't jump over even a lazy dog";
        bslmt::Barrier barrier(2);

        bsl::string message(MESSAGE);
        balb::PipeControlChannel channel(bdlf::BindUtil::bind(
                                                        &verifyPayload,
                                                        message,
                                                        bdlf::PlaceHolders::_1,
                                                        &barrier));

        ASSERT(0 == channel.start(argv[3]));

        if (verbose) {
            cerr << "\tCase 9 client: channel started, sending A..."
                 << endl;
        }
        cout << "A\n" << flush;

        bslmt::ThreadGroup threadGroup;
        threadGroup.addThreads(bdlf::BindUtil::bind(&threadSend,
                                                    bsl::string(argv[3]),
                                                    message + "\n",
                                                    (int)NUM_ITERATIONS),
                               NUM_CLIENTS);

        for (int i = 0; i < NUM_CLIENTS * NUM_ITERATIONS; ++i) {
            barrier.wait();
        }

        threadGroup.joinAll();

        if (verbose) {
            cerr << "\tCase 9 client: sleeping..." << endl;
        }

        bslmt::ThreadUtil::sleep(bsls::TimeInterval(2.0));

        if (verbose) {
            cerr << "\tCase 9 client: sending B and shutting down..." << endl;
        }

        cout << "B\n" << flush;

        channel.shutdown();
        channel.stop();

        bslmt::ThreadUtil::sleep(bsls::TimeInterval(1.0));

        break;
      }
      case 9: {
        // --------------------------------------------------------------------
        // TESTING PIPE-IN-USE SAFETY
        //
        // Concern: That if a process opens a PipeControlChannel, another one
        // cannot open the same pipe or interfere with its operation.
        //
        //---------------------------------------------------------------------

#if defined(BSLS_PLATFORM_OS_WINDOWS) || defined(BSLS_PLATFORM_OS_CYGWIN)
        if (verbose) {
            cout << "Skipping case 9 on Windows and Cygwin..." << endl;
        }
#else
        if (verbose) {
            cout << "Pipe-in-use safety test" << endl
                 << "=======================" << endl;
        }

        bsl::string pipeName;

        ASSERT(0 == bdls::PipeUtil::makeCanonicalName
               (&pipeName, "ctrl.safttest9"));

        char buffer[512];
        snprintf(buffer, 512, "%s -9 %c %s", argv[0],
                 verbose? 'Z' : 'Y', pipeName.c_str());
        FILE* childFD = popen(buffer, "r");
        BSLS_ASSERT(0 != childFD); // test invariant
        do {
            fgets(buffer, 3, childFD);
        } while (buffer[0] == '\n');

        if (buffer[0] != 'A') {
            cout << "Communication error with child process!!" << endl;
            cout << "...(received \"" << buffer << "\"" << endl;
            exit(9);
        }
        ControlServer server;

        int rc;
        for (int i = 0; i < 5; ++i) {
            rc = server.start(pipeName);
            LOOP_ASSERT(i, 0 != rc);
            bslmt::ThreadUtil::microSleep(100);
        }

        do {
            fgets(buffer, 50, childFD);
        } while (buffer[0] == '\n');
        if (buffer[0] != 'B') {
            cout << "Communication error with child process!!" << endl;
            cout << "...(received \"" << buffer << "\"" << endl;
            exit(9);
        }
        pclose(childFD);
#endif
      } break;

      case -8: {
        if (argc != 4 ||
            (argv[2][0] != 'Y' &&
             argv[2][0] != 'Z'))
        {
            cout << "Case -8 is for internal use only.  Go away!" << endl;
            exit(99);
        }
        verbose = argv[2][0] == 'Z';

        if (verbose) {
            cerr << "Case 8 client process starting" << endl;
        }

#ifdef BSLS_PLATFORM_OS_UNIX
        unlink(argv[3]);
#endif

        balb::PipeControlChannel *channel =
                                           new balb::PipeControlChannel(&noop);
        ASSERT(0 == channel->start(argv[3]));

        if (verbose) {
            cerr << "\tCase 8 client: channel started, seg faulting..."
                 << endl;
        }
        cout << "A\n" << flush;
#ifndef BSLS_PLATFORM_OS_DARWIN
        *(int*)10 = 10;
#else
        _exit(1);
#endif

        ASSERT(!"unreachable");
        break;
      }
      case 8: {
        // --------------------------------------------------------------------
        // TESTING CRASH RECOVERY
        //
        // Concern: That if a process opens a PipeControlChannel and
        // subsequently crashes, another process can then open the same named
        // pipe.
        //---------------------------------------------------------------------

#if defined(BSLS_PLATFORM_OS_WINDOWS) || defined(BSLS_PLATFORM_OS_CYGWIN)
        if (verbose) {
            cout << "Skipping case 8 on Windows and Cygwin..." << endl;
        }
#else
        if (verbose) {
            cout << "Crash recovery test: \"Bus Error\" is OK" << endl
                 << "======================================" << endl;
        }

        bsl::string pipeName;

        ASSERT(0 == bdls::PipeUtil::makeCanonicalName
               (&pipeName, "ctrl.restarttest8"));

        char buffer[512];
        snprintf(buffer, 512, "%s -8 %c %s", argv[0],
                 verbose? 'Z' : 'Y', pipeName.c_str());
        FILE* childFD = popen(buffer, "r");
        BSLS_ASSERT(0 != childFD); // test invariant
        do {
            fgets(buffer, 3, childFD);
        } while (buffer[0] == '\n');
        if (buffer[0] != 'A') {
            cout << "Communication error with child process!!" << endl;
            cout << "...(received \"" << buffer << "\"" << endl;
            exit(9);
        }
        bslmt::ThreadUtil::sleep(bsls::TimeInterval(1.0));

        ControlServer server;

        int rc = server.start(pipeName);
        ASSERT(0 == rc && "Failed to start pipe control channel");

        if (0 == rc) {
            const char MSG0[]  = "this is the first message";
            const char MSG1[]  = "this is the second message";

            rc = bdls::PipeUtil::send(pipeName, bsl::string(MSG0) + "\n");
            ASSERT(0 == rc);
            rc = bdls::PipeUtil::send(pipeName, bsl::string(MSG1) + "\n");
            ASSERT(0 == rc);
            rc = bdls::PipeUtil::send(pipeName, "EXIT\n");
            ASSERT(0 == rc);

            server.stop();  // blocks until shutdown

            ASSERT(2 == server.numMessages());
            ASSERT(bsl::string(MSG0) == server.message(0));
            ASSERT(bsl::string(MSG1) == server.message(1));
        }
#endif
      } break;
      case 7: {
        // --------------------------------------------------------------------
        // TESTING USAGE EXAMPLE
        //
        // Concerns:
        //   The usage example provided in the component header file must
        //   compile, link, and execute as shown.
        //
        // Plan:
        //   Incorporate the usage example from the header file into the test
        //   driver.  Additionally, replace all calls to 'BSLS_ASSERT' in
        //   the usage example with calls to 'ASSERT'.  This now becomes the
        //   source, which is then "copied" back to the header file by
        //   reversing the above process.
        //
        // Testing:
        //   USAGE EXAMPLE
        // --------------------------------------------------------------------

#if defined(BSLS_PLATFORM_OS_CYGWIN)
        if (verbose) {
            cout << "Skipping case 7 on Cygwin..." << endl;
        }
#else
        if (verbose) {
            cout << "Usage Example" << endl
                 << "=============" << endl;
        }

// Now, construct and run the server using a canonical name for the pipe:
//..
    bsl::string pipeName;
    bdls::PipeUtil::makeCanonicalName(&pipeName, "ctrl.pcctest");

    ControlServer server;

    int rc = server.start(pipeName);
    if (0 != rc) {
        cout << "ERROR: Failed to start pipe control channel" << endl;
    }
//..
// Once the server is started, clients can send messages to the server.
//..
    const char MSG0[]  = "this is the first message";
    const char MSG1[]  = "this is the second message";

    rc = bdls::PipeUtil::send(pipeName, bsl::string(MSG0) + "\n");
    ASSERT(0 == rc);
    rc = bdls::PipeUtil::send(pipeName, bsl::string(MSG1) + "\n");
    ASSERT(0 == rc);
    rc = bdls::PipeUtil::send(pipeName, "EXIT\n");
    ASSERT(0 == rc);
//..
// The server shuts down once it processes the "EXIT" control message.
//..
    server.stop();  // block until shutdown
//..
// Finally, let's ensure the server received each control message sent.
//..
    ASSERT(2 == server.numMessages());
    ASSERT(bsl::string(MSG0) == server.message(0));
    ASSERT(bsl::string(MSG1) == server.message(1));
//..

#endif
      } break;
      case 6: {
        // --------------------------------------------------------------------
        // TESTING CONCERN: CONCURRENT WRITES
        //
        // Concerns:
        //   When multiple clients concurrent write to the pipe each write
        //   occurs "atomically", i.e., the data in a message is not
        //   interleaved with the data from any other message.
        //
        // Plan:
        //    Create a pipe control channel and start the channel processing
        //    a named pipe.  Start a group of threads that write a payload to
        //    the named pipe, and verify the pipe control channel's control
        //    callback is invoked the expected number of times with an argument
        //    having the same value as the payload written to the pipe.
        //
        // Testing:
        //   Concurrent writes
        // --------------------------------------------------------------------

#if defined(BSLS_PLATFORM_OS_WINDOWS) || defined(BSLS_PLATFORM_OS_CYGWIN)
        if (verbose) {
            cout << "Skipping case 6 on Windows and Cygwin..." << endl;
        }
#else
        if (verbose) {
            cout << "TESTING CONCERN: CONCURRENT WRITES" << endl
                 << "==================================" << endl;
        }

        bslma::TestAllocator ta(veryVeryVeryVerbose);
        {
            bsl::string pipeName;

            ASSERT(0 == bdls::PipeUtil::makeCanonicalName
                   (&pipeName, "ctrl.balb.pcctest6"));
#ifdef BSLS_PLATFORM_OS_UNIX
            unlink(pipeName.c_str());
#endif
            enum { NUM_CLIENTS = 10, NUM_ITERATIONS = 100 };

            int DATA[] = {
                1, 32, 256
            };
            enum { NUM_DATA = sizeof DATA / sizeof *DATA };

            for (int iteration = 0; iteration < NUM_DATA; ++iteration) {
                int messageLength = DATA[iteration];

                bsl::string message;
                loadData(&message, messageLength);

                bslmt::Barrier  barrier(2);

                balb::PipeControlChannel channel(bdlf::BindUtil::bind(
                                                        &verifyPayload,
                                                        message,
                                                        bdlf::PlaceHolders::_1,
                                                        &barrier),
                                                 &ta);

                int rc = channel.start(pipeName);
                ASSERT(0 == rc);

                bslmt::ThreadGroup threadGroup;
                threadGroup.addThreads(bdlf::BindUtil::bind(
                                                         &threadSend,
                                                         bsl::string(pipeName),
                                                         message + "\n",
                                                         (int)NUM_ITERATIONS),
                                       NUM_CLIENTS);

                for (int i = 0; i < NUM_CLIENTS * NUM_ITERATIONS; ++i) {
                    barrier.wait();
                }

                threadGroup.joinAll();

                channel.shutdown();
                channel.stop();
            }
        }
        ASSERT(0 < ta.numAllocations());
        ASSERT(0 == ta.numBytesInUse());
#endif
      } break;
      case 5: {
        // --------------------------------------------------------------------
        // TESTING CONCERN: DATA SENT BY A CLIENT IS READ FROM THE PIPE
        //
        // Concerns:
        //   Data sent through the named pipe by a client is read by the object
        //   and specified as the argument to the control callback.
        //
        // Plan:
        //    Create a pipe control channel and start the channel processing
        //    a named pipe.  Write a payload to the named pipe, and verify the
        //    pipe control channel's control callback is invoked with an
        //    argument having the same value as the payload written to the
        //    pipe.
        //
        // Testing:
        //   Reading data from the named pipe
        // --------------------------------------------------------------------

#if defined(BSLS_PLATFORM_OS_WINDOWS) || defined(BSLS_PLATFORM_OS_CYGWIN)
        if (verbose) {
            cout << "Skipping case 5 on Windows and Cygwin..." << endl;
        }
#else
        if (verbose) {
            cout << "TESTING CONCERN: READING DATA FROM NAMED PIPE" << endl
                 << "=============================================" << endl;
        }

        bslma::TestAllocator ta(veryVeryVeryVerbose);
        {
            bsl::string pipeName;

            ASSERT(0 == bdls::PipeUtil::makeCanonicalName
                   (&pipeName, "ctrl.balb.pcctest5"));
#ifdef BSLS_PLATFORM_OS_UNIX
            unlink(pipeName.c_str());
#endif

            const char MESSAGE[]  = "Hello, world!";

            bslmt::Barrier barrier(2);

            balb::PipeControlChannel channel(bdlf::BindUtil::bind(
                                                        &verifyPayload,
                                                        bsl::string(MESSAGE),
                                                        bdlf::PlaceHolders::_1,
                                                        &barrier),
                                             &ta);

            int rc = channel.start(pipeName);
            ASSERT(0 == rc);

            bdls::PipeUtil::send(pipeName, bsl::string(MESSAGE) + "\n");
            barrier.wait();

            channel.shutdown();
            channel.stop();
        }
        ASSERT(0 < ta.numAllocations());
        ASSERT(0 == ta.numBytesInUse());
#endif
      } break;
      case 4: {
        // --------------------------------------------------------------------
        // TESTING CONCERN: THE DESTRUCTOR INVOKES 'shutdown'
        //
        // Concerns:
        //   Destroying the object shuts down the object and joins the
        //   background thread.
        //
        // Plan:
        //   Create a pipe control channel and start processing a name pipe.
        //   Do no explicitly shut down the pipe control channel.  Destroy the
        //   object, and verify the thread destroying the object is eventually
        //   unblocked.
        //
        // Testing:
        //   ~PipeControlChannel()
        // --------------------------------------------------------------------
        if (verbose) {
            cout << "TESTING CONCERN: DESTRUCTOR INVOKES 'shutdown'" << endl
                 << "=============================================" << endl;
        }

        bslma::TestAllocator ta(veryVeryVeryVerbose);
        {
            bsl::string pipeName;

            ASSERT(0 == bdls::PipeUtil::makeCanonicalName
                   (&pipeName, "ctrl.balb.pcctest4"));
#ifdef BSLS_PLATFORM_OS_UNIX
            unlink(pipeName.c_str());
#endif

            {
            balb::PipeControlChannel channel(&noop, &ta);

            int rc = channel.start(pipeName);
            ASSERT(0 == rc);
            }

            ASSERT("The calling thread is unblocked.");
        }
        ASSERT(0 == ta.numBytesInUse());
      } break;
      case 3: {
        // --------------------------------------------------------------------
        // TESTING CONCERN: 'shutdown' IS IDEMPOTENT
        //
        // Concerns:
        //   The 'shutdown' function is idempotent.
        //
        // Plan:
        //   Start the pipe control channel then explicitly call shutdown and
        //   then stop, validating the result.  Explicitly call shutdown and
        //   then stop again.
        //
        // Testing:
        //   shutdown
        // --------------------------------------------------------------------
        if (verbose) {
            cout << "TESTING CONCERN: 'shutdown' IS IDEMPOTENT" << endl
                 << "=========================================" << endl;
        }

        bslma::TestAllocator ta(veryVeryVeryVerbose);
        {
#ifdef BSLS_PLATFORM_OS_WINDOWS
            const char PIPE_NAME[] = "\\\\.\\pipe\\ctrl.balb.pcctest3";
#else
            const char PIPE_NAME[] = "/tmp/ctrl.balb.pcctest3";
            unlink(PIPE_NAME);
#endif

            balb::PipeControlChannel channel(&noop, &ta);

            int rc = channel.start(PIPE_NAME);
            ASSERT(0 == rc);

            channel.shutdown();
            channel.shutdown();
            channel.stop();

            channel.shutdown();
            channel.shutdown();
            channel.stop();
        }
        ASSERT(0 == ta.numBytesInUse());
      } break;
      case 2: {
        // --------------------------------------------------------------------
        // TESTING CONCERN: THE COMPONENT MAY BE RESTARTED
        //
        // Concerns:
        //   The pipe control channel may be restarted after it has been
        //   shutdown and stopped.
        //
        // Plan:
        //   Create a pipe control channel.  Start, shutdown, and stop the
        //   object.  Restart the pipe control channel with a different name
        //   and verify the result.  Stop the pipe control channel, then
        //   restart the object with the same name as the previous iteration
        //   and verify the result.
        //
        // Testing:
        //   run, shutdown, stop
        // --------------------------------------------------------------------
        if (verbose) {
            cout << "TESTING CONCERN: The Component May Be Restarted" << endl
                 << "===============================================" << endl;
        }

        bslma::TestAllocator ta(veryVeryVeryVerbose);
        {
            int rc;

#ifdef BSLS_PLATFORM_OS_WINDOWS
            const char PIPE_NAME1[] = "\\\\.\\pipe\\ctrl.pcctest2-1";
            const char PIPE_NAME2[] = "\\\\.\\pipe\\ctrl.pcctest2-2";
#else
            const char PIPE_NAME1[] = "/tmp/ctrl.balb.pcctest2-1";
            const char PIPE_NAME2[] = "/tmp/ctrl.balb.pcctest2-2";
            unlink(PIPE_NAME1);
            unlink(PIPE_NAME2);
#endif

            balb::PipeControlChannel channel(&noop, &ta);

            rc = channel.start(PIPE_NAME1);
            ASSERT(0 == rc);

            ASSERT(PIPE_NAME1 == channel.pipeName());

            channel.shutdown();
            channel.stop();

            rc = channel.start(PIPE_NAME2);
            ASSERT(0 == rc);

            ASSERT(PIPE_NAME2 == channel.pipeName());

            channel.shutdown();
            channel.stop();

            rc = channel.start(PIPE_NAME2);
            ASSERT(0 == rc);

            ASSERT(PIPE_NAME2 == channel.pipeName());

            channel.shutdown();
            channel.stop();
        }
        ASSERT(0 == ta.numBytesInUse());
      } break;
      case 1: {
        // --------------------------------------------------------------------
        // BREATHING TEST
        //
        // Concerns:
        //   Exercise default implementation of task manager component by
        //   calling primary manipulators and accessors and verifying their
        //   return codes.
        //
        // Plan:
        //   Create a pipe control channel, ensure the object reports the pipe
        //   name correctly, start, shutdown, and stop the object.  Verify
        //   each function's return code.
        //
        // Testing:
        //   BREATHING TEST
        // --------------------------------------------------------------------
        if (verbose) {
            cout << "Breathing Test" << endl
                 << "==============" << endl;
        }

        bslma::TestAllocator ta(veryVeryVeryVerbose);
        {
#ifdef BSLS_PLATFORM_OS_WINDOWS
            const char PIPE_NAME[] = "\\\\.\\pipe\\ctrl.balb.pcctest1";
#else
            const char PIPE_NAME[] = "/tmp/ctrl.balb.pcctest1";
            unlink(PIPE_NAME);
#endif

            balb::PipeControlChannel channel(&noop, &ta);

            int rc = channel.start(PIPE_NAME);
            ASSERT(0 == rc);

            ASSERT(PIPE_NAME == channel.pipeName());

            channel.shutdown();
            channel.stop();
        }
        ASSERT(0 == ta.numBytesInUse());
      } break;
      default: {
        cerr << "WARNING: CASE `" << test << "' NOT FOUND." << endl;
        testStatus = -1;
      }
    }

    if (testStatus > 0) {
        cerr << "Error, non-zero test status = " << testStatus << "."
             << endl;
    }
    return testStatus;
}

// ----------------------------------------------------------------------------
// Copyright 2017 Bloomberg Finance L.P.
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
