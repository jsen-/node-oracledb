/* Copyright (c) 2015, Oracle and/or its affiliates. All rights reserved. */

/******************************************************************************
 *
 * You may not use the identified files except in compliance with the Apache
 * License, Version 2.0 (the "License.")
 *
 * You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * NAME
 *   njsPool.h
 *
 * DESCRIPTION
 *   Pool class
 *
 *****************************************************************************/

#ifndef __NJSPOOL_H__
#define __NJSPOOL_H__

#include "dpi.h"
#include <node.h>
#include "nan.h"
#include <string>

using namespace v8;
using namespace node;


class Pool: public ObjectWrap {
public:

   static void Init(Handle<Object> target);

   void setPool ( dpi::SPool *, Oracledb* oracledb, unsigned int poolMax,
                  unsigned int poolMin, unsigned int poolIncrement,
                  unsigned int poolTimeout, unsigned stmtCacheSize );

   // Define Pool Constructor
   static Persistent<FunctionTemplate> poolTemplate_s ;

private:

   static NAN_METHOD(New);

   // Get Connection Methods 
   static NAN_METHOD(GetConnection);
   static void Async_GetConnection(uv_work_t* req);
   static void Async_AfterGetConnection(uv_work_t* req);

  // Terminate Methods
   static NAN_METHOD(Terminate);
   static void Async_Terminate(uv_work_t* req);
   static void Async_AfterTerminate(uv_work_t* req);

  // Define Getter Accessors to properties
  static NAN_PROPERTY_GETTER(GetPoolMax);
  static NAN_PROPERTY_GETTER(GetPoolMin);
  static NAN_PROPERTY_GETTER(GetPoolIncrement);
  static NAN_PROPERTY_GETTER(GetPoolTimeout);
  static NAN_PROPERTY_GETTER(GetConnectionsOpen);
  static NAN_PROPERTY_GETTER(GetConnectionsInUse);
  static NAN_PROPERTY_GETTER(GetStmtCacheSize);

  static Handle<Value> getPoolProperty(Pool* njsPool, unsigned int poolProperty);

  // Define Setter Accessors to properties
  static NAN_SETTER(SetPoolMax);
  static NAN_SETTER(SetPoolMin);
  static NAN_SETTER(SetPoolIncrement);
  static NAN_SETTER(SetPoolTimeout);
  static NAN_SETTER(SetConnectionsOpen);
  static NAN_SETTER(SetConnectionsInUse);
  static NAN_SETTER(SetStmtCacheSize);

  static void setPoolProperty(Pool* njsPool, string property);

   Pool();
   ~Pool();

   dpi::SPool *dpipool_;
   bool isValid_;

   Oracledb* oracledb_;
   unsigned int poolMin_;
   unsigned int poolMax_;
   unsigned int poolIncrement_;
   unsigned int poolTimeout_;
   unsigned int stmtCacheSize_;
};

typedef struct poolBaton
{
  uv_work_t req;
  std::string error;
  std::string connClass;
  Persistent<Function> cb;
  dpi::Conn*  dpiconn;
  Pool*       njspool;

  poolBaton() :  error(""), connClass(""),
                 dpiconn(NULL), njspool(NULL)
  {}

  ~poolBaton()
   {
     NanDisposePersistent(cb);
   }

}poolBaton;


#endif                                          /* __NJSPOOL_H__ */
