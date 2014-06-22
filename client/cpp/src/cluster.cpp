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

#include "cluster.h"
#include "endpoint.h"

#include <algorithm>


namespace ba = boost::asio;

using namespace std::placeholders;
using sopmq::error::connection_error;

namespace sopmq {
    namespace client {
        
        cluster::~cluster()
        {
            
        }
        
        void cluster::connect(boost::asio::io_service &ioService, connect_handler handler)
        {
            check_for_expired_deaths();
            
            if (_liveEndpoints.size() == 0)
            {
                handler(nullptr, connection_error("no nodes available"));
                return;
            }
            
            //connect to whatever endpoint is now at the top of the list
            cluster_endpoint::ptr ep = this->random_endpoint();
            
            auto resolver = std::make_shared<ba::ip::tcp::resolver>(ioService);
            
            auto query = std::make_shared<ba::ip::tcp::resolver::query>(ep->network_endpoint().host_name(), "");
            
            connect_context ctx { handler, ep, resolver, query };
            
            resolver->async_resolve(*query,
                                    std::bind(&cluster::after_resolve, this,
                                              _1, _2, ctx));
        }
        
        void cluster::after_resolve(const boost::system::error_code& err,
                                    boost::asio::ip::tcp::resolver::iterator endpoint_iterator,
                                    connect_context ctx)
        {
            if (!err)
            {
                //we have an endpoint, let's try a connect
            }
            else
            {
                if (_deadEndpoints.find(ctx.endpoint) == _deadEndpoints.end())
                {
                    //endpoint is still marked up, kill it
                    ctx.endpoint->mark_failed();
                    _deadEndpoints.insert(ctx.endpoint);
                    
                    _liveEndpoints.erase(std::remove(_liveEndpoints.begin(), _liveEndpoints.end(), ctx.endpoint));
                }
                
                //any more endpoints left?
                //try resolving the next endpoint
            }
        }
        
        void cluster::check_for_expired_deaths()
        {
            std::vector<cluster_endpoint::ptr> retryEps;
            for (cluster_endpoint::ptr ep : _deadEndpoints)
            {
                if (ep->ready_for_retry())
                {
                    ep->mark_up();
                    retryEps.push_back(ep);
                }
            }
            
            std::for_each(retryEps.begin(), retryEps.end(),
                          [&](cluster_endpoint::ptr ep)
            {
                _deadEndpoints.erase(ep);
                _liveEndpoints.push_back(ep);
            });
        }
        
    }
}