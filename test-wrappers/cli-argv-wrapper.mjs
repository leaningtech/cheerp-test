#!/usr/bin/env node
/**
 * Test wrapper for unit/cli/argv.cpp
 * 
 * This wrapper:
 * - Imports the compiled ES6 module
 * - Sets up argv array as expected by the test
 * - Handles module initialization and error reporting
 */

import { createRequire } from 'node:module';
const require = createRequire(import.meta.url);

// Get the module path from command line
const modulePath = process.argv[2];
if (!modulePath) {
  console.error('Usage: cli-argv-wrapper.mjs <path-to-compiled-module.mjs>');
  process.exit(1);
}

// Set up environment for the test
globalThis.argv = ["argv.cpp"];  // Expected by the test
globalThis.env = [];

// Import and run the module
try {
  const module = await import(modulePath);
  
  // The module export is a function that returns a Promise
  if (typeof module.default === 'function') {
    await module.default({
      relativePath: '',
      argv: globalThis.argv,
      env: globalThis.env
    });
  } else {
    console.error('Module did not export expected function');
    process.exit(1);
  }
} catch (err) {
  console.error('Test execution failed:', err);
  process.exit(1);
}
