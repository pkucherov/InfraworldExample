// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: vizor_demonstration.proto

#include "vizor_demonstration.pb.h"
#include "vizor_demonstration.grpc.pb.h"

#include <grpc++/impl/codegen/async_stream.h>
#include <grpc++/impl/codegen/async_unary_call.h>
#include <grpc++/impl/codegen/channel_interface.h>
#include <grpc++/impl/codegen/client_unary_call.h>
#include <grpc++/impl/codegen/method_handler_impl.h>
#include <grpc++/impl/codegen/rpc_service_method.h>
#include <grpc++/impl/codegen/service_type.h>
#include <grpc++/impl/codegen/sync_stream.h>
namespace vizor_proto_demostration {

static const char* HelloService_method_names[] = {
  "/vizor_proto_demostration.HelloService/Hello",
  "/vizor_proto_demostration.HelloService/ServerTime",
};

std::unique_ptr< HelloService::Stub> HelloService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  std::unique_ptr< HelloService::Stub> stub(new HelloService::Stub(channel));
  return stub;
}

HelloService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : channel_(channel), rpcmethod_Hello_(HelloService_method_names[0], ::grpc::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_ServerTime_(HelloService_method_names[1], ::grpc::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status HelloService::Stub::Hello(::grpc::ClientContext* context, const ::vizor_proto_demostration::HelloRequest& request, ::vizor_proto_demostration::HelloResponse* response) {
  return ::grpc::BlockingUnaryCall(channel_.get(), rpcmethod_Hello_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::vizor_proto_demostration::HelloResponse>* HelloService::Stub::AsyncHelloRaw(::grpc::ClientContext* context, const ::vizor_proto_demostration::HelloRequest& request, ::grpc::CompletionQueue* cq) {
  return new ::grpc::ClientAsyncResponseReader< ::vizor_proto_demostration::HelloResponse>(channel_.get(), cq, rpcmethod_Hello_, context, request);
}

::grpc::Status HelloService::Stub::ServerTime(::grpc::ClientContext* context, const ::vizor_proto_demostration::ServerTimeRequest& request, ::vizor_proto_demostration::ServerTimeResponse* response) {
  return ::grpc::BlockingUnaryCall(channel_.get(), rpcmethod_ServerTime_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::vizor_proto_demostration::ServerTimeResponse>* HelloService::Stub::AsyncServerTimeRaw(::grpc::ClientContext* context, const ::vizor_proto_demostration::ServerTimeRequest& request, ::grpc::CompletionQueue* cq) {
  return new ::grpc::ClientAsyncResponseReader< ::vizor_proto_demostration::ServerTimeResponse>(channel_.get(), cq, rpcmethod_ServerTime_, context, request);
}

HelloService::Service::Service() {
  AddMethod(new ::grpc::RpcServiceMethod(
      HelloService_method_names[0],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< HelloService::Service, ::vizor_proto_demostration::HelloRequest, ::vizor_proto_demostration::HelloResponse>(
          std::mem_fn(&HelloService::Service::Hello), this)));
  AddMethod(new ::grpc::RpcServiceMethod(
      HelloService_method_names[1],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< HelloService::Service, ::vizor_proto_demostration::ServerTimeRequest, ::vizor_proto_demostration::ServerTimeResponse>(
          std::mem_fn(&HelloService::Service::ServerTime), this)));
}

HelloService::Service::~Service() {
}

::grpc::Status HelloService::Service::Hello(::grpc::ServerContext* context, const ::vizor_proto_demostration::HelloRequest* request, ::vizor_proto_demostration::HelloResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status HelloService::Service::ServerTime(::grpc::ServerContext* context, const ::vizor_proto_demostration::ServerTimeRequest* request, ::vizor_proto_demostration::ServerTimeResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace vizor_proto_demostration

