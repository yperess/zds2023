// Copyright 2023 The Pigweed Authors
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not
// use this file except in compliance with the License. You may obtain a copy of
// the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
// WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
// License for the specific language governing permissions and limitations under
// the License.

const LETTERS = 'abcdefghijklmnopqrstuvwxyz';
const NUMBERS = '1234567890';
const BASE64CHARS = (LETTERS + LETTERS.toUpperCase() + NUMBERS + '+/-=$').split('')

/**
 * Usage:
 * function onLogs(str){
 *  str = detokenizer.detokenizeBase64String(str);
 *  console.log(str);
 * }
 * let parser = new StreamParser(onLogs);
 * ...
 * parser.read(frame);
 */
export default class StreamParser {
  private buffer: string = '';
  private isInsideMessage: boolean = false;
  constructor(private logHandler: (str: string) => void) { }

  read(bytes: Uint8Array) {
    console.log('processing: ' + bytes);
    bytes.forEach(byte => {
      const char = new TextDecoder().decode(new Uint8Array([byte]))[0];
      if (char === '$') {
        console.log('Entered frame');
        if (this.isInsideMessage) {
          this.logHandler(this.buffer);
          this.buffer = '$';
        }
        this.isInsideMessage = true;
      } else if (this.isInsideMessage && BASE64CHARS.indexOf(char) !== -1) {
        this.buffer += char;
      } else {
        this.logHandler(char);
      }
    })
  }
}
