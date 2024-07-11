#include <gtest/gtest.h>
#include "gmock/gmock.h"

#include "RabinEncryptor.h"
#include "RabinDecryptor.h"
#include "RabinAttack.h"
#include "PublicKeyAlgorithmBox.h"
#include "RSAOracle.h"
#include "RSAEncryptor.h"
#include "RSADecryptor.h"
#include "RSAAttack.h"

TEST(PublicKey, ExtendedEuclidean1) {
    PublicKeyAlgorithmBox pkab;

    Integer a { "2987" };
    Integer b { "1279865" };
    Integer d, x, y;
    bool r;
    
    r = pkab.EEA(a, b, d, x, y);

    EXPECT_EQ(d, Integer("1"));         // 1 -> coprime
    EXPECT_EQ(x, Integer("-128972"));   // Inverse of 2987 mod 1279865
    EXPECT_EQ(y, Integer("301"));

    EXPECT_EQ(a_times_b_mod_c(a, x, b), Integer("1"));
}

TEST(PublicKey, ExtendedEuclidean2) {
    PublicKeyAlgorithmBox pkab;

    Integer a { "78845945" };
    Integer b { "2503417846" };
    Integer d, x, y;
    bool r;
    
    r = pkab.EEA(a, b, d, x, y);

    EXPECT_EQ(d, Integer("33767"));
    EXPECT_EQ(x, Integer("10573"));
    EXPECT_EQ(y, Integer("-333"));
}

TEST(PublicKey, ExtendedEuclidean3) {
    PublicKeyAlgorithmBox pkab;

    Integer a { "57913313" };
    Integer b { "173739939" };
    Integer d, x, y;
    bool r;
    
    r = pkab.EEA(a, b, d, x, y);

    EXPECT_EQ(d, Integer("57913313"));
    EXPECT_EQ(x, Integer("1"));
    EXPECT_EQ(y, Integer("0"));
}

TEST(PublicKey, ModularExponentiation1) {
    PublicKeyAlgorithmBox pkab;

    Integer a { "2" };
    Integer b { "100000" };
    Integer n { "23" };
    
    EXPECT_EQ(pkab.modularExponentation(a, b, n), Integer("12"));
}

TEST(PublicKey, ModularExponentiation2) {
    PublicKeyAlgorithmBox pkab;

    Integer a { "2343947997" };
    Integer b { "765" };
    Integer n { "111" };
    
    EXPECT_EQ(pkab.modularExponentation(a, b, n), Integer("105"));
}

TEST(PublicKey, MultInverse1) {
    PublicKeyAlgorithmBox pkab;
    
    Integer m { "279226292160650115722581212551219487007" };
    Integer x { "123456789" };
    Integer y;
    
    EXPECT_EQ(pkab.multInverse(x, m, y), true);
    EXPECT_EQ(y, Integer("272353385455894914305465547778055961739"));
    EXPECT_EQ(a_times_b_mod_c(x, y, m), Integer("1"));
}


TEST(PublicKey, MultInverse2) {
    PublicKeyAlgorithmBox pkab;
    
    Integer m { "279226292160650115722581212551219487007" };
    Integer x { "97691289525151824619390196760216553507" };
    Integer y;
    
    EXPECT_EQ(pkab.multInverse(x, m, y), true);
    EXPECT_EQ(y, Integer("258862557463978211041021544779703688829"));
    EXPECT_EQ(a_times_b_mod_c(x, y, m), Integer("1"));
}

TEST(PublicKey, MultInverse3) {
    PublicKeyAlgorithmBox pkab;
    
    Integer m { "279226292160650115722581212551219487007" };
    Integer x { "189171817297990285918833894121197777185" };
    Integer y;
    
    EXPECT_EQ(pkab.multInverse(x, m, y), true);
    EXPECT_EQ(y, Integer("61808709124418473181132227388676392263"));
    EXPECT_EQ(a_times_b_mod_c(x, y, m), Integer("1"));
}

TEST(PublicKey, MultInverse4) {
    PublicKeyAlgorithmBox pkab;
    
    Integer m { "192172622525902080249109244057747132168" };
    Integer x { "179500567634383616285796288330028857942" };
    Integer y;
    
    EXPECT_EQ(pkab.multInverse(x, m, y), false);
}

TEST(PublicKey, Witness1) {
    PublicKeyAlgorithmBox pkab;

    Integer m { "192172622525902080249109244057747132168" };
    Integer w { "179500567634383616285796288330028857942" };

    EXPECT_EQ(pkab.witness(w, m), true);
}

TEST(PublicKey, MillerRabinTest) {
    PublicKeyAlgorithmBox pkab;

    Integer z1 { "279226292160650115722581212551219487007" };
    Integer z2 { "247278711133334795867191516244139839983" };
    Integer z3 { "192172622525902080249109244057747132167" };
    Integer z4 { "177387942943728133030691912202779547031" };
    
    EXPECT_EQ(pkab.millerRabinTest(z1, 100), true);
    EXPECT_EQ(pkab.millerRabinTest(z2, 100), false);
    EXPECT_EQ(pkab.millerRabinTest(z3, 100), false);
    EXPECT_EQ(pkab.millerRabinTest(z4, 100), true);
}

