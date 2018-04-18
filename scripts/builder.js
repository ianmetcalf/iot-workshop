'use strict';

const path = require('path');
const url = require('url');
const Metalsmith = require('metalsmith');
const concat = require('metalsmith-concat');
const metallic = require('metalsmith-metallic');
const permalinks = require('metalsmith-permalinks');
const prefixer = require('metalsmith-prefix');
const renamer = require('metalsmith-renamer');
const sass = require('metalsmith-sass');
const assets = require('../lib/plugins/assets');
const code = require('../lib/plugins/code');
const markdown = require('../lib/plugins/markdown');
const meta = require('./meta');

const builder = Metalsmith(path.resolve(__dirname, '..'));

builder.metadata(meta);
builder.source('./');

builder.ignore([
  'node_modules',
  'theme',
  '!*.+(html|md|ino)',
]);

builder.use(code({
  pattern: 'examples/**/*.ino',
  language: 'c',
}));

builder.use(renamer({
  examples: {
    pattern: 'examples/**/*.md',
    rename: 'index.md',
  },
  readme: {
    pattern: '**/README.md',
    rename: 'index.md',
  },
}));

builder.use(metallic());

let offline = {};

if (process.env.OFFLINE) {
  Object.assign(offline, require(path.resolve(process.env.OFFLINE)));
}

builder.use(markdown({
  layout: 'theme/_layouts/default.html',
  offline,
}));

builder.use(permalinks({
  relative: false,
}));

if (process.env.PREFIX) {
  builder.use(prefixer(process.env.PREFIX));
}

builder.use(assets({
  source: 'theme/assets',
}));

builder.use(assets({
  source: 'assets',
}));

builder.use(sass({
  includePaths: [
    'theme/_sass',
    'node_modules/highlight.js/styles',
  ],
}));

builder.use(concat({
  files: [
    'css/style.css',
    'css/customize.css',
  ],
  output: 'css/style.css',
  forceOutput: true,
}));

module.exports = builder;

if (require.main === module) {
  builder.build(err => {
    if (err) {
      console.error(err);
      process.exit(1);
    } else {
      console.log('Build complete');
      process.exit(0);
    }
  });
}
