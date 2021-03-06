/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2014 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#include <alloy/test/util.h>

#include <cfloat>

using namespace alloy;
using namespace alloy::hir;
using namespace alloy::runtime;
using namespace alloy::test;
using alloy::frontend::ppc::PPCContext;

TEST_CASE("VECTOR_MAX_I8_SIGNED", "[instr]") {
  TestFunction test([](hir::HIRBuilder& b) {
    StoreVR(b, 3, b.VectorMax(LoadVR(b, 4), LoadVR(b, 5), INT8_TYPE));
    b.Return();
  });
  test.Run([](PPCContext* ctx) {
             ctx->v[4] =
                 vec128b(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
             ctx->v[5] = vec128b(-100, 1, 100, -3, 4, -5, 60, 7, -80, 9, 10,
                                 INT8_MIN, INT8_MAX, 13, 2, 0);
           },
           [](PPCContext* ctx) {
             auto result = ctx->v[3];
             REQUIRE(result == vec128b(0, 1, 100, 3, 4, 5, 60, 7, 8, 9, 10, 11,
                                       INT8_MAX, 13, 14, 15));
           });
}

TEST_CASE("VECTOR_MAX_I8_UNSIGNED", "[instr]") {
  TestFunction test([](hir::HIRBuilder& b) {
    StoreVR(b, 3, b.VectorMax(LoadVR(b, 4), LoadVR(b, 5), INT8_TYPE,
                              ARITHMETIC_UNSIGNED));
    b.Return();
  });
  test.Run([](PPCContext* ctx) {
             ctx->v[4] =
                 vec128b(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
             ctx->v[5] = vec128b(-100, 1, 100, -3, 4, -5, 60, 7, -80, 9, 10,
                                 INT8_MIN, INT8_MAX, 13, 2, 0);
           },
           [](PPCContext* ctx) {
             auto result = ctx->v[3];
             REQUIRE(result == vec128b(-100, 1, 100, -3, 4, -5, 60, 7, -80, 9,
                                       10, INT8_MIN, INT8_MAX, 13, 14, 15));
           });
}

TEST_CASE("VECTOR_MAX_I16_SIGNED", "[instr]") {
  TestFunction test([](hir::HIRBuilder& b) {
    StoreVR(b, 3, b.VectorMax(LoadVR(b, 4), LoadVR(b, 5), INT16_TYPE));
    b.Return();
  });
  test.Run([](PPCContext* ctx) {
             ctx->v[4] = vec128s(0, 1, 2, 3, 4, 5, -6000, 7);
             ctx->v[5] = vec128s(-1000, 1, -2000, 3, 4, SHRT_MAX, 6, 0);
           },
           [](PPCContext* ctx) {
             auto result = ctx->v[3];
             REQUIRE(result == vec128s(0, 1, 2, 3, 4, SHRT_MAX, 6, 7));
           });
}

TEST_CASE("VECTOR_MAX_I16_UNSIGNED", "[instr]") {
  TestFunction test([](hir::HIRBuilder& b) {
    StoreVR(b, 3, b.VectorMax(LoadVR(b, 4), LoadVR(b, 5), INT16_TYPE,
                              ARITHMETIC_UNSIGNED));
    b.Return();
  });
  test.Run([](PPCContext* ctx) {
             ctx->v[4] = vec128s(0, 1, 2, 3, 4, 5, -6000, 7);
             ctx->v[5] = vec128s(-1000, 1, -2000, 3, 4, USHRT_MAX, 6, 0);
           },
           [](PPCContext* ctx) {
             auto result = ctx->v[3];
             REQUIRE(result ==
                     vec128s(-1000, 1, -2000, 3, 4, USHRT_MAX, -6000, 7));
           });
}

TEST_CASE("VECTOR_MAX_I32_SIGNED", "[instr]") {
  TestFunction test([](hir::HIRBuilder& b) {
    StoreVR(b, 3, b.VectorMax(LoadVR(b, 4), LoadVR(b, 5), INT32_TYPE));
    b.Return();
  });
  test.Run([](PPCContext* ctx) {
             ctx->v[4] = vec128i(0, 1, 123, 3);
             ctx->v[5] = vec128i(-1000000, 0, INT_MAX, 0);
           },
           [](PPCContext* ctx) {
             auto result = ctx->v[3];
             REQUIRE(result == vec128i(0, 1, INT_MAX, 3));
           });
}

TEST_CASE("VECTOR_MAX_I32_UNSIGNED", "[instr]") {
  TestFunction test([](hir::HIRBuilder& b) {
    StoreVR(b, 3, b.VectorMax(LoadVR(b, 4), LoadVR(b, 5), INT32_TYPE,
                              ARITHMETIC_UNSIGNED));
    b.Return();
  });
  test.Run([](PPCContext* ctx) {
             ctx->v[4] = vec128i(0, 1, 123, 3);
             ctx->v[5] = vec128i(-1000000, 0, UINT_MAX, 0);
           },
           [](PPCContext* ctx) {
             auto result = ctx->v[3];
             REQUIRE(result == vec128i(-1000000, 1, UINT_MAX, 3));
           });
}
