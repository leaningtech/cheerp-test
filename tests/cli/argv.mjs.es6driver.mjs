import module from './argv.mjs'
import { createRequire } from 'node:module';
const require = createRequire(import.meta.url);
globalThis.Blob = require('node:buffer').Blob;
globalThis.Worker = require('../../webworker-polyfill.js').Worker;
var env=[];
var argv=["argv.cpp"];

function onInstantiation(_) {
}
function onError(err)
{
	console.log("unexpected exit : FAILURE");
	throw err;
}

module({relativePath:'', argv: typeof argv == 'undefined' ? [] : argv, env: typeof env == 'undefined' ? [] : env}).then(_ => {onInstantiation(_)}).catch(err => {onError(err)})