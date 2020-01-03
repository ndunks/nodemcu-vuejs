const CopyWebpackPlugin = require('copy-webpack-plugin')
const CompressionPlugin = require('compression-webpack-plugin');

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
            chunks: ['chunk-vendors', 'chunk-common', 'index'],

        }
    },
    productionSourceMap: false,
    configureWebpack: config => {
        config.resolve.alias['@'] = require('path').join(__dirname, 'src-web')
        config.plugins.push(
            new CopyWebpackPlugin(
                [{ context: `${__dirname}/src-web/data`, from: '*.*' }]
            )
        );


        if (config.mode == 'production') {
            config.output.filename = '[hash:8].js'
            config.output.chunkFilename = '[hash:8].js'
            delete config.optimization.splitChunks
            config.plugins.push(new CompressionPlugin({
                algorithm: 'gzip',
                deleteOriginalAssets: true,
                exclude: /\.(ico|png|jpg)$/
            }))
        }
    },
    chainWebpack: config => {
        config.module.delete('images')
        config.module.delete('svg')
        config.module.delete('fonts')
        config.module.delete('pug')
        //config.plugin('html-index').delete('chunks')
        config.plugins.delete('preload-index')
        config.plugins.delete('prefetch-index')
    },
    css: {
        extract: {
            filename: '[hash:8].css',
            chunkFilename: '[hash:8].css',
        },
    }
}