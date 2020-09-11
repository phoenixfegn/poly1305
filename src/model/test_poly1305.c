//======================================================================
//
// test_poly1305.c
// ---------------
// A program to generate test data for Poly1305 using Monocypher.
//
// (c) 2020 Joachim Strombergson.
//
// Redistribution and use in source and binary forms, with or
// without modification, are permitted provided that the following
// conditions are met:
//
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in
//    the documentation and/or other materials provided with the
//    distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//======================================================================

#include <stdio.h>
#include <stdint.h>
#include "monocypher.h"


//------------------------------------------------------------------
// check_tag()
// Check the generated tag against an expected tag.
// The tag is expected to be 16 bytes.
//------------------------------------------------------------------
int check_tag(uint8_t *tag, uint8_t *expected) {
  uint8_t error = 0;
  for (uint8_t i = 0 ; i < 16 ; i++) {
    if (tag[i] != expected[i])
      error = 1;
  }

  if (!error) {
    printf("Correct tag generated.\n");
  }
  else {
    printf("Correct tag NOT generated.\n");
    printf("Expected:\n");
    print_hexdata(&expected[0], 16);
    printf("Got:\n");
    print_hexdata(&tag[0], 16);
  }
  return error;
}


//------------------------------------------------------------------
// p1305_rfc8439()
//
// Test with the test vectors from RFC 8439.
// Se Section 2.5.2.
//------------------------------------------------------------------
int p1305_rfc8439() {

  const uint8_t my_key[32] = {0x85, 0xd6, 0xbe, 0x78, 0x57, 0x55, 0x6d, 0x33,
                              0x7f, 0x44, 0x52, 0xfe, 0x42, 0xd5, 0x06, 0xa8,
                              0x01, 0x03, 0x80, 0x8a, 0xfb, 0x0d, 0xb2, 0xfd,
                              0x4a, 0xbf, 0xf6, 0xaf, 0x41, 0x49, 0xf5, 0x1b};


  const uint8_t my_message[34] = {0x43, 0x72, 0x79, 0x70, 0x74, 0x6f, 0x67, 0x72,
                                  0x61, 0x70, 0x68, 0x69, 0x63, 0x20, 0x46, 0x6f,
                                  0x72, 0x75, 0x6d, 0x20, 0x52, 0x65, 0x73, 0x65,
                                  0x61, 0x72, 0x63, 0x68, 0x20, 0x47, 0x72, 0x6f,
                                  0x75, 0x70};

  uint8_t my_expected[16] = {0xa8, 0x06, 0x1d, 0xc1, 0x30, 0x51, 0x36, 0xc6,
                             0xc2, 0x2b, 0x8b, 0xaf, 0x0c, 0x01, 0x27, 0xa9};

  uint8_t my_tag[16];
  crypto_poly1305_ctx my_ctx;

  int res;

  printf("\nTest p1305_rfc8439 started. Check that the RFC test vectors work.\n");

  printf("Calling poly1305_init()\n");
  crypto_poly1305_init(&my_ctx, &my_key[0]);
  printf("Context after poly1305_init()\n");
  print_context(&my_ctx);

  printf("Calling poly1305_update() with 34 byte message.\n");
  crypto_poly1305_update(&my_ctx, &my_message[0], 34);
  printf("Context after poly1305_update()\n");
  print_context(&my_ctx);

  printf("Calling poly1305_final() to get tag.\n");
  crypto_poly1305_final(&my_ctx, &my_tag[0]);
  printf("Context after poly1305_final()\n");
  print_context(&my_ctx);

  res = check_tag(&my_tag[0], &my_expected[0]);
  printf("\nTest p1305_rfc8439 completed.\n\n");
  return res;
}


//------------------------------------------------------------------
// p1305_bytes1()
//
// Test with a single byte message. Key is from the RFC.
// Se Section 2.5.2.
//------------------------------------------------------------------
int p1305_bytes1() {

  const uint8_t my_key[32] = {0x85, 0xd6, 0xbe, 0x78, 0x57, 0x55, 0x6d, 0x33,
                              0x7f, 0x44, 0x52, 0xfe, 0x42, 0xd5, 0x06, 0xa8,
                              0x01, 0x03, 0x80, 0x8a, 0xfb, 0x0d, 0xb2, 0xfd,
                              0x4a, 0xbf, 0xf6, 0xaf, 0x41, 0x49, 0xf5, 0x1b};


  const uint8_t my_message[34] = {0x01};

  uint8_t my_expected[16] = {0x90, 0x5e, 0x15, 0x52, 0x58, 0xb7, 0x74, 0x6e,
                             0xca, 0x7f, 0x8d, 0x10, 0x90, 0x5e, 0xd1, 0x2a};

  uint8_t my_tag[16];
  crypto_poly1305_ctx my_ctx;

  int res;

  printf("\nTest p1305_byte1 started.\n");

  printf("Test p1305_byte1: Calling poly1305_init()\n");
  crypto_poly1305_init(&my_ctx, &my_key[0]);
  printf("Test p1305_byte1: Context after poly1305_init()\n");
  print_context(&my_ctx);

  printf("Test p1305_byte1: Calling poly1305_update() with 1 byte message.\n");
  crypto_poly1305_update(&my_ctx, &my_message[0], 1);
  printf("Test p1305_byte1: Context after poly1305_update()\n");
  print_context(&my_ctx);

  printf("Test p1305_byte1: Calling poly1305_final() to get tag.\n");
  crypto_poly1305_final(&my_ctx, &my_tag[0]);
  printf("Test p1305_byte1: Context after poly1305_final()\n");
  print_context(&my_ctx);
  printf("Test p1305_byte1: The generated tag:\n");
  print_hexdata(&my_tag[0], 16);

  res = check_tag(&my_tag[0], &my_expected[0]);
  printf("Test p1305_byte1 completed.\n");
  return res;
}


//------------------------------------------------------------------
// p1305_test2()
//
// Test that we can get a mac for a message with multiple blocks.
//------------------------------------------------------------------
int p1305_test2() {
  const uint8_t my_key[32] = {0xde, 0xde, 0xde, 0xde, 0xde, 0xde, 0xde, 0xde,
                              0xde, 0xde, 0xde, 0xde, 0xde, 0xde, 0xde, 0xde,
                              0xde, 0xde, 0xde, 0xde, 0xde, 0xde, 0xde, 0xde,
                              0xde, 0xde, 0xde, 0xde, 0xde, 0xde, 0xde, 0xde};

  const uint8_t my_indata[32] = {0xab, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55,
                                 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55,
                                 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55,
                                 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55};

  uint8_t my_expected[16] = {0x70, 0x30, 0xe6, 0x86, 0x63, 0x34, 0x27, 0x1e,
                             0x22, 0x1c, 0x01, 0xd8, 0x16, 0x81, 0x2a, 0xfe};

  uint8_t my_tag[16];

  crypto_poly1305_ctx my_ctx;

  printf("p1305_test2. A multiblock message.\n");

  crypto_poly1305_init(&my_ctx, &my_key[0]);
  crypto_poly1305_update(&my_ctx, &my_indata[0], 32);
  crypto_poly1305_update(&my_ctx, &my_indata[0], 32);
  crypto_poly1305_update(&my_ctx, &my_indata[0], 32);
  crypto_poly1305_final(&my_ctx, &my_tag[0]);

  return check_tag(&my_tag[0], &my_expected[0]);
}


//------------------------------------------------------------------
// p1305_test1()
//
// A first simple test that we get a non-zero tag for a block.
// This function uses the crypto_poly1305 function.
//------------------------------------------------------------------
int p1305_test1() {
  uint8_t my_key[32] = {0xde, 0xde, 0xde, 0xde, 0xde, 0xde, 0xde, 0xde,
                        0xde, 0xde, 0xde, 0xde, 0xde, 0xde, 0xde, 0xde,
                        0xde, 0xde, 0xde, 0xde, 0xde, 0xde, 0xde, 0xde,
                        0xde, 0xde, 0xde, 0xde, 0xde, 0xde, 0xde, 0xde};

  uint8_t my_indata[32] = {0xab, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55,
                           0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55,
                           0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55,
                           0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55};

  uint8_t my_expected[16] = {0x2c, 0xdb, 0x68, 0xe9, 0xde, 0xc7, 0xb7, 0x3e,
                             0x93, 0xba, 0x00, 0x9a, 0x41, 0xb3, 0x43, 0xfb};

  uint8_t my_tag[16];

  printf("p1305_test1. A simple one block message.\n");
  crypto_poly1305(my_tag, &my_indata[0], 32, &my_key[0]);

  printf("Generated tag:\n");
  print_hexdata(&my_tag[0], 16);
  return check_tag(&my_tag[0], &my_expected[0]);
}


//------------------------------------------------------------------
//------------------------------------------------------------------
int testcase_0() {
  const uint8_t my_key[32] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  const uint8_t my_message[0] = {};

  uint8_t my_expected[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  uint8_t my_tag[16];
  crypto_poly1305_ctx my_ctx;

  crypto_poly1305_init(&my_ctx, &my_key[0]);
  crypto_poly1305_update(&my_ctx, &my_message[0], 0);
  crypto_poly1305_final(&my_ctx, &my_tag[0]);
  return check_tag(&my_tag[0], &my_expected[0]);
}


//------------------------------------------------------------------
//------------------------------------------------------------------
int testcase_1() {
  const uint8_t my_key[32] = {0x36, 0xe5, 0xf6, 0xb5, 0xc5, 0xe0, 0x60, 0x70,
                              0xf0, 0xef, 0xca, 0x96, 0x22, 0x7a, 0x86, 0x3e,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  const uint8_t my_message[0] = {};

  uint8_t my_expected[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  uint8_t my_tag[16];
  crypto_poly1305_ctx my_ctx;

  crypto_poly1305_init(&my_ctx, &my_key[0]);
  crypto_poly1305_update(&my_ctx, &my_message[0], 0);
  crypto_poly1305_final(&my_ctx, &my_tag[0]);
  return check_tag(&my_tag[0], &my_expected[0]);
}


//------------------------------------------------------------------
//------------------------------------------------------------------
int testcase_2() {
  const uint8_t my_key[32] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x36, 0xe5, 0xf6, 0xb5, 0xc5, 0xe0, 0x60, 0x70,
                              0xf0, 0xef, 0xca, 0x96, 0x22, 0x7a, 0x86, 0x3e};

  const uint8_t my_message[0] = {};

  uint8_t my_expected[16] = {0x36, 0xe5, 0xf6, 0xb5, 0xc5, 0xe0, 0x60, 0x70,
                             0xf0, 0xef, 0xca, 0x96, 0x22, 0x7a, 0x86, 0x3e};

  uint8_t my_tag[16];
  crypto_poly1305_ctx my_ctx;

  crypto_poly1305_init(&my_ctx, &my_key[0]);
  crypto_poly1305_update(&my_ctx, &my_message[0], 0);
  crypto_poly1305_final(&my_ctx, &my_tag[0]);
  return check_tag(&my_tag[0], &my_expected[0]);
}


//------------------------------------------------------------------
//------------------------------------------------------------------
int testcase_3() {
  const uint8_t my_key[32] = {0x79, 0x20, 0x73, 0x75, 0x62, 0x6d, 0x69, 0x73,
                              0x73, 0x69, 0x6f, 0x6e, 0x20, 0x74, 0x6f, 0x20,
                              0x36, 0xe5, 0xf6, 0xb5, 0xc5, 0xe0, 0x60, 0x70,
                              0xf0, 0xef, 0xca, 0x96, 0x22, 0x7a, 0x86, 0x3e};

  const uint8_t my_message[0] = {};

  uint8_t my_expected[16] = {0x36, 0xe5, 0xf6, 0xb5, 0xc5, 0xe0, 0x60, 0x70,
                             0xf0, 0xef, 0xca, 0x96, 0x22, 0x7a, 0x86, 0x3e};

  uint8_t my_tag[16];
  crypto_poly1305_ctx my_ctx;

  crypto_poly1305_init(&my_ctx, &my_key[0]);
  crypto_poly1305_update(&my_ctx, &my_message[0], 0);
  crypto_poly1305_final(&my_ctx, &my_tag[0]);
  return check_tag(&my_tag[0], &my_expected[0]);
}


//------------------------------------------------------------------
//------------------------------------------------------------------
int testcase_4() {
  const uint8_t my_key[32] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  const uint8_t my_message[64] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  uint8_t my_expected[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  uint8_t my_tag[16];
  crypto_poly1305_ctx my_ctx;

  crypto_poly1305_init(&my_ctx, &my_key[0]);
  crypto_poly1305_update(&my_ctx, &my_message[0], 64);
  crypto_poly1305_final(&my_ctx, &my_tag[0]);
  return check_tag(&my_tag[0], &my_expected[0]);
}


//------------------------------------------------------------------
//------------------------------------------------------------------
int testcase_5() {
  const uint8_t my_key[32] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x36, 0xe5, 0xf6, 0xb5, 0xc5, 0xe0, 0x60, 0x70,
                              0xf0, 0xef, 0xca, 0x96, 0x22, 0x7a, 0x86, 0x3e};

  const uint8_t my_message[375] = {0x41, 0x6e, 0x79, 0x20, 0x73, 0x75, 0x62, 0x6d,
                                   0x69, 0x73, 0x73, 0x69, 0x6f, 0x6e, 0x20, 0x74,
                                   0x6f, 0x20, 0x74, 0x68, 0x65, 0x20, 0x49, 0x45,
                                   0x54, 0x46, 0x20, 0x69, 0x6e, 0x74, 0x65, 0x6e,
                                   0x64, 0x65, 0x64, 0x20, 0x62, 0x79, 0x20, 0x74,
                                   0x68, 0x65, 0x20, 0x43, 0x6f, 0x6e, 0x74, 0x72,
                                   0x69, 0x62, 0x75, 0x74, 0x6f, 0x72, 0x20, 0x66,
                                   0x6f, 0x72, 0x20, 0x70, 0x75, 0x62, 0x6c, 0x69,
                                   0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x61,
                                   0x73, 0x20, 0x61, 0x6c, 0x6c, 0x20, 0x6f, 0x72,
                                   0x20, 0x70, 0x61, 0x72, 0x74, 0x20, 0x6f, 0x66,
                                   0x20, 0x61, 0x6e, 0x20, 0x49, 0x45, 0x54, 0x46,
                                   0x20, 0x49, 0x6e, 0x74, 0x65, 0x72, 0x6e, 0x65,
                                   0x74, 0x2d, 0x44, 0x72, 0x61, 0x66, 0x74, 0x20,
                                   0x6f, 0x72, 0x20, 0x52, 0x46, 0x43, 0x20, 0x61,
                                   0x6e, 0x64, 0x20, 0x61, 0x6e, 0x79, 0x20, 0x73,
                                   0x74, 0x61, 0x74, 0x65, 0x6d, 0x65, 0x6e, 0x74,
                                   0x20, 0x6d, 0x61, 0x64, 0x65, 0x20, 0x77, 0x69,
                                   0x74, 0x68, 0x69, 0x6e, 0x20, 0x74, 0x68, 0x65,
                                   0x20, 0x63, 0x6f, 0x6e, 0x74, 0x65, 0x78, 0x74,
                                   0x20, 0x6f, 0x66, 0x20, 0x61, 0x6e, 0x20, 0x49,
                                   0x45, 0x54, 0x46, 0x20, 0x61, 0x63, 0x74, 0x69,
                                   0x76, 0x69, 0x74, 0x79, 0x20, 0x69, 0x73, 0x20,
                                   0x63, 0x6f, 0x6e, 0x73, 0x69, 0x64, 0x65, 0x72,
                                   0x65, 0x64, 0x20, 0x61, 0x6e, 0x20, 0x22, 0x49,
                                   0x45, 0x54, 0x46, 0x20, 0x43, 0x6f, 0x6e, 0x74,
                                   0x72, 0x69, 0x62, 0x75, 0x74, 0x69, 0x6f, 0x6e,
                                   0x22, 0x2e, 0x20, 0x53, 0x75, 0x63, 0x68, 0x20,
                                   0x73, 0x74, 0x61, 0x74, 0x65, 0x6d, 0x65, 0x6e,
                                   0x74, 0x73, 0x20, 0x69, 0x6e, 0x63, 0x6c, 0x75,
                                   0x64, 0x65, 0x20, 0x6f, 0x72, 0x61, 0x6c, 0x20,
                                   0x73, 0x74, 0x61, 0x74, 0x65, 0x6d, 0x65, 0x6e,
                                   0x74, 0x73, 0x20, 0x69, 0x6e, 0x20, 0x49, 0x45,
                                   0x54, 0x46, 0x20, 0x73, 0x65, 0x73, 0x73, 0x69,
                                   0x6f, 0x6e, 0x73, 0x2c, 0x20, 0x61, 0x73, 0x20,
                                   0x77, 0x65, 0x6c, 0x6c, 0x20, 0x61, 0x73, 0x20,
                                   0x77, 0x72, 0x69, 0x74, 0x74, 0x65, 0x6e, 0x20,
                                   0x61, 0x6e, 0x64, 0x20, 0x65, 0x6c, 0x65, 0x63,
                                   0x74, 0x72, 0x6f, 0x6e, 0x69, 0x63, 0x20, 0x63,
                                   0x6f, 0x6d, 0x6d, 0x75, 0x6e, 0x69, 0x63, 0x61,
                                   0x74, 0x69, 0x6f, 0x6e, 0x73, 0x20, 0x6d, 0x61,
                                   0x64, 0x65, 0x20, 0x61, 0x74, 0x20, 0x61, 0x6e,
                                   0x79, 0x20, 0x74, 0x69, 0x6d, 0x65, 0x20, 0x6f,
                                   0x72, 0x20, 0x70, 0x6c, 0x61, 0x63, 0x65, 0x2c,
                                   0x20, 0x77, 0x68, 0x69, 0x63, 0x68, 0x20, 0x61,
                                   0x72, 0x65, 0x20, 0x61, 0x64, 0x64, 0x72, 0x65,
                                   0x73, 0x73, 0x65, 0x64, 0x20, 0x74, 0x6f};

  uint8_t my_expected[16] = {0x36, 0xe5, 0xf6, 0xb5, 0xc5, 0xe0, 0x60, 0x70,
                             0xf0, 0xef, 0xca, 0x96, 0x22, 0x7a, 0x86, 0x3e};

  uint8_t my_tag[16];
  crypto_poly1305_ctx my_ctx;

  crypto_poly1305_init(&my_ctx, &my_key[0]);
  crypto_poly1305_update(&my_ctx, &my_message[0], 375);
  crypto_poly1305_final(&my_ctx, &my_tag[0]);
  return check_tag(&my_tag[0], &my_expected[0]);
}


//------------------------------------------------------------------
//------------------------------------------------------------------
int testcase_6() {
  const uint8_t my_key[32] = {0x36, 0xe5, 0xf6, 0xb5, 0xc5, 0xe0, 0x60, 0x70,
                              0xf0, 0xef, 0xca, 0x96, 0x22, 0x7a, 0x86, 0x3e,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  const uint8_t my_message[375] = {0x41, 0x6e, 0x79, 0x20, 0x73, 0x75, 0x62, 0x6d,
                                   0x69, 0x73, 0x73, 0x69, 0x6f, 0x6e, 0x20, 0x74,
                                   0x6f, 0x20, 0x74, 0x68, 0x65, 0x20, 0x49, 0x45,
                                   0x54, 0x46, 0x20, 0x69, 0x6e, 0x74, 0x65, 0x6e,
                                   0x64, 0x65, 0x64, 0x20, 0x62, 0x79, 0x20, 0x74,
                                   0x68, 0x65, 0x20, 0x43, 0x6f, 0x6e, 0x74, 0x72,
                                   0x69, 0x62, 0x75, 0x74, 0x6f, 0x72, 0x20, 0x66,
                                   0x6f, 0x72, 0x20, 0x70, 0x75, 0x62, 0x6c, 0x69,
                                   0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x61,
                                   0x73, 0x20, 0x61, 0x6c, 0x6c, 0x20, 0x6f, 0x72,
                                   0x20, 0x70, 0x61, 0x72, 0x74, 0x20, 0x6f, 0x66,
                                   0x20, 0x61, 0x6e, 0x20, 0x49, 0x45, 0x54, 0x46,
                                   0x20, 0x49, 0x6e, 0x74, 0x65, 0x72, 0x6e, 0x65,
                                   0x74, 0x2d, 0x44, 0x72, 0x61, 0x66, 0x74, 0x20,
                                   0x6f, 0x72, 0x20, 0x52, 0x46, 0x43, 0x20, 0x61,
                                   0x6e, 0x64, 0x20, 0x61, 0x6e, 0x79, 0x20, 0x73,
                                   0x74, 0x61, 0x74, 0x65, 0x6d, 0x65, 0x6e, 0x74,
                                   0x20, 0x6d, 0x61, 0x64, 0x65, 0x20, 0x77, 0x69,
                                   0x74, 0x68, 0x69, 0x6e, 0x20, 0x74, 0x68, 0x65,
                                   0x20, 0x63, 0x6f, 0x6e, 0x74, 0x65, 0x78, 0x74,
                                   0x20, 0x6f, 0x66, 0x20, 0x61, 0x6e, 0x20, 0x49,
                                   0x45, 0x54, 0x46, 0x20, 0x61, 0x63, 0x74, 0x69,
                                   0x76, 0x69, 0x74, 0x79, 0x20, 0x69, 0x73, 0x20,
                                   0x63, 0x6f, 0x6e, 0x73, 0x69, 0x64, 0x65, 0x72,
                                   0x65, 0x64, 0x20, 0x61, 0x6e, 0x20, 0x22, 0x49,
                                   0x45, 0x54, 0x46, 0x20, 0x43, 0x6f, 0x6e, 0x74,
                                   0x72, 0x69, 0x62, 0x75, 0x74, 0x69, 0x6f, 0x6e,
                                   0x22, 0x2e, 0x20, 0x53, 0x75, 0x63, 0x68, 0x20,
                                   0x73, 0x74, 0x61, 0x74, 0x65, 0x6d, 0x65, 0x6e,
                                   0x74, 0x73, 0x20, 0x69, 0x6e, 0x63, 0x6c, 0x75,
                                   0x64, 0x65, 0x20, 0x6f, 0x72, 0x61, 0x6c, 0x20,
                                   0x73, 0x74, 0x61, 0x74, 0x65, 0x6d, 0x65, 0x6e,
                                   0x74, 0x73, 0x20, 0x69, 0x6e, 0x20, 0x49, 0x45,
                                   0x54, 0x46, 0x20, 0x73, 0x65, 0x73, 0x73, 0x69,
                                   0x6f, 0x6e, 0x73, 0x2c, 0x20, 0x61, 0x73, 0x20,
                                   0x77, 0x65, 0x6c, 0x6c, 0x20, 0x61, 0x73, 0x20,
                                   0x77, 0x72, 0x69, 0x74, 0x74, 0x65, 0x6e, 0x20,
                                   0x61, 0x6e, 0x64, 0x20, 0x65, 0x6c, 0x65, 0x63,
                                   0x74, 0x72, 0x6f, 0x6e, 0x69, 0x63, 0x20, 0x63,
                                   0x6f, 0x6d, 0x6d, 0x75, 0x6e, 0x69, 0x63, 0x61,
                                   0x74, 0x69, 0x6f, 0x6e, 0x73, 0x20, 0x6d, 0x61,
                                   0x64, 0x65, 0x20, 0x61, 0x74, 0x20, 0x61, 0x6e,
                                   0x79, 0x20, 0x74, 0x69, 0x6d, 0x65, 0x20, 0x6f,
                                   0x72, 0x20, 0x70, 0x6c, 0x61, 0x63, 0x65, 0x2c,
                                   0x20, 0x77, 0x68, 0x69, 0x63, 0x68, 0x20, 0x61,
                                   0x72, 0x65, 0x20, 0x61, 0x64, 0x64, 0x72, 0x65,
                                   0x73, 0x73, 0x65, 0x64, 0x20, 0x74, 0x6f, };

  uint8_t my_expected[16] = {0xf3, 0x47, 0x7e, 0x7c, 0xd9, 0x54, 0x17, 0xaf,
                             0x89, 0xa6, 0xb8, 0x79, 0x4c, 0x31, 0x0c, 0xf0};

  uint8_t my_tag[16];
  crypto_poly1305_ctx my_ctx;

  crypto_poly1305_init(&my_ctx, &my_key[0]);
  crypto_poly1305_update(&my_ctx, &my_message[0], 375);
  crypto_poly1305_final(&my_ctx, &my_tag[0]);
  return check_tag(&my_tag[0], &my_expected[0]);
}


int testcase_7() {
  const uint8_t my_key[32] = {0x1c, 0x92, 0x40, 0xa5, 0xeb, 0x55, 0xd3, 0x8a,
                              0xf3, 0x33, 0x88, 0x86, 0x04, 0xf6, 0xb5, 0xf0,
                              0x47, 0x39, 0x17, 0xc1, 0x40, 0x2b, 0x80, 0x09,
                              0x9d, 0xca, 0x5c, 0xbc, 0x20, 0x70, 0x75, 0xc0};

  const uint8_t my_message[127] = {0x27, 0x54, 0x77, 0x61, 0x73, 0x20, 0x62, 0x72,
                                   0x69, 0x6c, 0x6c, 0x69, 0x67, 0x2c, 0x20, 0x61,
                                   0x6e, 0x64, 0x20, 0x74, 0x68, 0x65, 0x20, 0x73,
                                   0x6c, 0x69, 0x74, 0x68, 0x79, 0x20, 0x74, 0x6f,
                                   0x76, 0x65, 0x73, 0x0a, 0x44, 0x69, 0x64, 0x20,
                                   0x67, 0x79, 0x72, 0x65, 0x20, 0x61, 0x6e, 0x64,
                                   0x20, 0x67, 0x69, 0x6d, 0x62, 0x6c, 0x65, 0x20,
                                   0x69, 0x6e, 0x20, 0x74, 0x68, 0x65, 0x20, 0x77,
                                   0x61, 0x62, 0x65, 0x3a, 0x0a, 0x41, 0x6c, 0x6c,
                                   0x20, 0x6d, 0x69, 0x6d, 0x73, 0x79, 0x20, 0x77,
                                   0x65, 0x72, 0x65, 0x20, 0x74, 0x68, 0x65, 0x20,
                                   0x62, 0x6f, 0x72, 0x6f, 0x67, 0x6f, 0x76, 0x65,
                                   0x73, 0x2c, 0x0a, 0x41, 0x6e, 0x64, 0x20, 0x74,
                                   0x68, 0x65, 0x20, 0x6d, 0x6f, 0x6d, 0x65, 0x20,
                                   0x72, 0x61, 0x74, 0x68, 0x73, 0x20, 0x6f, 0x75,
                                   0x74, 0x67, 0x72, 0x61, 0x62, 0x65, 0x2e};

  uint8_t my_expected[16] = {0x45, 0x41, 0x66, 0x9a, 0x7e, 0xaa, 0xee, 0x61,
                             0xe7, 0x08, 0xdc, 0x7c, 0xbc, 0xc5, 0xeb, 0x62};

  uint8_t my_tag[16];
  crypto_poly1305_ctx my_ctx;

  crypto_poly1305_init(&my_ctx, &my_key[0]);
  crypto_poly1305_update(&my_ctx, &my_message[0], 127);
  crypto_poly1305_final(&my_ctx, &my_tag[0]);
  return check_tag(&my_tag[0], &my_expected[0]);
}


int testcase_8() {
  const uint8_t my_key[32] = {0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  const uint8_t my_message[16] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

  uint8_t my_expected[16] = {0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  uint8_t my_tag[16];
  crypto_poly1305_ctx my_ctx;

  crypto_poly1305_init(&my_ctx, &my_key[0]);
  crypto_poly1305_update(&my_ctx, &my_message[0], 16);
  crypto_poly1305_final(&my_ctx, &my_tag[0]);
  return check_tag(&my_tag[0], &my_expected[0]);
}


int testcase_9() {
  const uint8_t my_key[32] = {0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                              0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

  const uint8_t my_message[16] = {0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  uint8_t my_expected[16] = {0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  uint8_t my_tag[16];
  crypto_poly1305_ctx my_ctx;

  crypto_poly1305_init(&my_ctx, &my_key[0]);
  crypto_poly1305_update(&my_ctx, &my_message[0], 16);
  crypto_poly1305_final(&my_ctx, &my_tag[0]);
  return check_tag(&my_tag[0], &my_expected[0]);
}


int testcase_10() {
  const uint8_t my_key[32] = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  const uint8_t my_message[48] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  uint8_t my_expected[16] = {0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  uint8_t my_tag[16];
  crypto_poly1305_ctx my_ctx;

  crypto_poly1305_init(&my_ctx, &my_key[0]);
  crypto_poly1305_update(&my_ctx, &my_message[0], 48);
  crypto_poly1305_final(&my_ctx, &my_tag[0]);
  return check_tag(&my_tag[0], &my_expected[0]);
}


int testcase_11() {
  const uint8_t my_key[32] = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  const uint8_t my_message[48] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xfb, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe,
                                  0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe,
                                  0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
                                  0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01};

  uint8_t my_expected[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  uint8_t my_tag[16];
  crypto_poly1305_ctx my_ctx;

  crypto_poly1305_init(&my_ctx, &my_key[0]);
  crypto_poly1305_update(&my_ctx, &my_message[0], 48);
  crypto_poly1305_final(&my_ctx, &my_tag[0]);
  return check_tag(&my_tag[0], &my_expected[0]);
}


int testcase_12() {
  const uint8_t my_key[32] = {0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              };

  const uint8_t my_message[16] = {0xfd, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  };

  uint8_t my_expected[16] = {0xfa, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                             0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                             };

  uint8_t my_tag[16];
  crypto_poly1305_ctx my_ctx;

  crypto_poly1305_init(&my_ctx, &my_key[0]);
  crypto_poly1305_update(&my_ctx, &my_message[0], 16);
  crypto_poly1305_final(&my_ctx, &my_tag[0]);
  return check_tag(&my_tag[0], &my_expected[0]);
}


int testcase_13() {
  const uint8_t my_key[32] = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  const uint8_t my_message[64] = {0xe3, 0x35, 0x94, 0xd7, 0x50, 0x5e, 0x43, 0xb9,
                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                  0x33, 0x94, 0xd7, 0x50, 0x5e, 0x43, 0x79, 0xcd,
                                  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  uint8_t my_expected[16] = {0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                             0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  uint8_t my_tag[16];
  crypto_poly1305_ctx my_ctx;

  crypto_poly1305_init(&my_ctx, &my_key[0]);
  crypto_poly1305_update(&my_ctx, &my_message[0], 64);
  crypto_poly1305_final(&my_ctx, &my_tag[0]);
  return check_tag(&my_tag[0], &my_expected[0]);
}


int testcase_14() {
  const uint8_t my_key[32] = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  const uint8_t my_message[48] = {0xe3, 0x35, 0x94, 0xd7, 0x50, 0x5e, 0x43, 0xb9,
                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                  0x33, 0x94, 0xd7, 0x50, 0x5e, 0x43, 0x79, 0xcd,
                                  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  uint8_t my_expected[16] = {0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  uint8_t my_tag[16];
  crypto_poly1305_ctx my_ctx;

  crypto_poly1305_init(&my_ctx, &my_key[0]);
  crypto_poly1305_update(&my_ctx, &my_message[0], 48);
  crypto_poly1305_final(&my_ctx, &my_tag[0]);
  return check_tag(&my_tag[0], &my_expected[0]);
}


int testcase_15() {
  const uint8_t my_key[32] = {0x85, 0xd6, 0xbe, 0x78, 0x57, 0x55, 0x6d, 0x33,
                              0x7f, 0x44, 0x52, 0xfe, 0x42, 0xd5, 0x06, 0xa8,
                              0x01, 0x03, 0x80, 0x8a, 0xfb, 0x0d, 0xb2, 0xfd,
                              0x4a, 0xbf, 0xf6, 0xaf, 0x41, 0x49, 0xf5, 0x1b};

  const uint8_t my_message[34] = {0x43, 0x72, 0x79, 0x70, 0x74, 0x6f, 0x67, 0x72,
                                  0x61, 0x70, 0x68, 0x69, 0x63, 0x20, 0x46, 0x6f,
                                  0x72, 0x75, 0x6d, 0x20, 0x52, 0x65, 0x73, 0x65,
                                  0x61, 0x72, 0x63, 0x68, 0x20, 0x47, 0x72, 0x6f,
                                  0x75, 0x70};

  uint8_t my_expected[16] = {0xa8, 0x06, 0x1d, 0xc1, 0x30, 0x51, 0x36, 0xc6,
                             0xc2, 0x2b, 0x8b, 0xaf, 0x0c, 0x01, 0x27, 0xa9};

  uint8_t my_tag[16];
  crypto_poly1305_ctx my_ctx;

  crypto_poly1305_init(&my_ctx, &my_key[0]);
  crypto_poly1305_update(&my_ctx, &my_message[0], 34);
  crypto_poly1305_final(&my_ctx, &my_tag[0]);
  return check_tag(&my_tag[0], &my_expected[0]);
}


//------------------------------------------------------------------
//------------------------------------------------------------------
void run_tests() {
  int test_result = 0;

//  test_result += p1305_rfc8439();
  test_result += p1305_bytes1();
//  test_result += p1305_test1();
//  test_result += p1305_test2();
//  test_result += testcase_0();
//  test_result += testcase_1();
//  test_result += testcase_2();
//  test_result += testcase_3();
//  test_result += testcase_4();
//  test_result += testcase_5();
//  test_result += testcase_6();
//  test_result += testcase_7();
//  test_result += testcase_8();
//  test_result += testcase_9();
//  test_result += testcase_10();
//  test_result += testcase_11();
//  test_result += testcase_12();
//  test_result += testcase_13();
//  test_result += testcase_14();
//  test_result += testcase_15();

  printf("Number of failing test cases: %d\n", test_result);
}


//------------------------------------------------------------------
// int main()
//------------------------------------------------------------------
int main(void) {
  printf("Test of Monocypher Poly1305 function.\n");
  run_tests();

  return 0;
}

//======================================================================
// EOF test_poly1305.c
//======================================================================
