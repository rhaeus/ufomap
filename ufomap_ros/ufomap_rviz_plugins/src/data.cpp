/*!
 * UFOMap: An Efficient Probabilistic 3D Mapping Framework That Embraces the Unknown
 *
 * @author Daniel Duberg (dduberg@kth.se)
 * @see https://github.com/UnknownFreeOccupied/ufomap
 * @version 1.0
 * @date 2022-05-13
 *
 * @copyright Copyright (c) 2022, Daniel Duberg, KTH Royal Institute of Technology
 *
 * BSD 3-Clause License
 *
 * Copyright (c) 2022, Daniel Duberg, KTH Royal Institute of Technology
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *     list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *     contributors may be used to endorse or promote products derived from
 *     this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

// UFO
#include <ufomap_rviz_plugins/data.h>

namespace ufomap_ros::rviz_plugins
{
Data::Data()
    : min_position_(std::numeric_limits<Ogre::Real>::max(),
                    std::numeric_limits<Ogre::Real>::max(),
                    std::numeric_limits<Ogre::Real>::max()),
      max_position_(std::numeric_limits<Ogre::Real>::lowest(),
                    std::numeric_limits<Ogre::Real>::lowest(),
                    std::numeric_limits<Ogre::Real>::lowest()),
      min_time_(std::numeric_limits<ufo::map::time_t>::max()),
      max_time_(std::numeric_limits<ufo::map::time_t>::lowest())
{
}

void Data::swap(Data& other)
{
	occupancy_.swap(other.occupancy_);
	time_.swap(other.time_);
	color_.swap(other.color_);
	semantics_.swap(other.semantics_);
}

std::vector<Voxels::Voxel> Data::generateVoxels(RenderMode const& render,
                                                Filter const& filter, double size,
                                                Heatmap const& heatmap) const
{
	std::vector<Voxels::Voxel> voxels;
	voxels.reserve(position_.size());

	for (size_t i = 0; i < position_.size(); ++i) {
		if (includeVoxel(filter, size, i)) {
			voxels.emplace_back(position_[i], getColor(render, heatmap, i));
		}
	}

	return voxels;
}

void Data::clear()
{
	occupancy_.clear();
	time_.clear();
	color_.clear();
	semantics_.clear();
}

bool Data::includeVoxel(Filter const& filter, double size, size_t index) const
{
	if (filter.filter_occupancy && (occupancy_.size() == position_.size()) &&
	    (filter.min_occupancy > occupancy_[index] ||
	     occupancy_[index] > filter.max_occupancy)) {
		return false;
	}

	if (filter.filter_time && (time_.size() == position_.size()) &&
	    (filter.min_time > time_[index] || time_[index] > filter.max_time)) {
		return false;
	}

	if (filter.filter_bounding_volume) {
		ufo::geometry::AAEBB aaebb(position_[index][0], position_[index][1],
		                           position_[index][2], size / 2.0);

		if (!ufo::geometry::intersects(aaebb, filter.bounding_volume)) {
			return false;
		}
	}

	return true;
}

Ogre::ColourValue Data::getColor(RenderMode const& render, Heatmap const& heatmap,
                                 size_t index) const
{
	// FIXME: Use alpha?

	switch (render.coloring_mode) {
		case ColoringMode::FIXED:
			return render.color;
		case ColoringMode::X_AXIS:
			return render.normalized_value
			           ? Heatmap::getColor(position_[index].x, heatmap.min_position.x,
			                               heatmap.max_position.x, render.color_factor)
			           : Heatmap::getColor(position_[index].x, render.min_normalized_value,
			                               render.max_normalized_value, render.color_factor);
		case ColoringMode::Y_AXIS:
			return render.normalized_value
			           ? Heatmap::getColor(position_[index].y, heatmap.min_position.y,
			                               heatmap.max_position.y, render.color_factor)
			           : Heatmap::getColor(position_[index].y, render.min_normalized_value,
			                               render.max_normalized_value, render.color_factor);
		case ColoringMode::Z_AXIS:
			return render.normalized_value
			           ? Heatmap::getColor(position_[index].z, heatmap.min_position.z,
			                               heatmap.max_position.z, render.color_factor)
			           : Heatmap::getColor(position_[index].z, render.min_normalized_value,
			                               render.max_normalized_value, render.color_factor);
		case ColoringMode::TIME:
			assert(position_.size() == time_.size());
			return render.normalized_value
			           ? Heatmap::getColor(time_[index], heatmap.min_time, heatmap.max_time,
			                               render.color_factor)
			           : Heatmap::getColor(time_[index], render.min_normalized_value,
			                               render.max_normalized_value, render.color_factor);
		case ColoringMode::OCCUPANCY:
			assert(position_.size() == occupancy_.size());
			return Heatmap::getColor(occupancy_[index], 0, 100, render.color_factor);
		case ColoringMode::VOXEL:
			assert(position_.size() == color_.size());
			return color_[index];
		case ColoringMode::SEMANTIC:
		case ColoringMode::SURFEL_NORMAL:
		case ColoringMode::COUNT:
		case ColoringMode::REFLECTIVENESS:
		case ColoringMode::HITS:
		case ColoringMode::MISSES:
		case ColoringMode::INTENSITY:
			return Ogre::ColourValue(0, 0, 0, 1);
	}
}
}  // namespace ufomap_ros::rviz_plugins