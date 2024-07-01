
# DreamBerd

This is an interpreter for the perfect programming language, written in C.

## Credits

- [DreamBerd](https://github.com/TodePond/DreamBerd) by [TodePond](https://github.com/TodePond)
- 2800 lines of Java code I wrote a long time ago by following [a Python programming language tutorial by CodePulse](https://www.youtube.com/playlist?list=PLZQftyCk7_SdoVexSmwy_tBgs7P0b97yD)
- [The DreamBerd interpreter written in Python](https://github.com/vivaansinghvi07/dreamberd-interpreter) by [vivaansinghvi07](https://github.com/vivaansinghvi07). I will probably use it at some point to see if my implementation is consistent with theirs, but I don't plan on looking at their code unless absolutely necessary.

## Implementation differences

Things that won't be exactly as the documentation states or are not mentioned in the documentation:

- Negative lifetimes will probably be preprocessed so
  ```java
  print(name)!
  const const name<-1> = "Luke"!
  ```
  turns into
  ```java
  const const name<1> = "Luke"!
  print(name)!
  ```
- To make the `RegularExpression` (mentioned in the docs) type possible, the regex has been changed from `/Reg(ular)?[eE]x(press|p)?/` (which doesn't allow for `RegularExpression`) to `/Reg(ular)?[eE]x(press(ion)?|p)?/`
- String interpolation will work for all documented currencies no matter where you are, so if you're visiting the US, but your locale is still set to the UK, you can use `$`!
- Euros will (also) be in front of the curly brackets (e.g. `€{name}`), because where I live, that's the local typographical norm (and, in English writing, [the official norm](https://en.wikipedia.org/wiki/Euro_sign#Use)). I'll probably implement `{name}€` as well.
- Booleans won't actually be stored as 1.5 bits, because that's quite impossible.

Things I *may* implement if I so desire:

- ABI
- AQMI

Things I probably *won't* be implementing include:

- AI
- DBX, because I don't want to parse any form of XML
- Changing the time (`Date.now() -= 3600000!`). I don't want to come up with a cross-platform solution to changing the system time.
- ```java
  const const player = { name: "Lu" }!
  print("Hello {player$name}!")!
  ```

## Checklist

- [ ] Implement basic language features (arithmetic, basic syntax, print, strings, etc.). This means none of the crazy features no other language has yet.
- [ ] Transition from parentheses to significant whitespace
- [ ] Strings
- [ ] Booleans
- [ ] Question marks
- [ ] Arrays
- [ ] Functions
- [ ] The typing syntax
- [ ] The `/Reg(ular)?[eE]x(press(ion)?|p)?/` type that doesn't do anything because types don't do anything
- [ ] Enforce 3-space indentation
- [ ] String interpolation
- [ ] `when`
- [ ] `previous`
- [ ] `delete`
- [ ] Implement variable quotes for strings.
- [ ] Zero-quote strings and AEMI
- [ ] Equality
- [ ] `Date.now()`
- [ ] Unicode support (including 1️⃣ hopefully)
- [x] Loops
- [ ] Overloading variables
- [ ] `reverse`
- [ ] Classes
- [ ] Lifetimes
- [ ] Fractions
- [ ] Async functions
- [ ] Number names (`one`, `two`, `three`, etc.)
- [ ] Signals
- [ ] Files (`=====`)
- [ ] Exporting and importing
- [ ] Different types of variables (`const const`, `const var`, `var const`, `var var`). This shouldn't be *too* hard.
- [ ] Locally store `const const const` and `<Infinity>` variables
- [ ] The DreamBerd installer and the DreamBerd installer installer
- [ ] The "Create DreamBerd App" app
- [ ] Make a server to host the `const const const` variables