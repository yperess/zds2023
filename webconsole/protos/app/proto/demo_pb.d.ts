// package: rpc_demo.remoteconfig
// file: app/proto/demo.proto

import * as jspb from "google-protobuf";

export class SetValueResponse extends jspb.Message {
  serializeBinary(): Uint8Array;
  toObject(includeInstance?: boolean): SetValueResponse.AsObject;
  static toObject(includeInstance: boolean, msg: SetValueResponse): SetValueResponse.AsObject;
  static extensions: {[key: number]: jspb.ExtensionFieldInfo<jspb.Message>};
  static extensionsBinary: {[key: number]: jspb.ExtensionFieldBinaryInfo<jspb.Message>};
  static serializeBinaryToWriter(message: SetValueResponse, writer: jspb.BinaryWriter): void;
  static deserializeBinary(bytes: Uint8Array): SetValueResponse;
  static deserializeBinaryFromReader(message: SetValueResponse, reader: jspb.BinaryReader): SetValueResponse;
}

export namespace SetValueResponse {
  export type AsObject = {
  }
}

export class SetValueRequest extends jspb.Message {
  getKey(): string;
  setKey(value: string): void;

  getIntValue(): number;
  setIntValue(value: number): void;

  serializeBinary(): Uint8Array;
  toObject(includeInstance?: boolean): SetValueRequest.AsObject;
  static toObject(includeInstance: boolean, msg: SetValueRequest): SetValueRequest.AsObject;
  static extensions: {[key: number]: jspb.ExtensionFieldInfo<jspb.Message>};
  static extensionsBinary: {[key: number]: jspb.ExtensionFieldBinaryInfo<jspb.Message>};
  static serializeBinaryToWriter(message: SetValueRequest, writer: jspb.BinaryWriter): void;
  static deserializeBinary(bytes: Uint8Array): SetValueRequest;
  static deserializeBinaryFromReader(message: SetValueRequest, reader: jspb.BinaryReader): SetValueRequest;
}

export namespace SetValueRequest {
  export type AsObject = {
    key: string,
    intValue: number,
  }
}

