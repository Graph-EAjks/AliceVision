// This file is part of the AliceVision project.
// Copyright (c) 2025 AliceVision contributors.
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <aliceVision/sfmData/SfMData.hpp>

namespace aliceVision {
namespace sfmDataIO {

/**
* @brief export sfmData to an USD file. 
* All internal data are removed and we only keep simple pointcloud and camera animation
* @param sfmData the input SfmData to export
* @param usdFilename the output file path
* @return false if some problem appear
*/
bool exportAnimatedUSD(const sfmData::SfMData & sfmData, const std::string & usdFilename);

}  // namespace sfmDataIO
}  // namespace aliceVision
