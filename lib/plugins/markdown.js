'use strict';

const path = require('path');
const url = require('url');
const marked = require('marked');
const lodash = require('lodash');
const Liquid = require('liquidjs');

class Renderer extends marked.Renderer {
  link(href, title, text) {
    const {offline} = this.options;

    const local = offline && (offline[href] || offline[href.split(/\?|#/).shift()])

    if (local) {
      return super.link(local, title, text);
    }

    const result = super.link(href.replace(/\.md$/, ''), title, text);

    if (url.parse(href).hostname) {
      return result.replace(/a href=/g, 'a target="_blank" ref="noopener noreferrer" href=');
    }

    return result;
  }
}

function plugin(options = {}) {
  const markedOptions = Object.assign({}, marked.defaults, options, {
    renderer: new Renderer(),
  });

  const liquidEngine = Liquid({
    cache: true,
  });

  return (files, metalsmith) => {
    const meta = metalsmith.metadata();

    return Promise.all(Object.keys(files).map(async file => {
      const {dir, name, ext} = path.parse(file);

      if (ext === '.md') {
        const data = files[file];
        delete files[file];

        let html = `${ name }.html`;
        if (dir !== '.') html = path.join(dir, html);

        const tokens = marked.lexer(data.contents.toString(), markedOptions);

        const site = Object.assign({}, data.site);
        const [t1, t2] = tokens;

        if (t1 && t1.type === 'heading' && t1.depth === 1) {
          site.title = t1.text.split('[![').shift() || site.title;
          tokens.shift();

          if (t2 && t2.type === 'paragraph') {
            site.description = t2.text.split('[![').shift() || site.description;
            tokens.shift();
          }

          Object.assign(data, {site});
        }

        let contents = marked.parser(tokens, markedOptions);

        if (options.layout) {
          const locals = lodash.merge({}, meta, data, {contents});
          contents = await liquidEngine.renderFile(options.layout, locals);
        }

        files[html] = Object.assign(data, {
          contents: new Buffer(contents),
        });
      }
    }));
  };
}

module.exports = plugin;
