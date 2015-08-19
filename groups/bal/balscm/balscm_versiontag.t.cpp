// balscm_versiontag.t.cpp                                            -*-C++-*-

#include <balscm_versiontag.h>

#include <bsl_iostream.h>

#include <bsl_cstdlib.h>     // atoi()
#include <bsl_cstdio.h>

using namespace BloombergLP;

//=============================================================================
//                  STANDARD BDE ASSERT TEST MACRO
//-----------------------------------------------------------------------------
static int testStatus = 0;
static bool verbose = false;
static bool veryVerbose = false;

static void aSsErT(int c, const char *s, int i) {
    if (c) {
        bsl::fprintf(stderr, "Error " __FILE__ "(%d): %s    (failed)\n", i, s);
        if (testStatus >= 0 && testStatus <= 100) ++testStatus;
    }
}

#define ASSERT(X) { aSsErT(!(X), #X, __LINE__); }

#define LOOP_ASSERT(I,X) { \
    if (!(X)) { bsl::printf("%s: %d\n", #I, I); \
                aSsErT(1, #X, __LINE__); } }

//-----------------------------------------------------------------------------

//=============================================================================
//                  SEMI-STANDARD TEST OUTPUT MACROS
//-----------------------------------------------------------------------------
 #define P(X) cout << #X " = " << (X) << endl; // Print identifier and value.
#define Q(X) bsl::printf("<| " #X " |>\n");  // Quote identifier literally.
#define P_(X) cout << #X " = " << (X) << ", " << flush; // P(X) without '\n'
#define L_ __LINE__                           // current Line number
#define T_ bsl::printf("\t");             // Print a tab (w/o newline)

//=============================================================================
//                  USAGE EXAMPLE HELPER FUNCTIONS
//-----------------------------------------------------------------------------

int newFunction()
    // Return 1
{
    return 1;
}

// int OldFunction()
// Not defined and never called due to conditional compilation

//=============================================================================
//                              MAIN PROGRAM
//-----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    int test = argc > 1 ? bsl::atoi(argv[1]) : 0;

    verbose = (argc > 2);
    veryVerbose = (argc > 3);

    bsl::printf("TEST %s CASE %d\n", __FILE__, test);

    switch (test) { case 0:
      case 2: {
        //--------------------------------------------------------------------
        // TEST USAGE EXAMPLE
        //
        // Concern:
        //   That the usage example in the user documentation compiles and
        //   runs as expected.
        //
        // Plan:
        //   Use the exact text of the usage example from the user
        //   documentation, but change uses of 'assert' to 'ASSERT'.
        //
        // Testing:
        //   USAGE EXAMPLE
        //--------------------------------------------------------------------

        if (verbose) bsl::printf("\nTEST USAGE EXAMPLE"
                                 "\n==================\n");

    #if BAL_VERSION > BSL_MAKE_VERSION(1, 2)
        // Call 'newFunction' for BAL version 1.2 and later:
        int result = newFunction();
    #else
        // Call 'oldFunction' for BAE older than version 1.2:
        int result = oldFunction();
    #endif

        ASSERT(result);
      } break;

      case 1: {
        //--------------------------------------------------------------------
        // TEST VERSION CONSISTENCY
        //
        // Concerns:
        //   That BAL_VERSION corresponds to the three components,
        //   BAL_VERSION_MAJOR, BAL_VERSION_MINOR, and BAL_VERSION_PATCH.
        //
        // Plan:
        //   Decompose BAL_VERSION into its three components and verify
        //   that they correspond to the defined macros.
        //
        // Testing:
        //   BAL_VERSION
        //   BAL_VERSION_MAJOR
        //   BAL_VERSION_MINOR
        //   BAL_VERSION_PATCH
        //--------------------------------------------------------------------

        if (verbose) bsl::printf("\nTEST VERSION CONSISTENCY"
                                 "\n========================\n");

        int major = (BAL_VERSION / 10000) % 100;
        int minor = (BAL_VERSION / 100) % 100;
        int patch = BAL_VERSION % 100;

        ASSERT(BAL_VERSION_MAJOR == major);
        ASSERT(BAL_VERSION_MINOR == minor);

      } break;

      default: {
        bsl::fprintf(stderr, "WARNING: CASE `%d' NOT FOUND.\n", test);
        testStatus = -1;
      }
    }

    if (testStatus > 0) {
        bsl::fprintf(stderr, "Error, non-zero test status = %d.\n",
                     testStatus);
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
