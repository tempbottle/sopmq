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

#include "cluster_connection.h"

namespace ba = boost::asio;

namespace sopmq {
    namespace client {
        
        cluster_connection::cluster_connection(cluster_endpoint::ptr ep,
                                               ba::io_service& ioService)
        : _endpoint(ep), _ioService(ioService)
        {
            
        }
        
        cluster_connection::~cluster_connection()
        {
            
        }
        
        void cluster_connection::connect()
        {
            
        }
        
    }
}