#include <gtest/gtest.h>
#include <vector>
#include "BlumBlumShubGenerator.h" // reexports integer.h

class BbsTest: public ::testing::Test {
// Fixture members should be accessable from sub classes.
protected:
    void SetUp() override {
        Integer n { "192649" };
        Integer s0 { "101355" };
        bbs = new BlumBlumShubGenerator { n, s0 };

        Integer p { "1252910265243849922375596598575099209083498535192739493227403" };
        Integer q { "1476222059624949757818143837507324048590620075519516306265283" };
        Integer s { "42" };
        bbs2 = new BlumBlumShubGenerator { p * q, s };
    }

    void TearDown() override {
        delete bbs;
        delete bbs2;
        bbs = nullptr;
        bbs2 = nullptr;
    }

    BlumBlumShubGenerator* bbs = nullptr;
    BlumBlumShubGenerator* bbs2 = nullptr;
};

TEST_F(BbsTest, GetBit) {
    // See Kryptographische Protokolle, Lerneinheit 1: Zufallszahlengeneratoren, p. 59.
    std::vector<bool> bitSequence {1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 0};
    // See Prakikum-Cryptopp, Aufgabe 6.c
    std::vector<bool> bitSequence2 {0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0};
    
    int counter = 0;
    for(auto v : bitSequence) {
        EXPECT_EQ(bbs->getBit(), v) << "BitSequence and getBit() differ at index " << counter;
        counter++;
    }

    counter = 0;
    for(auto v : bitSequence2) {
        EXPECT_EQ(bbs2->getBit(), v) << "BitSequence2 and getBit() differ at index " << counter;
        counter++;
    }
}

TEST_F(BbsTest, GetByte) {
    EXPECT_EQ(bbs->getByte(), 0xce);
    EXPECT_EQ(bbs->getByte(), 0x13);

    // See Prakikum-Cryptopp, Aufgabe 6.f
    std::vector<byte> byteBuffer {
        0x7, 0xd, 0x2a, 0x3e, 0x7a, 0x2b, 0xf3, 0xa2, 0xf0, 0x94, 
        0xc1, 0x31, 0xfc, 0xac, 0xc5, 0x6b, 0x40, 0xbb, 0x95, 0x9e, 
        0x27, 0x39, 0x52, 0x29, 0x9f, 0xc6, 0x8, 0xb6, 0x78, 0x1f, 
        0x6, 0xb1, 0xd8, 0x59, 0xe2, 0xed, 0x4c, 0x92, 0x4d, 0x3f, 
        0x1b, 0xa1, 0xc5, 0xee, 0xee, 0x62, 0x85, 0xe9, 0xc2, 0x9e, 
        0xfb, 0x8, 0x11, 0x2b, 0xa2, 0x52, 0x2f, 0x70, 0xb4, 0xa0, 
        0x55, 0x77, 0x4a, 0x56, 0x59, 0xd3, 0x31, 0x45, 0xe5, 0x3c, 
        0x46, 0x4, 0x28, 0x8f, 0x2b, 0x63, 0x16, 0x4f, 0xae, 0xed, 
        0x90, 0xae, 0x74, 0x32, 0x67, 0x23, 0xa9, 0x44, 0x88, 0x9d, 
        0x24, 0x8, 0xa3, 0x5f, 0x5a, 0xf4, 0x9b, 0x37, 0x58, 0xb5
    };
    
    int counter = 0;
    for(auto v : byteBuffer) {
        EXPECT_EQ(bbs2->getByte(), v) << "byteBuffer and getByte() differ at index " << counter;
        counter++;
    }
}

TEST_F(BbsTest, GetBlock) {
    byte buffer[2] { 0 };
    bbs->getBlock(buffer, 2);
    EXPECT_EQ(buffer[0], 0xce);
    EXPECT_EQ(buffer[1], 0x13);
}

TEST_F(BbsTest, GetInteger) {
    EXPECT_EQ(bbs->getInteger(2), Integer("0xce13"));
    
    // See Prakikum-Cryptopp, Aufgabe 6.h
    EXPECT_EQ(bbs2->getInteger(200), Integer("1224700908562876304896674460468936706321336908719948887176298333736047960197293771838882325384870983738564115083909700719730828680048467140885089420080415503309265870836648877591960917676947181531881627788061646753895054605084890076325998373940802811863529539511270066586236579000383730255064863180762061193043163417053225340621871316726563054813731421895460464348594230285471929209449003999777703890486952270306173278738624160543822940288445016268964123278221665204840310575909129"));
}

TEST_F(BbsTest, GetInteger2) {
    // See Prakikum-Cryptopp, Aufgabe 6.j
    Integer m = Integer("2") << 30;
    EXPECT_EQ(bbs2->getInteger(m), Integer("118303294"));
}
