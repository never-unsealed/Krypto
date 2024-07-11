#include <gtest/gtest.h>
#include "gmock/gmock.h"
#include "DESCipher.h"

TEST(DesTest, GetBit) {
    DESCipher des;
    byte buffer[2] {0b11001010, 0b00110101};

    EXPECT_EQ(des.getBit(buffer, 2, 0), true);
    EXPECT_EQ(des.getBit(buffer, 2, 1), true);
    EXPECT_EQ(des.getBit(buffer, 2, 2), false);
    EXPECT_EQ(des.getBit(buffer, 2, 3), false);
    EXPECT_EQ(des.getBit(buffer, 2, 4), true);
    EXPECT_EQ(des.getBit(buffer, 2, 5), false);
    EXPECT_EQ(des.getBit(buffer, 2, 6), true);
    EXPECT_EQ(des.getBit(buffer, 2, 7), false);

    EXPECT_EQ(des.getBit(buffer, 2, 8), false);
    EXPECT_EQ(des.getBit(buffer, 2, 9), false);
    EXPECT_EQ(des.getBit(buffer, 2, 10), true);
    EXPECT_EQ(des.getBit(buffer, 2, 11), true);
    EXPECT_EQ(des.getBit(buffer, 2, 12), false);
    EXPECT_EQ(des.getBit(buffer, 2, 13), true);
    EXPECT_EQ(des.getBit(buffer, 2, 14), false);
    EXPECT_EQ(des.getBit(buffer, 2, 15), true);
}

TEST(DesTest, SetBit) {
    DESCipher des;
    byte buffer[2] {0b00000000, 0b11111111};

    des.setBit(buffer, 2, 0, true);
    des.setBit(buffer, 2, 1, true);
    des.setBit(buffer, 2, 4, true);
    des.setBit(buffer, 2, 6, true);
    des.setBit(buffer, 2, 8, false);
    des.setBit(buffer, 2, 9, false);
    des.setBit(buffer, 2, 12, false);
    des.setBit(buffer, 2, 14, false);

    EXPECT_EQ(des.getBit(buffer, 2, 0), true);
    EXPECT_EQ(des.getBit(buffer, 2, 1), true);
    EXPECT_EQ(des.getBit(buffer, 2, 2), false);
    EXPECT_EQ(des.getBit(buffer, 2, 3), false);
    EXPECT_EQ(des.getBit(buffer, 2, 4), true);
    EXPECT_EQ(des.getBit(buffer, 2, 5), false);
    EXPECT_EQ(des.getBit(buffer, 2, 6), true);
    EXPECT_EQ(des.getBit(buffer, 2, 7), false);

    EXPECT_EQ(des.getBit(buffer, 2, 8), false);
    EXPECT_EQ(des.getBit(buffer, 2, 9), false);
    EXPECT_EQ(des.getBit(buffer, 2, 10), true);
    EXPECT_EQ(des.getBit(buffer, 2, 11), true);
    EXPECT_EQ(des.getBit(buffer, 2, 12), false);
    EXPECT_EQ(des.getBit(buffer, 2, 13), true);
    EXPECT_EQ(des.getBit(buffer, 2, 14), false);
    EXPECT_EQ(des.getBit(buffer, 2, 15), true);
}

TEST(DesTest, PrintBitField) {
    
    DESCipher des;

    byte buffer[2] { 0x2f, 0x6b };

    testing::internal::CaptureStdout();
    des.printBitField(buffer, 1, 2);
    auto output { testing::internal::GetCapturedStdout() };
    EXPECT_EQ(output, "00 10 11 11");

    testing::internal::CaptureStdout();
    des.printBitField(buffer, 2, 8);
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "00101111 01101011");

    buffer[0] = 0xf3;
    buffer[1] = 0xc0;

    testing::internal::CaptureStdout();
    des.printBitField(buffer, 1, 2);
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "11 11 00 11");

    testing::internal::CaptureStdout();
    des.printBitField(buffer, 2, 8);
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "11110011 11000000");
}

TEST(DesTest, Permutate) {
    using namespace ::testing;

    DESCipher des;
    byte buffer[1] { 0xd2 };
    byte pBuffer[13] { 
        0x01, 0x03, 0x06, 0x05, 0x04, 
        0x03, 0x02, 0x01, 0x06, 0x07, 
        0x03, 0x02, 0x01 
    };

    byte buffer2[2] { 0 };
    des.permutate(pBuffer, 13, buffer, 7, buffer2, 13);
    
    EXPECT_THAT(buffer2, ElementsAreArray({ 0b10001011, 0b01011000 }));
}

TEST(DesTest, ComputeKeySchedule) {
    DESCipher des;

    // Gegebene Rundenkeytabellen
    byte roundKeySchedule[16][48] {
        /* R1 */
        10, 51, 34, 60, 49, 17, 33, 57, 2, 9, 19, 42,
        3, 35, 26, 25, 44, 58, 59, 1, 36, 27, 18, 41,
        22, 28, 39, 54, 37, 4, 47, 30, 5, 53, 23, 29,
        61, 21, 38, 63, 15, 20, 45, 14, 13, 62, 55, 31,
        
        /* R2 */
        2, 43, 26, 52, 41, 9, 25, 49, 59, 1, 11, 34,
        60, 27, 18, 17, 36, 50, 51, 58, 57, 19, 10, 33,
        14, 20, 31, 46, 29, 63, 39, 22, 28, 45, 15, 21,
        53, 13, 30, 55, 7, 12, 37, 6, 5, 54, 47, 23,

        /* R3 */
        51, 27, 10, 36, 25, 58, 9, 33, 43, 50, 60, 18,
        44, 11, 2, 1, 49, 34, 35, 42, 41, 3, 59, 17,
        61, 4, 15, 30, 13, 47, 23, 6, 12, 29, 62, 5,
        37, 28, 14, 39, 54, 63, 21, 53, 20, 38, 31, 7,

        /* R4 */
        35, 11, 59, 49, 9, 42, 58, 17, 27, 34, 44, 2,
        57, 60, 51, 50, 33, 18, 19, 26, 25, 52, 43, 1,
        45, 55, 62, 14, 28, 31, 7, 53, 63, 13, 46, 20,
        21, 12, 61, 23, 38, 47, 5, 37, 4, 22, 15, 54,

        /* R5 */
        19, 60, 43, 33, 58, 26, 42, 1, 11, 18, 57, 51,
        41, 44, 35, 34, 17, 2, 3, 10, 9, 36, 27, 50,
        29, 39, 46, 61, 12, 15, 54, 37, 47, 28, 30, 4,
        5, 63, 45, 7, 22, 31, 20, 21, 55, 6, 62, 38,
        
        /* R6 */
        3, 44, 27, 17, 42, 10, 26, 50, 60, 2, 41, 35,
        25, 57, 19, 18, 1, 51, 52, 59, 58, 49, 11, 34,
        13, 23, 30, 45, 63, 62, 38, 21, 31, 12, 14, 55,
        20, 47, 29, 54, 6, 15, 4, 5, 39, 53, 46, 22,
                                                                                                                                            
        /* R7 */    
        52, 57, 11, 1, 26, 59, 10, 34, 44, 51, 25, 19,    
        9, 41, 3, 2, 50, 35, 36, 43, 42, 33, 60, 18,     
        28, 7, 14 ,29, 47, 46, 22, 5, 15, 63, 61, 39,    
        4, 31, 13, 38, 53, 62, 55, 20, 23, 37, 30, 6,    
    
        /* R8 */    
        36, 41, 60, 50, 10, 43, 59, 18, 57, 35, 9, 3,    
        58, 25, 52, 51, 34, 19, 49, 27, 26, 17, 44, 2,    
        12, 54, 61, 13, 31, 30, 6, 20, 62, 47, 45, 23,    
        55, 15, 28, 22, 37, 46, 39, 4, 7, 21, 14, 53,    
    
        /* R9 */    
        57, 33, 52, 42, 2, 35, 51, 10, 49, 27, 1, 60,    
        50, 17, 44, 43, 26, 11, 41, 19, 18,9, 36, 59,    
        4, 46, 53, 5, 23, 22, 61, 12, 54, 39, 37, 15,    
        47, 7, 20, 14, 29, 38, 31, 63, 62, 13, 6, 45,    
    
        /* R10 */    
        41, 19, 36, 26, 51, 19, 35, 59, 33, 11, 50, 44,    
        34, 1, 57, 27, 10, 60, 25, 3, 2, 58, 49, 43,    
        55, 30, 37, 20, 7, 6, 45, 63, 38, 23, 21, 62,    
        31, 54, 4, 61, 13, 22, 15, 47, 46, 28, 53, 29,

        /* R11 */
        25, 1, 49, 10, 35, 3, 19, 43, 17, 60, 34, 57,
        18, 50, 41, 11, 59, 44, 9, 52, 51, 42, 33, 27,
        39, 14, 21, 4, 54, 53, 29, 47, 22, 7, 5, 46,
        15, 38, 55, 45, 28, 6, 62, 31, 30, 12, 37, 13,

        /* R12 */
        9, 50, 33, 59, 19, 52, 3, 27, 1, 44, 18, 41,
        2, 34, 25, 60, 43, 57, 58, 36, 35, 26, 17, 11,
        23, 61, 5, 55, 38, 37, 13, 31, 6, 54, 20, 30,
        62, 22, 39, 29, 12, 53, 46, 15, 14, 63, 21, 28,

        /* R13 */
        58, 34, 17, 43, 3, 36, 52, 11, 50, 57, 2, 25,
        51, 18, 9, 44, 27, 41, 42, 49, 19, 10, 1, 60,
        7, 45, 20, 39, 22, 21, 28, 15, 53, 38, 4, 14,
        46, 6, 23, 13, 63, 37, 30, 62, 61, 47, 5, 12,

        /* R14 */
        42, 18, 1, 27, 52, 49, 36, 60, 34, 41, 51, 9,
        35, 2, 58, 57, 11, 25, 26, 33, 3, 59, 50, 44,
        54, 29, 4, 23, 6, 5, 12, 62, 37, 22, 55, 61,
        30, 53, 7, 28, 47, 21, 14, 46, 45, 31, 20, 63,
        
        /* R15 */
        26, 2, 50, 11, 36, 33, 49, 44, 18, 25, 35, 58, 
        19, 51, 42, 41, 60, 9, 10, 17, 52, 43, 34, 57,
        38, 13, 55, 7, 53, 20, 63, 46, 21, 6, 39, 45,
        14, 37, 54, 12, 31, 5, 61, 30, 29, 15, 4, 47,
        
        /* R16 */
        18, 59, 42, 3, 57, 25, 41, 36, 10, 17, 27, 50,
        11, 43, 34, 33, 52, 1, 2, 9, 44, 35, 26, 49,
        30, 5, 47, 62, 45, 12, 55, 38, 13, 61, 31, 37,
        6, 29, 46, 4, 23, 28, 53, 22, 21, 7, 63, 39
    };
    
    byte key[8] { 0x13, 0x34, 0x57, 0x79, 0x9b, 0xbc, 0xdf, 0xf1 };
    
    // Berechnung der Key Tabellen mittels computeKeySchedule,
    // sowie Rundenschlüsseltabellen.
    des.computeKeySchedule(key, 1);

    byte key_schedule_test[16][6];
    for( size_t i = 0; i < 16; i++ ) {
        for( size_t j = 0; j < 48; j++ ) {
            des.setBit( key_schedule_test[i], 6, j, des.getBit( key,8,roundKeySchedule[i][j] -1 ) );
        }         
    } 

    byte key_schedule[16][6] { 0 };
    des.getKeySchedule((byte*)&key_schedule[0]);

    for(int i = 0; i < 16; ++i) {
        for(int j = 0; j < 6; ++j) {
            EXPECT_EQ(key_schedule_test[i][j], key_schedule[i][j]);
        }
    }
}

TEST(DesTest, ComputeSBox) {
    DESCipher des;
    byte id, line, col, result;

    // Berechne SBox 0, Zeile 0 und Spalte 0.
    id = 0, line = 0, col = 0;
    EXPECT_EQ(des.computeSBox(id, line, col), 14);

    // Berechne SBox 4, Zeile 1 und Spalte 5.
    id = 4, line = 1, col = 5;
    EXPECT_EQ(des.computeSBox(id, line, col), 7);

    // Berechne SBox 7, Zeile 3 und Spalte 15.
    id = 7, line = 3, col = 15;
    EXPECT_EQ(des.computeSBox(id, line, col), 11);
}

TEST(DesTest, Ffunction) {
    using namespace ::testing;

    DESCipher des;
    byte r_out[4] { 0 };

    // F-function von Runde 1.
    byte key[6] { 0x1B, 0x02, 0xEF, 0xFC, 0x70, 0x72 };
    byte r_in[4] { 0xF0, 0xAA, 0xF0, 0xAA };
    des.functionF( r_in, key, r_out, 0 );
    EXPECT_THAT(r_out, ElementsAreArray({ 0b00100011, 0b01001010, 0b10101001, 0b10111011 }));
    
    // F-function von Runde 2.
    byte key2[6] { 0x79, 0xAE, 0xD9, 0xDB, 0xC9, 0xE5 };
    byte r_in2[4] { 0xEF, 0x4A, 0x65, 0x44 };
    des.functionF( r_in2, key2, r_out, 1 );
    EXPECT_THAT(r_out, ElementsAreArray({ 0b00111100, 0b10101011, 0b10000111, 0b10100011 }));
    
    // F-function von Runde 16
    byte key3[6] { 0xCB, 0x3D, 0x8B, 0x0E, 0x17, 0xF5 };
    byte r_in3[4] { 0x43, 0x42, 0x32, 0x34 };
    des.functionF( r_in3, key3, r_out, 15 );
    EXPECT_THAT(r_out, ElementsAreArray({ 0b11001000, 0b11000000, 0b01001111, 0b10011000 }));
}

TEST(DesTest, Feistel) {
    using namespace ::testing;

    DESCipher des;
    byte r_out[4] { 0 };
    byte l_out[4] { 0 };

    // Feistel Runde 1
    byte key[6] { 0x1B, 0x02, 0xEF, 0xFC, 0x70, 0x72 };
    byte l_in[4] { 0xCC, 0x00, 0xCC, 0xFF };
    byte r_in[4] { 0xF0, 0xAA, 0xF0, 0xAA };

    des.feistel(l_in, r_in, key, l_out, r_out, 0 );
    EXPECT_THAT(l_out, ElementsAreArray({ 0xF0, 0xAA, 0xF0, 0xAA }));
    EXPECT_THAT(r_out, ElementsAreArray({ 0xEF, 0x4A, 0x65, 0x44 }));

    // Feistel Runde 2
    byte key2[6] { 0x79, 0xAE, 0xD9, 0xDB, 0xC9, 0xE5 };
    byte r_in2[4] { 0xEF, 0x4A, 0x65, 0x44 };
    byte l_in2[4] { 0xF0, 0xAA, 0xF0, 0xAA };

    des.feistel( l_in2, r_in2, key2, l_out, r_out, 0 ); 
    EXPECT_THAT(l_out, ElementsAreArray({ 0xEF, 0x4A, 0x65, 0x44 }));
    EXPECT_THAT(r_out, ElementsAreArray({ 0xCC, 0x01, 0x77, 0x09 }));

    // Feistel Runde 16
    byte key3[6] { 0xCB, 0x3D, 0x8B, 0x0E, 0x17, 0xF5 };
    byte r_in3[4] { 0x43, 0x42, 0x32, 0x34 };
    byte l_in3[4] { 0xC2, 0x8C, 0x96, 0x0D };

    des.feistel(l_in3, r_in3, key3, l_out, r_out, 0);
    EXPECT_THAT(l_out, ElementsAreArray({ 0x43, 0x42, 0x32, 0x34 }));
    EXPECT_THAT(r_out, ElementsAreArray({ 0x0A, 0x4C, 0xD9, 0x95 }));
}

TEST(DesTest, ProcessBlock) {
    using namespace ::testing;

    DESCipher des;

    const byte plainText[8] { 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF };
    byte key[8] { 0x13, 0x34, 0x57, 0x79, 0x9B, 0xBC, 0xDF, 0xF1 };
    byte outText[8];

    des.computeKeySchedule(key, true);
    des.processBlock(&plainText[0], &outText[0]);

    EXPECT_THAT(outText, ElementsAreArray({ 0x85, 0xe8, 0x13, 0x54, 0x0f, 0x0a, 0xb4, 0x05 }));
}

TEST(DesTest, ProcessBlock2) {
    using namespace ::testing;

    DESCipher des;

    const byte plainText[8] { 0x63,0x61,0x66,0x65,0x62,0x61,0x62,0x65 };
    byte key[8] { 0x68,0x73,0x61,0x61,0x6c,0x65,0x6e,0x31 };
    byte outText[8];

    des.computeKeySchedule(key, true);
    des.processBlock(&plainText[0], &outText[0]);

    EXPECT_THAT(outText, ElementsAreArray({ 0x04, 0x0c, 0x7d, 0x8f, 0x3e, 0x5a, 0xfb, 0x47 }));
}

TEST(DesTest, Encrypt) {
    using namespace ::testing;

    DESCipher des;

    const byte plainText[8] { 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF };
    byte key[8] { 0x13, 0x34, 0x57, 0x79, 0x9B, 0xBC, 0xDF, 0xF1 };
    byte outText[8];

    des.encrypt(plainText, 8, key, 8, outText, 8);
    EXPECT_THAT(outText, ElementsAreArray({ 0x85, 0xe8, 0x13, 0x54, 0x0f, 0x0a, 0xb4, 0x05 }));
}

TEST(DesTest, Decrypt) {
    using namespace ::testing;

    DESCipher des;

    byte result[104] { 0x48, 0x65, 0x72, 0x7a, 0x6c, 0x69, 0x63, 0x68, 0x65, 0x6e, 0x20, 0x47, 0x6c, 0xfc, 0x63,
        0x6b, 0x77, 0x75, 0x6e, 0x73, 0x63, 0x68, 0x21, 0x0a, 0x0a, 0x53, 0x69, 0x65, 0x20, 0x68, 0x61, 0x62, 0x65,
        0x6e, 0x20, 0x73, 0x6f, 0x65, 0x62, 0x65, 0x6e, 0x20, 0x50, 0x72, 0x61, 0x6b, 0x74, 0x69, 0x6b, 0x75, 0x6d,
        0x20, 0x33, 0x20, 0x62, 0x65, 0x73, 0x74, 0x61, 0x6e, 0x64, 0x65, 0x6e, 0x21, 0x0a, 0x0a, 0x4a, 0x65, 0x74,
        0x7a, 0x74, 0x20, 0x66, 0x65, 0x68, 0x6c, 0x74, 0x20, 0x6e, 0x75, 0x72, 0x20, 0x6e, 0x6f, 0x63, 0x68, 0x20,
        0x50, 0x72, 0x61, 0x6b, 0x74, 0x69, 0x6b, 0x75, 0x6d, 0x20, 0x34, 0x2e, 0x2e, 0x2e, 0x0a, 0x00, 0x00};

    const byte deschipher[104] { 0xe6, 0x6c, 0x98, 0x02, 0x02, 0x4c, 0x91, 0x0a, 0x3a, 0xef, 0x96, 0x4e, 0x56, 0x2c, 
        0x2f, 0xa3, 0x6f, 0xa5, 0x11, 0x28, 0x02, 0x16, 0x5e, 0xc5, 0xa7, 0xe3, 0xad, 0x3b, 0x43, 0x13, 0xfd, 0xfb, 0xbc, 
        0xe8, 0xec, 0xf2, 0x07, 0x34, 0x9e, 0x7a, 0xb6, 0x4d, 0x09, 0x7a, 0xed, 0xa4, 0x30, 0x01, 0x5b, 0x76, 0x75, 0xc4, 
        0xcc, 0x25, 0x7c, 0x40, 0x9f, 0x97, 0xbb, 0x38, 0x6d, 0x9e, 0xd4, 0x17, 0x02, 0xf3, 0x9e, 0xf7, 0x3e, 0x76, 0xec,
        0x36, 0xb7, 0xfb, 0x06, 0x10, 0x84, 0xe8, 0x15, 0x83, 0x28, 0xdb, 0x1f, 0x4b, 0xdc, 0x83, 0x80, 0x9a, 0x8b, 0xe7,
        0x6c, 0x64, 0xf7, 0x56, 0xe2, 0xdb, 0xf0, 0xa5, 0x25, 0xcd, 0xd3, 0xc1, 0xa0, 0x74 };
    
    byte key[8] { 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10};
    byte outText[104] { 0 };
    
    des.decrypt(deschipher, 104, key, 8, outText, 104);
    EXPECT_THAT(outText, ElementsAreArray(result));
}




















