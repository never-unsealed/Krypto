/*
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * + WICHTIG: DIESE DATEI NICHT EDITIEREN!
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */

#include <gtest/gtest.h>
#include <vector>
#include "FortunaGenerator.h"
#include "MillerRabinTest.h"

TEST(MillerRabinTestTest, Witness) {
    Integer n { "121399589920585397786647347761633" };
    Integer witness { "3" };
    EXPECT_EQ(MillerRabinTest::witness(witness, n), true);
}

TEST(MillerRabinTestTest, IsPrime) {
    PRNG* fgen = new FortunaGenerator;  // Pseudo random number generator
    const int s = 100;                  // Quality factor

    std::vector<Integer> primeBuffer {
        Integer( "249236727971798483921811058804041930204535828339" ),
        Integer( "167376539261987877304745111417636335723391598799" ),
        Integer( "207241657224647904979925877995751997643166331621" ),
        Integer( "203681457354349143050092586190992078578082388049" ),
        Integer( "697694207308110472291028681461" ),
        Integer( "27091882327650834855566547011222266014790734797710551975087512778337806115447543" ),
        Integer( "46903886933782520466507618195790616853953273550403946749423255995939973881120037" ),
        Integer( "83627235236709618705980501848715833723933430392252715617842893177316997495853821" ),
        Integer( "557023164688608208546433006199011453898440769083756253282157" ),
        Integer( "8220777" ),
        Integer( "20138200" ),
    };

    std::vector<bool> primeResult {
        true, true, false, false, true, true, true, true, false, false, false
    };

    EXPECT_EQ(primeBuffer.size(), primeResult.size()) << "vectors must be of equal size";

    for(int i = 0; i < primeBuffer.size(); ++i) {
        EXPECT_EQ(MillerRabinTest::isPrime(fgen, primeBuffer[i], s), primeResult[i])
            << "prime test for the number at index " << i << " doesn't return expected result";
    }
}

TEST(MillerRabinTestTest, ModExp) {
    Integer b { "13" };
    int e { 57 };
    EXPECT_EQ(MillerRabinTest::exp(b, e), Integer( "3124432031290254610011894949223517352998211575328796815860858733" ));
}

TEST(MillerRabinTestTest, SearchSqrt) {
    std::vector<Integer> intBuffer {
        Integer( "11944789308433963188362869029471041566864144" ),
        Integer( "35834367925301889565088607088413124700592432" ),
        Integer( "82459222190377411904521629217281504933358750190521" ),
        Integer( "61374642523762897753653566439246517537628495634413694129" ),
    };

    std::vector<bool> sqrtResult1 {
        true, false, true, true,
    };

    std::vector<Integer> sqrtResult2 {
        Integer( "3456123450982901911012" ),
        Integer( "5986181414332670353062" ),
        Integer( "9080706040302010123456789" ),
        Integer( "7834196992912732326199111623" ),
    };

    EXPECT_EQ(intBuffer.size(), sqrtResult1.size()) << "vectors must be of equal size";

    for(int i = 0; i < intBuffer.size(); ++i) {
        Integer ret;

        EXPECT_EQ(MillerRabinTest::searchSqrt(intBuffer[i], ret), sqrtResult1[i])
            << "sqrt test for the number at index " << i << " doesn't return expected result";
        EXPECT_EQ(ret, sqrtResult2[i])
            << "sqrt test for the number at index " << i << " doesn't return expected result";
    }
}

TEST(MillerRabinTestTest, SearchRoot) {
    std::vector<Integer> intBuffer {
        Integer( "433769751949605918939094356724813641" ),
        Integer( "5639006775344876946208226637422577333" ),
        Integer( "70462039047496669316177512730246460325447" ),
        Integer( "70462039047496669316177512730246460325447" ),
    };

    std::vector<int> kBuffer {
        7, 7, 5, 9
    };

    std::vector<bool> rootResult1 {
        true, false, false, true,
    };

    std::vector<Integer> rootResult2 {
        Integer( "123321" ),
        Integer( "177898" ),
        Integer( "147771622" ),
        Integer( "34567" ),
    };

    for(int i = 0; i < intBuffer.size(); ++i) {
        Integer ret;
        
        EXPECT_EQ(MillerRabinTest::searchRoot(intBuffer[i], kBuffer[i], ret), rootResult1[i])
            << "root test for the number at index " << i << " doesn't return expected result";
        EXPECT_EQ(ret, rootResult2[i])
            << "root test for the number at index " << i << " doesn't return expected result";
    }
}

TEST(MillerRabinTestTest, IntegerPower) {
    std::vector<Integer> intBuffer {
        Integer( "3124432031290254610011894949223517352998211575328796815860858733" ),
        Integer( "11944789308433963188362869029471041566864144" ),
        Integer( "35834367925301889565088607088413124700592432" ),
        Integer( "82459222190377411904521629217281504933358750190521" ),
        Integer( "61374642523762897753653566439246517537628495634413694129" ),
    };

    std::vector<Integer> baseBuffer {
        Integer( "1461920290375446110677" ),
        Integer( "3456123450982901911012" ),
        Integer( "1" ),
        Integer( "9080706040302010123456789" ),
        Integer( "7834196992912732326199111623" ),
    };

    std::vector<int> expBuffer {
        3, 2, 2, 2, 2
    };

    std::vector<bool> retBuffer {
        true, true, false, true, true,
    };

    for(int i = 0; i < intBuffer.size(); ++i) {
        Integer b;
        unsigned int e;
        bool res = MillerRabinTest::isIntegerPower(intBuffer[i], b, e);

        EXPECT_EQ(res, retBuffer[i]);
        if(res) {
            EXPECT_EQ(b, baseBuffer[i]) << "bases at index " << i << " don't match";
            EXPECT_EQ(e, expBuffer[i]) << "exponents at index " << i << "don't match";
        }
    }
}

TEST(FortunaTest, RandBytes) {
    byte seed[] { 0x7a, 0xb2, 0xaf, 0x1a, 0x45, 0x61, 0xab, 0xf8 };
    byte rand[1000] { 0 };
    byte expected[1000] {
0x7f, 0xf2, 0x74, 0x8b, 0xaf, 0x86, 0xb2, 0x62, 0x66, 0x25, 0x4a, 0x80, 0x9d, 0xfe, 0x73, 0x4f, 
0x7e, 0xc4, 0x3a, 0x32, 0x86, 0xef, 0x22, 0x17, 0xb9, 0xe9, 0x6a, 0x45, 0xad, 0x72, 0x60, 0x99, 
0x8e, 0xc4, 0x78, 0xe7, 0xa6, 0xea, 0xcb, 0x8e, 0x91, 0xc8, 0x33, 0x58, 0xd5, 0x97, 0x78, 0xef, 
0x26, 0x5b, 0x2d, 0x65, 0xe0, 0x5b, 0x59, 0xbf, 0xc2, 0x44, 0x92, 0x7c, 0xe3, 0x49, 0x28, 0xa9, 
0x2b, 0xa5, 0x5a, 0xe2, 0xba, 0xfb, 0xbf, 0x51, 0xec, 0x55, 0x7a, 0x46, 0xc3, 0x0c, 0xad, 0x67, 
0xac, 0x5b, 0xd1, 0x2e, 0x21, 0xb4, 0x59, 0xfa, 0xba, 0x56, 0x2f, 0x09, 0x74, 0x0c, 0x93, 0x46, 
0x4c, 0x89, 0xcb, 0xcf, 0xfe, 0x39, 0x7b, 0x20, 0xac, 0x55, 0x67, 0x71, 0xe5, 0xeb, 0x62, 0x49, 
0xe1, 0x7b, 0xb7, 0x08, 0xc6, 0x62, 0xdd, 0x8a, 0x4a, 0x96, 0x06, 0x0a, 0xbb, 0xc4, 0xdc, 0x88, 
0x9f, 0xfe, 0x03, 0x7f, 0x8d, 0x71, 0x73, 0x8e, 0x2c, 0xf6, 0x07, 0xba, 0xdd, 0xea, 0xc9, 0xdd, 
0x8a, 0x03, 0xca, 0xf9, 0x30, 0x54, 0x93, 0x28, 0x91, 0x0d, 0xd4, 0xc6, 0x4e, 0x93, 0x48, 0x81, 
0x38, 0x17, 0xa0, 0x94, 0x32, 0x39, 0x27, 0x76, 0x87, 0x6d, 0xb6, 0x35, 0x0d, 0x5f, 0x1a, 0xe7, 
0xab, 0xa2, 0x8b, 0x57, 0xb8, 0xc1, 0x42, 0x75, 0x7a, 0xbb, 0x76, 0x04, 0xeb, 0x55, 0xc9, 0xc5, 
0x49, 0x3f, 0x1b, 0x1d, 0xda, 0x16, 0xed, 0xa0, 0x67, 0x9a, 0xc0, 0x19, 0x9f, 0xdf, 0xfc, 0x54, 
0x46, 0xeb, 0x08, 0x42, 0xb2, 0xde, 0xa7, 0xf5, 0x8e, 0x8b, 0x06, 0x05, 0xb1, 0xe3, 0x44, 0xb4, 
0xc2, 0x6a, 0x7f, 0x7f, 0x87, 0x7d, 0xbc, 0x66, 0x5a, 0x43, 0x24, 0x9a, 0xf5, 0xa0, 0x1a, 0x7f, 
0xe6, 0xa4, 0x64, 0xb4, 0x2c, 0x77, 0x5d, 0x2b, 0x8d, 0x6c, 0x33, 0x85, 0x09, 0x20, 0x0d, 0x47, 
0x68, 0x28, 0xdd, 0x73, 0xb7, 0xb0, 0x30, 0xdb, 0x09, 0xb1, 0x3c, 0x31, 0xf4, 0x76, 0x33, 0xcf, 
0x6b, 0x04, 0xb5, 0x50, 0x6b, 0x19, 0x31, 0xce, 0x3c, 0x53, 0x58, 0xd6, 0x96, 0x8e, 0x25, 0xe5, 
0xdf, 0xdf, 0xae, 0xdc, 0x93, 0x7f, 0xb3, 0x5b, 0xbb, 0x7e, 0x5f, 0xa2, 0xbc, 0xa3, 0xa7, 0x52, 
0x1d, 0xd9, 0x33, 0xf4, 0xaf, 0x5c, 0x7f, 0x1a, 0x50, 0x25, 0x78, 0x2c, 0x5f, 0x44, 0xbb, 0x70, 
0x8e, 0x34, 0x0a, 0x7d, 0xd7, 0xaa, 0xf4, 0xc9, 0xb3, 0x5f, 0xaa, 0xa6, 0xab, 0xf6, 0x92, 0x21, 
0x8b, 0x2d, 0xb0, 0x8e, 0x8c, 0x81, 0x5e, 0x9a, 0x4a, 0xa5, 0x0f, 0x96, 0x74, 0x98, 0x51, 0xbb, 
0x82, 0x60, 0xc4, 0xe9, 0xae, 0x1e, 0xfc, 0xdd, 0x6c, 0xcd, 0x22, 0x38, 0xce, 0x3a, 0xb3, 0xd5, 
0x11, 0x61, 0xa6, 0xa0, 0x6f, 0xbf, 0x4b, 0xf3, 0x99, 0x9a, 0x30, 0x8f, 0xee, 0x1d, 0xef, 0xe2, 
0x01, 0x84, 0x00, 0x39, 0x5d, 0xa7, 0xda, 0xd3, 0x6a, 0x0f, 0x0b, 0x76, 0x56, 0x0a, 0x8a, 0x19, 
0xd6, 0xb8, 0x26, 0xe2, 0xce, 0xc1, 0x30, 0xd7, 0x71, 0xb2, 0xf1, 0x53, 0xf0, 0xcc, 0x0b, 0x2e, 
0x25, 0x84, 0x6b, 0x8c, 0xf1, 0x69, 0x5a, 0x2e, 0xc4, 0x98, 0x10, 0x20, 0xdd, 0x31, 0x1f, 0xa7, 
0x52, 0xf1, 0x65, 0xc0, 0x32, 0x93, 0x83, 0x73, 0xe3, 0x07, 0x17, 0x5e, 0xe8, 0x5f, 0x0e, 0x35, 
0x8d, 0x21, 0x53, 0x83, 0xfe, 0x2c, 0xaf, 0xdc, 0xd3, 0xab, 0xdf, 0x4b, 0xa2, 0x8f, 0x26, 0x0f, 
0x4c, 0xe5, 0x76, 0x33, 0x51, 0x0d, 0x65, 0xf3, 0x9a, 0xb6, 0x85, 0x19, 0x5a, 0x3f, 0xf7, 0x9c, 
0x93, 0x9c, 0xc4, 0x66, 0x51, 0xd3, 0xb1, 0x29, 0x28, 0xdb, 0xff, 0x61, 0xaa, 0xfe, 0xa1, 0xcb, 
0x1a, 0x40, 0x0e, 0xca, 0xc0, 0x4b, 0x1f, 0x5e, 0x3b, 0x93, 0x0e, 0x16, 0xf3, 0xf0, 0x95, 0xf4, 
0x4a, 0x35, 0x06, 0xb9, 0x13, 0x45, 0x33, 0x24, 0x53, 0xd1, 0xe9, 0x76, 0x08, 0x74, 0x49, 0x15, 
0x30, 0x4c, 0x8c, 0x47, 0x69, 0x42, 0x2b, 0x1d, 0xc5, 0x98, 0xcf, 0x7c, 0x80, 0xa0, 0xcb, 0x6a, 
0x96, 0x9a, 0xc6, 0x0a, 0xf7, 0x80, 0x6e, 0x19, 0x68, 0xaf, 0xe7, 0x47, 0x07, 0xb2, 0x75, 0x29, 
0xb1, 0x28, 0x3b, 0x03, 0xe0, 0x06, 0xb8, 0xc5, 0x22, 0x9e, 0x3e, 0x99, 0x3e, 0x25, 0x6b, 0x1f, 
0x84, 0xcf, 0x86, 0xeb, 0xf8, 0x3e, 0x6d, 0x53, 0xad, 0x9c, 0xbe, 0xc6, 0x37, 0xc7, 0x6f, 0x18, 
0xfc, 0x98, 0xa1, 0x11, 0xeb, 0x74, 0x0b, 0x62, 0xe5, 0xee, 0xb1, 0x05, 0x46, 0x8c, 0x4d, 0xf8, 
0x78, 0x7a, 0x0b, 0xb3, 0x7e, 0x93, 0x60, 0x7c, 0x16, 0xa5, 0x5c, 0x9e, 0x8b, 0x88, 0x8e, 0xfe, 
0xb6, 0x42, 0x46, 0xfa, 0x30, 0x12, 0x4a, 0x7a, 0xe8, 0xb2, 0xa5, 0xb8, 0xd3, 0x14, 0x6d, 0x78, 
0xd1, 0x73, 0xc8, 0xe0, 0x37, 0xed, 0xfb, 0xd2, 0xdd, 0x30, 0xfd, 0xf3, 0xc9, 0x78, 0x14, 0xd5, 
0x50, 0xcf, 0xc5, 0x43, 0x06, 0x4f, 0xe1, 0x32, 0x12, 0x3a, 0x66, 0x32, 0x24, 0x79, 0xdc, 0x75, 
0xc9, 0x07, 0xbb, 0xf3, 0x45, 0xe7, 0x4c, 0x9e, 0xc5, 0x0c, 0xdc, 0x39, 0x7d, 0x79, 0x39, 0xf1, 
0x4d, 0x94, 0x39, 0x77, 0x07, 0x0a, 0xdd, 0x6d, 0x9c, 0x7d, 0xf2, 0x36, 0x3f, 0x24, 0xa0, 0xd2, 
0x56, 0xea, 0x97, 0x93, 0xe5, 0x6f, 0x67, 0x74, 0xc6, 0x93, 0xb7, 0xec, 0x1c, 0x50, 0xc6, 0x08, 
0x88, 0x49, 0xad, 0x76, 0x4f, 0xdd, 0xae, 0xb4, 0x12, 0xba, 0xf4, 0x08, 0x0d, 0xb2, 0xac, 0x10, 
0x00, 0xb6, 0x74, 0x91, 0x3c, 0xc1, 0x4f, 0x7c, 0x6e, 0x12, 0x21, 0xb7, 0x50, 0x9f, 0x03, 0x6f, 
0x0e, 0x3f, 0xae, 0xd1, 0xe1, 0xcb, 0x9b, 0x6b, 0xc1, 0x49, 0x3f, 0x67, 0x03, 0x96, 0x81, 0x70, 
0x0d, 0x75, 0xfc, 0x02, 0x6f, 0x0d, 0xfa, 0x49, 0xdf, 0x55, 0x52, 0xd0, 0x97, 0xe8, 0xf7, 0x82, 
0xa3, 0xde, 0x99, 0x64, 0x84, 0x54, 0xb8, 0x86, 0x38, 0x44, 0xb6, 0x8d, 0x19, 0x77, 0xb7, 0xfa, 
0x70, 0xf7, 0x0e, 0x31, 0x15, 0x0e, 0x13, 0xcd, 0x10, 0xd1, 0x7e, 0x3c, 0xee, 0x50, 0xf5, 0x4c, 
0xe0, 0xd7, 0x13, 0x02, 0xa5, 0xc3, 0x2d, 0xba, 0x7d, 0x36, 0xdd, 0x02, 0xc3, 0xdc, 0x52, 0x2d, 
0x24, 0x7b, 0xa1, 0x09, 0x08, 0xdb, 0xb4, 0x9a, 0xea, 0x81, 0x19, 0x21, 0xa4, 0xa9, 0x73, 0x33, 
0x20, 0x2c, 0x1e, 0x50, 0xb4, 0x1e, 0x56, 0x64, 0xdc, 0xe1, 0xc0, 0x76, 0x7b, 0xf4, 0xae, 0xf0, 
0x0d, 0xdc, 0xad, 0x2d, 0xf6, 0x75, 0xf9, 0xf7, 0x79, 0x16, 0xa4, 0x9d, 0x6f, 0x73, 0x06, 0x36, 
0x09, 0x4b, 0x93, 0x45, 0xf8, 0xac, 0x6d, 0xd0, 0x42, 0xcb, 0x8e, 0xeb, 0xf8, 0x2a, 0x22, 0x43, 
0x59, 0xab, 0x2e, 0x78, 0xeb, 0x50, 0x30, 0xe0, 0xab, 0x14, 0x32, 0x42, 0xd7, 0x94, 0xa4, 0x10, 
0xe9, 0x19, 0x53, 0x8d, 0xa6, 0x24, 0x95, 0x2d, 0x4d, 0x5d, 0x1e, 0x48, 0x1f, 0x6e, 0xda, 0x86, 
0xb9, 0xeb, 0x03, 0x2b, 0x2d, 0x72, 0xc1, 0xdb, 0xe7, 0xe1, 0x5a, 0xd9, 0xbc, 0x2c, 0x5a, 0xae, 
0xba, 0xbb, 0x32, 0xfe, 0xe0, 0x13, 0xa4, 0x74, 0x43, 0x12, 0xf7, 0x89, 0x4c, 0x1d, 0x1e, 0x42, 
0xa0, 0x68, 0x6d, 0xf0, 0x05, 0x50, 0xad, 0xf9, 0x4f, 0x2a, 0x2e, 0xc3, 0x46, 0xc9, 0x19, 0xc0, 
0x5a, 0x4d, 0xe3, 0x6b, 0x7c, 0xac, 0x56, 0xae, 0x7a, 0x35, 0x52, 0xaa, 0x43, 0x10, 0x39, 0x85, 
0x19, 0x5c, 0xb3, 0x1d, 0x36, 0x8b, 0xb7, 0xf0
    };

    FortunaGenerator f;
    f.reseed(seed, 8);

    // getBlock must not reseed itself automaticalliy.
    f.getBlockV0(rand, 1000);

    for(int i = 0; i < 1000; ++i) {
        EXPECT_EQ(rand[i], expected[i]);
	}
}
