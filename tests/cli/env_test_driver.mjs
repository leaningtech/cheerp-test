import module from './env.mjs'
import { createRequire } from 'node:module';
const require = createRequire(import.meta.url);
globalThis.Blob = require('node:buffer').Blob;
var env=["TEST=1"];
var argv=[];

function onInstantiation(_) {
}
function onError(err)
{
	console.log("unexpected exit : FAILURE");
	throw err;
}

module({relativePath:'', argv: typeof argv == 'undefined' ? [] : argv, env: typeof env == 'undefined' ? [] : env}).then(_ => {onInstantiation(_)}).catch(err => {onError(err)})
