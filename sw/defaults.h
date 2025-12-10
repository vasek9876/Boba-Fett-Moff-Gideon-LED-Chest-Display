#ifndef defaults_h
#define defaults_h

#ifdef OLD

// 88888
#define PATTERN_51 0b11101111
#define PATTERN_52 0b11101111
#define PATTERN_53 0b11101111
#define PATTERN_54 0b11101111
#define PATTERN_55 0b11101111

// DUEPI
#define PATTERN_21 0b00000010
#define PATTERN_22 0b10001111
#define PATTERN_23 0b11000111
#define PATTERN_24 0b11101010
#define PATTERN_25 0b11101001

// AEAOE
#define PATTERN_11 0b10000011
#define PATTERN_12 0b11101100
#define PATTERN_13 0b10101101
#define PATTERN_14 0b11000111
#define PATTERN_15 0b10100111

// caj
#define PATTERN_41 0b00000101
#define PATTERN_42 0b10000010
#define PATTERN_43 0b01000100
#define PATTERN_44 0b00100010
#define PATTERN_45 0b00001001

// DUEPI again

// H5291
#define PATTERN_31 0b10000010
#define PATTERN_32 0b00101111
#define PATTERN_33 0b11001101
#define PATTERN_34 0b01100011
#define PATTERN_35 0b10101011

#else

// 88888
#define PATTERN_51 0b00001000
#define PATTERN_52 0b00001000
#define PATTERN_53 0b00001000
#define PATTERN_54 0b00001000
#define PATTERN_55 0b00001000

// DUEPI
#define PATTERN_21 0b10111111
#define PATTERN_22 0b00001011
#define PATTERN_23 0b00011001
#define PATTERN_24 0b10101000
#define PATTERN_25 0b11001000

// AEAOE
#define PATTERN_11 0b10011011
#define PATTERN_12 0b01101000
#define PATTERN_13 0b01001010
#define PATTERN_14 0b00011001
#define PATTERN_15 0b00011010

// caj
#define PATTERN_41 0b01011111
#define PATTERN_42 0b10111011
#define PATTERN_43 0b01111101
#define PATTERN_44 0b10111110
#define PATTERN_45 0b11001111

// DUEPI again

// H5291
#define PATTERN_31 0b10111011
#define PATTERN_32 0b00001110
#define PATTERN_33 0b01001001
#define PATTERN_34 0b10011100
#define PATTERN_35 0b10001010

// now afgbhedc (default abcdefgh)

#define PATTERN_0 0b00101000  // 0 // OK
#define PATTERN_1 0b10111011  // 1 // OK
#define PATTERN_2 0b01001001  // 2 // OK
#define PATTERN_3 0b01001100  // 3 // OK
#define PATTERN_4 0b10001110  // 4 // OK
#define PATTERN_5 0b00011100  // 5
#define PATTERN_6 0b00011000  // 6
#define PATTERN_7 0b01101110  // 7
#define PATTERN_8 0b00001000  // 8
#define PATTERN_9 0b00001110  // 9

#define PATTERN_0_DP 0b00100000  // 0 // OK
#define PATTERN_1_DP 0b10110011  // 1 // OK
#define PATTERN_2_DP 0b01000001  // 2 // OK
#define PATTERN_3_DP 0b01000100  // 3 // OK
#define PATTERN_4_DP 0b10000110  // 4 // OK
#define PATTERN_5_DP 0b00010100  // 5
#define PATTERN_6_DP 0b00010000  // 6
#define PATTERN_7_DP 0b01100110  // 7
#define PATTERN_8_DP 0b00000000  // 8
#define PATTERN_9_DP 0b00000110  // 9

#define PATTERN_U 0b10101000

#endif

#ifdef TEST



#define PATTERN_11 0b01111111
#define PATTERN_12 0b01110111
#define PATTERN_13 0b01111011
#define PATTERN_14 0b01111101
#define PATTERN_15 0b01111110

#endif

// in sec
#define REPEAT_FUNC1 27*2 // 0.49
#define REPEAT_FUNC2 28*2 // 0.51
#define REPEAT_FUNC3 26*2 // 0.48
#define REPEAT_FUNC4 40*2 // 0.72  
#define REPEAT_FUNC5 30*2 // 0.54
#define REPEAT_FUNC6 27*2 // 0.50

// digits

const byte DIGITS[10] = {
    PATTERN_0,
    PATTERN_1,
    PATTERN_2,
    PATTERN_3,
    PATTERN_4,
    PATTERN_5,
    PATTERN_6,
    PATTERN_7,
    PATTERN_8,
    PATTERN_9
};


const byte bar_map[16][4] = {
  // D, B, A, C (DCBA - 1111)
  {0,0,0,0},
  {0,0,1,0},
  {0,1,0,0},
  {0,1,1,0},
  {0,0,0,1},
  {0,0,1,1},
  {0,1,0,1},
  {0,1,1,1},

  {1,0,0,0},
  {1,0,1,0},
  {1,1,0,0},
  {1,1,1,0},
  {1,0,0,1},
  {1,0,1,1},
  {1,1,0,1},
  {1,1,1,1},
};

// Old
#ifdef OLD
const byte bar_map[16][4] = {
    {0,0,0,1},
    {0,1,0,0},
    {0,0,1,0},
    {0,0,0,0},
    {1,0,0,0},
    {1,1,0,0},
    {0,1,0,1},
    {1,1,0,1},
    {0,0,1,0},
    {1,0,1,0},
    {0,0,1,1},
    {1,0,1,1},
    {0,1,1,0},
    {1,1,1,0},
    {0,1,1,1},
    {1,1,1,1}
};
#endif



const byte pattern_numbers[] = {PATTERN_0, PATTERN_1, PATTERN_2, PATTERN_3, PATTERN_4,
                                PATTERN_5, PATTERN_6, PATTERN_7, PATTERN_8, PATTERN_9};

const byte pattern_numbers_DP[] = {PATTERN_0_DP, PATTERN_1_DP, PATTERN_2_DP, PATTERN_3_DP, PATTERN_4_DP,
                                PATTERN_5_DP, PATTERN_6_DP, PATTERN_7_DP, PATTERN_8_DP, PATTERN_9_DP};


const byte pattern1[] = {PATTERN_11, PATTERN_12, PATTERN_13, PATTERN_14, PATTERN_15};
const byte pattern2[] = {PATTERN_21, PATTERN_22, PATTERN_23, PATTERN_24, PATTERN_25};
const byte pattern3[] = {PATTERN_31, PATTERN_32, PATTERN_33, PATTERN_34, PATTERN_35};
const byte pattern4[] = {PATTERN_41, PATTERN_42, PATTERN_43, PATTERN_44, PATTERN_45};
const byte pattern5[] = {PATTERN_51, PATTERN_52, PATTERN_53, PATTERN_54, PATTERN_55};


#endif