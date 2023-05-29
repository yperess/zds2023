// Copyright 2022 The Pigweed Authors
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

import {useEffect, useRef, useState} from "react";
import {pw_tokenizer, Device, WebSerial} from "pigweedjs";
import {AutoSizer, Table, Column} from 'react-virtualized';
import {listenToDefaultLogService} from "../common/logService";
import 'react-virtualized/styles.css';
import styles from "../styles/log.module.css";
import StreamParser from "../common/streamParser";

type Detokenizer = pw_tokenizer.Detokenizer;

interface LogProps {
  device: Device | undefined,
  transport: WebSerial.WebSerialTransport | undefined,
  tokenDB: string | undefined,
  mode: string
}

interface LogEntry {
  msg: string,
  timestamp: number,
  humanTime: string,
  module: string,
  file: string
}

function parseLogMsg(msg: string): LogEntry {
  const pairs = msg.split("■").slice(1).map(pair => pair.split("♦"));

  // Not a valid message, print as-is.
  if (pairs.length === 0) {
    return {
      msg,
      module: "",
      file: "",
      timestamp: Date.now(),
      humanTime: new Date(Date.now()).toLocaleTimeString("en-US")
    }
  }

  let map: any = {};
  pairs.forEach(pair => map[pair[0]] = pair[1])
  return {
    msg: map.msg,
    module: map.module,
    file: map.file,
    timestamp: Date.now(),
    humanTime: new Date(Date.now()).toLocaleTimeString("en-US")
  }
}

const keyToDisplayName: {[key: string]: string} = {
  "msg": "Message",
  "humanTime": "Time",
  "module": "Module",
  "file": "File"
}

export default function Log({device, transport, tokenDB, mode}: LogProps) {
  const [logs, setLogs] = useState<LogEntry[]>([]);
  const [detokenizer, setDetokenizer] = useState<Detokenizer | null>(null);
  const logTable = useRef<Table | null>(null);
  const _headerRenderer = ({dataKey, sortBy, sortDirection}: any) => {
    return (
      <div>
        {keyToDisplayName[dataKey]}
      </div>
    );
  }

  const processFrame = (decodedFrame: string) => {
    if (detokenizer) {
      console.log('trying to detokenize');
      const detokenized = detokenizer.detokenizeBase64String(decodedFrame, 0);
      setLogs(oldLogs => [...oldLogs, parseLogMsg(detokenized)]);
    } else {
      setLogs(oldLogs => [...oldLogs, parseLogMsg(decodedFrame)]);
    }
    setTimeout(() => {
      logTable.current!.scrollToRow(logs.length - 1);
    }, 100);
  }

  const processFrameUint8 = (frame: Uint8Array) => {
    if (detokenizer) {
      const detokenized = detokenizer.detokenizeUint8Array(frame);
      console.log('frame: "' + frame + '", detokenized: "' + detokenized + '"');
      setLogs(oldLogs => [...oldLogs, parseLogMsg(detokenized)]);
    }
    else {
      const decoded = new TextDecoder().decode(frame);
      setLogs(oldLogs => [...oldLogs, parseLogMsg(decoded)]);
    }
    setTimeout(() => {
      logTable.current!.scrollToRow(logs.length - 1);
    }, 100);
  }

  useEffect(() => {
    let cleanupFn: () => void;
    if (mode === "rpc" && device) {
        listenToDefaultLogService(device, processFrameUint8).then((unsub) => cleanupFn = unsub);
    } else if (transport) {
      console.log('mode = "' + mode + '"');
      if (mode === "raw") {
        console.log("Registering raw parser, transport=" + transport);
        const subscription = transport.chunks.subscribe((item: Uint8Array) => {
          setLogs(oldLogs =>
            [...oldLogs, parseLogMsg(Buffer.from(item.buffer).toString())]);
        });
        cleanupFn = () => subscription.unsubscribe();
      } else if (mode === "stream") {
        console.log("Registering stream parser, transport=" + transport);
        let streamParser = new StreamParser((str) => processFrame(str));
        const subscription = transport.chunks.subscribe((item: Uint8Array) => {
          streamParser.read(item);
        });
        cleanupFn = () => subscription.unsubscribe();
      }

      return () => {
        if (cleanupFn) {
          console.log("Calling cleanup function");
          cleanupFn();
        }
      }
    }
  }, [device, transport, detokenizer, mode]);

  useEffect(() => {
    if (tokenDB && tokenDB.length > 0) {
      const detokenizer = new pw_tokenizer.Detokenizer(tokenDB);
      setDetokenizer(detokenizer);
    }
  }, [tokenDB])

  return (
    <>
      {/* @ts-ignore */}
      <AutoSizer>
        {({height, width}) => (
          <>
            {/* @ts-ignore */}
            <Table
              className={styles.logsContainer}
              headerHeight={40}
              height={height}
              rowCount={logs.length}
              rowGetter={({index}) => logs[index]}
              rowHeight={30}
              ref={logTable}
              width={width}
            >
              {/* @ts-ignore */}
              <Column
                dataKey="humanTime"
                width={190}
                headerRenderer={_headerRenderer}
              />
              {/* @ts-ignore */}
              <Column
                dataKey="msg"
                flexGrow={1}
                width={290}
                headerRenderer={_headerRenderer}
              />
              {/* @ts-ignore */}
              <Column
                dataKey="module"
                width={190}
                headerRenderer={_headerRenderer}
              />
              {/* @ts-ignore */}
              <Column
                dataKey="file"
                flexGrow={1}
                width={190}
                headerRenderer={_headerRenderer}
              />
            </Table>
          </>
        )}
      </AutoSizer>
    </>
  )
}
