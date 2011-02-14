/*

    This file is part of GeoGen.

    GeoGen is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    GeoGen is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with GeoGen.  If not, see <http://www.gnu.org/licenses/>.

*/

/** 
 * @file ggen_path.h GGen_Path represents a continuous linear sequence of GGen_Point objects.
 **/

#pragma once

#include "ggen_support.h"
#include "ggen_point.h"
#include "ggen_data_2d.h"
#include <list>

struct GGen_VelocityVector{
	double x;
	double y;
};

struct GGen_OutflowValues{
	double left;
	double right;
	double top;
	double bottom;
};

/**
 * GGen_Path represents a continuous linear sequence of GGen_Point objects.
 **/
class GGen_ErosionSimulator{
	protected:
		GGen_Size width;
		GGen_Size height;
		GGen_TotalSize length;
		double deltaT;
		double pipeLength;
		double graviationalAcceleration;
		double sedimentCapacityConstant;
		double dissolvingConstant;
		double depositionConstant;
		double minimumComputedSurfaceTilt;
	public:
		GGen_ErosionSimulator(GGen_Size width, GGen_Size height);
		double* ImportHeightMap(GGen_Data_2D& heightMap);
		double GetSurfaceTilt(double* heightMap, GGen_Coord x, GGen_Coord y );
		void ExportHeightMap(double* heightMap, GGen_Data_2D& ggenHeightMap);
		void ExportVelocityVectorMap(GGen_VelocityVector* velocityVectorMap, GGen_Data_2D& ggenHeightMap);
		void Erode(GGen_Data_2D& ggenHeightMap);
		void ApplyWaterSources(double* waterMap);
		void ApplyEvaporation(double* waterMap);
		void ApplyFlowSimulation(double* heightMap, double* waterMap, double* sedimentMap, GGen_OutflowValues* outflowFluxMap, GGen_VelocityVector* velocityVectorMap );
		void ApplyErosion(double* heightMap, GGen_VelocityVector* velocityVectorMap, double* sedimentMap);
};