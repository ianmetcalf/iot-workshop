'use strict';

module.exports = {
  frame(...args) {
    const {fn, data, hash} = args.pop() || {};

    if (typeof fn !== 'function') {
      throw new Error('Must use #frame as a block helper');
    }

    return fn(this, {data: hash}) || '';
  },

  relative_url(arg) {
    return (typeof arg === 'string' && arg.replace(/^\/assets/, '')) || '';
  },

  append(...args) {
    return args.filter(arg => (typeof arg === 'string')).join('');
  },

  default(...args) {
    return args.find(arg => (arg && typeof arg === 'string')) || '';
  },
};
