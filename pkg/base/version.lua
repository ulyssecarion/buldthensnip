--[[
    This file is part of Ice Lua Components.

    Ice Lua Components is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Ice Lua Components is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Ice Lua Components.  If not, see <http://www.gnu.org/licenses/>.
]]

VERSION_ENGINE = {
	cmp={0,0,0,0,17},
	num=17,
	str="0.0-17",
}

VERSION_BUGS = {
{intro=nil, fix=1, msg="PMF models have the wrong Z value when close to the screen edges, and can be seen through walls"},
{intro=nil, fix=1, msg="PMF models are sometimes saved with garbage following the name"},
{intro=nil, fix=1, msg="Client does pathname security checks for non-clsave files"},
{intro=nil, fix=3, msg="common.img_fill not implemented (this wrapper will be somewhat slow)"},
{intro=nil, fix=4, msg="Sound is not supported"},
{intro=4, fix=5, msg="Dedicated server build was broken"},
{intro=5, fix=6, msg="CRASHES ON CHANNEL WRAPAROUND - PLEASE UPDATE TO 0.0-6!"},
{intro=nil, fix=7, msg="Camera roll / camera_point_sky not implemented - drunken cam will not roll properly"},
{intro=nil, fix=8, msg="Renderer uses double-rect approximation of cube instead of using trapezia"},
{intro=nil, fix=9, msg="Mouse warping not implemented"},
{intro=nil, fix=10, msg="Immediate ceiling isn't drawn"},
{intro=nil, fix=11, msg="Blocks appear inverted in common cases"},
{intro=nil, fix=12, msg="TGA loader prone to crashing on unsanitised data"},
{intro=nil, fix=13, msg="No per-face shading"},
{intro=nil, fix=14, msg="Color conversion functions are using hacky Lua code"},
{intro=nil, fix=16, msg="Per-face shading is only preliminary"},
{intro=nil, fix=17, msg="OpenMP not supported"},
}
