/*
 * SOPMQ - Scalable optionally persistent message queue
 * Copyright 2014 InWorldz, LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __Project__node__
#define __Project__node__

#include <boost/shared_ptr.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/noncopyable.hpp>
#include <boost/chrono.hpp>

#include <string>

#include "endpoint.h"

namespace sopmq {
    namespace node {
        
        ///
        /// Represents a node that we're aware of in our ring that can service
        /// requests. This could be the local node, or a remote node
        ///
        class node : public boost::noncopyable
        {
        private:
            ///
            /// Number of seconds between heartbeats
            ///
            const unsigned short HEARTBEAT_INTERVAL_SECS = 10;
            
            ///
            /// Number of seconds before a node is considered dead
            ///
            const unsigned short HEARTBEAT_TIMEOUT_SECS = 30;
            
            
            
            ///
            /// The beginning of the range that we handle
            ///
            boost::multiprecision::uint128_t _rangeStart;
            
            ///
            /// The endpoint to contact this node at
            ///
            net::endpoint _endpoint;
            
            ///
            /// The last time we got a heartbeat from this node
            ///
            boost::chrono::time_point<boost::chrono::steady_clock> _lastHeartbeat;
            
            
        public:
            node(boost::multiprecision::uint128_t rangeStart, net::endpoint endPoint);
            virtual ~node();
            
            ///
            /// Returns the beginning of the range we handle
            ///
            boost::multiprecision::uint128_t range_start() const;
            
            ///
            /// Returns the endpoint to reach this node
            ///
            net::endpoint endpoint() const;
            
            ///
            /// Whether or not this node is known to be up
            ///
            bool is_alive() const;
        };
        
        typedef boost::shared_ptr<node> node_ptr;
    }
}

#endif /* defined(__Project__node__) */