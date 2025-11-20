# Visual Diagrams: True Substitution Flow

## Overview Diagram: The Full Pipeline

```
User Command
    │
    ├─→ lit . --param TARGETS=js
    │
    └─→ lit.cfg reads TARGETS parameter
        │
        ├─→ Determines selected_targets = {'js'}
        │
        └─→ For each macro in test file:
            ├─→ If target IN selected_targets:  %macro{target} → replaced with command
            │   └─→ Enabled target RUN line → executes normally
            │
            └─→ If target NOT IN selected_targets: %macro{target} → replaced with 'true'
                └─→ Disabled target RUN line → executes as no-op
    │
    ├─→ Lit processes test file with expanded macros
    │
    ├─→ Shell executes expanded RUN lines
    │
    └─→ Test Pass/Fail determined by exit codes
```

---

## Scenario Flow Chart: With 'true' Substitution ✅

```
Test: global_variables.cpp
Target Selection: TARGETS=js

┌─────────────────────────────────────────────────────────────────┐
│                    Substitution Phase                           │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  Line 1: // RUN: %compile_for{wasm} -O1 -o %t/w %s             │
│          ├─ wasm NOT in {js}                                   │
│          ├─ Match regex: %compile_for{wasm}[^\n]*              │
│          └─ Replace with: 'true'                               │
│             ↓                                                   │
│          // RUN: true                                          │
│                                                                 │
│  Line 2: // RUN: %compile_for{js} -O1 -o %t/j %s              │
│          ├─ js IN {js}                                         │
│          ├─ Match regex: %compile_for{js}                      │
│          └─ Replace with: '/opt/cheerp/bin/clang++ ...'        │
│             ↓                                                   │
│          // RUN: /opt/cheerp/bin/clang++ -target cheerp -O1 -o %t/j %s
│                                                                 │
│  Line 3: // RUN: %run_if{wasm} test -f %t/w.wasm               │
│          ├─ wasm NOT in {js}                                   │
│          ├─ Match regex: %run_if{wasm}[^\n]*                   │
│          └─ Replace with: 'true'                               │
│             ↓                                                   │
│          // RUN: true                                          │
│                                                                 │
│  Line 4: // RUN: %run_if{js} test -f %t/j                      │
│          ├─ js IN {js}                                         │
│          ├─ Match regex: %run_if{js}[^\n]*                     │
│          └─ Replace with: ''  (remove prefix)                  │
│             ↓                                                   │
│          // RUN: test -f %t/j                                  │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
         ↓
┌─────────────────────────────────────────────────────────────────┐
│                    Execution Phase                              │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  Shell: true                                                   │
│         └─ Exit Code: 0 ✅                                     │
│                                                                 │
│  Shell: /opt/cheerp/bin/clang++ -target cheerp ...             │
│         └─ Compiles JavaScript                                 │
│         └─ Exit Code: 0 ✅                                     │
│                                                                 │
│  Shell: true                                                   │
│         └─ Exit Code: 0 ✅                                     │
│                                                                 │
│  Shell: test -f %t/j                                           │
│         └─ File exists                                         │
│         └─ Exit Code: 0 ✅                                     │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
         ↓
    TEST RESULT: PASS ✅
```

---

## Scenario Flow Chart: Without 'true' Substitution ❌

### Scenario A: Empty String Replacement

```
Test: global_variables.cpp
Target Selection: TARGETS=js

┌─────────────────────────────────────────────────────────────────┐
│                    Substitution Phase                           │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  Line 1: // RUN: %compile_for{wasm} -O1 -o %t/w %s             │
│          ├─ wasm NOT in {js}                                   │
│          ├─ Replace with: ''  (WRONG!)                         │
│             ↓                                                   │
│          // RUN: -O1 -o %t/w %s                                │
│                                                                 │
│  Line 2: // RUN: %compile_for{js} -O1 -o %t/j %s              │
│          ├─ js IN {js}                                         │
│          └─ Replace with: '/opt/cheerp/bin/clang++ ...'        │
│             ↓                                                   │
│          // RUN: /opt/cheerp/bin/clang++ -target cheerp -O1 -o %t/j %s
│                                                                 │
│  Line 3: // RUN: %run_if{wasm} test -f %t/w.wasm               │
│          ├─ wasm NOT in {js}                                   │
│          ├─ Replace with: ''  (WRONG!)                         │
│             ↓                                                   │
│          // RUN: test -f %t/w.wasm                             │
│                                                                 │
│  Line 4: // RUN: %run_if{js} test -f %t/j                      │
│          ├─ Replace with: 'test -f %t/j'                       │
│             ↓                                                   │
│          // RUN: test -f %t/j                                  │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
         ↓
┌─────────────────────────────────────────────────────────────────┐
│                    Execution Phase                              │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  Shell: -O1 -o %t/w %s                                         │
│         ├─ Shell sees: "-O1" as a command                      │
│         ├─ Searches PATH for "-O1"                             │
│         ├─ Cannot find command                                 │
│         └─ Exit Code: 127 ❌ ERROR!                            │
│            "command not found: -O1"                            │
│                                                                 │
│  [Next RUN lines not executed due to test failure]             │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
         ↓
    TEST RESULT: FAIL ❌
    Error: command not found: -O1
```

### Scenario B: No Replacement at All

```
Test: global_variables.cpp
Target Selection: TARGETS=js

┌─────────────────────────────────────────────────────────────────┐
│                    Substitution Phase                           │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  Line 1: // RUN: %compile_for{wasm} -O1 -o %t/w %s             │
│          ├─ wasm NOT in {js}                                   │
│          ├─ No substitution (WRONG!)                           │
│             ↓                                                   │
│          // RUN: %compile_for{wasm} -O1 -o %t/w %s             │
│          (unchanged)                                           │
│                                                                 │
│  [Other lines...]                                              │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
         ↓
┌─────────────────────────────────────────────────────────────────┐
│                    Execution Phase                              │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  Shell: %compile_for{wasm} -O1 -o %t/w %s                      │
│         ├─ Shell sees: "%compile_for{wasm}" as a command       │
│         ├─ Searches PATH for "%compile_for{wasm}"              │
│         ├─ Cannot find command (% is not valid in commands)    │
│         └─ Exit Code: 127 ❌ ERROR!                            │
│            "command not found: %compile_for{wasm}"             │
│                                                                 │
│  [Test terminates immediately]                                 │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
         ↓
    TEST RESULT: FAIL ❌
    Error: command not found: %compile_for{wasm}
```

---

## State Diagram: Macro Substitution Decision Tree

```
                         Test File with Macros
                                │
                                ├─ Read TARGETS parameter
                                │
                    ┌───────────┴────────────┐
                    ↓                        ↓
            selected_targets          Examine each RUN line
                = {js}                      │
                    │              ┌────────┴─────────┐
                    │              │                  │
              Process each     Find %macro{T}    Process next
              macro in         │                 ├─ No match
              test files       ├─ Found          │
                    │          │                └─ Continue
                    │      ┌───┴────┐
                    │      ↓        ↓
                    │   T in      T not in
                    │ selected_  selected_
                    │ targets    targets
                    │   │          │
                    │   │   ┌──────┴────────┐
                    │   │   │               │
                    │   │   │         Match entire
                    │   │   │         line with regex
                    │   │   │         %macro{T}[^\n]*
                    │   │   │               │
                    │   │   │         Replace with
                    │   │   │         'true'
                    │   │   │               │
                  ┌─┴───┴───┴───┐     ┌─────┴──────┐
                  ↓             ↓     ↓            ↓
            Remove prefix   Command  No-op   Continue to
            Keep command    executes succeeds next RUN
            Executes    ✅    ✅  ✅     line ✅
            normally         
                    │         │         │
                    └────┬────┴────┬────┘
                         │        │
                    All RUN lines exit normally
                         │
                      TEST PASSES ✅
```

---

## Regex Matching Diagram

### Pattern for Disabled Targets

```
Regular Expression:
    r'%compile_for\{' + target + r'\}[^\n]*'

Breakdown for target='wasm':
    r'%compile_for\{wasm\}[^\n]*'

Text in test file:
    // RUN: %compile_for{wasm} -O1 -o %t/w %s
               ↑                               ↑
            Matches start           Matches to end of line
            %compile_for{wasm}      [^\n]* = everything except newline
                                    = " -O1 -o %t/w %s"

Complete Match:
    %compile_for{wasm} -O1 -o %t/w %s
    └──────────────────────────────────┘
           Entire content replaced with 'true'
```

### Why `[^\n]*` is Critical

```
WITHOUT [^\n]*:
    Pattern: r'%compile_for\{wasm\}'
    Match result:
        // RUN: %compile_for{wasm} -O1 -o %t/w %s
                 └─ only this part matches
    
    Replacement:
        // RUN: true -O1 -o %t/w %s
                      ↑
                    Still broken! These flags remain

WITH [^\n]*:
    Pattern: r'%compile_for\{wasm\}[^\n]*'
    Match result:
        // RUN: %compile_for{wasm} -O1 -o %t/w %s
                 └────────────────────────────────┘
                        entire content to line end
    
    Replacement:
        // RUN: true
                ↑
                Clean! Everything replaced
```

---

## Command Execution Diagram

### With 'true' Substitution (Correct)

```
RUN line in test: %compile_for{wasm} -O1 -o %t/w %s
(for disabled target wasm, running with TARGETS=js)

Step 1: Substitution
    Input:  %compile_for{wasm} -O1 -o %t/w %s
    Regex match: %compile_for{wasm}[^\n]*
    Output: true

Step 2: Shell interprets command
    Command: true
    │
    └─ Built-in shell command
       - Always returns exit code 0
       - Ignores any arguments
       - No side effects

Step 3: Execution result
    Exit Code: 0 ✅
    Status: SUCCESS (no-op)
```

### Without 'true' Substitution (Broken)

```
RUN line in test: %compile_for{wasm} -O1 -o %t/w %s
(for disabled target wasm, running with TARGETS=js)

Step 1: Substitution (WRONG - empty string)
    Input:  %compile_for{wasm} -O1 -o %t/w %s
    Replace with: ''
    Output: -O1 -o %t/w %s

Step 2: Shell interprets command
    Command: -O1 -o %t/w %s
    │
    ├─ Shell looks for executable named "-O1"
    ├─ Searches PATH
    └─ Not found anywhere!

Step 3: Execution result
    Exit Code: 127 ❌
    Status: ERROR - command not found
    Message: "-O1: command not found"
```

---

## Timeline Diagram: Test Execution with Target Selection

```
Event Timeline: lit . --param TARGETS=js

Time    Event                           With 'true'     Without 'true'
────    ─────                           ───────────     ──────────────
T0      User runs lit command
        ↓
T1      lit reads lit.cfg
        selected_targets = {'js'}
        ↓
T2      lit reads test file
        Finds 4 RUN lines
        ↓
T3      Substitution phase begins
        ↓
T4      Line 1: %compile_for{wasm}...   Becomes 'true' ✅  Becomes '-O1 ...' ❌
        ↓
T5      Line 2: %compile_for{js}...     Becomes compiler  Becomes compiler ✅
        ↓
T6      Line 3: %run_if{wasm}...        Becomes 'true' ✅  Becomes 'test ...' ❌
        ↓
T7      Line 4: %run_if{js}...          Becomes cmd    Becomes cmd ✅
        ↓
T8      Expansion complete
        Passes all RUN lines to shell
        ↓
T9      Shell executes RUN lines
        ↓
T10     Line 1: true                    Exit 0 ✅         N/A (broken)
        ↓
T11     Line 2: clang++                 Compiles ✅       Compiles ✅
        ↓
T12     Line 3: true                    Exit 0 ✅         N/A (broken)
        ↓
T13     Line 4: test -f                 Exit 0 ✅         N/A (broken)
        ↓
T14     All lines completed
        ↓
T15     Test evaluation
        ↓
T16     Result                          PASS ✅            FAIL ❌
                                        (at T9-10)
```

---

## Summary: Visual Explanation

The key insight:

```
┌─ ENABLED TARGET (js)
│  └─ Macro → Full command → Execute → Verify output ✅
│
MACRO EXPANSION
│
└─ DISABLED TARGET (wasm)
   WITHOUT 'true': Macro → Garbage command → Shell error ❌
   WITH 'true':    Macro → 'true' → No-op → Continue ✅
```

The `true` replacement is essential because it's the ONLY valid shell command that:
- Always succeeds
- Ignores all arguments
- Has no side effects
- Is universally available
