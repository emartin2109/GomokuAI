/*
** EPITECH PROJECT, 2024
** B-AIA-500-BDX-5-1-gomoku-eliot1.martin
** File description:
** Precomputed
*/

#pragma once

#include <bitset>
#include <cstdint>
#include <tuple>
#include <vector>

const std::vector<uint16_t> TRANSPOTITION_0 = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
    25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
    48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
    70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91,
    92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110,
    111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128,
    129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145,
    146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162,
    163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179,
    180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196,
    197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213,
    214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230,
    231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247,
    248, 249, 250, 251, 252, 253, 254, 255, 256, 257, 258, 259, 260, 261, 262, 263, 264,
    265, 266, 267, 268, 269, 270, 271, 272, 273, 274, 275, 276, 277, 278, 279, 280, 281,
    282, 283, 284, 285, 286, 287, 288, 289, 290, 291, 292, 293, 294, 295, 296, 297, 298,
    299, 300, 301, 302, 303, 304, 305, 306, 307, 308, 309, 310, 311, 312, 313, 314, 315,
    316, 317, 318, 319, 320, 321, 322, 323, 324, 325, 326, 327, 328, 329, 330, 331, 332,
    333, 334, 335, 336, 337, 338, 339, 340, 341, 342, 343, 344, 345, 346, 347, 348, 349,
    350, 351, 352, 353, 354, 355, 356, 357, 358, 359, 360, 361, 362, 363, 364, 365, 366,
    367, 368, 369, 370, 371, 372, 373, 374, 375, 376, 377, 378, 379, 380, 381, 382,
    383, 384, 385, 386, 387, 388, 389, 390, 391, 392, 393, 394, 395, 396, 397, 398, 399

};

const std::vector<uint16_t> TRANSPOTITION_45 = {
    0, 20, 1, 40, 21, 2, 60, 41, 22, 3, 80, 61, 42, 23, 4, 100, 81, 62, 43, 24, 5, 120,
    101, 82, 63, 44, 25, 6, 140, 121, 102, 83, 64, 45, 26, 7, 160, 141, 122, 103, 84, 65,
    46, 27, 8, 180, 161, 142, 123, 104, 85, 66, 47, 28, 9, 200, 181, 162, 143, 124, 105,
    86, 67, 48, 29, 10, 220, 201, 182, 163, 144, 125, 106, 87, 68, 49, 30, 11, 240, 221,
    202, 183, 164, 145, 126, 107, 88, 69, 50, 31, 12, 260, 241, 222, 203, 184, 165, 146,
    127, 108, 89, 70, 51, 32, 13, 280, 261, 242, 223, 204, 185, 166, 147, 128, 109, 90,
    71, 52, 33, 14, 300, 281, 262, 243, 224, 205, 186, 167, 148, 129, 110, 91, 72, 53, 34,
    15, 320, 301, 282, 263, 244, 225, 206, 187, 168, 149, 130, 111, 92, 73, 54, 35, 16, 340,
    321, 302, 283, 264, 245, 226, 207, 188, 169, 150, 131, 112, 93, 74, 55, 36, 17, 360, 341,
    322, 303, 284, 265, 246, 227, 208, 189, 170, 151, 132, 113, 94, 75, 56, 37, 18, 380, 361,
    342, 323, 304, 285, 266, 247, 228, 209, 190, 171, 152, 133, 114, 95, 76, 57, 38, 19, 381,
    362, 343, 324, 305, 286, 267, 248, 229, 210, 191, 172, 153, 134, 115, 96, 77, 58, 39,
    382,363, 344, 325, 306, 287, 268, 249, 230, 211, 192, 173, 154, 135, 116, 97, 78, 59,
    383, 364, 345, 326, 307, 288, 269, 250, 231, 212, 193, 174, 155, 136, 117, 98, 79, 384,
    365, 346, 327, 308, 289, 270, 251, 232, 213, 194, 175, 156, 137, 118, 99, 385, 366, 347,
    328, 309, 290, 271, 252, 233, 214, 195, 176, 157, 138, 119, 386, 367, 348, 329, 310, 291,
    272, 253, 234, 215, 196, 177, 158, 139, 387, 368, 349, 330, 311, 292, 273, 254, 235, 216,
    197, 178, 159, 388, 369, 350, 331, 312, 293, 274, 255, 236, 217, 198, 179, 389, 370, 351,
    332, 313, 294, 275, 256, 237, 218, 199, 390, 371, 352, 333, 314, 295, 276, 257, 238, 219,
    391, 372, 353, 334, 315, 296, 277, 258, 239, 392, 373, 354, 335, 316, 297, 278, 259, 393,
    374, 355, 336, 317, 298, 279, 394, 375, 356, 337, 318, 299, 395, 376, 357, 338, 319, 396,
    377, 358, 339, 397, 378, 359, 398, 379, 399
};

const std::vector<uint16_t> TRANSPOTITION_90 {
    380, 360, 340, 320, 300, 280, 260, 240, 220, 200, 180, 160, 140, 120, 100, 80, 60, 40, 20,
    0, 381, 361, 341, 321, 301, 281, 261, 241, 221, 201, 181, 161, 141, 121, 101, 81, 61, 41,
    21, 1, 382, 362, 342, 322, 302, 282, 262, 242, 222, 202, 182, 162, 142, 122, 102, 82, 62,
    42, 22, 2, 383, 363, 343, 323, 303, 283, 263, 243, 223, 203, 183, 163, 143, 123, 103, 83,
    63, 43, 23, 3, 384, 364, 344, 324, 304, 284, 264, 244, 224, 204, 184, 164, 144, 124, 104,
    84, 64, 44, 24, 4, 385, 365, 345, 325, 305, 285, 265, 245, 225, 205, 185, 165, 145, 125,
    105, 85, 65, 45, 25, 5, 386, 366, 346, 326, 306, 286, 266, 246, 226, 206, 186, 166, 146,
    126, 106, 86, 66, 46, 26, 6, 387, 367, 347, 327, 307, 287, 267, 247, 227, 207, 187, 167,
    147, 127, 107, 87, 67, 47, 27, 7, 388, 368, 348, 328, 308, 288, 268, 248, 228, 208, 188,
    168, 148, 128, 108, 88, 68, 48, 28, 8, 389, 369, 349, 329, 309, 289, 269, 249, 229, 209,
    189, 169, 149, 129, 109, 89, 69, 49, 29, 9, 390, 370, 350, 330, 310, 290, 270, 250, 230,
    210, 190, 170, 150, 130, 110, 90, 70, 50, 30, 10, 391, 371, 351, 331, 311, 291, 271, 251,
    231, 211, 191, 171, 151, 131, 111, 91, 71, 51, 31, 11, 392, 372, 352, 332, 312, 292, 272,
    252, 232, 212, 192, 172, 152, 132, 112, 92, 72, 52, 32, 12, 393, 373, 353, 333, 313, 293,
    273, 253, 233, 213, 193, 173, 153, 133, 113, 93, 73, 53, 33, 13, 394, 374, 354, 334, 314,
    294, 274, 254, 234, 214, 194, 174, 154, 134, 114, 94, 74, 54, 34, 14, 395, 375, 355, 335,
    315, 295, 275, 255, 235, 215, 195, 175, 155, 135, 115, 95, 75, 55, 35, 15, 396, 376, 356,
    336, 316, 296, 276, 256, 236, 216, 196, 176, 156, 136, 116, 96, 76, 56, 36, 16, 397, 377,
    357, 337, 317, 297, 277, 257, 237, 217, 197, 177, 157, 137, 117, 97, 77, 57, 37, 17, 398,
    378, 358, 338, 318, 298, 278, 258, 238, 218, 198, 178, 158, 138, 118, 98, 78, 58, 38, 18,
    399, 379, 359, 339, 319, 299, 279, 259, 239, 219, 199, 179, 159, 139, 119, 99, 79, 59, 39,
    19 
};

const std::vector<uint16_t> TRANSPOTITION_135 {
    380, 381, 360, 382, 361, 340, 383, 362, 341, 320, 384, 363, 342, 321, 300, 385, 364, 343,
    322, 301, 280, 386, 365, 344, 323, 302, 281, 260, 387, 366, 345, 324, 303, 282, 261, 240,
    388, 367, 346, 325, 304, 283, 262, 241, 220, 389, 368, 347, 326, 305, 284, 263, 242, 221,
    200, 390, 369, 348, 327, 306, 285, 264, 243, 222, 201, 180, 391, 370, 349, 328, 307, 286,
    265, 244, 223, 202, 181, 160, 392, 371, 350, 329, 308, 287, 266, 245, 224, 203, 182, 161,
    140, 393, 372, 351, 330, 309, 288, 267, 246, 225, 204, 183, 162, 141, 120, 394, 373, 352,
    331, 310, 289, 268, 247, 226, 205, 184, 163, 142, 121, 100, 395, 374, 353, 332, 311, 290,
    269, 248, 227, 206, 185, 164, 143, 122, 101, 80, 396, 375, 354, 333, 312, 291, 270, 249,
    228, 207, 186, 165, 144, 123, 102, 81, 60, 397, 376, 355, 334, 313, 292, 271, 250, 229,
    208, 187, 166, 145, 124, 103, 82, 61, 40, 398, 377, 356, 335, 314, 293, 272, 251, 230, 209,
    188, 167, 146, 125, 104, 83, 62, 41, 20, 399, 378, 357, 336, 315, 294, 273, 252, 231, 210,
    189, 168, 147, 126, 105, 84, 63, 42, 21, 0, 379, 358, 337, 316, 295, 274, 253, 232, 211,
    190, 169, 148, 127, 106, 85, 64, 43, 22, 1, 359, 338, 317, 296, 275, 254, 233, 212, 191,
    170, 149, 128, 107, 86, 65, 44, 23, 2, 339, 318, 297, 276, 255, 234, 213, 192, 171, 150,
    129, 108, 87, 66, 45, 24, 3, 319, 298, 277, 256, 235, 214, 193, 172, 151, 130, 109, 88, 67,
    46, 25, 4, 299, 278, 257, 236, 215, 194, 173, 152, 131, 110, 89, 68, 47, 26, 5, 279, 258,
    237, 216, 195, 174, 153, 132, 111, 90, 69, 48, 27, 6, 259, 238, 217, 196, 175, 154, 133,
    112, 91, 70, 49, 28, 7, 239, 218, 197, 176, 155, 134, 113, 92, 71, 50, 29, 8, 219, 198,
    177, 156, 135, 114, 93, 72, 51, 30, 9, 199, 178, 157, 136, 115, 94, 73, 52, 31, 10, 179,
    158, 137, 116, 95, 74, 53, 32, 11, 159, 138, 117, 96, 75, 54, 33, 12, 139, 118, 97, 76, 55,
    34, 13, 119, 98, 77, 56, 35, 14, 99, 78, 57, 36, 15, 79, 58, 37, 16, 59, 38, 17, 39, 18, 19
};

const std::bitset<400> WIN_MASK = {0b11111};


