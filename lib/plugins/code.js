'use strict';

const path = require('path');
const minimatch = require('minimatch');
const isUtf8 = require('is-utf8');

const TEMPLATE = (language, code) => `
\`\`\`${ language || '' }
${ code || '' }
\`\`\`
`;

function plugin(options = {}) {
  const matcher = minimatch.Minimatch(options.pattern);

  return (files, metalsmith, done) => {
    Object.keys(files).forEach(file => {
      const data = files[file];

      if (matcher.match(file) && isUtf8(data.contents)) {
        const {dir, name} = path.parse(file);

        let md = `${ name }.md`;
        if (dir !== '.') md = path.join(dir, md);

        files[md] = Object.assign(data, {
          contents: new Buffer(TEMPLATE(options.language, data.contents.toString())),
        });

        delete files[file];
      }
    });

    setImmediate(done);
  };
}

module.exports = plugin;
