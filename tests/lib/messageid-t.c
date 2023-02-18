/*
**  Test suite for message identifiers.
*/

#define LIBTEST_NEW_FORMAT 1

#include "inn/libinn.h"
#include "tap/basic.h"

void testMessageIDs(bool stripspaces, bool laxsyntax);
void testDomains(void);

/*
**  Test a bunch of message-IDs that are always either bad or good, no matter
**  how lax we are.
*/
void
testMessageIDs(bool strip, bool lax)
{
    is_bool(false, IsValidMessageID(NULL, strip, lax), "bad ID 1");
    is_bool(false, IsValidMessageID("", strip, lax), "bad ID 2");
    is_bool(false, IsValidMessageID("<invalid@test", strip, lax), "bad ID 3");
    is_bool(false, IsValidMessageID("invalid@test>", strip, lax), "bad ID 4");
    is_bool(false, IsValidMessageID("<inva\177lid@test>", strip, lax),
            "bad ID 5");
    is_bool(false, IsValidMessageID("<inva lid@test>", strip, lax),
            "bad ID 6");
    is_bool(false, IsValidMessageID("<invalid@te\tst>", strip, lax),
            "bad ID 7");
    is_bool(false, IsValidMessageID("<invalid>", strip, lax), "bad ID 8");
    is_bool(false, IsValidMessageID("<inva\r\nlid@test>", strip, lax),
            "bad ID 9");
    is_bool(false, IsValidMessageID("<inva(lid@test>", strip, lax),
            "bad ID 10");
    is_bool(false, IsValidMessageID("<inva;lid@test>", strip, lax),
            "bad ID 11");
    is_bool(false, IsValidMessageID("<inva\"lid@test>", strip, lax),
            "bad ID 12");
    is_bool(false, IsValidMessageID("<inva>lid@test>", strip, lax),
            "bad ID 13");
    is_bool(false, IsValidMessageID("<inva<lid@test>", strip, lax),
            "bad ID 14");
    is_bool(false, IsValidMessageID("<>", strip, lax), "bad ID 15");
    is_bool(false, IsValidMessageID("<a@>", strip, lax), "bad ID 16");
    is_bool(
        false,
        IsValidMessageID(
            "<1234567890123456789012345678901234567890123456789012345678901234"
            "56789012345678901234567890123456789012345678901234567890123456789"
            "01234567890123456789012345678901234567890123456789012345678901234"
            "5678901234567890123456789012345678901234567890@1234567890>",
            strip, lax),
        "bad ID 17");
    is_bool(false, IsValidMessageID("<inva...lid@test>", strip, lax),
            "bad ID 18");
    is_bool(false, IsValidMessageID("<invalid@yEnc@twice@test>", strip, lax),
            "bad ID 19");
    is_bool(false, IsValidMessageID("<invalid.@test>", strip, lax),
            "bad ID 20");
    is_bool(false, IsValidMessageID("<invalid@>", strip, lax), "bad ID 21");
    is_bool(false, IsValidMessageID("<@invalid>", strip, lax), "bad ID 22");
    is_bool(false, IsValidMessageID("<invalid@test.>", strip, lax),
            "bad ID 23");
    is_bool(false, IsValidMessageID("<inva[lid@test>", strip, lax),
            "bad ID 24");
    is_bool(false, IsValidMessageID("<invalid@t[es]t>", strip, lax),
            "bad ID 25");
    is_bool(false, IsValidMessageID("<valid@[t@].[e<s].t>", strip, lax),
            "bad ID 26");

    is_bool(true, IsValidMessageID("<valid@test>", strip, lax), "good ID 1");
    is_bool(true, IsValidMessageID("<v4l.#%-{T`?*!.id@te|st>", strip, lax),
            "good ID 2");
    is_bool(true, IsValidMessageID("<a@b>", strip, lax), "good ID 3");
    is_bool(true, IsValidMessageID("<a.valid.id@testing.fr>", strip, lax),
            "good ID 4");
    is_bool(true, IsValidMessageID("<valid@[te.st]>", strip, lax),
            "good ID 5");
    is_bool(true, IsValidMessageID("<valid@[te;s@<t]>", strip, lax),
            "good ID 6");
}


/*
**  Test a few domains that are always either bad or good.  Note that the
**  IsValidDomain() function is already implicitly tested in all the tests for
**  IsValidMessageID().
*/
void
testDomains(void)
{
    is_bool(false, IsValidDomain(NULL), "bad domain 1");
    is_bool(false, IsValidDomain(""), "bad domain 2");
    is_bool(false, IsValidDomain("@test"), "bad domain 3");
    is_bool(false, IsValidDomain("inva\177lid"), "bad domain 4");
    is_bool(false, IsValidDomain("inva lid"), "bad domain 5");
    is_bool(false, IsValidDomain("inva\r\nlid"), "bad domain 6");
    is_bool(false, IsValidDomain("inva\"lid"), "bad domain 7");

    is_bool(true, IsValidDomain("test"), "good domain 1");
    is_bool(true, IsValidDomain("v4l.#%-{T`?*!.id.te|st"), "good domain 2");
    is_bool(true, IsValidDomain("[te.st]"), "good domain 3");
    is_bool(true, IsValidDomain("[te;s@<t]"), "good domain 4");
}


int
main(void)
{
    plan(4 * (26 + 6) + (7 + 4) + 2 + 5);

    /* Test several message-IDs with and without stripping spaces and lax
     * syntax. */
    testMessageIDs(true, true);
    testMessageIDs(true, false);
    testMessageIDs(false, true);
    testMessageIDs(false, false);

    /* Also test the right-hand side. */
    testDomains();

    /* Test whether stripping spaces works. */
    is_bool(true, IsValidMessageID(" \t\t <valid@test>\t  ", true, false),
            "good ID stripspaces 1");
    is_bool(false, IsValidMessageID(" \t\t <invalid@test>\t  ", false, false),
            "bad ID stripspaces 1");

    /* Test whether lax syntax works. */
    is_bool(true, IsValidMessageID("<valid@yEnc@test>", false, true),
            "good ID laxsyntax 1");
    is_bool(false, IsValidMessageID("<invalid@yEnc@test>", false, false),
            "bad ID laxsyntax 1");
    is_bool(true, IsValidMessageID("<va..lid@test>", false, true),
            "good ID laxsyntax 2");
    is_bool(false, IsValidMessageID("<inva..lid@test>", false, false),
            "bad ID laxsyntax 2");
    is_bool(false, IsValidMessageID("<invalid@te..st>", false, true),
            "bad ID laxsyntax 3");

    return 0;
}
