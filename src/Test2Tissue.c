/*
 ************************************************************
 * BIOSYSTEMS COURSE - Algonquin College
 * Code version: Fall, 2026
 * Author: Connor Guenette - 04107333
 * Professors: Paulo Sousa
 ************************************************************
 #
 # ECHO "###########################################"
 # ECHO "#                                         #"
 # ECHO "#                  .+*+.                  #"
 # ECHO "#                .*#####*.                #"
 # ECHO "#               *#########*               #"
 # ECHO "#              +###########*              #"
 # ECHO "#              *####*+*####*              #"
 # ECHO "#               *###. .###*               #"
 # ECHO "#                +##. .##+                #"
 # ECHO "#                 *#####*                 #"
 # ECHO "#               .+#######*.               #"
 # ECHO "#              *###########*              #"
 # ECHO "#             +#############+             #"
 # ECHO "#              *###########*              #"
 # ECHO "#               *#########*               #"
 # ECHO "#                 +#####*                 #"
 # ECHO "#                   *+                    #"
 # ECHO "#                                         #"
 # ECHO "#####     S T E L C O M P I L E R     #####"
 # ECHO "#                                         #"
 # ECHO "########        S T E L L A        ########"
 # ECHO "#                                         #"
 # ECHO "###########################################"
 * ---------------------------------------------------------------------
 * Test2Tissue.c
 * ---------------------------------------------------------------------
 * Functional self-test for the Tissue module (Step2Tissue.c/.h),
 * the biological analog of the original Reader/Buffer stage.
 *
 * Biological role: Tissue = Reader + Memory -> "Store and manage
 * information". A TissuePointer is a growable store of "cells" (chars)
 * with store/access/mark positions and vital-sign flags, mirroring how
 * biological tissue stores and organizes cells.
 *
 * Note: tissueLoad() automatically runs the loaded file through
 * cellTransformMem(..., TRANSLATE), so (like the rest of the pipeline)
 * it expects its input file to already be Cell-encoded with the same
 * key (STR_CompilerName). This test encodes its own fixture file first,
 * exactly like main1Cell would.
 *
 * NOTE: this is now called as testXxx(argc, argv) from Biology.c's
 * main() dispatcher (options 6-0), not run as its own program --
 * having a main() in every file was the whole problem this fixes
 * (Visual Studio reports "multiple main" if every Test*.c keeps its
 * own). Build/run it as part of the whole project, e.g.:
 *   gcc -std=gnu99 -fcommon -o biology \
 *       Biology.c Main1Cell.c Main2Tissue.c Main3Organ.c Main4System.c \
 *       Main5Body.c Step1Cell.c Step2Tissue.c Step3Organ.c \
 *       Step4System.c Step5Body.c Test1Cell.c Test2Tissue.c \
 *       Test3Organ.c Test4System.c Test5Body.c -lm
 *   ./biology 6   (Cell)   ./biology 7 (Tissue)  ./biology 8 (Organ)
 *   ./biology 9   (System) ./biology 0 (Body)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/Biology.h"
#include "../include/Step1Cell.h"
#include "../include/Step2Tissue.h"

static int failures = 0;

static void check(const char *label, int condition) {
    printf("[%s] %s\n", condition ? " OK " : "FAIL", label);
    if (!condition) failures++;
}

STEL_int testTissue(STEL_int argc, STEL_string* argv) {
    const char *plain_text = "Tissue stores cells: alpha beta gamma delta.";
    TissuePointer tissue;
    STEL_int loadSize;

    printf("=== Test2Tissue: Tissue (Reader + Memory) module ===\n\n");

    /* Fixture: a plain file, Cell-encoded the same way main1Cell does */
    {
        FILE *f = fopen("t2_plain.txt", "w");
        fputs(plain_text, f);
        fclose(f);
    }
    cellEncode("t2_plain.txt", "t2_encoded.txt", (STEL_string)STR_CompilerName);

    /* Basic lifecycle: create, check empty, add cells, check full/empty */
    tissue = tissueCreate(16);
    check("tissueCreate returns a non-NULL tissue", tissue != NULL);
    check("freshly created tissue is empty", tissueIsEmpty(tissue));
    check("freshly created tissue is not full", !tissueIsFull(tissue));

    tissueAddCell(tissue, 'A');
    tissueAddCell(tissue, 'C');
    tissueAddCell(tissue, 'G');
    tissueAddCell(tissue, 'T');
    check("tissue is no longer empty after adding cells", !tissueIsEmpty(tissue));
    check("tissueGetPosStore advanced to 4 after 4 cells", tissueGetPosStore(tissue) == 4);
    check("tissueGetCell retrieves cells in FIFO order ('A' first)",
          tissueGetCell(tissue) == 'A');
    check("tissueGetCell retrieves 'C' next", tissueGetCell(tissue) == 'C');

    tissueSetMark(tissue, tissueGetPosStore(tissue));
    check("tissueGetPosMark reflects the mark just set",
          tissueGetPosMark(tissue) == tissueGetPosStore(tissue));

    tissueFree(tissue);

    /* Loading a real (Cell-encoded) file end to end */
    tissue = tissueCreate(8); /* deliberately small, to also exercise growth */
    loadSize = tissueLoad(tissue, (STEL_string)"t2_encoded.txt");
    check("tissueLoad reports the correct decoded size",
          loadSize == (STEL_int)strlen(plain_text));
    check("tissue grew to accommodate content larger than initial capacity",
          tissueGetSize(tissue) >= loadSize);

    tissueRecover(tissue);
    {
        STEL_string content = tissueGetContent(tissue, 0);
        check("tissueLoad + tissueRecover reproduces the original plain text",
              content != NULL && strncmp(content, plain_text, strlen(plain_text)) == 0);
    }

    check("tissueChecksum is non-zero for non-empty content",
          tissueChecksum(tissue) != 0);
    check("tissueNumErrors is 0 for a clean load", tissueNumErrors(tissue) == 0);

    tissueFree(tissue);

    /* Loading a file that does not exist should fail cleanly */
    {
        TissuePointer badTissue = tissueCreate(8);
        STEL_int badLoad = tissueLoad(badTissue, (STEL_string)"t2_does_not_exist.txt");
        check("tissueLoad on a missing file returns TISSUE_ERROR",
              badLoad == TISSUE_ERROR);
        tissueFree(badTissue);
    }

    printf("\n=== %s (%d failure%s) ===\n",
           failures == 0 ? "ALL TESTS PASSED" : "SOME TESTS FAILED",
           failures, failures == 1 ? "" : "s");

    remove("t2_plain.txt");
    remove("t2_encoded.txt");

    return failures == 0 ? 0 : 1;
}
