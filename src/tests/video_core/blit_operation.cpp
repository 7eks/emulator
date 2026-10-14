// SPDX-FileCopyrightText: Copyright 2026 citron Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#include <array>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <ios>
#include <span>

#include <catch2/catch_test_macros.hpp>

#include "common/common_types.h"
#include "video_core/engines/sw_blitter/blitter.h"
#include "video_core/engines/sw_blitter/converter.h"

using Tegra::Engines::Fermi2D;
using Tegra::Engines::Blitter::ApplyOperation;

namespace {
using Pixel = std::array<f32, 4>; // R, G, B, A

Pixel Apply(Fermi2D::Operation operation, u32 beta1, u32 beta4, Pixel src, Pixel dst) {
    ApplyOperation(operation, beta1, beta4, src, dst);
    return dst;
}

bool Near(const Pixel& a, const Pixel& b) {
    for (size_t i = 0; i < a.size(); ++i) {
        if (std::abs(a[i] - b[i]) > 1e-6f) {
            return false;
        }
    }
    return true;
}

constexpr Pixel dst{0.0f, 0.0f, 1.0f, 1.0f};
} // namespace

TEST_CASE("Fermi2D SRCCOPY_AND copies only pixels with alpha", "[video_core]") {
    const Pixel transparent{1.0f, 0.5f, 0.25f, 0.0f};
    const Pixel visible{1.0f, 0.5f, 0.25f, 0.5f};
    REQUIRE(Near(Apply(Fermi2D::Operation::SrcCopyAnd, 0, 0, transparent, dst), dst));
    REQUIRE(Near(Apply(Fermi2D::Operation::SrcCopyAnd, 0, 0, visible, dst), visible));
}

TEST_CASE("Fermi2D BLEND weights the source by SRC.A * BETA1", "[video_core]") {
    const Pixel red{1.0f, 0.0f, 0.0f, 1.0f};
    // BETA1 bits 30:23 = 0x80: a factor of 128 / 255.
    constexpr f32 a = 128.0f / 255.0f;
    REQUIRE(Near(Apply(Fermi2D::Operation::Blend, 0x40000000, 0, red, dst),
                 Pixel{a, 0.0f, 1.0f - a, 1.0f}));
    // A negative BETA1 is 0, which leaves the destination.
    REQUIRE(Near(Apply(Fermi2D::Operation::Blend, 0x80000000, 0, red, dst), dst));
}

TEST_CASE("Fermi2D SRCCOPY_PREMULT scales by BETA4, B in the low byte", "[video_core]") {
    const Pixel white{1.0f, 1.0f, 1.0f, 1.0f};
    REQUIRE(Near(Apply(Fermi2D::Operation::SrcCopyPremult, 0, 0x80408020, white, dst),
                 Pixel{0x40 / 255.0f, 0x80 / 255.0f, 0x20 / 255.0f, 0x80 / 255.0f}));
}

TEST_CASE("Fermi2D BLEND_PREMULT adds SRC * BETA4 over (1 - SRC.A * BETA4.A) * DST",
          "[video_core]") {
    const Pixel half{0.5f, 0.25f, 0.0f, 0.5f};
    REQUIRE(Near(Apply(Fermi2D::Operation::BlendPremult, 0, 0xFFFFFFFF, half, dst),
                 Pixel{0.5f, 0.25f, 0.5f, 1.0f}));
}

TEST_CASE("Fermi2D SRCCOPY and ROP copy the source", "[video_core]") {
    const Pixel src{0.1f, 0.2f, 0.3f, 0.0f};
    REQUIRE(Near(Apply(Fermi2D::Operation::SrcCopy, 0, 0, src, dst), src));
    REQUIRE(Near(Apply(Fermi2D::Operation::ROP, 0, 0, src, dst), src));
}

