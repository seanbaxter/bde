// bdlb_strtokenrefiter.cpp                                           -*-C++-*-
#include <bdlb_strtokenrefiter.h>

#include <bsls_ident.h>
BSLS_IDENT_RCSID(bdlb_strtokenrefiter_cpp,"$Id$ $CSID$")

#include <bsls_assert.h>

#include <bsl_cstring.h>    // 'bsl::memset', 'bsl::strlen'

// MACROS

#define AT_EOS(p)  ((p) == d_end_p)

// STATICS

namespace {

// The character inputs break down into the following types:

enum InputType {
    TOK = 0,  // token
    SFT = 1,  // soft delimiter
    HRD = 2,  // hard delimiter
    EOS = 3   // end of string
};
const int NUM_INPUTS = 4;

// The following defines the set of similar actions to be performed:
enum Action {
    ACT_AT = 0,  // accumulate token
    ACT_AD = 1,  // accumulate delimiter
    ACT_RT = 2,  // return from function
    ACT_ER = 3   // an error has occurred or string is invalid
};

// The following defines the set of states:
enum State {
    START = 0,  // No token or hard delimiter character seen yet and
                // possibly some soft delimiter characters seen.
    SOFTD = 1,  // One or more soft delimiter characters seen after one or
                // more token characters have been seen.
    HARDD = 2,  // Exactly one hard delimiter character seen and
                // possibly some soft delimiter characters seen.
    TOKEN = 3   // One or more token characters have been seen.
};
const int NUM_STATES = 4;

const State nextStateTable[NUM_STATES][NUM_INPUTS] = {
    // TOK     SFT     HRD     EOS
    // -----   -----   -----   -----
    {  TOKEN,  START,  HARDD,  START  },  // START
    {  START,  SOFTD,  HARDD,  START  },  // SOFTD
    {  START,  HARDD,  START,  START  },  // HARDD
    {  TOKEN,  SOFTD,  HARDD,  START  },  // TOKEN
};

const Action actionTable[NUM_STATES][NUM_INPUTS] = {
    // TOK     SFT     HRD     EOS
    // ------  ------  ------  ------
    {  ACT_AT, ACT_ER, ACT_AD, ACT_ER },  // START
    {  ACT_RT, ACT_AD, ACT_AD, ACT_RT },  // SOFTD
    {  ACT_RT, ACT_AD, ACT_RT, ACT_RT },  // HARDD
    {  ACT_AT, ACT_AD, ACT_AD, ACT_RT },  // TOKEN
};

void loadDelimTable(char       *table,
                    int         tableSize,
                    const char *softDelims,
                    const char *hardDelims)
    // Load the specified 'softDelims' and 'hardDelims' characters into the
    // specified 'table' having the specified 'tableSize'.  If a character
    // occurs in both 'softDelims' and 'hardDelims', then the one in
    // 'hardDelims' takes precedence.  The behavior is undefined unless
    // 'tableSize > 0', 'table', 'softDelims', and 'hardDelims' are
    // null-terminated, and no character in 'softDelims' or 'hardDelims' has a
    // value greater than 'tableSize - 1'.
{
    bsl::memset(table, TOK, tableSize);
    table['\0'] = EOS;
    if (softDelims) {
        while (*softDelims) {
            table[(unsigned char)*softDelims++] = SFT;
        }
    }
    if (hardDelims) {
        while (*hardDelims) {
            table[(unsigned char)*hardDelims++] = HRD;
        }
    }
}

}  // close unnamed namespace

namespace BloombergLP {

namespace bdlb {
                       // ---------------------------
                       // class StrTokenRefIter
                       // ---------------------------

// CREATORS
StrTokenRefIter::StrTokenRefIter(const char *input,
                                             const char *softDelims)
: d_cursor_p(0)
, d_prevDelim_p(0)
, d_token_p(0)
, d_postDelim_p(0)
, d_end_p(0)
{
    loadDelimTable(d_delimTable, k_TABLE_SIZE, softDelims, 0);
    reset(input);
}

StrTokenRefIter::StrTokenRefIter(const char *input,
                                             const char *softDelims,
                                             const char *hardDelims)
: d_cursor_p(0)
, d_prevDelim_p(0)
, d_token_p(0)
, d_postDelim_p(0)
, d_end_p(0)
{
    loadDelimTable(d_delimTable, k_TABLE_SIZE, softDelims, hardDelims);
    reset(input);
}

StrTokenRefIter::StrTokenRefIter(
                                            const bslstl::StringRef&  input,
                                            const char             *softDelims)
: d_cursor_p(0)
, d_prevDelim_p(0)
, d_token_p(0)
, d_postDelim_p(0)
, d_end_p(0)
{
    loadDelimTable(d_delimTable, k_TABLE_SIZE, softDelims, 0);
    reset(input);
}

StrTokenRefIter::StrTokenRefIter(
                                            const bslstl::StringRef&  input,
                                            const char             *softDelims,
                                            const char             *hardDelims)
: d_cursor_p(0)
, d_prevDelim_p(0)
, d_token_p(0)
, d_postDelim_p(0)
, d_end_p(0)
{
    loadDelimTable(d_delimTable, k_TABLE_SIZE, softDelims, hardDelims);
    reset(input);
}

StrTokenRefIter::~StrTokenRefIter()
{
    BSLS_ASSERT(                   d_prevDelim_p <= d_token_p);
    BSLS_ASSERT(                   d_token_p     <= d_postDelim_p);
    BSLS_ASSERT(                   d_postDelim_p <= d_end_p);
    BSLS_ASSERT(0 == d_cursor_p || d_cursor_p    <= d_end_p);
}

// MANIPULATORS
void StrTokenRefIter::operator++()
{
    BSLS_ASSERT(d_cursor_p);

    if (AT_EOS(d_cursor_p)) {
        d_cursor_p = 0;             // invalidate iterator
        return;                                                       // RETURN
    }

    d_prevDelim_p = d_postDelim_p;  // current delimiter becomes previous

    d_token_p     = d_cursor_p;
    d_postDelim_p = d_cursor_p;

    int currentState = START;
    int inputType    = AT_EOS(d_cursor_p)
                     ? (char)EOS
                     : d_delimTable[(unsigned char)*d_cursor_p];

    while (true) {
        switch (actionTable[currentState][inputType]) {
          case ACT_AT: {  // accumulate token
            ++d_postDelim_p;
          } break;
          case ACT_AD: {  // accumulate delimiter
            // Nothing to do.
          } break;
          case ACT_RT: {  // return from operator++
            return;                                                   // RETURN
          } break;
          default: {
            BSLS_ASSERT(0);
          } break;
        }

        currentState = nextStateTable[currentState][inputType];
        ++d_cursor_p;
        inputType = AT_EOS(d_cursor_p)
                  ? (char)EOS
                  : d_delimTable[(unsigned char)*d_cursor_p];

    }
    BSLS_ASSERT(0);
}

void StrTokenRefIter::reset(const char *input)
{
    d_cursor_p    = input;
    d_prevDelim_p = input;
    d_token_p     = input;
    d_postDelim_p = input;
    d_end_p       = input ? input + bsl::strlen(input) : 0;

    if (d_cursor_p) {                // accumulate leader in previous delimiter
        while (!AT_EOS(d_cursor_p)
            && SFT == d_delimTable[(unsigned char)*d_cursor_p]) {
            ++d_cursor_p;
        }

        if (AT_EOS(d_cursor_p)) {
            d_token_p     = d_end_p;
            d_postDelim_p = d_end_p;
            d_cursor_p    = 0;       // invalidate iterator
        }
        else {
            ++*this;                 // "wake up" iterator
        }
    }
}

void StrTokenRefIter::reset(const bslstl::StringRef& input)
{
    d_cursor_p    = input.begin();
    d_prevDelim_p = input.begin();
    d_token_p     = input.begin();
    d_postDelim_p = input.begin();
    d_end_p       = input.end();

    if (d_cursor_p) {                // accumulate leader in previous delimiter
        while (!AT_EOS(d_cursor_p)
            && SFT == d_delimTable[(unsigned char)*d_cursor_p]) {
            ++d_cursor_p;
        }

        if (AT_EOS(d_cursor_p)) {
            d_token_p     = d_end_p;
            d_postDelim_p = d_end_p;
            d_cursor_p    = 0;       // invalidate iterator
        }
        else {
            ++*this;                 // "wake up" iterator
        }
    }
}

// ACCESSORS
bool StrTokenRefIter::isHard() const
{
    if (!d_postDelim_p) {
        return false;                                                 // RETURN
    }

    const char *p   = d_postDelim_p;
    const char *end = d_cursor_p ? d_cursor_p : d_end_p;

    while (p != end) {
        if (HRD == d_delimTable[(unsigned char)*p]) {
            return true;                                              // RETURN
        }
        ++p;
    }
    return false;
}

bool StrTokenRefIter::isPreviousHard() const
{
    if (!d_prevDelim_p) {
        return false;                                                 // RETURN
    }

    const char *p = d_prevDelim_p;

    while (p != d_token_p) {
        if (HRD == d_delimTable[(unsigned char)*p]) {
            return true;                                              // RETURN
        }
        ++p;
    }
    return false;
}

bool StrTokenRefIter::hasSoft() const
{
    if (!d_postDelim_p) {
        return false;                                                 // RETURN
    }

    const char *p = d_postDelim_p;

    if (AT_EOS(p) || p == d_cursor_p) {
        return false;                                                 // RETURN
    }

    if (SFT == d_delimTable[(unsigned char)*p]) {
        return true;                                                  // RETURN
    }

    ++p;
    if (AT_EOS(p) || p == d_cursor_p) {
        return false;                                                 // RETURN
    }

    return SFT == d_delimTable[(unsigned char)*p];
}

bool StrTokenRefIter::hasPreviousSoft() const
{
    if (!d_prevDelim_p) {
        return false;                                                 // RETURN
    }

    const char *p = d_prevDelim_p;

    if (AT_EOS(p) || p == d_token_p) {
        return false;                                                 // RETURN
    }

    if (SFT == d_delimTable[(unsigned char)*p]) {
        return true;                                                  // RETURN
    }

    ++p;
    if (AT_EOS(p) || p == d_token_p) {
        return false;                                                 // RETURN
    }

    return SFT == d_delimTable[(unsigned char)*p];
}
}  // close package namespace

}  // close enterprise namespace

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
