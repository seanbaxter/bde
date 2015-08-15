// btlso_socketoptutil.cpp                                            -*-C++-*-
#include <btlso_socketoptutil.h>

#include <bsls_ident.h>
BSLS_IDENT_RCSID(btlso_socketoptutil_cpp,"$Id$ $CSID$")

#include <btlso_socketoptions.h>
#include <btlso_lingeroptions.h>

#ifdef BTE_FOR_TESTING_ONLY
// These dependencies need to be here for the the bde_build.pl script to
// generate the proper makefiles, but do not need to be compiled into the
// component's .o file.  The symbol BTE_FOR_TESTING_ONLY should remain
// undefined, and is here only because '#if 0' is optimized away by the
// bde_build.pl script.

#include <btlso_socketimputil.h>                // for testing only
#include <btlso_ipv4address.h>                  // for testing only
#endif

namespace BloombergLP {

namespace btlso {
int SocketOptUtil::setSocketOptions(SocketHandle::Handle handle,
                                          const SocketOptions& options)
{
    if (!options.debugFlag().isNull()) {
        const int rc = setOption(handle,
                                 SocketOptUtil::k_SOCKETLEVEL,
                                 SocketOptUtil::k_DEBUGINFO,
                                 (int) options.debugFlag().value());
        if (rc) {
            return rc;                                                // RETURN
        }
    }

    if (!options.allowBroadcasting().isNull()) {
        const int rc = setOption(handle,
                                 SocketOptUtil::k_SOCKETLEVEL,
                                 SocketOptUtil::k_BROADCAST,
                                 (int) options.allowBroadcasting().value());
        if (rc) {
            return rc;                                                // RETURN
        }
    }

    if (!options.reuseAddress().isNull()) {
        const int rc = setOption(handle,
                                 SocketOptUtil::k_SOCKETLEVEL,
                                 SocketOptUtil::k_REUSEADDRESS,
                                 (int) options.reuseAddress().value());
        if (rc) {
            return rc;                                                // RETURN
        }
    }

    if (!options.keepAlive().isNull()) {
        const int rc = setOption(handle,
                                 SocketOptUtil::k_SOCKETLEVEL,
                                 SocketOptUtil::k_KEEPALIVE,
                                 (int) options.keepAlive().value());
        if (rc) {
            return rc;                                                // RETURN
        }
    }

    if (!options.bypassNormalRouting().isNull()) {
        const int rc = setOption(handle,
                                 SocketOptUtil::k_SOCKETLEVEL,
                                 SocketOptUtil::k_DONTROUTE,
                                 (int) options.bypassNormalRouting().value());
        if (rc) {
            return rc;                                                // RETURN
        }
    }

    if (!options.linger().isNull()) {
        const LingerOptions&      lingerOptions =
                                                     options.linger().value();
        SocketOptUtil::LingerData lingerData;

        lingerData.l_onoff  = lingerOptions.lingerFlag();
        lingerData.l_linger = lingerOptions.timeout();

        const int rc = setOption(handle,
                                 SocketOptUtil::k_SOCKETLEVEL,
                                 SocketOptUtil::k_LINGER,
                                 lingerData);
        if (rc) {
            return rc;                                                // RETURN
        }
    }

    if (!options.leaveOutOfBandDataInline().isNull()) {
        const int rc = setOption(
                              handle,
                              SocketOptUtil::k_SOCKETLEVEL,
                              SocketOptUtil::k_OOBINLINE,
                             (int) options.leaveOutOfBandDataInline().value());
        if (rc) {
            return rc;                                                // RETURN
        }
    }

    if (!options.sendBufferSize().isNull()) {
        const int rc = setOption(handle,
                                 SocketOptUtil::k_SOCKETLEVEL,
                                 SocketOptUtil::k_SENDBUFFER,
                                 options.sendBufferSize().value());
        if (rc) {
            return rc;                                                // RETURN
        }
    }

    if (!options.receiveBufferSize().isNull()) {
        const int rc = setOption(handle,
                                 SocketOptUtil::k_SOCKETLEVEL,
                                 SocketOptUtil::k_RECEIVEBUFFER,
                                 options.receiveBufferSize().value());
        if (rc) {
            return rc;                                                // RETURN
        }
    }

    if (!options.minimumSendBufferSize().isNull()) {
        const int rc = setOption(handle,
                                 SocketOptUtil::k_SOCKETLEVEL,
                                 SocketOptUtil::k_SENDLOWATER,
                                 options.minimumSendBufferSize().value());
        if (rc) {
            return rc;                                                // RETURN
        }
    }

    if (!options.minimumReceiveBufferSize().isNull()) {
        const int rc = setOption(handle,
                                 SocketOptUtil::k_SOCKETLEVEL,
                                 SocketOptUtil::k_RECEIVELOWATER,
                                 options.minimumReceiveBufferSize().value());
        if (rc) {
            return rc;                                                // RETURN
        }
    }

    if (!options.sendTimeout().isNull()) {
        const int rc = setOption(handle,
                                 SocketOptUtil::k_SOCKETLEVEL,
                                 SocketOptUtil::k_SENDTIMEOUT,
                                 options.sendTimeout().value());
        if (rc) {
            return rc;                                                // RETURN
        }
    }

    if (!options.receiveTimeout().isNull()) {
        const int rc = setOption(handle,
                                 SocketOptUtil::k_SOCKETLEVEL,
                                 SocketOptUtil::k_RECEIVETIMEOUT,
                                 options.receiveTimeout().value());
        if (rc) {
            return rc;                                                // RETURN
        }
    }

    if (!options.tcpNoDelay().isNull()) {
        const int rc = setOption(handle,
                                 SocketOptUtil::k_TCPLEVEL,
                                 SocketOptUtil::k_TCPNODELAY,
                                 (int) options.tcpNoDelay().value());
        if (rc) {
            return rc;                                                // RETURN
        }
    }

    return 0;
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
