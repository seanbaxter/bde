// ball_loggercategoryutil.cpp                                        -*-C++-*-
#include <ball_loggercategoryutil.h>

#include <bsls_ident.h>
BSLS_IDENT_RCSID(ball_loggercategoryutil_cpp,"$Id$ $CSID$")

#include <ball_categorymanager.h>
#include <ball_loggermanager.h>
#include <ball_testobserver.h>                // for testing only

#include <bsls_assert.h>

#include <bsl_cstring.h>    // strlen(), strncmp()

namespace BloombergLP {

namespace ball {
                            // ------------------------------
                            // struct LoggerCategoryUtil
                            // ------------------------------

// CLASS METHODS

Category *LoggerCategoryUtil::addCategoryHierarchically(
                                             LoggerManager *loggerManager,
                                             const char         *categoryName)
{
    BSLS_ASSERT(loggerManager);
    BSLS_ASSERT(categoryName);

    if (loggerManager->lookupCategory(categoryName)) {
        return 0;                                                     // RETURN
    }

    Category *longestPrefixCategory =  0;
    int            longestPrefixLength   = -1;

    for (LoggerCategoryManip manip(loggerManager);
         manip;
         manip.advance()) {
        int length = bsl::strlen(manip().categoryName());
        if (longestPrefixLength < length
         && 0 == bsl::strncmp(manip().categoryName(), categoryName, length)) {
            longestPrefixCategory = &manip();
            longestPrefixLength   = length;
        }
    }

    Category *category = 0;
    if (longestPrefixCategory) {
        category = loggerManager->addCategory(
                                     categoryName,
                                     longestPrefixCategory->recordLevel(),
                                     longestPrefixCategory->passLevel(),
                                     longestPrefixCategory->triggerLevel(),
                                     longestPrefixCategory->triggerAllLevel());
    }
    else {
        category = loggerManager->addCategory(
                             categoryName,
                             loggerManager->defaultRecordThresholdLevel(),
                             loggerManager->defaultPassThresholdLevel(),
                             loggerManager->defaultTriggerThresholdLevel(),
                             loggerManager->defaultTriggerAllThresholdLevel());
    }

    return category;
}

int LoggerCategoryUtil::setThresholdLevelsHierarchically(
                                        LoggerManager *loggerManager,
                                        const char         *categoryNamePrefix,
                                        int                 recordLevel,
                                        int                 passLevel,
                                        int                 triggerLevel,
                                        int                 triggerAllLevel)
{
    BSLS_ASSERT(loggerManager);
    BSLS_ASSERT(categoryNamePrefix);

    if (!Category::areValidThresholdLevels(recordLevel,
                                                passLevel,
                                                triggerLevel,
                                                triggerAllLevel)) {
        return -1;                                                    // RETURN
    }

    int matchCount = 0;
    bsl::size_t length = bsl::strlen(categoryNamePrefix);

    for (LoggerCategoryManip manip(loggerManager);
         manip;
         manip.advance()) {
        if (0 == bsl::strncmp(manip().categoryName(),
                              categoryNamePrefix,
                              length)) {
            const int s = manip().setLevels(recordLevel,
                                            passLevel,
                                            triggerLevel,
                                            triggerAllLevel);
            BSLS_ASSERT(0 == s);
            ++matchCount;
        }
    }
    return matchCount;
}

int LoggerCategoryUtil::setThresholdLevels(
                                         LoggerManager *loggerManager,
                                         const char         *regularExpression,
                                         int                 recordLevel,
                                         int                 passLevel,
                                         int                 triggerLevel,
                                         int                 triggerAllLevel)
{
    BSLS_ASSERT(loggerManager);
    BSLS_ASSERT(regularExpression);

    if (!Category::areValidThresholdLevels(recordLevel,
                                                passLevel,
                                                triggerLevel,
                                                triggerAllLevel)) {
        return -1;                                                    // RETURN
    }

    bsl::size_t len = bsl::strlen(regularExpression);

    if (0 == len || '*' != regularExpression[len - 1]) {
        Category *category =
                              loggerManager->lookupCategory(regularExpression);
        if (!category) {
            return 0;                                                 // RETURN
        }

        const int s = category->setLevels(recordLevel,
                                          passLevel,
                                          triggerLevel,
                                          triggerAllLevel);
        BSLS_ASSERT(0 == s);
        return 1;                                                     // RETURN
    }

    int matchCount = 0;

    if (1 == len) {  // all categories match "*"
        BSLS_ASSERT('*' == regularExpression[0]);
        for (LoggerCategoryManip manip(loggerManager); manip;
                                                             manip.advance()) {
            const int s = manip().setLevels(recordLevel,
                                            passLevel,
                                            triggerLevel,
                                            triggerAllLevel);
            BSLS_ASSERT(0 == s);
            ++matchCount;
        }
        return matchCount;                                            // RETURN
    }

    // 'regularExpression' is of the form "X*".
    --len;

    for (LoggerCategoryManip manip(loggerManager); manip;
                                                             manip.advance()) {
        if (0 == bsl::strncmp(manip().categoryName(),
                              regularExpression, len)) {
            const int s = manip().setLevels(recordLevel,
                                            passLevel,
                                            triggerLevel,
                                            triggerAllLevel);
            BSLS_ASSERT(0 == s);
            ++matchCount;
        }
    }
    return matchCount;
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
