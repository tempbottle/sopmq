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

#include "endpoint.h"
#include "cluster_endpoint.h"
#include "connection_error.h"
#include "session.h"

#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>

#include <memory>
#include <vector>
#include <set>

namespace sopmq {
    namespace client {
        
        ///
        /// A cluster is a collection of endpoints that is managed to provide
        /// fault tolerance in the face of errors
        ///
        class cluster : public boost::noncopyable
        {
        public:
            typedef std::shared_ptr<cluster> ptr;
            
            typedef std::function<void(session::ptr, sopmq::error::connection_error)> connect_handler;
            
        public:
            template <typename ColType>
            cluster(ColType epCol)
            {
                for (auto ep : epCol)
                {
                    _liveEndpoints.push_back(std::make_shared<cluster_endpoint>(ep));
                }
            }
            
            virtual ~cluster();
            
            ///
            /// Connects to the cluster and calls the handler on completion or error
            ///
            void connect(boost::asio::io_service& ioService, connect_handler handler);
            
        private:
            ///
            /// Context used when attempting to connect to a new cluster
            ///
            struct connect_context
            {
                connect_handler handler;
                cluster_endpoint::ptr endpoint;
                std::shared_ptr<boost::asio::ip::tcp::resolver> resolver;
                std::shared_ptr<boost::asio::ip::tcp::resolver::query> query;
            };
            
            ///
            ///
            ///
            void after_resolve(const boost::system::error_code& err,
                               boost::asio::ip::tcp::resolver::iterator endpoint_iterator,
                               connect_context ctx);
            
            ///
            /// Marks the endpoint dead, removes it from the live list,
            /// and inserts it into the dead list
            ///
            void kill_endpoint(cluster_endpoint::ptr ep);
            
            ///
            /// Checks the cluster for dead nodes who have
            ///
            void check_for_expired_deaths();
            
            ///
            /// Tries to resolve an endpoint after
            ///
            void try_resolve_next_endpoint(std::shared_ptr<boost::asio::ip::tcp::resolver> resolver, connect_handler handler);
            
            ///
            /// Returns a random endpoint from the live endpoints collection
            ///
            cluster_endpoint::ptr random_endpoint();
            
            ///
            /// Endpoints that we either know to be good, or haven't tried to connect to yet
            ///
            std::vector<cluster_endpoint::ptr> _liveEndpoints;
            
            ///
            /// Endpoints that died on us
            ///
            std::set<cluster_endpoint::ptr> _deadEndpoints;
        };
        
    }
}