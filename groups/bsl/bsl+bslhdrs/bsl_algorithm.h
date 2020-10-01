// bsl_algorithm.h                                                    -*-C++-*-
#ifndef INCLUDED_BSL_ALGORITHM
#define INCLUDED_BSL_ALGORITHM

#include <bsls_ident.h>
BSLS_IDENT("$Id: $")

//@PURPOSE: Provide functionality of the corresponding C++ Standard header.
//
//@SEE_ALSO: package bos+stdhdrs in the bos package group
//
//@DESCRIPTION: Provide types, in the 'bsl' namespace, equivalent to those
// defined in the corresponding C++ standard header.  Include the native
// compiler-provided standard header, and also directly include Bloomberg's
// implementation of the C++ standard type (if one exists).  Finally, place the
// included symbols from the 'std' namespace (if any) into the 'bsl' namespace.

#include <bsls_libraryfeatures.h>
#include <bsls_nativestd.h>
#include <bsls_platform.h>

#include <algorithm>

namespace bsl {

    // Import selected symbols into bsl namespace
    using native_std::adjacent_find;
    using native_std::binary_search;
    using native_std::copy;
    using native_std::copy_backward;

    // 'count' and 'count_if' are provided in 'bslstl_algorithmworkaround' in
    // order to work-around the Sun standard library, libCstd.
    //
    // using native_std::count;
    // using native_std::count_if;

    using native_std::equal;
    using native_std::equal_range;
    using native_std::fill;
    using native_std::fill_n;
    using native_std::find;
    using native_std::find_end;
    using native_std::find_first_of;
    using native_std::find_if;
    using native_std::for_each;
    using native_std::generate;
    using native_std::generate_n;
    using native_std::includes;
    using native_std::inplace_merge;
    using native_std::iter_swap;
    using native_std::lexicographical_compare;
    using native_std::lower_bound;

    using native_std::make_heap;
    using native_std::max;
    using native_std::max_element;
    using native_std::merge;
    using native_std::min;
    using native_std::min_element;
    using native_std::mismatch;
    using native_std::next_permutation;
    using native_std::nth_element;
    using native_std::partial_sort;
    using native_std::partial_sort_copy;
    using native_std::partition;
    using native_std::pop_heap;
    using native_std::prev_permutation;
    using native_std::push_heap;
    using native_std::remove;
    using native_std::remove_copy;
    using native_std::remove_copy_if;
    using native_std::remove_if;
    using native_std::replace;
    using native_std::replace_copy;
    using native_std::replace_copy_if;
    using native_std::replace_if;
    using native_std::reverse;
    using native_std::reverse_copy;
    using native_std::rotate;
    using native_std::rotate_copy;
    using native_std::search;
    using native_std::search_n;
    using native_std::set_difference;
    using native_std::set_intersection;
    using native_std::set_new_handler;
    using native_std::set_symmetric_difference;
    using native_std::set_union;
    using native_std::sort;
    using native_std::sort_heap;
    using native_std::stable_partition;
    using native_std::stable_sort;
    using native_std::swap;
    using native_std::swap_ranges;
    using native_std::transform;
    using native_std::unique;
    using native_std::unique_copy;
    using native_std::upper_bound;

#if ! defined BSLS_LIBRARYFEATURES_HAS_CPP17_DEPRECATED_REMOVED
    // These names are removed by C++17
    using native_std::random_shuffle;
#endif

#ifdef BSLS_LIBRARYFEATURES_HAS_CPP11_BASELINE_LIBRARY
    using native_std::all_of;
    using native_std::any_of;
    using native_std::copy_if;

    // 'copy_n' is implemented separately in 'bslstp_exalgorithm' for backwards
    // compatibility with the previous STLport definition (which differs from
    // the platform implementation).
    //
    // using native_std::(copy_n);

    using native_std::find_if_not;
    using native_std::is_heap;
    using native_std::is_heap_until;
    using native_std::is_partitioned;
    using native_std::is_permutation;
    using native_std::is_sorted;
    using native_std::is_sorted_until;
    using native_std::minmax;
    using native_std::minmax_element;
    using native_std::move;
    using native_std::move_backward;
    using native_std::none_of;
    using native_std::partition_copy;
    using native_std::partition_point;
    using native_std::shuffle;
#endif  // BSLS_LIBRARYFEATURES_HAS_CPP11_BASELINE_LIBRARY

#ifndef BDE_OMIT_INTERNAL_DEPRECATED
    // Import additional names expected by existing code, but not mandated by
    // the standard header.
    using native_std::advance;
    using native_std::bad_alloc;
    using native_std::bidirectional_iterator_tag;
    using native_std::forward_iterator_tag;
    using native_std::get_temporary_buffer;
    using native_std::input_iterator_tag;
    using native_std::iterator;
    using native_std::new_handler;
    using native_std::nothrow;
    using native_std::nothrow_t;
    using native_std::output_iterator_tag;
    using native_std::random_access_iterator_tag;
    using native_std::return_temporary_buffer;
#endif  // BDE_OMIT_INTERNAL_DEPRECATED
}  // close package namespace

// Include Bloomberg's implementation, unless compilation is configured to
// override native types in the 'std' namespace with Bloomberg's
// implementation, in which case the implementation file will be included by
// the Bloomberg supplied standard header file.

#ifndef BSL_OVERRIDES_STD
#include <bslstl_algorithm.h>
#include <bslstl_algorithmworkaround.h>
#endif

#ifndef BDE_OPENSOURCE_PUBLICATION // STP
#ifndef BSL_OVERRIDES_STD
#include <bslstp_exalgorithm.h>
#endif
#endif  // BDE_OPENSOURCE_PUBLICATION -- STP

#endif

// ----------------------------------------------------------------------------
// Copyright 2013 Bloomberg Finance L.P.
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
