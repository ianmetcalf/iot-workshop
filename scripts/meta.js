'use strict';

const {URL} = require('url');
const pkg = require('../package');

module.exports = {
  site: {
    github: {
      repository_name: getRepoName(pkg.name),
      repository_url: getRepoUrl(pkg.repository),
      owner_name: pkg.author.name,
      owner_url: pkg.author.url,
      is_project_page: true,
      project_tagline: pkg.description,
      build_revision: pkg.version,
    },
  },
};

function getRepoName(name = '') {
  return name.replace(/[-_\s]+/g, ' ');
}

function getRepoUrl(repo = '') {
  const url = typeof repo === 'string' ? repo : repo.url;

  return url.replace(/^git\+|\.git$/ig, '');
}

if (require.main === module) {
  console.log(JSON.stringify(module.exports, null, '  '));
}
