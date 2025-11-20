#!/usr/bin/env node
/**
 * Test wrapper for threading tests (atomic1-4.cpp, thread_setup.cpp)
 * 
 * This wrapper:
 * - Loads Worker and Blob polyfills for Node.js
 * - Imports the compiled WebAssembly module with pthread support
 * - Handles async module initialization
 * - Reports test results
 */

import { createRequire } from 'node:module';
import { fileURLToPath } from 'node:url';
import { dirname } from 'node:path';

const require = createRequire(import.meta.url);
const __filename = fileURLToPath(import.meta.url);
const __dirname = dirname(__filename);

// Get the module path from command line
const modulePath = process.argv[2];
if (!modulePath) {
  console.error('Usage: threading-wrapper.mjs <path-to-compiled-module.mjs>');
  process.exit(1);
}

// Load polyfills for Worker and Blob APIs
globalThis.Blob = require('node:buffer').Blob;
globalThis.Worker = require('./webworker-polyfill.js').Worker;

// Set up minimal environment
globalThis.argv = [];
globalThis.env = [];

// Define callback functions expected by the module
function onInstantiation() {
  // Called after module is instantiated
}

function getPromise(_) {
  return Promise.resolve();
}

function getExports() {
  return {};
}

function onError(err) {
  console.error('Test execution failed:', err);
  process.exit(1);
}

// Make callbacks available globally
globalThis.onInstantiation = onInstantiation;
globalThis.getPromise = getPromise;
globalThis.getExports = getExports;
globalThis.onError = onError;

// Import and run the module
try {
  const module = await import(modulePath);
  
  // For ES6 modules with threading
  if (typeof module.default === 'function') {
    const result = await module.default({
      relativePath: '',
      argv: globalThis.argv,
      env: globalThis.env
    });
    
    // Wait for async operations to complete
    await getPromise(result);
    onInstantiation(result);
  } else {
    console.error('Module did not export expected function');
    process.exit(1);
  }
} catch (err) {
  onError(err);
}
