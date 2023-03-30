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

import type {NextPage} from 'next'
import Head from 'next/head'
import styles from '../styles/Home.module.scss';
import Log from "../components/log";
import Repl from "../components/repl";
import Connect from "../components/connect";
import BtnUploadDB from '../components/uploadDb';
import {WebSerial, Device} from "pigweedjs";
import {useState} from 'react';
type WebSerialTransport = WebSerial.WebSerialTransport

const Home: NextPage = () => {
  const [device, setDevice] = useState<WebSerialTransport | undefined>(undefined);
  const [tokenDB, setTokenDB] = useState("");
  return (
    <div className={styles.container}>
      <Head>
        <title>Pigweed Console</title>
        <meta name="description" content="Generated by create next app" />
        <link rel="icon" href="/favicon.png" />
      </Head>

      <main className={styles.main}>
        <div className={styles.toolbar}>
          <span className={styles.logo}><span>Pigweed</span> Web Console</span>
          <Connect onConnection={(device) => {
            setDevice(device);
          }} />
          <BtnUploadDB onUpload={(db) => {
            setTokenDB(db);
          }} />
        </div>

        <div className={styles.grid}>
          <div>
            <Log device={device} tokenDB={tokenDB}></Log>
          </div>
          {/*<div>*/}
          {/*  <Repl device={undefined}></Repl>*/}
          {/*</div>*/}
        </div>
      </main>
    </div>
  )
}

export default Home
