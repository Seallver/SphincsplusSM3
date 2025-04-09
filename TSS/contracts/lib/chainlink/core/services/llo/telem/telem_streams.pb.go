// Code generated by protoc-gen-go. DO NOT EDIT.
// versions:
// 	protoc-gen-go v1.36.6
// 	protoc        v5.29.3
// source: telem_streams.proto

package telem

import (
	protoreflect "google.golang.org/protobuf/reflect/protoreflect"
	protoimpl "google.golang.org/protobuf/runtime/protoimpl"
	reflect "reflect"
	sync "sync"
	unsafe "unsafe"
)

const (
	// Verify that this generated code is sufficiently up-to-date.
	_ = protoimpl.EnforceVersion(20 - protoimpl.MinVersion)
	// Verify that runtime/protoimpl is sufficiently up-to-date.
	_ = protoimpl.EnforceVersion(protoimpl.MaxVersion - 20)
)

type LLOBridgeTelemetry struct {
	state                    protoimpl.MessageState `protogen:"open.v1"`
	BridgeAdapterName        string                 `protobuf:"bytes,1,opt,name=bridge_adapter_name,json=bridgeAdapterName,proto3" json:"bridge_adapter_name,omitempty"`
	BridgeRequestData        []byte                 `protobuf:"bytes,2,opt,name=bridge_request_data,json=bridgeRequestData,proto3" json:"bridge_request_data,omitempty"`
	BridgeResponseData       []byte                 `protobuf:"bytes,3,opt,name=bridge_response_data,json=bridgeResponseData,proto3" json:"bridge_response_data,omitempty"`
	BridgeResponseError      *string                `protobuf:"bytes,4,opt,name=bridge_response_error,json=bridgeResponseError,proto3,oneof" json:"bridge_response_error,omitempty"`
	BridgeResponseStatusCode int32                  `protobuf:"varint,5,opt,name=bridge_response_status_code,json=bridgeResponseStatusCode,proto3" json:"bridge_response_status_code,omitempty"`
	RequestStartTimestamp    int64                  `protobuf:"varint,6,opt,name=request_start_timestamp,json=requestStartTimestamp,proto3" json:"request_start_timestamp,omitempty"`
	RequestFinishTimestamp   int64                  `protobuf:"varint,7,opt,name=request_finish_timestamp,json=requestFinishTimestamp,proto3" json:"request_finish_timestamp,omitempty"`
	LocalCacheHit            bool                   `protobuf:"varint,8,opt,name=local_cache_hit,json=localCacheHit,proto3" json:"local_cache_hit,omitempty"`
	SpecId                   int32                  `protobuf:"varint,9,opt,name=spec_id,json=specId,proto3" json:"spec_id,omitempty"`
	StreamId                 *uint32                `protobuf:"varint,10,opt,name=stream_id,json=streamId,proto3,oneof" json:"stream_id,omitempty"`
	DotId                    string                 `protobuf:"bytes,11,opt,name=dot_id,json=dotId,proto3" json:"dot_id,omitempty"`
	DonId                    uint32                 `protobuf:"varint,12,opt,name=don_id,json=donId,proto3" json:"don_id,omitempty"`
	SeqNr                    uint64                 `protobuf:"varint,13,opt,name=seq_nr,json=seqNr,proto3" json:"seq_nr,omitempty"`
	ConfigDigest             []byte                 `protobuf:"bytes,14,opt,name=config_digest,json=configDigest,proto3" json:"config_digest,omitempty"`
	ObservationTimestamp     int64                  `protobuf:"varint,15,opt,name=observation_timestamp,json=observationTimestamp,proto3" json:"observation_timestamp,omitempty"`
	unknownFields            protoimpl.UnknownFields
	sizeCache                protoimpl.SizeCache
}

func (x *LLOBridgeTelemetry) Reset() {
	*x = LLOBridgeTelemetry{}
	mi := &file_telem_streams_proto_msgTypes[0]
	ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
	ms.StoreMessageInfo(mi)
}

func (x *LLOBridgeTelemetry) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*LLOBridgeTelemetry) ProtoMessage() {}

func (x *LLOBridgeTelemetry) ProtoReflect() protoreflect.Message {
	mi := &file_telem_streams_proto_msgTypes[0]
	if x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use LLOBridgeTelemetry.ProtoReflect.Descriptor instead.
func (*LLOBridgeTelemetry) Descriptor() ([]byte, []int) {
	return file_telem_streams_proto_rawDescGZIP(), []int{0}
}

func (x *LLOBridgeTelemetry) GetBridgeAdapterName() string {
	if x != nil {
		return x.BridgeAdapterName
	}
	return ""
}

func (x *LLOBridgeTelemetry) GetBridgeRequestData() []byte {
	if x != nil {
		return x.BridgeRequestData
	}
	return nil
}

func (x *LLOBridgeTelemetry) GetBridgeResponseData() []byte {
	if x != nil {
		return x.BridgeResponseData
	}
	return nil
}

func (x *LLOBridgeTelemetry) GetBridgeResponseError() string {
	if x != nil && x.BridgeResponseError != nil {
		return *x.BridgeResponseError
	}
	return ""
}

func (x *LLOBridgeTelemetry) GetBridgeResponseStatusCode() int32 {
	if x != nil {
		return x.BridgeResponseStatusCode
	}
	return 0
}

func (x *LLOBridgeTelemetry) GetRequestStartTimestamp() int64 {
	if x != nil {
		return x.RequestStartTimestamp
	}
	return 0
}

func (x *LLOBridgeTelemetry) GetRequestFinishTimestamp() int64 {
	if x != nil {
		return x.RequestFinishTimestamp
	}
	return 0
}

func (x *LLOBridgeTelemetry) GetLocalCacheHit() bool {
	if x != nil {
		return x.LocalCacheHit
	}
	return false
}

func (x *LLOBridgeTelemetry) GetSpecId() int32 {
	if x != nil {
		return x.SpecId
	}
	return 0
}

func (x *LLOBridgeTelemetry) GetStreamId() uint32 {
	if x != nil && x.StreamId != nil {
		return *x.StreamId
	}
	return 0
}

func (x *LLOBridgeTelemetry) GetDotId() string {
	if x != nil {
		return x.DotId
	}
	return ""
}

func (x *LLOBridgeTelemetry) GetDonId() uint32 {
	if x != nil {
		return x.DonId
	}
	return 0
}

func (x *LLOBridgeTelemetry) GetSeqNr() uint64 {
	if x != nil {
		return x.SeqNr
	}
	return 0
}

func (x *LLOBridgeTelemetry) GetConfigDigest() []byte {
	if x != nil {
		return x.ConfigDigest
	}
	return nil
}

func (x *LLOBridgeTelemetry) GetObservationTimestamp() int64 {
	if x != nil {
		return x.ObservationTimestamp
	}
	return 0
}

// LLOObservationTelemetry packet sent for each stream on every call to
// Observation (once per round)
type LLOObservationTelemetry struct {
	state                 protoimpl.MessageState `protogen:"open.v1"`
	StreamId              uint32                 `protobuf:"varint,1,opt,name=stream_id,json=streamId,proto3" json:"stream_id,omitempty"`
	StreamValueType       int32                  `protobuf:"varint,2,opt,name=stream_value_type,json=streamValueType,proto3" json:"stream_value_type,omitempty"`
	StreamValueBinary     []byte                 `protobuf:"bytes,3,opt,name=stream_value_binary,json=streamValueBinary,proto3" json:"stream_value_binary,omitempty"`
	StreamValueText       string                 `protobuf:"bytes,4,opt,name=stream_value_text,json=streamValueText,proto3" json:"stream_value_text,omitempty"`
	ObservationError      *string                `protobuf:"bytes,5,opt,name=observation_error,json=observationError,proto3,oneof" json:"observation_error,omitempty"`
	ObservationTimestamp  int64                  `protobuf:"varint,6,opt,name=observation_timestamp,json=observationTimestamp,proto3" json:"observation_timestamp,omitempty"`
	ObservationFinishedAt int64                  `protobuf:"varint,7,opt,name=observation_finished_at,json=observationFinishedAt,proto3" json:"observation_finished_at,omitempty"`
	DonId                 uint32                 `protobuf:"varint,8,opt,name=don_id,json=donId,proto3" json:"don_id,omitempty"`
	SeqNr                 uint64                 `protobuf:"varint,9,opt,name=seq_nr,json=seqNr,proto3" json:"seq_nr,omitempty"`
	ConfigDigest          []byte                 `protobuf:"bytes,10,opt,name=config_digest,json=configDigest,proto3" json:"config_digest,omitempty"`
	unknownFields         protoimpl.UnknownFields
	sizeCache             protoimpl.SizeCache
}

func (x *LLOObservationTelemetry) Reset() {
	*x = LLOObservationTelemetry{}
	mi := &file_telem_streams_proto_msgTypes[1]
	ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
	ms.StoreMessageInfo(mi)
}

func (x *LLOObservationTelemetry) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*LLOObservationTelemetry) ProtoMessage() {}

func (x *LLOObservationTelemetry) ProtoReflect() protoreflect.Message {
	mi := &file_telem_streams_proto_msgTypes[1]
	if x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use LLOObservationTelemetry.ProtoReflect.Descriptor instead.
func (*LLOObservationTelemetry) Descriptor() ([]byte, []int) {
	return file_telem_streams_proto_rawDescGZIP(), []int{1}
}

func (x *LLOObservationTelemetry) GetStreamId() uint32 {
	if x != nil {
		return x.StreamId
	}
	return 0
}

func (x *LLOObservationTelemetry) GetStreamValueType() int32 {
	if x != nil {
		return x.StreamValueType
	}
	return 0
}

func (x *LLOObservationTelemetry) GetStreamValueBinary() []byte {
	if x != nil {
		return x.StreamValueBinary
	}
	return nil
}

func (x *LLOObservationTelemetry) GetStreamValueText() string {
	if x != nil {
		return x.StreamValueText
	}
	return ""
}

func (x *LLOObservationTelemetry) GetObservationError() string {
	if x != nil && x.ObservationError != nil {
		return *x.ObservationError
	}
	return ""
}

func (x *LLOObservationTelemetry) GetObservationTimestamp() int64 {
	if x != nil {
		return x.ObservationTimestamp
	}
	return 0
}

func (x *LLOObservationTelemetry) GetObservationFinishedAt() int64 {
	if x != nil {
		return x.ObservationFinishedAt
	}
	return 0
}

func (x *LLOObservationTelemetry) GetDonId() uint32 {
	if x != nil {
		return x.DonId
	}
	return 0
}

func (x *LLOObservationTelemetry) GetSeqNr() uint64 {
	if x != nil {
		return x.SeqNr
	}
	return 0
}

func (x *LLOObservationTelemetry) GetConfigDigest() []byte {
	if x != nil {
		return x.ConfigDigest
	}
	return nil
}

var File_telem_streams_proto protoreflect.FileDescriptor

const file_telem_streams_proto_rawDesc = "" +
	"\n" +
	"\x13telem_streams.proto\x12\x05telem\"\xba\x05\n" +
	"\x12LLOBridgeTelemetry\x12.\n" +
	"\x13bridge_adapter_name\x18\x01 \x01(\tR\x11bridgeAdapterName\x12.\n" +
	"\x13bridge_request_data\x18\x02 \x01(\fR\x11bridgeRequestData\x120\n" +
	"\x14bridge_response_data\x18\x03 \x01(\fR\x12bridgeResponseData\x127\n" +
	"\x15bridge_response_error\x18\x04 \x01(\tH\x00R\x13bridgeResponseError\x88\x01\x01\x12=\n" +
	"\x1bbridge_response_status_code\x18\x05 \x01(\x05R\x18bridgeResponseStatusCode\x126\n" +
	"\x17request_start_timestamp\x18\x06 \x01(\x03R\x15requestStartTimestamp\x128\n" +
	"\x18request_finish_timestamp\x18\a \x01(\x03R\x16requestFinishTimestamp\x12&\n" +
	"\x0flocal_cache_hit\x18\b \x01(\bR\rlocalCacheHit\x12\x17\n" +
	"\aspec_id\x18\t \x01(\x05R\x06specId\x12 \n" +
	"\tstream_id\x18\n" +
	" \x01(\rH\x01R\bstreamId\x88\x01\x01\x12\x15\n" +
	"\x06dot_id\x18\v \x01(\tR\x05dotId\x12\x15\n" +
	"\x06don_id\x18\f \x01(\rR\x05donId\x12\x15\n" +
	"\x06seq_nr\x18\r \x01(\x04R\x05seqNr\x12#\n" +
	"\rconfig_digest\x18\x0e \x01(\fR\fconfigDigest\x123\n" +
	"\x15observation_timestamp\x18\x0f \x01(\x03R\x14observationTimestampB\x18\n" +
	"\x16_bridge_response_errorB\f\n" +
	"\n" +
	"_stream_id\"\xc6\x03\n" +
	"\x17LLOObservationTelemetry\x12\x1b\n" +
	"\tstream_id\x18\x01 \x01(\rR\bstreamId\x12*\n" +
	"\x11stream_value_type\x18\x02 \x01(\x05R\x0fstreamValueType\x12.\n" +
	"\x13stream_value_binary\x18\x03 \x01(\fR\x11streamValueBinary\x12*\n" +
	"\x11stream_value_text\x18\x04 \x01(\tR\x0fstreamValueText\x120\n" +
	"\x11observation_error\x18\x05 \x01(\tH\x00R\x10observationError\x88\x01\x01\x123\n" +
	"\x15observation_timestamp\x18\x06 \x01(\x03R\x14observationTimestamp\x126\n" +
	"\x17observation_finished_at\x18\a \x01(\x03R\x15observationFinishedAt\x12\x15\n" +
	"\x06don_id\x18\b \x01(\rR\x05donId\x12\x15\n" +
	"\x06seq_nr\x18\t \x01(\x04R\x05seqNr\x12#\n" +
	"\rconfig_digest\x18\n" +
	" \x01(\fR\fconfigDigestB\x14\n" +
	"\x12_observation_errorBBZ@github.com/smartcontractkit/chainlink/v2/core/services/llo/telemb\x06proto3"

var (
	file_telem_streams_proto_rawDescOnce sync.Once
	file_telem_streams_proto_rawDescData []byte
)

func file_telem_streams_proto_rawDescGZIP() []byte {
	file_telem_streams_proto_rawDescOnce.Do(func() {
		file_telem_streams_proto_rawDescData = protoimpl.X.CompressGZIP(unsafe.Slice(unsafe.StringData(file_telem_streams_proto_rawDesc), len(file_telem_streams_proto_rawDesc)))
	})
	return file_telem_streams_proto_rawDescData
}

var file_telem_streams_proto_msgTypes = make([]protoimpl.MessageInfo, 2)
var file_telem_streams_proto_goTypes = []any{
	(*LLOBridgeTelemetry)(nil),      // 0: telem.LLOBridgeTelemetry
	(*LLOObservationTelemetry)(nil), // 1: telem.LLOObservationTelemetry
}
var file_telem_streams_proto_depIdxs = []int32{
	0, // [0:0] is the sub-list for method output_type
	0, // [0:0] is the sub-list for method input_type
	0, // [0:0] is the sub-list for extension type_name
	0, // [0:0] is the sub-list for extension extendee
	0, // [0:0] is the sub-list for field type_name
}

func init() { file_telem_streams_proto_init() }
func file_telem_streams_proto_init() {
	if File_telem_streams_proto != nil {
		return
	}
	file_telem_streams_proto_msgTypes[0].OneofWrappers = []any{}
	file_telem_streams_proto_msgTypes[1].OneofWrappers = []any{}
	type x struct{}
	out := protoimpl.TypeBuilder{
		File: protoimpl.DescBuilder{
			GoPackagePath: reflect.TypeOf(x{}).PkgPath(),
			RawDescriptor: unsafe.Slice(unsafe.StringData(file_telem_streams_proto_rawDesc), len(file_telem_streams_proto_rawDesc)),
			NumEnums:      0,
			NumMessages:   2,
			NumExtensions: 0,
			NumServices:   0,
		},
		GoTypes:           file_telem_streams_proto_goTypes,
		DependencyIndexes: file_telem_streams_proto_depIdxs,
		MessageInfos:      file_telem_streams_proto_msgTypes,
	}.Build()
	File_telem_streams_proto = out.File
	file_telem_streams_proto_goTypes = nil
	file_telem_streams_proto_depIdxs = nil
}
