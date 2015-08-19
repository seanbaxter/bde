// bdlaggxxx_aggregateutil.cpp                                        -*-C++-*-
#include <bdlaggxxx_aggregateutil.h>

#include <bsls_ident.h>
BSLS_IDENT_RCSID(bdlaggxxx_aggregateutil_cpp,"$Id$ $CSID$")

#include <bdlaggxxx_errorattributes.h>

namespace BloombergLP {

namespace bdlaggxxx {
                          // ------------------------
                          // class AggregateUtil
                          // ------------------------

// CLASS METHODS
const char *AggregateUtil::errorString(int errorCode)
{
    switch (errorCode) {
      case ErrorCode::BCEM_NOT_A_RECORD: {
        return "Attempt to access a field (by name, ID, or "
               "index) of an aggregate that does not reference a "
               "list, row, choice, or choice array item.";            // RETURN
      }
      case ErrorCode::BCEM_NOT_A_SEQUENCE: {
        return "Attempt to perform a list or row operation on an "
               "aggregate that does not refer to a list or row "
               "(e.g., initialize from a non-sequence record def).";  // RETURN
      }
      case ErrorCode::BCEM_NOT_A_CHOICE: {
        return "Attempt to perform a choice or choice array item "
               "operation (make selection, get selection, etc.) "
               "on an aggregate that is not a choice or choice "
               "array item.";                                         // RETURN
      }
      case ErrorCode::BCEM_NOT_AN_ARRAY: {
        return "Attempt to perform an array operation (index, "
               "insert, etc.) on an aggregate that is not an "
               "array, table, or choice array.";                      // RETURN
      }
      case ErrorCode::BCEM_BAD_FIELDNAME: {
        return "Field name does not exist in the record def.";        // RETURN
      }
      case ErrorCode::BCEM_BAD_FIELDID: {
        return "Field ID does not exist in record def.";              // RETURN
      }
      case ErrorCode::BCEM_BAD_FIELDINDEX: {
        return "Field index is not an integer less than "
               "the length of the field definition.";                 // RETURN
      }
      case ErrorCode::BCEM_BAD_ARRAYINDEX: {
        return "Array (or table) index is out of bounds.";            // RETURN
      }
      case ErrorCode::BCEM_NOT_SELECTED: {
        return "Attempt to access a choice field that is not "
               "the currently selected object.";                      // RETURN
      }
      case ErrorCode::BCEM_BAD_CONVERSION: {
        return "Attempt to set an aggregate using a value that "
               "is not convertible to the aggregate's type.";         // RETURN
      }
      case ErrorCode::BCEM_BAD_ENUMVALUE: {
        return "Attempt to set the value of an enumeration "
               "aggregate to a string that is not an enumerator "
               "name in the enumeration definition or to an "
               "integer that is not an enumerator ID in the "
               "enumeration definition. ";                            // RETURN
      }
      case ErrorCode::BCEM_NON_CONFORMANT: {
        return "Attempt to set a list, row, table, choice, "
               "choice array item, or choice array aggregate to "
               "a value of the correct type, but which doesn't "
               "conform to the aggregate's record definition.";       // RETURN
      }
      case ErrorCode::BCEM_AMBIGUOUS_ANON: {
        return "A reference to an anonymous field is ambiguous, "
               "typically because the aggregate contains more "
               "than one anonymous field.";                           // RETURN
      }
    }

    return "An unknown error has occurred.";
}
}  // close package namespace

}  // close enterprise namespace

// ----------------------------------------------------------------------------
// NOTICE:
//      Copyright (C) Bloomberg L.P., 2008
//      All Rights Reserved.
//      Property of Bloomberg L.P. (BLP)
//      This software is made available solely pursuant to the
//      terms of a BLP license agreement which governs its use.
// ------------------------------ END-OF-FILE ---------------------------------
