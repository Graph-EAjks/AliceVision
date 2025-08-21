// This file is part of the AliceVision project.
// Copyright (c) 2021 AliceVision contributors.
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <aliceVision/sfmData/SfMData.hpp>

namespace aliceVision {
namespace sfmDataIO {

/**
 * @brief Create an SfmData with some arbitrary content.
 * This is used in unit tests to validate read/write sfmData files.
 */
void generateSampleScene(sfmData::SfMData& output, std::string scene="cube", float positionNoise=0., float rotationNoise=0.);

void generateCubeScene(sfmData::SfMData& output);

void generateSphereScene(sfmData::SfMData& output, int pointsNb, int posesNb, float positionNoise=0., float rotationNoise=0.);

}  // namespace sfmDataIO
}  // namespace aliceVision
