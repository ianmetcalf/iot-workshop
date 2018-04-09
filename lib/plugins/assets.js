'use strict';

const path = require('path');
const assets = require('metalsmith-assets');
const matter = require('gray-matter');
const isUtf8 = require('is-utf8');

function plugin(options = {}) {
  const instance = assets(options);

  return (files, metalsmith, done) => {
    const keys = Object.keys(files).reduce((memo, key) => Object.assign(memo, {[key]: true}));

    instance(files, metalsmith, error => {
      if (error) return done(error);

      try {
        Object.keys(files).forEach(file => {
          const data = files[file];

          if (!keys[file] && isUtf8(data.contents)) {
            const {content} = matter(data.contents.toString());

            if (content) {
              Object.assign(data, {
                contents: new Buffer(content),
              });
            }
          }
        });
      } catch (err) {
        return done(err);
      }

      setImmediate(done);
    });
  };
}

module.exports = plugin;
