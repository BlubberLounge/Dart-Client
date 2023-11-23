/**
 * Writes compressed C arrays of data files (web interface)
 * How to use it?
 *
 * 1) Install Node 11+ and npm
 * 2) npm install
 * 3) npm run build
 *
 * If you change data folder often, you can run it in monitoring mode (it will recompile and update *.h on every file change)
 *
 * > npm run dev
 *
 * How it works?
 *
 * It uses NodeJS packages to inline, minify and GZIP files. See writeHtmlGzipped and writeChunks invocations at the bottom of the page.
 */

const fs = require("fs");
const inliner = require("inliner");
const zlib = require("zlib");
const CleanCSS = require("clean-css");
const MinifyHTML = require("html-minifier-terser").minify;
const packageJson = require("../package.json");

/**
 *
 */
function hexdump(buffer,isHex=false) {
  let lines = [];

  for (let i = 0; i < buffer.length; i +=(isHex?32:16)) {
    var block;
    let hexArray = [];
    if (isHex) {
      block = buffer.slice(i, i + 32)
      for (let j = 0; j < block.length; j +=2 ) {
        hexArray.push("0x" + block.slice(j,j+2))
      }
    } else {
      block = buffer.slice(i, i + 16); // cut buffer into blocks of 16
      for (let value of block) {
        hexArray.push("0x" + value.toString(16).padStart(2, "0"));
      }
    }

    let hexString = hexArray.join(", ");
    let line = `  ${hexString}`;
    lines.push(line);
  }

  return lines.join(",\n");
}

function strReplace(str, search, replacement) {
  return str.split(search).join(replacement);
}

function adoptVersionAndRepo(html) {
  let repoUrl = packageJson.repository ? packageJson.repository.url : undefined;
  if (repoUrl) {
    repoUrl = repoUrl.replace(/^git\+/, "");
    repoUrl = repoUrl.replace(/\.git$/, "");
    // Replace we
    html = strReplace(html, "https://github.com/atuline/WLED", repoUrl);
    html = strReplace(html, "https://github.com/Aircoookie/WLED", repoUrl);
  }
  let version = packageJson.version;
  if (version) {
    html = strReplace(html, "##VERSION##", version);
  }
  return html;
}

function filter(str, type) {
  str = adoptVersionAndRepo(str);
  if (type === undefined) {
    return str;
  } else if (type == "css-minify") {
    return new CleanCSS({}).minify(str).styles;
  } else if (type == "js-minify") {
    return MinifyHTML('<script>' + str + '</script>', {
      collapseWhitespace: true,
      minifyJS: true, 
      continueOnParseError: false,
      removeComments: true,
    }).replace(/<[\/]*script>/g,'');
  } else if (type == "html-minify") {
    return MinifyHTML(str, {
      collapseWhitespace: true,
      maxLineLength: 80,
      minifyCSS: true,
      minifyJS: true, 
      continueOnParseError: false,
      removeComments: true,
    });
  } else if (type == "html-minify-ui") {
    return MinifyHTML(str, {
      collapseWhitespace: true,
      conservativeCollapse: true,
      maxLineLength: 80,
      minifyCSS: true,
      minifyJS: true, 
      continueOnParseError: false,
      removeComments: true,
    });
  } else {
    console.warn("Unknown filter: " + type);
    return str;
  }
}

function writeHtmlGzipped(sourceFile, resultFile, page) {
  console.info("Reading " + sourceFile);
  new inliner(sourceFile, function (error, html) {
    console.info("Inlined " + html.length + " characters");
    html = filter(html, "html-minify-ui");
    console.info("Minified to " + html.length + " characters");

    if (error) {
      console.warn(error);
      throw error;
    }

    html = adoptVersionAndRepo(html);
    zlib.gzip(html, { level: zlib.constants.Z_BEST_COMPRESSION }, function (error, result) {
      if (error) {
        console.warn(error);
        throw error;
      }

      console.info("Compressed " + result.length + " bytes");
      const array = hexdump(result);
      const src = `/*
 * Binary array for the Web UI.
 * gzip is used for smaller size and improved speeds.
 * 
 */
 
// Autogenerated from ${sourceFile}, do not edit!!
const uint16_t HTML_${page}_L = ${result.length};
const uint8_t HTML_${page}[] PROGMEM = {
${array}
};
`;
      console.info("Writing " + resultFile);
      fs.writeFileSync(resultFile, src);
    });
  });
}

writeHtmlGzipped("src/web/index.html", "src/html_index.h", 'index');