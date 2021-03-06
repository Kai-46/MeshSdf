#define _CRT_SECURE_NO_WARNINGS

#include "MeshLoader.h"
#include "SignedDistance.h"
#include "Visualize.h"
#include <boost/gil/extension/io/pnm.hpp>
#include <iostream>

using namespace std;
using namespace MeshSdf;

// NOTE: argv[1] should contain path to mesh in obj format.
int main(int argc, char* argv[])
{
	if (argc <= 1) {
		std::cout << "Usage: Demo path/to/mesh z_value x_pixels";
		exit(-1);
	} 

	std::cout << "Loading mesh: " << argv[1] << '\n';
	auto mesh = LoadObj(argv[1]);

	std::cout << "Converting mesh to sdf...\n";
	SignedDistance df{ mesh };
	//UnsignedDistance df{ mesh.verts, mesh.tris };

	auto const DepthPercent = std::stoi(argv[2]);
	auto boundBox = Scale(BoundingBox(mesh), 1.3f);
	auto aspectRatio = (boundBox.ymax - boundBox.ymin) / (boundBox.xmax - boundBox.xmin);
	auto const ImWidth = std::stoi(argv[3]);
	auto const ImHeight = static_cast<int>(aspectRatio * ImWidth);
	std::cout << "Computing SDF slice for z=" << DepthPercent << ", image size=" << ImHeight << "x" << ImWidth << '\n';
	auto imgSlice = ColorCodedSlice(df, ImHeight, ImWidth, boundBox, DepthPercent);

	std::cout << "Writing to file...\n";
	write_view("./sdf_slice.pnm", const_view(imgSlice), boost::gil::pnm_tag{});
}
