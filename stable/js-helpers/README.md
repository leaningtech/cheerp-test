# JavaScript Helper Files

This directory contains helper JavaScript files used by the test driver system for jsexport tests.

## Files

### import-polyfills.js
Node.js polyfills for browser APIs that Cheerp-compiled code may expect:
- `Blob` - From Node.js buffer module
- `Worker` - Web Worker polyfill (from webworker-polyfill.js)

This file is automatically included by `create_driver.py` when creating test drivers.

### webworker-polyfill.js
Provides a Web Worker polyfill for Node.js environments, enabling tests that use Web Workers to run in Node.js.

## Source

These files are copied from `../test-wrappers/` and adapted for use within the `working_examples` directory. The paths have been adjusted to use relative references within this directory structure.

## Usage

These files are used automatically by the `create_driver.py` script. You typically don't need to reference them directly unless you're creating custom test harnesses.

The `create_driver.py` script will:
1. Locate `import-polyfills.js` in this directory
2. Include its contents in generated driver files
3. Adjust paths (like the webworker-polyfill.js reference) to be relative to the output driver location
