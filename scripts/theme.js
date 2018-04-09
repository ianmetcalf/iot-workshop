'use strict';

const os = require('os');
const path = require('path');
const fs = require('fs-extra');
const clone = require('gh-clone');
const hekyll = require('hekyll');
const yaml = require('js-yaml');

async function fromConfig(file) {
  const content = await fs.readFile(file);
  const config = yaml.safeLoad(content);
  const theme = config.theme || config.remote_theme;

  if (theme && theme.indexOf('/') === -1) {
    return `pages-themes/${ theme }`;
  }

  return theme;
}

async function install(repo, dest) {
  const tmp = path.join(os.tmpdir(), repo);

  await fs.remove(tmp);
  await clone({repo, dest: tmp});

  await fs.remove(dest);
  await hekyll.build({
    cwd: tmp,
    destBase: dest,
  });
}

async function fixUp(file, replacements) {
  let contents = await fs.readFile(file, 'utf8');

  Object.keys(replacements).forEach(search => {
    contents = contents.replace(search, replacements[search]);
  });

  await fs.writeFile(file, contents, 'utf8');
}

module.exports = {
  fromConfig,
  install,
  fixUp,
};

if (require.main === module) {
  fromConfig('_config.yml')

  .then(theme => {
    console.log(`Theme ${ theme }`);

    return install(theme, 'theme')
  })

  .then(() => {
    console.log('Theme installed');

    return fixUp('./theme/_layouts/default.hbs', {
      '{% body %}': '{{{ contents }}}',
      '<p class="copyright">{{default site.title': '<p class="copyright">{{',
    });
  })

  .then(() => {
    console.log('Theme layout fixed');
    process.exit(0);
  })

  .catch(err => {
    console.error(err);
    process.exit(1);
  });
}
