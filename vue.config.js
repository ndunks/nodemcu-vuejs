/** @type {import("@vue/cli-service").ProjectOptions} */
module.exports = {
    outputDir: "data",
    pages: {
        index: {
            // entry for the page
            entry: 'src-web/main.ts',
            // the source template
            template: 'src-web/index.html',
            // output as dist/index.html
            filename: 'index.html',
            // chunks to include on this page, by default includes
            // extracted common chunks and vendor chunks.
            chunks: ['chunk-vendors', 'chunk-common', 'index']
        }
    },
    productionSourceMap: false,
    configureWebpack: config => {
        config.resolve.alias['@'] = require('path').join(__dirname, 'src-web')
    }
}