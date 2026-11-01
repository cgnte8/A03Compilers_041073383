BioCompiler - data-driven front end for the Bio (Genoma) language
=================================================================

Pipeline (biological analogy)
-----------------------------
  1 Cell    (coder)    Step1Cell.c    encode/decode the source (key = language name)
  2 Tissue  (reader)   Step2Tissue.c  buffered reader of characters
  3 Organ   (scanner)  Step3Organ.c   Signals (tokens), driven by compiler_scanner_dfa.csv
  4 System  (parser)   Step4System.c  LL(1) parser, driven by compiler_parser_grammar.txt
  5 Body    (runner)   Step5Body.c    semantic analysis + execution, driven by
                                      compiler_runner_config.txt

Config files (source of truth)
------------------------------
  compiler_scanner_dfa.csv        (loaded by loadScannerDFA)
      [CHAR_CLASSES]  column names; their number must equal CHAR_CLASSES (12)
      [KEYWORDS]      reserved words, loaded into keywordTable (keyword code =
                      position in the list; the parser looks names up here)
      [DFA]           STATE | one column per class | STATE_TYPE | ACTION

  compiler_parser_grammar.txt     (loaded by loadGrammar - DFA must be loaded first)
      [META] / [PRODUCTIONS]  NON_TERMINAL | LOOKAHEAD_SET | PRODUCTION
      Unknown Signal/keyword/operator names are reported and the grammar is
      rejected (no silent wildcards). Up to MAX_GRAMMAR (2048) entries.

  compiler_runner_config.txt      (loaded by loadRunnerConfig)
      [META] language=   [OUTPUT] show_variables=
      [TYPES]    keyword | INTEGER|FLOAT|STRING | suffix
      [BUILTINS] method& | OUTPUT|INPUT
      [LIMITS]   int_min= int_max= max_iterations=
      [MATH]     MATH_FN | name | arity   (validated against the implementation)

Command line
------------
  biology 1 1 <plain> <encoded>      encode (Cell)        biology 6   test Cell
  biology 1 0 <encoded> <plain>      decode (Cell)        biology 7   test Tissue
  biology 2 <encoded>                load/print (Tissue)  biology 8   test Organ
  biology 3 <encoded> [dfa]          scan (Organ)         biology 9   test System
  biology 4 <encoded> [dfa [gram]]   parse (System)       biology 0   test Body
  biology 5 <encoded> [gram [cfg [dfa]]]  check (Organ+System) and run (Body)
  Option 5 runs a program only when it has 0 lexical and 0 syntax errors;
  input& reads one value per line from stdin (use  < InputDataN.txt).

Scripts
-------
  runbioN_<stage>.bat            build + basic run of InputExample1 (stage N)
  runbioN_<stage>_complete.bat   build + unit tests + InputExample0-6
                                 (+ ErrorExample* must be rejected, stages 3-5)
  runbio_complete.sh             Linux/macOS equivalent of runbio5_body_complete.bat
  clean.bat                      remove the executable and generated files
  Exit code of every script = number of failures.

Test inputs
-----------
  InputExample0-6.txt   valid programs (InputData3.txt feeds input& of example 3)
  ErrorExample1.txt     lexical errors          ErrorExample3.txt  runtime error
  ErrorExample2.txt     syntax errors           ErrorExample4.txt  semantic error

Adapt the language
------------------
  1. Edit [KEYWORDS] and [DFA] in compiler_scanner_dfa.csv (keep charClass()
     and CHAR_CLASSES consistent with [CHAR_CLASSES]).
  2. Edit compiler_parser_grammar.txt (keep it LL(1)).
  3. Edit compiler_runner_config.txt ([TYPES], [BUILTINS], [MATH]).
  4. Re-encode every input after changing STR_CompilerName (cipher key).

File update: Oct 2026 (review: bug fixes, config-driven runner, error examples).
