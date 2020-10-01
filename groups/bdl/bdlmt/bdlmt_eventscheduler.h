// bdlmt_eventscheduler.h                                             -*-C++-*-

// ----------------------------------------------------------------------------
//                                   NOTICE
//
// This component is not up to date with current BDE coding standards, and
// should not be used as an example for new development.
// ----------------------------------------------------------------------------

#ifndef INCLUDED_BDLMT_EVENTSCHEDULER
#define INCLUDED_BDLMT_EVENTSCHEDULER

#include <bsls_ident.h>
BSLS_IDENT("$Id: $")

//@PURPOSE: Provide a thread-safe recurring and one-time event scheduler.
//
//@CLASSES:
//  bdlmt::EventScheduler: a thread-safe event scheduler
//  bdlmt::EventSchedulerEventHandle: handle to a single scheduled event
//  bdlmt::EventSchedulerRecurringEventHandle: handle to a recurring event
//
//@SEE_ALSO: bdlmt_timereventscheduler
//
//@DESCRIPTION: This component provides a thread-safe event scheduler.
// 'bdlmt::EventScheduler', that implements methods to schedule and cancel
// recurring and one-time events.  All of the callbacks for these events are
// processed by a separate thread (called the dispatcher thread).  By default
// the callbacks are also executed in the dispatcher thread, but that behavior
// can be altered by providing a dispatcher functor at creation time (see the
// section "The Dispatcher Thread and the Dispatcher Functor").
//
// Events may be referred to by 'bdlmt::EventSchedulerEventHandle' and
// 'bdlmt::EventSchedulerRecurringEventHandle' objects, which clean up after
// themselves when they go out of scope, or by 'Event' and 'RecurringEvent'
// pointers, which must be released using 'releaseEventRaw'.  Such pointers are
// used in the "Raw" API of this class and must be used carefully.  Note that
// the Handle objects have an implicit conversion to the corresponding 'Event'
// or 'RecurringEvent' pointer types, effectively providing extra overloads for
// methods that take a 'const Event*' to also take a 'const EventHandle&'.
//
///Comparison to 'bdlmt::TimerEventScheduler'
/// - - - - - - - - - - - - - - - - - - - - -
// This component was written after 'bdlmt_timereventscheduler', which suffered
// from a couple of short-comings: 1) there was a maximum number of events it
// could manage, and 2) it was inefficient at dealing with large numbers of
// events.  This component addresses both those problems -- there is no limit
// on the number of events it can manage, and it is more efficient at dealing
// with large numbers of events.  The disadvantage of this component relative
// to 'bdlmt_timereventscheduler' is that handles referring to managed events
// in a 'bdlmt::EventScheduler' are reference-counted and need to be released,
// while handles of events in a 'bdlmt::TimerEventScheduler' are integral types
// that do not need to be released.
//
///Thread Safety and "Raw" Event Pointers
///--------------------------------------
// 'bdlmt::EventScheduler' is thread-safe and thread-enabled, meaning that
// multiple threads may use their own instances of the class or use a shared
// instance without further synchronization.  The thread safety and correct
// behavior of the component depend on the correct usage of 'Event' pointers,
// which refer to scheduled events in the "Raw" API of this class.  In
// particular:
//..
//   * Every 'Event*'  and 'RecurringEvent*' populated by 'scheduleEventRaw'
// and 'scheduleRecurringEventRaw' must be released using 'releaseEventRaw.'
//       - Pointers are not released automatically when events are completed.
//       - Pointers are not released automatically when events are canceled.
//       - Events are not canceled when pointers to them are released.
//   * Pointers must not be used after being released.
//   * Pointers must never be shared or duplicated without using
//    'addEventRefRaw' and 'addRecurringEventRefRaw' to get additional
//     references; *each* such added reference must be released separately.
//..
// 'bdlmt::EventSchedulerEventHandle' and
// 'bdlmt::EventSchedulerRecurringEventHandle' are *const* *thread-safe*.  It
// is not safe for multiple threads to invoke non-'const' methods on the same
// 'EventHandle' or 'RecurringEventHandle' object concurrently.
//
///The Dispatcher Thread and the Dispatcher Functor
///------------------------------------------------
// The scheduler creates a single separate thread (called the *dispatcher*
// *thread*) to process all the callbacks.  The dispatcher thread executes the
// callbacks by passing them to the dispatcher functor (optionally specified at
// creation time).  The default dispatcher functor simply invokes the passed
// callback, effectively executing it in the dispatcher thread.  Users can
// alter this behavior by defining their own dispatcher functor (for example in
// order to use a thread pool or a separate thread to run the callbacks).  Note
// that the user-supplied functor will still be run in the dispatcher thread.
//
// CAVEAT: Using a dispatcher functor such as the example above (to execute the
// callback in a separate thread) violates the guarantees of
// cancelEventAndWait().  Users who specify a dispatcher functor that transfers
// the event to another thread for execution should not use
// cancelEventAndWait(), and should instead ensure that the lifetime of any
// object bound to an event exceeds the lifetime of the mechanism used by the
// customized dispatcher functor.
//
///Timer Resolution and Order of Execution
///---------------------------------------
// It is intended that recurring and one-time events are processed as closely
// as possible to their respective time values, and that they are processed in
// the order scheduled.  However, this component *guarantees* only that events
// will not be executed before their scheduled time.  Generally, events that
// are scheduled more than 1 microsecond apart will be executed in the order
// scheduled; but different behavior may be observed when events are submitted
// after (or shortly before) their scheduled time.
//
// When events are executed in the dispatcher thread and take longer to
// complete than the time between events, the dispatcher can fall behind.  In
// this case, events will be executed in the correct order as soon as the
// dispatcher thread becomes available; once the backlog is worked off, events
// will be executed at or near their scheduled times.
//
///Supported Clock-Types
///---------------------
// The component 'bsls::SystemClockType' supplies the enumeration indicating
// the system clock on which times supplied to other methods should be based.
// If the clock type indicated at construction is
// 'bsls::SystemClockType::e_REALTIME', time should be expressed as an absolute
// offset since 00:00:00 UTC, January 1, 1970 (which matches the epoch used in
// 'bdlt::CurrentTime::now(bsls::SystemClockType::e_REALTIME)'.  If the clock
// type indicated at construction is 'bsls::SystemClockType::e_MONOTONIC', time
// should be expressed as an absolute offset since the epoch of this clock
// (which matches the epoch used in
// 'bdlt::CurrentTime::now(bsls::SystemClockType::e_MONOTONIC)'.
//
// The current epoch time for a particular 'bdlmt::EventScheduler' instance
// according to the correct clock is available via the
// 'bdlmt::EventScheduler::now' accessor.
//
///Event Clock Substitution
///------------------------
// For testing purposes, a class 'bdlmt::EventSchedulerTestTimeSource' is
// provided to allow manual manipulation of the system-time observed by a
// 'bdlmt::EventScheduler'.  A test driver that interacts with a
// 'bdlmt::EventScheduler' can use a 'bdlmt::EventSchedulerTestTimeSource'
// object to control when scheduled events are triggered, allowing more
// reliable tests.
//
// A 'bdlmt::EventSchedulerTestTimeSource' can be constructed for any existing
// 'bdlmt::EventScheduler' object that has not been started and has not had any
// events scheduled.  When the 'bdlmt::EventSchedulerTestTimeSource' is
// constructed, it will replace the clock of the 'bdlmt::EventScheduler' to
// which it is attached.  The internal clock of the
// 'bdlmt::EventSchedulerTestTimeSource' will be initialized with an arbitrary
// value on construction, and will advance only when explicitly instructed to
// do so by a call to 'bdlt::EventSchedulerTestTimeSource::advanceTime'.  The
// current value of the internal clock can be accessed by calling
// 'bdlt::EventSchedulerTestTimeSource::now', or 'bdlmt::EventScheduler::now'
// on the instance supplied to the 'bdlmt::EventSchedulerTestTimeSource'.
//
// Note that the initial value of 'bdlt::EventSchedulerTestTimeSource::now' is
// intentionally not synchronized with 'bdlt::CurrentTime::now'.  All test
// events scheduled for a 'bdlmt::EventScheduler' that is instrumented with a
// 'bdlt::EventSchedulerTestTimeSource' should be scheduled in terms of an
// offset from whatever arbitrary time is reported by
// 'bdlt::EventSchedulerTestTimeSource'.  See Example 3 below for an
// illustration of how this is done.
//
///Usage
///-----
// This section illustrates intended use of this component.
//
///Example 1: Simple Clock
///- - - - - - - - - - - -
// In this example we wish to log some statistics periodically.  We define a
// method to store the value of a variable into an array, and set up a
// scheduler to call that as a recurring event.
//..
//   bsls::AtomicInt  g_data;  // Some global data we want to track
//   typedef pair<bdlt::Datetime, int> Value;
//
//   void saveData(vector<Value> *array)
//   {
//      array->push_back(Value(bdlt::CurrentTime::utc(), g_data));
//   }
//..
// We allow the scheduler to run for a short time while changing this value and
// observe that the callback is executed:
//..
//   bdlmt::EventScheduler scheduler;
//   vector<Value> values;
//
//   scheduler.scheduleRecurringEvent(bsls::TimeInterval(1.5),
//                                  bdlf::BindUtil::bind(&saveData, &values)));
//   scheduler.start();
//   bdlt::Datetime start = bdlt::CurrentTime::utc();
//   while ((bdlt::CurrentTime::utc() - start).totalSeconds() < 7) {
//     ++g_data;
//   }
//   scheduler.stop();
//   assert(values.size() >= 4);
//   for (int i = 0; i < values.size(); ++i) {
//     bsl::cout << "At " << values[i].first << " g_data was "
//               << values[i].second << bsl::endl;
//   }
//..
// This will display, e.g.:
//..
//     At 06MAY2008_21:19:17.092 g_data was 816196
//     At 06MAY2008_21:19:18.592 g_data was 1620749
//     At 06MAY2008_21:19:20.092 g_data was 2443358
//     At 06MAY2008_21:19:21.592 g_data was 3267721
//..
//
///Example 2: Server Timeouts
/// - - - - - - - - - - - - -
// The following example shows how to use a 'bdlmt::EventScheduler' to
// implement a timeout mechanism in a server.  'my_Session' maintains several
// connections.  It closes a connection if the data for it does not arrive
// before a timeout (specified at the server creation time).
//
// Because the timeout is relative to the arrival of data, it is best to use a
// "monotonic" clock that advances at a steady rate, rather than a "wall" clock
// that may fluctuate to reflect real time adjustments.
//..
//    class my_Session{
//        // This class encapsulates the data and state associated with a
//        // connection and provides a method 'processData' to process the
//        // incoming data for the connection.
//      public:
//        int processData(void *data, int length);
//            // Process the specified 'data' of the specified 'length'.  (TBD)
//    };
//
//    class my_Server {
//     // This class implements a server maintaining several connections.
//     // A connection is closed if the data for it does not arrive
//     // before a timeout (specified at the server creation time).
//
//     struct Connection {
//         bdlmt::EventSchedulerEventHandle d_timerId;   // handle for timeout
//                                                     // event
//
//         my_Session *d_session_p;                    // session for this
//                                                     // connection
//     };
//
//     bsl::vector<Connection*> d_connections; // maintained connections
//     bdlmt::EventScheduler    d_scheduler;   // timeout event scheduler
//     bsls::TimeInterval       d_ioTimeout;   // time out
//
//     void newConnection(Connection *connection);
//         // Add the specified 'connection' to this server and schedule
//         // the timeout event that closes this connection if the data
//         // for this connection does not arrive before the timeout.
//
//     void closeConnection(Connection *connection);
//         // Close the specified 'connection' and remove it from this server.
//
//     void dataAvailable(Connection *connection, void *data, int length);
//         // Return if the specified 'connection' has already timed-out.
//         // If not, cancel the existing timeout event for the 'connection',
//         // process the specified 'data' of the specified 'length' and
//         // schedule a new timeout event that closes the 'connection' if
//         // the data does not arrive before the timeout.
//
//   public:
//     my_Server(const bsls::TimeInterval&  ioTimeout,
//               bslma::Allocator         *allocator = 0);
//         // Construct a 'my_Server' object with a timeout value of
//         // 'ioTimeout' seconds.  Optionally specify a 'allocator' used to
//         // supply memory.  If 'allocator' is 0, the currently installed
//         // default allocator is used.
//
//     ~my_Server();
//         // Perform the required clean-up and destroy this object.
// };
//
// my_Server::my_Server(const bsls::TimeInterval&  ioTimeout,
//                      bslma::Allocator          *alloc)
// : d_connections(alloc)
// , d_scheduler(bsls::SystemClockType::e_MONOTONIC, alloc)
// , d_ioTimeout(ioTimeout)
// {
//      // TBD: logic to start monitoring the arriving connections or data
//
//      d_scheduler.start();
// }
//
// my_Server::~my_Server()
// {
//     // TBD: logic to clean up
//
//     d_scheduler.stop();
// }
//
// void my_Server::newConnection(my_Server::Connection *connection)
// {
//     // TBD: logic to add 'connection' to 'd_connections'
//
//     // setup the timeout for data arrival
//     d_scheduler.scheduleEvent(
//        &connection->d_timerId,
//        d_scheduler.now() + d_ioTimeout,
//        bdlf::BindUtil::bind(&my_Server::closeConnection, this, connection));
// }
//
// void my_Server::closeConnection(my_Server::Connection *connection)
// {
//     // TBD: logic to close the 'connection' and remove it from 'd_ioTimeout'
// }
//
// void my_Server::dataAvailable(my_Server::Connection *connection,
//                               void                  *data,
//                               int                    length)
// {
//     // If connection has already timed out and closed, simply return.
//     if (d_scheduler.cancelEvent(connection->d_timerId)) {
//         return;                                                    // RETURN
//     }
//
//     // process the data
//     connection->d_session_p->processData(data, length);
//
//     // setup the timeout for data arrival
//     d_scheduler.scheduleEvent(
//        &connection->d_timerId,
//        d_scheduler.now() + d_ioTimeout,
//        bdlf::BindUtil::bind(&my_Server::closeConnection, this, connection));
// }
//..
//
///Example 3: Using the Test Time Source
///- - - - - - - - - - - - - - - - - - -
// For testing purposes, the class 'bdlmt::EventSchedulerTestTimeSource' is
// provided to allow a test to manipulate the system-time observed by a
// 'bdlmt::EventScheduler' in order to control when events are triggered.
// After a scheduler is constructed, a 'bdlmt::EventSchedulerTestTimeSource'
// object can be created atop the scheduler.  A test can then use the test
// time-source to advance the scheduler's observed system-time in order to
// dispatch events in a manner coordinated by the test.  Note that a
// 'bdlmt::EventSchedulerTestTimeSource' *must* be created on an
// event-scheduler before any events are scheduled, or the event-scheduler is
// started.
//
// This example shows how the clock may be altered:
//
//..
// void myCallbackFunction() {
//     puts("Event triggered!");
// }
//
// void testCase() {
//     // Construct the scheduler
//     bdlmt::EventScheduler scheduler;
//
//     // Construct the time-source.
//     // Install the time-source in the scheduler.
//     bdlmt::EventSchedulerTestTimeSource timeSource(&scheduler);
//
//     // Retrieve the initial time held in the time-source.
//     bsls::TimeInterval initialAbsoluteTime = timeSource.now();
//
//     // Schedule a single-run event at a 35s offset.
//     scheduler.scheduleEvent(initialAbsoluteTime + 35,
//                             bsl::function<void()>(&myCallbackFunction));
//
//     // Schedule a 30s recurring event.
//     scheduler.scheduleRecurringEvent(bsls::TimeInterval(30),
//                                      bsl::function<void()>(
//                                                       &myCallbackFunction));
//
//     // Start the dispatcher thread.
//     scheduler.start();
//
//     // Advance the time by 40 seconds so that each
//     // event will run once.
//     timeSource.advanceTime(bsls::TimeInterval(40));
//
//     // The line "Event triggered!" should now have
//     // been printed to the console twice.
//
//     scheduler.stop();
// }
//..
//
// Note that this feature should be used only for testing purposes, never in
// production code.

#include <bdlscm_version.h>

#include <bdlcc_skiplist.h>

#include <bslma_usesbslmaallocator.h>

#include <bslmf_nestedtraitdeclaration.h>

#include <bslmt_condition.h>
#include <bslmt_mutex.h>
#include <bslmt_threadattributes.h>
#include <bslmt_threadutil.h>

#include <bsls_atomic.h>
#include <bsls_systemclocktype.h>
#include <bsls_timeinterval.h>

#include <bslma_allocator.h>

#include <bsls_types.h>

#include <bsl_functional.h>
#include <bsl_memory.h>
#include <bsl_utility.h>

namespace BloombergLP {
namespace bdlmt {

class EventSchedulerEventHandle;
class EventSchedulerRecurringEventHandle;
class EventSchedulerTestTimeSource_Data;

                            // ====================
                            // class EventScheduler
                            // ====================

class EventScheduler {
    // This class provides a thread-safe event scheduler that executes
    // callbacks in a separate "dispatcher thread."  'start' must be invoked to
    // start dispatching the callbacks.  'stop' pauses the dispatching of the
    // callbacks without removing the pending events.

  private:
    // PRIVATE TYPES
    typedef bsl::pair<bsl::function<void()>, bsls::TimeInterval>
                                                           RecurringEventData;

    typedef bdlcc::SkipList<bsls::Types::Int64,
                            RecurringEventData>            RecurringEventQueue;

    typedef bdlcc::SkipList<bsls::Types::Int64,
                            bsl::function<void()> >        EventQueue;

    typedef bsl::function<bsls::TimeInterval()>            CurrentTimeFunctor;

    // FRIENDS
    friend class EventSchedulerEventHandle;
    friend class EventSchedulerRecurringEventHandle;
    friend class EventSchedulerTestTimeSource;

  public:
    // PUBLIC TYPES
    struct Event {};
    struct RecurringEvent {};
        // Pointers to the opaque structures 'Event' and 'RecurringEvent' are
        // populated by the "Raw" API of 'EventScheduler'.

    typedef EventSchedulerEventHandle          EventHandle;

    typedef EventSchedulerRecurringEventHandle RecurringEventHandle;

    typedef bsl::function<void(const bsl::function<void()>&)>
                                               Dispatcher;
        // Defines a type alias for the dispatcher functor type.

  private:
    // NOT IMPLEMENTED
    EventScheduler(const EventScheduler&);
    EventScheduler& operator=(const EventScheduler&);

  private:
    // PRIVATE DATA
    CurrentTimeFunctor    d_currentTimeFunctor; // when called, returns the
                                                // current time the scheduler
                                                // should use for the event
                                                // timeline

    EventQueue            d_eventQueue;         // events

    RecurringEventQueue   d_recurringQueue;     // recurring events

    Dispatcher            d_dispatcherFunctor;  // dispatch events

    bslmt::ThreadUtil::Handle
                          d_dispatcherThread;   // dispatcher thread handle

    bslmt::Mutex          d_dispatcherMutex;    // serialize starting/stopping
                                                // dispatcher thread

    bslmt::Mutex          d_mutex;              // synchronizes access to
                                                // condition variables

    bslmt::Condition      d_queueCondition;     // condition variable used to
                                                // signal when the queues need
                                                // to be checked again (when
                                                // they become non-empty or get
                                                // a new front member)

    bslmt::Condition      d_iterationCondition; // condition variable used to
                                                // signal when the dispatcher
                                                // is ready to enter next
                                                // iteration (synchronizes
                                                // 'wait' methods)

    bool                  d_running;            // controls the looping of the
                                                // dispatcher thread

    bool                  d_dispatcherAwaited;  // A thread is waiting for the
                                                // dispatcher to complete an
                                                // iteration

    RecurringEventQueue::Pair
                         *d_currentRecurringEvent;
                                                // Raw reference to the
                                                // scheduled event being
                                                // executed
    EventQueue::Pair     *d_currentEvent;
                                                // Raw reference to the
                                                // scheduled recurring event
                                                // being executed

    unsigned int          d_waitCount;          // count of the number of waits
                                                // performed in the main
                                                // dispatch loop, used in
                                                // 'advanceTime' to determine
                                                // when to return

    bsls::SystemClockType::Enum
                          d_clockType;          // clock type used

    // PRIVATE MANIPULATORS
    bsls::Types::Int64 chooseNextEvent(bsls::Types::Int64 *now);
        // Pick either 'd_currentEvent' or 'd_currentRecurringEvent' as the
        // next event to be executed, given that the current time is the
        // specified (absolute) 'now' interval, and return the (absolute)
        // interval of the chosen event.  If both 'd_currentEvent' and
        // 'd_currentRecurringEvent' are valid, release whichever one was not
        // chosen.  If both 'd_currentEvent' and 'd_currentRecurringEvent' are
        // scheduled before 'now', choose 'd_currentEvent'.  The behavior is
        // undefined if neither 'd_currentEvent' nor 'd_currentRecurringEvent'
        // is valid.  Note that the argument and return value of this method
        // are expressed in terms of the number of microseconds elapsed since
        // some epoch, which is determined by the clock indicated at
        // construction (see {Supported Clock-Types} in the component
        // documentation).  Also note that this method may update the value of
        // 'now' with the current system time if necessary.

    void dispatchEvents();
        // While d_running is true, execute events in the event and recurring
        // event queues at their scheduled times.  Note that this method
        // implements the dispatching thread.

    void releaseCurrentEvents();
        // Release 'd_currentRecurringEvent' and 'd_currentEvent', if they
        // refer to valid events.

  public:
    // TRAITS
    BSLMF_NESTED_TRAIT_DECLARATION(EventScheduler, bslma::UsesBslmaAllocator);

    // CREATORS
    explicit EventScheduler(bslma::Allocator *basicAllocator = 0);
        // Construct an event scheduler using the default dispatcher functor
        // (see the "The dispatcher thread and the dispatcher functor" section
        // in component-level doc) and use the realtime clock epoch for all
        // time intervals (see {Supported Clock-Types} in the component
        // documentation).  Optionally specify a 'basicAllocator' used to
        // supply memory.  If 'basicAllocator' is 0, the currently installed
        // default allocator is used.

    explicit EventScheduler(bsls::SystemClockType::Enum  clockType,
                            bslma::Allocator            *basicAllocator = 0);
        // Construct an event scheduler using the default dispatcher functor
        // (see the "The dispatcher thread and the dispatcher functor" section
        // in component-level doc) and use the specified 'clockType' to
        // indicate the epoch used for all time intervals (see {Supported
        // Clock-Types} in the component documentation).  Optionally specify a
        // 'basicAllocator' used to supply memory.  If 'basicAllocator' is 0,
        // the currently installed default allocator is used.

    explicit EventScheduler(const Dispatcher&  dispatcherFunctor,
                            bslma::Allocator  *basicAllocator = 0);
        // Construct an event scheduler using the specified 'dispatcherFunctor'
        // (see "The dispatcher thread and the dispatcher functor" section in
        // component-level doc) and use the realtime clock epoch for all time
        // intervals (see {Supported Clock-Types} in the component
        // documentation).  Optionally specify a 'basicAllocator' used to
        // supply memory.  If 'basicAllocator' is 0, the currently installed
        // default allocator is used.

    explicit EventScheduler(const Dispatcher&            dispatcherFunctor,
                            bsls::SystemClockType::Enum  clockType,
                            bslma::Allocator            *basicAllocator = 0);
        // Construct an event scheduler using the specified 'dispatcherFunctor'
        // (see "The dispatcher thread and the dispatcher functor" section in
        // component-level doc) and use the specified 'clockType' to indicate
        // the epoch used for all time intervals (see {Supported Clock-Types}
        // in the component documentation).  Optionally specify a
        // 'basicAllocator' used to supply memory.  If 'basicAllocator' is 0,
        // the currently installed default allocator is used.

    ~EventScheduler();
        // Discard all unprocessed events and destroy this object.  The
        // behavior is undefined unless the scheduler is stopped.

    // MANIPULATORS
    void cancelAllEvents();
        // Cancel all recurring and one-time events scheduled in this
        // EventScheduler.

    void cancelAllEventsAndWait();
        // Cancel all recurring and one-time events scheduled in this
        // EventScheduler.  Block until all events have either been cancelled
        // or dispatched before this call returns.  The behavior is undefined
        // if this method is invoked from the dispatcher thread.

    int cancelEvent(const Event          *handle);
    int cancelEvent(const RecurringEvent *handle);
        // Cancel the event having the specified 'handle'.  Return 0 on
        // successful cancellation, and a non-zero value if the 'handle' is
        // invalid *or* if the event has already been dispatched or canceled.
        // Note that due to the implicit conversion from Handle types, these
        // methods also match the following:
        //..
        //  int cancelEvent(const EventHandle&          handle);
        //  int cancelEvent(const RecurringEventHandle& handle);
        //..
        // Compared to the version taking a pointer to Handle, the managed
        // reference to the event is not released until the Handle goes out of
        // scope.

    int cancelEvent(EventHandle          *handle);
    int cancelEvent(RecurringEventHandle *handle);
        // Cancel the event having the specified 'handle' and release the
        // handle.  Return 0 on successful cancellation, and a non-zero value
        // if the 'handle' is invalid *or* if the event has already been
        // dispatched or canceled.  Note that 'handle' is released whether this
        // call is successful or not.

    int cancelEventAndWait(const Event          *handle);
    int cancelEventAndWait(const RecurringEvent *handle);
        // Cancel the event having the specified 'handle'.  Block until the
        // event having 'handle' (if it is valid) is either successfully
        // canceled or dispatched before the call returns.  Return 0 on
        // successful cancellation, and a non-zero value if 'handle' is invalid
        // *or* if the event has already been dispatched or canceled.  The
        // behavior is undefined if this method is invoked from the dispatcher
        // thread.  Note that if the event is being executed when this method
        // is invoked, this method will block until it is completed and then
        // return a nonzero value.

    int cancelEventAndWait(EventHandle          *handle);
    int cancelEventAndWait(RecurringEventHandle *handle);
        // Cancel the event having the specified 'handle' and release
        // '*handle'.  Block until the event having 'handle' (if it is valid)
        // is either successfully canceled or dispatched before the call
        // returns.  Return 0 on successful cancellation, and a non-zero value
        // if 'handle' is invalid *or* if the event has already been dispatched
        // or canceled.  The behavior is undefined if this method is invoked
        // from the dispatcher thread.  Note that if the event is being
        // executed when this method is invoked, this method will block until
        // it is completed and then return a nonzero value.  Also note that it
        // is guaranteed that '*handle' will be released whether this call is
        // successful or not.

    void releaseEventRaw(Event          *handle);
    void releaseEventRaw(RecurringEvent *handle);
        // Release the specified 'handle'.  Every handle reference added by
        // 'scheduleEventRaw', 'addEventRefRaw', 'scheduleRecurringEventRaw',
        // or 'addRecurringEventRefRaw' must be released using this method to
        // avoid leaking resources.  The behavior is undefined if the value of
        // 'handle' is used for any purpose after being released.

    int rescheduleEvent(const Event               *handle,
                        const bsls::TimeInterval&  newEpochTime);
        // Reschedule the event referred to by the specified 'handle' at the
        // specified 'newEpochTime' truncated to microseconds.  Return 0 on
        // successful reschedule, and a non-zero value if the 'handle' is
        // invalid *or* if the event has already been dispatched.  The
        // 'newEpochTime' is an absolute time represented as an interval from
        // some epoch, which is determined by the clock indicated at
        // construction (see {Supported Clock-Types} in the component
        // documentation).

    int rescheduleEventAndWait(const Event               *handle,
                               const bsls::TimeInterval&  newEpochTime);
        // Reschedule the event referred to by the specified 'handle' at the
        // specified 'newEpochTime' truncated to microseconds.  Block until the
        // event having 'handle' (if it is valid) is either successfully
        // rescheduled or dispatched before the call returns.  Return 0 on
        // successful reschedule, and a non-zero value if 'handle' is invalid
        // *or* if the event has already been dispatched.  The 'newEpochTime'
        // is an absolute time represented as an interval from some epoch,
        // which is determined by the clock indicated at construction (see
        // {Supported Clock-Types} in the component documentation).  The
        // behavior is undefined if this method is invoked from the dispatcher
        // thread.

    void scheduleEvent(const bsls::TimeInterval&     epochTime,
                       const bsl::function<void()>&  callback);
    void scheduleEvent(EventHandle                  *event,
                       const bsls::TimeInterval&     epochTime,
                       const bsl::function<void()>&  callback);
        // Schedule the specified 'callback' to be dispatched at the specified
        // 'epochTime' truncated to microseconds.  Load into the optionally
        // specified 'event' a handle that can be used to cancel the event (by
        // invoking 'cancelEvent').  The 'epochTime' is an absolute time
        // represented as an interval from some epoch, which is determined by
        // the clock indicated at construction (see {Supported Clock-Types} in
        // the component documentation).  Note that 'epochTime' may be in the
        // past, in which case the event will be executed as soon as possible.

    void scheduleEventRaw(Event                        **event,
                          const bsls::TimeInterval&      epochTime,
                          const bsl::function<void()>&   callback);
        // Schedule the specified 'callback' to be dispatched at the specified
        // 'epochTime' truncated to microseconds.  Load into the specified
        // 'event' pointer a handle that can be used to cancel the event (by
        // invoking 'cancelEvent').  The 'epochTime' is an absolute time
        // represented as an interval from some epoch, which is determined by
        // the clock indicated at construction (see {Supported Clock-Types} in
        // the component documentation).  The 'event' pointer must be released
        // by invoking 'releaseEventRaw' when it is no longer needed.

    void scheduleRecurringEvent(const bsls::TimeInterval&     interval,
                                const bsl::function<void()>&  callback,
                                const bsls::TimeInterval&     startEpochTime
                                                      = bsls::TimeInterval(0));
    void scheduleRecurringEvent(RecurringEventHandle         *event,
                                const bsls::TimeInterval&     interval,
                                const bsl::function<void()>&  callback,
                                const bsls::TimeInterval&     startEpochTime
                                                      = bsls::TimeInterval(0));
        // Schedule a recurring event that invokes the specified 'callback' at
        // every specified 'interval' truncated to microseconds, with the first
        // event dispatched at the optionally specified 'startEpochTime'
        // truncated to microseconds.  If 'startEpochTime' is not specified,
        // the first event is dispatched at one 'interval' from now.  Load into
        // the optionally specified 'event' a handle that can be used to cancel
        // the event (by invoking 'cancelEvent').  The 'startEpochTime' is an
        // absolute time represented as an interval from some epoch, which is
        // determined by the clock indicated at construction (see {Supported
        // Clock-Types} in the component documentation).  The behavior is
        // undefined unless 'interval' is at least one microsecond.  Note that
        // if 'startEpochTime' is in the past, the first event is dispatched
        // immediately, and additional '(now - startEpochTime) / interval'
        // events will be submitted serially.

    void scheduleRecurringEventRaw(
                                  RecurringEvent               **event,
                                  const bsls::TimeInterval&      interval,
                                  const bsl::function<void()>&   callback,
                                  const bsls::TimeInterval&      startEpochTime
                                                      = bsls::TimeInterval(0));
        // Schedule a recurring event that invokes the specified 'callback' at
        // every specified 'interval' truncated to microseconds, with the first
        // event dispatched at the optionally specified 'startEpochTime'
        // truncated to microseconds.  If 'startEpochTime' is not specified,
        // the first event is dispatched at one 'interval' from now.  Load into
        // the specified 'event' pointer a handle that can be used to cancel
        // the event (by invoking 'cancelEvent').  The 'startEpochTime' is an
        // absolute time represented as an interval from some epoch, which is
        // determined by the clock indicated at construction (see {Supported
        // Clock-Types} in the component documentation).  The 'event' pointer
        // must be released by invoking 'releaseEventRaw' when it is no longer
        // needed.  The behavior is undefined unless 'interval' is at least one
        // microsecond.  Note that if 'startEpochTime' is in the past, the
        // first event is dispatched immediately, and additional
        // '(now - startEpochTime) / interval' events will be submitted
        // serially.

    int start();
        // Begin dispatching events on this scheduler using default attributes
        // for the dispatcher thread.  Return 0 on success, and a nonzero value
        // otherwise.  If another thread is currently executing 'stop', wait
        // until the dispatcher thread stops before starting a new one.  If
        // this scheduler has already started (and is not currently being
        // stopped by another thread) then this invocation has no effect and 0
        // is returned.  The behavior is undefined if this method is invoked in
        // the dispatcher thread (i.e., in a job executed by this scheduler).
        // Note that any event whose time has already passed is pending and
        // will be dispatched immediately.

    int start(const bslmt::ThreadAttributes& threadAttributes);
        // Begin dispatching events on this scheduler using the specified
        // 'threadAttributes' for the dispatcher thread (except that the
        // DETACHED attribute is ignored).  Return 0 on success, and a nonzero
        // value otherwise.  If another thread is currently executing 'stop',
        // wait until the dispatcher thread stops before starting a new one.
        // If this scheduler has already started (and is not currently being
        // stopped by another thread) then this invocation has no effect and 0
        // is returned.  The behavior is undefined if this method is invoked in
        // the dispatcher thread (i.e., in a job executed by this scheduler).
        // Note that any event whose time has already passed is pending and
        // will be dispatched immediately.

    void stop();
        // End the dispatching of events on this scheduler (but do not remove
        // any pending events), and wait for any (one) currently executing
        // event to complete.  If the scheduler is already stopped then this
        // method has no effect.  This scheduler can be restarted by invoking
        // 'start'.  The behavior is undefined if this method is invoked from
        // the dispatcher thread.

    // ACCESSORS
    Event *addEventRefRaw(Event *handle) const;
        // Increment the reference count for the event referred to by the
        // specified 'handle' and return 'handle'.  There must be a
        // corresponding call to 'releaseEventRaw' when the reference is no
        // longer needed.

    RecurringEvent *addRecurringEventRefRaw(RecurringEvent *handle) const;
        // Increment the reference count for the recurring event referred to by
        // the specified 'handle' and return 'handle'.  There must be a
        // corresponding call to 'releaseEventRaw' when the reference is no
        // longer needed.

    bsls::SystemClockType::Enum clockType() const;
        // Return the value of the clock type that this object was created
        // with.

    bsls::TimeInterval now() const;
        // Return the current epoch time, an absolute time represented as an
        // interval from some epoch, which is determined by the clock indicated
        // at construction (see {Supported Clock-Types} in the component
        // documentation).

    int numEvents() const;
        // Return the number of pending one-time events in this scheduler.

    int numRecurringEvents() const;
        // Return the number of recurring events registered with this
        // scheduler.

                                  // Aspects

    bslma::Allocator *allocator() const;
        // Return the allocator used by this object to supply memory.
};

                      // ===============================
                      // class EventSchedulerEventHandle
                      // ===============================

class EventSchedulerEventHandle
{
    // Objects of this type refer to events in the 'EventScheduler' API.  They
    // are convertible to 'const Event*' references and may be used in any
    // method that expects them.

    // PRIVATE TYPES
    typedef bdlcc::SkipList<bsls::Types::Int64,
                            bsl::function<void()> > EventQueue;

    // DATA
    EventQueue::PairHandle  d_handle;

    // FRIENDS
    friend class EventScheduler;

  public:
    // PUBLIC TYPES
    typedef EventScheduler::Event Event;

    // CREATORS
    EventSchedulerEventHandle();
        // Create a new handle object that does not refer to an event.

    EventSchedulerEventHandle(const EventSchedulerEventHandle& original);
        // Create a new handle object referring to the same event as the
        // specified 'rhs' handle.

    ~EventSchedulerEventHandle();
        // Destroy this object and release the managed reference, if any.

    // MANIPULATORS
    EventSchedulerEventHandle& operator=(const EventSchedulerEventHandle& rhs);
        // Release this handle's reference, if any; then make this handle refer
        // to the same event as the specified 'rhs' handle.  Return a
        // modifiable reference to this handle.

    void release();
        // Release the reference (if any) held by this object.

    // ACCESSORS
    operator const Event*() const;
        // Return a "raw" pointer to the event managed by this handle, or 0 if
        // this handle does not manage a reference.
};

                  // ========================================
                  // class EventSchedulerRecurringEventHandle
                  // ========================================

class EventSchedulerRecurringEventHandle
{
    // Objects of this type refer to recurring events in the 'EventScheduler'
    // API.  They are convertible to 'const RecurringEvent*' references and may
    // be used in any method which expects these.

    // PRIVATE TYPES
    typedef bsl::pair<bsl::function<void()>, bsls::TimeInterval>
                                                       RecurringEventData;
    typedef bdlcc::SkipList<bsls::Types::Int64,
                            RecurringEventData>        RecurringEventQueue;

    // DATA
    RecurringEventQueue::PairHandle  d_handle;

    // FRIENDS
    friend class EventScheduler;

  public:
    // PUBLIC TYPES
    typedef EventScheduler::RecurringEvent RecurringEvent;

    // CREATORS
    EventSchedulerRecurringEventHandle();
        // Create a new handle object.

    EventSchedulerRecurringEventHandle(
                           const EventSchedulerRecurringEventHandle& original);
        // Create a new handle object referring to the same recurring event as
        // the specified 'rhs' handle.

    ~EventSchedulerRecurringEventHandle();
        // Destroy this object and release the managed reference, if any.

    // MANIPULATORS
    EventSchedulerRecurringEventHandle& operator=(
                                const EventSchedulerRecurringEventHandle& rhs);
        // Release the reference managed by this handle, if any; then make this
        // handle refer to the same recurring event as the specified 'rhs'
        // handle.  Return a modifiable reference to this event handle.

    void release();
        // Release the reference managed by this handle, if any.

    // ACCESSORS
    operator const RecurringEvent*() const;
        // Return a "raw" pointer to the recurring event managed by this
        // handle, or 0 if this handle does not manage a reference.

};

                     // ==================================
                     // class EventSchedulerTestTimeSource
                     // ==================================

class EventSchedulerTestTimeSource {
    // This class provides a means to change the clock that is used by a given
    // event-scheduler to determine when events should be triggered.
    // Constructing a 'EventSchedulerTestTimeSource' alters the behavior of the
    // supplied event-scheduler.  After a test time-source is created, the
    // underlying scheduler will run events according to a discrete timeline,
    // whose successive values are determined by calls to 'advanceTime' on the
    // test time-source, and can be retrieved by calling 'now' on that test
    // time-source.  Note that the "system-time" held by a test time-source
    // *does* *not* correspond to the current system time.  Test writers must
    // use caution when scheduling absolute-time events so that they are
    // scheduled relative to the test time-source's value for 'now'.

  private:
    // DATA
    bsl::shared_ptr<EventSchedulerTestTimeSource_Data>
                          d_data_p;       // shared pointer to the state whose
                                          // lifetime must be as long as
                                          // '*this' and '*d_scheduler_p'

    EventScheduler       *d_scheduler_p;  // pointer to the scheduler that we
                                          // are augmenting

  public:
    // CREATORS
    EventSchedulerTestTimeSource(EventScheduler *scheduler);
        // Construct a test time-source object that will control the
        // "system-time" observed by the specified 'scheduler'.  Initialize
        // 'now' to be an arbitrary time value.  The behavior is undefined if
        // any methods have previously been called on 'scheduler'.

    // MANIPULATORS
    bsls::TimeInterval advanceTime(bsls::TimeInterval amount);
        // Advance this object's current-time value by the specified 'amount'
        // of time, notify the scheduler that the time has changed, and wait
        // for the scheduler to process the events triggered by this change in
        // time.  Return the updated current-time value.  The behavior is
        // undefined unless 'amount' is positive, and 'now + amount' is within
        // the range that can be represented with a 'bsls::TimeInterval'.

    // ACCESSORS
    bsls::TimeInterval now();
        // Return this object's current-time value.  Upon construction, this
        // method will return an arbitrary value.  Subsequent calls to
        // 'advanceTime' will adjust the arbitrary value forward.
};

// ============================================================================
//                            INLINE DEFINITIONS
// ============================================================================

                      // -------------------------------
                      // class EventSchedulerEventHandle
                      // -------------------------------

// CREATORS
inline
EventSchedulerEventHandle::EventSchedulerEventHandle()
{
}

inline
EventSchedulerEventHandle::EventSchedulerEventHandle(
                                     const EventSchedulerEventHandle& original)
: d_handle(original.d_handle)
{
}

inline
EventSchedulerEventHandle::~EventSchedulerEventHandle()
{
}

// MANIPULATORS
inline
EventSchedulerEventHandle&
EventSchedulerEventHandle::operator=(const EventSchedulerEventHandle& rhs)
{
    d_handle = rhs.d_handle;
    return *this;
}

inline
void EventSchedulerEventHandle::release()
{
    d_handle.release();
}
}  // close package namespace

// ACCESSORS

// The scoping of "Event" below should not be necessary, but xlc (versions 8
// and 9) requires it
inline
bdlmt::EventSchedulerEventHandle::
operator const bdlmt::EventSchedulerEventHandle::Event*() const
{
    return (const Event*)((const EventQueue::Pair*)d_handle);
}

namespace bdlmt {

                  // ----------------------------------------
                  // class EventSchedulerRecurringEventHandle
                  // ----------------------------------------

// CREATORS
inline
EventSchedulerRecurringEventHandle::EventSchedulerRecurringEventHandle()
{
}

inline
EventSchedulerRecurringEventHandle::EventSchedulerRecurringEventHandle(
                            const EventSchedulerRecurringEventHandle& original)
: d_handle(original.d_handle)
{
}

inline
EventSchedulerRecurringEventHandle::~EventSchedulerRecurringEventHandle()
{
}

// MANIPULATORS
inline
void EventSchedulerRecurringEventHandle::release()
{
    d_handle.release();
}

inline
EventSchedulerRecurringEventHandle&
EventSchedulerRecurringEventHandle::operator=(
                                 const EventSchedulerRecurringEventHandle& rhs)
{
    d_handle = rhs.d_handle;
    return *this;
}
}  // close package namespace

// ACCESSORS

// The scoping of "RecurringEvent" below should not be necessary, but xlc
// (versions 8 and 9) requires it
inline
bdlmt::EventSchedulerRecurringEventHandle::operator
       const bdlmt::EventSchedulerRecurringEventHandle::RecurringEvent*() const
{
    return (const RecurringEvent*)((const RecurringEventQueue::Pair*)d_handle);
}

namespace bdlmt {

                            // --------------------
                            // class EventScheduler
                            // --------------------

// MANIPULATORS
inline
int EventScheduler::cancelEvent(const Event *handle)
{
    const EventQueue::Pair *itemPtr =
                        reinterpret_cast<const EventQueue::Pair*>(
                                        reinterpret_cast<const void*>(handle));

    return d_eventQueue.remove(itemPtr);
}

inline
int EventScheduler::cancelEvent(const RecurringEvent *handle)
{
    const RecurringEventQueue::Pair *itemPtr =
                reinterpret_cast<const RecurringEventQueue::Pair*>(
                                        reinterpret_cast<const void*>(handle));

    return d_recurringQueue.remove(itemPtr);
}

inline
void EventScheduler::scheduleEvent(const bsls::TimeInterval&    epochTime,
                                   const bsl::function<void()>& callback)
{
    scheduleEventRaw(0, epochTime, callback);
}

inline
void EventScheduler::releaseEventRaw(Event *handle)
{
    d_eventQueue.releaseReferenceRaw(reinterpret_cast<EventQueue::Pair*>(
                                             reinterpret_cast<void*>(handle)));
}

inline
void EventScheduler::releaseEventRaw(RecurringEvent *handle)
{
    d_recurringQueue.releaseReferenceRaw(
                         reinterpret_cast<RecurringEventQueue::Pair*>(
                                             reinterpret_cast<void*>(handle)));
}

inline
void EventScheduler::scheduleRecurringEvent(
                                   const bsls::TimeInterval&    interval,
                                   const bsl::function<void()>& callback,
                                   const bsls::TimeInterval&    startEpochTime)
{
    scheduleRecurringEventRaw(0, interval, callback, startEpochTime);
}

// ACCESSORS
inline
EventScheduler::Event*
EventScheduler::addEventRefRaw(Event *handle) const
{
    EventQueue::Pair *h = reinterpret_cast<EventQueue::Pair*>(
                                              reinterpret_cast<void*>(handle));
    return reinterpret_cast<Event*>(d_eventQueue.addPairReferenceRaw(h));
}

inline
EventScheduler::RecurringEvent*
EventScheduler::addRecurringEventRefRaw(RecurringEvent *handle) const
{
    RecurringEventQueue::Pair *h =
                               reinterpret_cast<RecurringEventQueue::Pair*>(
                                              reinterpret_cast<void*>(handle));
    return reinterpret_cast<RecurringEvent*>(
                                     d_recurringQueue.addPairReferenceRaw(h));
}

inline
bsls::SystemClockType::Enum EventScheduler::clockType() const
{
    return d_clockType;
}

inline
bsls::TimeInterval EventScheduler::now() const
{
    return d_currentTimeFunctor();
}

inline
int EventScheduler::numEvents() const
{
    return d_eventQueue.length();
}

inline
int EventScheduler::numRecurringEvents() const
{
    return d_recurringQueue.length();
}

                                  // Aspects

inline
bslma::Allocator *EventScheduler::allocator() const
{
    return d_eventQueue.allocator();
}

}  // close package namespace
}  // close enterprise namespace

#endif

// ----------------------------------------------------------------------------
// Copyright 2019 Bloomberg Finance L.P.
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
