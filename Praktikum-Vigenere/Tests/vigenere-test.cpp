/*
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * + WICHTIG: DIESE DATEI NICHT EDITIEREN!
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */

#include <gtest/gtest.h>
#include "gmock/gmock.h"
#include "ClassicCipher.h"
#include "VigenereBreaker.h"
#include "VigenereCipher.h"

vector<byte> string_to_vec(const string s) {
    vector<byte> v;

    for (auto c : s) {
        if (isalpha(c)) {
            v.push_back(tolower(c) - 'a');
        }
    }

    return v;
}

string vec_to_string(const vector<byte> v, bool upper = false) {
    string s;

    for (auto x : v) {
        byte c { (byte)(x + 'a') };

        if (upper==true) {
            s += toupper(c);
        }
        else {
            s += c;
        }
    }

    return s;
}

TEST(VigenereTest, Encrypt) {
    VigenereCipher cipher;
    vector<byte> pt1 { string_to_vec("abcdefghijklmnopqrstuvwxyz") };
    vector<byte> k1 { 3 };
    vector<byte> pt2 { string_to_vec("THESUNANDTHEMANINTHEMOON") };
    vector<byte> k2 { string_to_vec("KING") };
    vector<byte> pt3 { string_to_vec("CRYPTOGRAPHYISSUPERCOOL") };
    vector<byte> k3 { string_to_vec("MATH") };
    vector<byte> pt4 { string_to_vec("angewandtekryptographie") };
    vector<byte> k4 { string_to_vec("blau") };
    vector<byte> ct;
    
    cipher.encrypt(pt1, k1, ct);
    EXPECT_EQ(vec_to_string(ct), "defghijklmnopqrstuvwxyzabc");

    cipher.encrypt(pt2, k2, ct);
    EXPECT_EQ(vec_to_string(ct, true), "DPRYEVNTNBUKWIAOXBUKWWBT");
    
    cipher.encrypt(pt3, k3, ct);
    EXPECT_EQ(vec_to_string(ct, true), "ORRWFOZYMPAFUSLBBEKJAOE");

    cipher.encrypt(pt4, k4, ct);
    EXPECT_EQ(vec_to_string(ct), "bygyxlnxupklzatihcajite");
}

TEST(VigenereTest, Decrypt) {
    VigenereCipher cipher;
    vector<byte> ct1 { string_to_vec("defghijklmnopqrstuvwxyzabc") };
    vector<byte> k1 { 3 };
    vector<byte> ct2 { string_to_vec("DPRYEVNTNBUKWIAOXBUKWWBT") };
    vector<byte> k2 { string_to_vec("KING") };
    vector<byte> ct3 { string_to_vec("ORRWFOZYMPAFUSLBBEKJAOE") };
    vector<byte> k3 { string_to_vec("MATH") };
    vector<byte> ct4 { string_to_vec("RBZVKTERMVYKPDMFUKRDAZS") };
    vector<byte> k4 { string_to_vec("rot") };
    vector<byte> pt;
    
    cipher.decrypt(ct1, k1, pt);
    EXPECT_EQ(vec_to_string(pt), "abcdefghijklmnopqrstuvwxyz");

    cipher.decrypt(ct2, k2, pt);
    EXPECT_EQ(vec_to_string(pt, true), "THESUNANDTHEMANINTHEMOON");
    
    cipher.decrypt(ct3, k3, pt);
    EXPECT_EQ(vec_to_string(pt, true), "CRYPTOGRAPHYISSUPERCOOL");

    cipher.decrypt(ct4, k4, pt);
    EXPECT_EQ(vec_to_string(pt), "angewandtekryptographie");
}

TEST(VigenereTest, Kasiski) {
    string ct {"TAIJJOHKTWVXRJBHXBRAIVSFNLBFVINWDVITATYITBSFNEREJPAIEVDEKYFVEPHFPALGFVEMHIWUZHKPEFAULEGIEOEQEIQEGCVWFAAMQNZHZAGKOLXSLLRCGATVZEWRZOHMAELLXFKJYLODMSMRFVGXRGTAREIQNTHZOHAAELIVAGOAFEBVOPSKPEBMGWRMAEKEHFKPILSLJJXCKAEKIFCSEIWMAGDUMAMHGZOULVUSFAPIRBSVWNEYRNEPTZUELIEBHXCFCRLEFNTAERDEKAXMGTMVJUMWYMNMHVGDHAIQSXTYMYWEJMROEKWBXRVIDHUKKAKEWCLEYDQSMABMSFAUMIGTYMMMEELTHBVUOKETWSMLPBHTNRVYHTYMRDIELOYMZATTKVEHTTZATAEZASNEZVAEIWMAGDUMAMHJQTNAKQOGMFATEIWMAGDUMAMHGZOULVUSBNSWODSJMEFTFJEIRVAEGTVLIGTYMFHLCWWBNXUAGNVZBEATSTHPCIYTNUSIELNPIMEFZWAIKMTHPCIYTNUTIOEFZBEATSTHPCIYTNUTIOEZVKHEKKTAIJNOKMRBHTSKPETDMINMAXMTAAKMVXNNQTAOLBLHOBQNZAKBHXAEAWXRPWUNSLILEYBVOPWYMNROLPAXSFTVXDKPEIRFJLXMLVFHRKCNTTVTYAONMVXRUCRBNXIGTMVBHXRVQSGOFVELTRVDBNXITROLZEEBFETHTVTLROLBHTTKPILGIWUICRVBXKZTLXDFZTAAKWNXCRVBXSRDEWIESOXTTEHTTPWUAAMMTHDFLUKIEOAVTLILILRGILTFEOKKFCTMHVATTTLAOYERKHZRFCPMHRBCHMVAIGTFYUXSKQOGIWIGKOLXILACQVXAJQTLTRVDROLPAOEKWKGONBHTTJWYHUNQLENFBMTKVINNNEMEWEUUOOEJIVBNXQTLIDQLTRCGYHUUWNHTNINMTFEALTVIMHVVUADIEOAGACZETDPLETDXZONPUMAWEIVOKDFGONWRVTMOKZYMOBQLEACQVXEEMMRGIWUIUETELSPWUTRVTOLIEOAGDUMSXRRBELIEKEMHRBTXNUAOGLPBOKEZVFHRTMIMAELRXMFDEPHRBEOEIXOMEEBITLPWUAAUNOKMRSIGGKPRXAKAAZAZVSMIK"};
    vector<byte> ctv { string_to_vec(ct) };

    string ct2 {"JAKXQ SWECW MMJBK TQMCM LWCXJ BNEWS XKRBO IAOBI NOMLJ GUIMH YTACF ICVOE BGOVC WYRCV KXJZV SMRXY VPOVB UBIJH OVCVK RXBOE ASZVR AOXQS WECVO QJHSG ROXWJ MCXQF OIRGZ VRAOJ RJOMB DBMVS CIESX MBDBM VSKRM GYFHA KXQSW ECWME UWXHD QDMXB KPUCN HWIWF NFCKA SKXNF DLJBY RNOBI YFSQN HRIYV IWRQS WCGKC BHRVN SSWYF SQNTS ZNWCT AWWIB SFIWW CTAWW IWWXI RGKRN LZIAW WIWHK PNFBS ASVIE SXMBD BMVSK RMGYC NGKPU CNHWI WFNFC KASKX NFDLJ BYRNO BIYFS QNHRI NBQMW SOVBO IWCVB INWCT AWWIO WFIRG ZVRAO WNJOR RGZVR AORRB OMBDB MVSOP NJORR GZVRA OXQWB XNSXM BDBMV SPMOH OIWWC TAWWI"};
    vector<byte> ctv2 { string_to_vec(ct2) };

    string ct3 {"CVJTNAFENMCDMKBXFSTKLHGSOJWHOFUISFYFBEXEINFIMAYSSDYYIJNPWTOKFRHWVWTZFXHLUYUMSGVDURBWBIVXFAFMYFYXPIGBHWIFHHOJBEXAUNFIYLJWDKNHGAOVBHHGVINAULZFOFUQCVFBYNFTYGMMSVGXCFZFOKQATUIFUFERQTEWZFOKMWOJYLNZBKSHOEBPNAYTFKNXLBVUAXCXUYYKYTFRHRCFUYCLUKTVGUFQBESWYSSWLBYFEFZVUWTRLLNGIZGBMSZKBTNTSLNNMDPMYMIUBVMTLOBJHHFWTJNAUFIZMBZLIVHMBSUWLBYFEUYFUFENBRVJVKOLLGTVUZUAOJNVUWTRLMBATZMFSSOJQXLFPKNAULJCIOYVDRYLUJMVMLVMUKBTNAMFPXXJPDYFIJFYUWSGVIUMBWSTUXMSSNYKYDJMCGASOUXBYSMCMEUNFJNAUFUYUMWSFJUKQWSVXXUVUFFBPWBCFYLWFDYGUKDRYLUJMFPXXEFZQXYHGFLACEBJBXQSTWIKNMORNXCJFAIBWWBKCMUKIVQTMNBCCTHLJYIGIMSYCFVMURMAYOBJUFVAUZINMATCYPBANKBXLWJJNXUJTWIKBATCIOYBPPZHLZJJZHLLVEYAIFPLLYIJIZMOUDPLLTHVEVUMBXPIBBMSNSCMCGONBHCKIVLXMGCRMXNZBKQHODESYTVGOUGTHAGRHRMHFREYIJIZGAUNFZIYZWOUYWQZPZMAYJFJIKOVFKBTNOPLFWHGUSYTLGNRHBZSOPMIYSLWIKBANYUOYAPWZXHVFUQAIATYYKYKPMCEYLIRNPCDMEIMFGWVBBMUPLHMLQJWUGSKQVUDZGSYCFBSWVCHZXFEXXXAQROLYXPIUKYHMPNAYFOFHXBSWVCHZXFEXXXAIRPXXGOVHHGGSVNHWSFJUKNZBESHOKIRFEXGUFVKOLVJNAYIVVMMCGOFZACKEVUMBATVHKIDMVXBHLIVWTJAUFFACKHCIKSFPKYQNWOLUMYVXYYKYAOYYPUKXFLMBQOFLACKPWZXHUFJYGZGSTYWZGSNBBWZIVMNZXFIYWXWBKBAYJFTIFYKIZMUIVZDINLFFUVRGSSBUGNGOPQAILIFOZBZFYUWHGIRHWCFIZMWYSUYMAUDMIYVYAWVNAYTFEYYCLPWBBMVZZHZUHMRWXCFUYYVIENFHPYSMKBTMOIZWAIXZFOLBSMCHHNOJKBMBATZXXJSSKNAULBJCLFWXDSUYKUCIOYJGFLMBWHFIWIXSFGXCZBMYMBWTRGXXSHXYKZGSDSLYDGNBXHAUJBTFDQCYTMWNPWHOFUISMIFFVXFSVFRNA"};
    vector<byte> ctv3 { string_to_vec(ct3) };

    VigenereBreaker vb;

    EXPECT_EQ(vb.kasiskiTest(ctv, 3), 5);
    EXPECT_EQ(vb.kasiskiTest(ctv2, 4), 4);
    EXPECT_EQ(vb.kasiskiTest(ctv3, 3), 6);
}

TEST(VigenereTest, CoincidenceIndex) {
    using namespace ::testing;
    VigenereBreaker vb;

    string alice {"Suddenly she came upon a little three-legged table, all made of solid glass; there was nothing on it except a tiny golden key, and Alice’s first thought was that it might belong to one of the doors of the hall; but, alas! either the locks were too large, or the key was too small, but at any rate it would not open any of them. However, on the second time round, she came upon a low curtain she had not noticed before, and behind it was a little door about fifteen inches high: she tried the little golden key in the lock, and to her great delight it fitted!"};

    string geheim {"Vxgghqob vkh fdph xsrq d olwwoh wkuhh-ohjjhg wdeoh, doo pdgh ri vrolg jodvv; wkhuh zdv qrwklqj rq lw hafhsw d wlqb jroghq nhb, dqg Dolfh’v iluvw wkrxjkw zdv wkdw lw pljkw ehorqj wr rqh ri wkh grruv ri wkh kdoo; exw, dodv! hlwkhu wkh orfnv zhuh wrr odujh, ru wkh nhb zdv wrr vpdoo, exw dw dqb udwh lw zrxog qrw rshq dqb ri wkhp. Krzhyhu, rq wkh vhfrqg wlph urxqg, vkh fdph xsrq d orz fxuwdlq vkh kdg qrw qrwlfhg ehiruh, dqg ehklqg lw zdv d olwwoh grru derxw iliwhhq lqfkhv kljk: vkh wulhg wkh olwwoh jroghq nhb lq wkh orfn, dqg wr khu juhdw gholjkw lw ilwwhg!"};

    EXPECT_THAT(vb.coincidenceIndex(string_to_vec(alice)), FloatNear(0.0707, 0.002));
    EXPECT_THAT(vb.coincidenceIndex(string_to_vec(geheim)), FloatNear(0.0707, 0.002));
}

TEST(VigenereTest, CoincidenceTest1) {
    VigenereBreaker vb;

    string geheim {"Vxgghqob vkh fdph xsrq d olwwoh wkuhh-ohjjhg wdeoh, doo pdgh ri vrolg jodvv; wkhuh zdv qrwklqj rq lw hafhsw d wlqb jroghq nhb, dqg Dolfh’v iluvw wkrxjkw zdv wkdw lw pljkw ehorqj wr rqh ri wkh grruv ri wkh kdoo; exw, dodv! hlwkhu wkh orfnv zhuh wrr odujh, ru wkh nhb zdv wrr vpdoo, exw dw dqb udwh lw zrxog qrw rshq dqb ri wkhp. Krzhyhu, rq wkh vhfrqg wlph urxqg, vkh fdph xsrq d orz fxuwdlq vkh kdg qrw qrwlfhg ehiruh, dqg ehklqg lw zdv d olwwoh grru derxw iliwhhq lqfkhv kljk: vkh wulhg wkh olwwoh jroghq nhb lq wkh orfn, dqg wr khu juhdw gholjkw lw ilwwhg!"};
    
    // Key len of 1 should alway be true...
    EXPECT_EQ(vb.coincidenceTest(string_to_vec(geheim), 1), true);
    EXPECT_EQ(vb.coincidenceTest(string_to_vec(geheim), 2), true);
    EXPECT_EQ(vb.coincidenceTest(string_to_vec(geheim), 3), true);
    EXPECT_EQ(vb.coincidenceTest(string_to_vec(geheim), 4), true);
}

TEST(VigenereTest, CoincidenceTest2) {
    VigenereBreaker vb;

    string geheim {"Sflfinwg ule nioi uawp e ltbvpe eptie-wmikeo bcflp, inp mllg sf dwnmd rtcws; epgve hiu roepkrg zv kx eikgtt l bkry rwnhey sgc, ayl Cpinm’u jicav xhzcilt hiu xhlb kx mtojx bptqrg ew qre zn vle owqvs zn vle sinp; bfb, cpad! mkxhpz vle wweos hmti tzw nerrm, qv tsm miy hiu xoz aoelw, jwx ae ipc rlbg mt hwwpd ywv sppv cry zn vlex. Pqaegmt, sn epg wenwph ttug vofvf, whp kcqe fxqr a wwy gucbcmn dpg lao vqx nzbkgeo jgjocm, crd mmjmno qv aad i nmtetg hozz cfofb hmfemgr iykjis sqil: ssm vvipl vle wqvxlp oqpdpv miy tv vle wweo, ayl vs hpz ivelb filtojx ie nkxtpl!"};
    
    EXPECT_EQ(vb.coincidenceTest(string_to_vec(geheim), 3, 0.058), false);
    EXPECT_EQ(vb.coincidenceTest(string_to_vec(geheim), 4, 0.058), false);
    EXPECT_EQ(vb.coincidenceTest(string_to_vec(geheim), 5, 0.058), true);
    EXPECT_EQ(vb.coincidenceTest(string_to_vec(geheim), 6, 0.058), false);
    EXPECT_EQ(vb.coincidenceTest(string_to_vec(geheim), 7, 0.058), false);
}

TEST(VigenereTest, CoincidenceTest3) {
    VigenereBreaker vb;

    string geheim {"TAIJJOHKTWVXRJBHXBRAIVSFNLBFVINWDVITATYITBSFNEREJPAIEVDEKYFVEPHFPALGFVEMHIWUZHKPEFAULEGIEOEQEIQEGCVWFAAMQNZHZAGKOLXSLLRCGATVZEWRZOHMAELLXFKJYLODMSMRFVGXRGTAREIQNTHZOHAAELIVAGOAFEBVOPSKPEBMGWRMAEKEHFKPILSLJJXCKAEKIFCSEIWMAGDUMAMHGZOULVUSFAPIRBSVWNEYRNEPTZUELIEBHXCFCRLEFNTAERDEKAXMGTMVJUMWYMNMHVGDHAIQSXTYMYWEJMROEKWBXRVIDHUKKAKEWCLEYDQSMABMSFAUMIGTYMMMEELTHBVUOKETWSMLPBHTNRVYHTYMRDIELOYMZATTKVEHTTZATAEZASNEZVAEIWMAGDUMAMHJQTNAKQOGMFATEIWMAGDUMAMHGZOULVUSBNSWODSJMEFTFJEIRVAEGTVLIGTYMFHLCWWBNXUAGNVZBEATSTHPCIYTNUSIELNPIMEFZWAIKMTHPCIYTNUTIOEFZBEATSTHPCIYTNUTIOEZVKHEKKTAIJNOKMRBHTSKPETDMINMAXMTAAKMVXNNQTAOLBLHOBQNZAKBHXAEAWXRPWUNSLILEYBVOPWYMNROLPAXSFTVXDKPEIRFJLXMLVFHRKCNTTVTYAONMVXRUCRBNXIGTMVBHXRVQSGOFVELTRVDBNXITROLZEEBFETHTVTLROLBHTTKPILGIWUICRVBXKZTLXDFZTAAKWNXCRVBXSRDEWIESOXTTEHTTPWUAAMMTHDFLUKIEOAVTLILILRGILTFEOKKFCTMHVATTTLAOYERKHZRFCPMHRBCHMVAIGTFYUXSKQOGIWIGKOLXILACQVXAJQTLTRVDROLPAOEKWKGONBHTTJWYHUNQLENFBMTKVINNNEMEWEUUOOEJIVBNXQTLIDQLTRCGYHUUWNHTNINMTFEALTVIMHVVUADIEOAGACZETDPLETDXZONPUMAWEIVOKDFGONWRVTMOKZYMOBQLEACQVXEEMMRGIWUIUETELSPWUTRVTOLIEOAGDUMSXRRBELIEKEMHRBTXNUAOGLPBOKEZVFHRTMIMAELRXMFDEPHRBEOEIXOMEEBITLPWUAAUNOKMRSIGGKPRXAKAAZAZVSMIK"};
    
    // Key len of 1 should alway be true...
    EXPECT_EQ(vb.coincidenceTest(string_to_vec(geheim), 3, 0.057), false);
    EXPECT_EQ(vb.coincidenceTest(string_to_vec(geheim), 4, 0.057), false);
    EXPECT_EQ(vb.coincidenceTest(string_to_vec(geheim), 5, 0.057), true);
    EXPECT_EQ(vb.coincidenceTest(string_to_vec(geheim), 6, 0.057), false);
    EXPECT_EQ(vb.coincidenceTest(string_to_vec(geheim), 7, 0.057), false);
}

TEST(VigenereTest, MutualCoindex) {
    VigenereBreaker vb;

    string geheim {"Sflfinwg ule nioi uawp e ltbvpe eptie-wmikeo bcflp, inp mllg sf dwnmd rtcws; epgve hiu roepkrg zv kx eikgtt l bkry rwnhey sgc, ayl Cpinm’u jicav xhzcilt hiu xhlb kx mtojx bptqrg ew qre zn vle owqvs zn vle sinp; bfb, cpad! mkxhpz vle wweos hmti tzw nerrm, qv tsm miy hiu xoz aoelw, jwx ae ipc rlbg mt hwwpd ywv sppv cry zn vlex. Pqaegmt, sn epg wenwph ttug vofvf, whp kcqe fxqr a wwy gucbcmn dpg lao vqx nzbkgeo jgjocm, crd mmjmno qv aad i nmtetg hozz cfofb hmfemgr iykjis sqil: ssm vvipl vle wqvxlp oqpdpv miy tv vle wweo, ayl vs hpz ivelb filtojx ie nkxtpl!"};
    
    EXPECT_EQ(vb.mutualCoinIndex(string_to_vec(geheim), 5, 0, 1), 11);
    EXPECT_EQ(vb.mutualCoinIndex(string_to_vec(geheim), 5, 0, 2), 8);
    EXPECT_EQ(vb.mutualCoinIndex(string_to_vec(geheim), 5, 0, 3), 2);
    EXPECT_EQ(vb.mutualCoinIndex(string_to_vec(geheim), 5, 0, 4), 4);

    EXPECT_EQ(vb.mutualCoinIndex(string_to_vec(geheim), 5, 1, 0), 15);
    EXPECT_EQ(vb.mutualCoinIndex(string_to_vec(geheim), 5, 1, 2), 23);
    EXPECT_EQ(vb.mutualCoinIndex(string_to_vec(geheim), 5, 1, 3), 17);
    EXPECT_EQ(vb.mutualCoinIndex(string_to_vec(geheim), 5, 1, 4, 0.062), 19);

    EXPECT_EQ(vb.mutualCoinIndex(string_to_vec(geheim), 5, 2, 0), 18);
    EXPECT_EQ(vb.mutualCoinIndex(string_to_vec(geheim), 5, 2, 1), 3);
    EXPECT_EQ(vb.mutualCoinIndex(string_to_vec(geheim), 5, 2, 3), 20);
    EXPECT_EQ(vb.mutualCoinIndex(string_to_vec(geheim), 5, 2, 4), 22);

    EXPECT_EQ(vb.mutualCoinIndex(string_to_vec(geheim), 5, 3, 0), 24);
    EXPECT_EQ(vb.mutualCoinIndex(string_to_vec(geheim), 5, 3, 1), 9);
    EXPECT_EQ(vb.mutualCoinIndex(string_to_vec(geheim), 5, 3, 2), 6);
    EXPECT_EQ(vb.mutualCoinIndex(string_to_vec(geheim), 5, 3, 4), 2);

    EXPECT_EQ(vb.mutualCoinIndex(string_to_vec(geheim), 5, 4, 0), 22);
    EXPECT_EQ(vb.mutualCoinIndex(string_to_vec(geheim), 5, 4, 1, 0.062), 7);
    EXPECT_EQ(vb.mutualCoinIndex(string_to_vec(geheim), 5, 4, 2), 4);
    EXPECT_EQ(vb.mutualCoinIndex(string_to_vec(geheim), 5, 4, 3), 24);

    EXPECT_EQ(vb.mutualCoinIndex(string_to_vec(geheim), 5, 1, 4), -1);
    EXPECT_EQ(vb.mutualCoinIndex(string_to_vec(geheim), 5, 4, 1), -1);
}
