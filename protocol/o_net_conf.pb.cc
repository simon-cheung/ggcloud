// Generated by the protocol buffer compiler.  DO NOT EDIT!

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "o_net_conf.pb.h"
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace oo {
namespace conf {

namespace {

const ::google::protobuf::Descriptor* local_node_conf_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  local_node_conf_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_o_5fnet_5fconf_2eproto() {
  protobuf_AddDesc_o_5fnet_5fconf_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "o_net_conf.proto");
  GOOGLE_CHECK(file != NULL);
  local_node_conf_descriptor_ = file->message_type(0);
  static const int local_node_conf_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(local_node_conf, local_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(local_node_conf, others_),
  };
  local_node_conf_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      local_node_conf_descriptor_,
      local_node_conf::default_instance_,
      local_node_conf_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(local_node_conf, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(local_node_conf, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(local_node_conf));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_o_5fnet_5fconf_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    local_node_conf_descriptor_, &local_node_conf::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_o_5fnet_5fconf_2eproto() {
  delete local_node_conf::default_instance_;
  delete local_node_conf_reflection_;
}

void protobuf_AddDesc_o_5fnet_5fconf_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::oo::proto::protobuf_AddDesc_o_5fnet_5fctrl_2eproto();
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\020o_net_conf.proto\022\007oo.conf\032\020o_net_ctrl."
    "proto\"Z\n\017local_node_conf\022\"\n\005local\030\001 \002(\0132"
    "\023.oo.proto.node_info\022#\n\006others\030\002 \003(\0132\023.o"
    "o.proto.node_infoB\013\n\007oo.confH\001", 150);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "o_net_conf.proto", &protobuf_RegisterTypes);
  local_node_conf::default_instance_ = new local_node_conf();
  local_node_conf::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_o_5fnet_5fconf_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_o_5fnet_5fconf_2eproto {
  StaticDescriptorInitializer_o_5fnet_5fconf_2eproto() {
    protobuf_AddDesc_o_5fnet_5fconf_2eproto();
  }
} static_descriptor_initializer_o_5fnet_5fconf_2eproto_;


// ===================================================================

#ifndef _MSC_VER
const int local_node_conf::kLocalFieldNumber;
const int local_node_conf::kOthersFieldNumber;
#endif  // !_MSC_VER

local_node_conf::local_node_conf()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void local_node_conf::InitAsDefaultInstance() {
  local_ = const_cast< ::oo::proto::node_info*>(&::oo::proto::node_info::default_instance());
}

local_node_conf::local_node_conf(const local_node_conf& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void local_node_conf::SharedCtor() {
  _cached_size_ = 0;
  local_ = NULL;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

local_node_conf::~local_node_conf() {
  SharedDtor();
}

void local_node_conf::SharedDtor() {
  if (this != default_instance_) {
    delete local_;
  }
}

void local_node_conf::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* local_node_conf::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return local_node_conf_descriptor_;
}

const local_node_conf& local_node_conf::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_o_5fnet_5fconf_2eproto();  return *default_instance_;
}

local_node_conf* local_node_conf::default_instance_ = NULL;

local_node_conf* local_node_conf::New() const {
  return new local_node_conf;
}

void local_node_conf::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (_has_bit(0)) {
      if (local_ != NULL) local_->::oo::proto::node_info::Clear();
    }
  }
  others_.Clear();
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool local_node_conf::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required .oo.proto.node_info local = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_local()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_others;
        break;
      }
      
      // repeated .oo.proto.node_info others = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_others:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
                input, add_others()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_others;
        if (input->ExpectAtEnd()) return true;
        break;
      }
      
      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void local_node_conf::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required .oo.proto.node_info local = 1;
  if (_has_bit(0)) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      1, this->local(), output);
  }
  
  // repeated .oo.proto.node_info others = 2;
  for (int i = 0; i < this->others_size(); i++) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      2, this->others(i), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* local_node_conf::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required .oo.proto.node_info local = 1;
  if (_has_bit(0)) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        1, this->local(), target);
  }
  
  // repeated .oo.proto.node_info others = 2;
  for (int i = 0; i < this->others_size(); i++) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        2, this->others(i), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int local_node_conf::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required .oo.proto.node_info local = 1;
    if (has_local()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->local());
    }
    
  }
  // repeated .oo.proto.node_info others = 2;
  total_size += 1 * this->others_size();
  for (int i = 0; i < this->others_size(); i++) {
    total_size +=
      ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
        this->others(i));
  }
  
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void local_node_conf::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const local_node_conf* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const local_node_conf*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void local_node_conf::MergeFrom(const local_node_conf& from) {
  GOOGLE_CHECK_NE(&from, this);
  others_.MergeFrom(from.others_);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from._has_bit(0)) {
      mutable_local()->::oo::proto::node_info::MergeFrom(from.local());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void local_node_conf::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void local_node_conf::CopyFrom(const local_node_conf& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool local_node_conf::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;
  
  if (has_local()) {
    if (!this->local().IsInitialized()) return false;
  }
  for (int i = 0; i < others_size(); i++) {
    if (!this->others(i).IsInitialized()) return false;
  }
  return true;
}

void local_node_conf::Swap(local_node_conf* other) {
  if (other != this) {
    std::swap(local_, other->local_);
    others_.Swap(&other->others_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata local_node_conf::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = local_node_conf_descriptor_;
  metadata.reflection = local_node_conf_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace conf
}  // namespace oo

// @@protoc_insertion_point(global_scope)