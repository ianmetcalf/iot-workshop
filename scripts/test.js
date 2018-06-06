'use strict';

const path = require('path');
const glob = require('glob');
const tape = require('tape');
const spawn = require('tape-spawn');

const boards = [
  'adafruit:samd:adafruit_feather_m0',
  'adafruit:wiced:feather',
  'esp8266:esp8266:huzzah:FlashSize=4M3M,CpuFrequency=80',
  'espressif:esp32:featheresp32:FlashFreq=80',
];

glob('{examples,solutions}/**/*.ino', (err, files) => {
  if (err) throw err;

  boards.forEach(board => files.forEach(file => {
    tape(`Verify "${ path.basename(file) }" compiles for "${ board }"`, t => {
      const st = spawn(t, `arduino --verify --board ${ board } ${ path.resolve(file) }}`);
      st.succeeds();
      st.end();
    });
  }));
});
