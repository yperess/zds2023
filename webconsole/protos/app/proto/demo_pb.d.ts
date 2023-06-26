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

  hasIntValue(): boolean;
  clearIntValue(): void;
  getIntValue(): number;
  setIntValue(value: number): void;

  hasBoolValue(): boolean;
  clearBoolValue(): void;
  getBoolValue(): boolean;
  setBoolValue(value: boolean): void;

  hasStringValue(): boolean;
  clearStringValue(): void;
  getStringValue(): string;
  setStringValue(value: string): void;

  getValueCase(): SetValueRequest.ValueCase;
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
    boolValue: boolean,
    stringValue: string,
  }

  export enum ValueCase {
    VALUE_NOT_SET = 0,
    INT_VALUE = 2,
    BOOL_VALUE = 3,
    STRING_VALUE = 4,
  }
}

