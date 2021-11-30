#include "clipper.hpp"
#include <iostream>
using namespace ClipperLib;
 
int main()
{
	Paths subj(2), clip(1), solution;
	
	//define outer blue 'subject' polygon
	subj[0] << 
	IntPoint(180,200) << IntPoint(260,200) <<
	IntPoint(260,150) << IntPoint(180,150);
	
	//define subject's inner triangular 'hole' (with reverse orientation)
	subj[1] << 
	IntPoint(215,160) << IntPoint(230,190) << IntPoint(200,190);
	
	//define orange 'clipping' polygon
	clip[0] << 
	IntPoint(190,210) << IntPoint(240,210) << 
	IntPoint(240,130) << IntPoint(190,130);
	
	//perform intersection ...
	Clipper c;
	c.AddPaths(subj, ptSubject, true);
	c.AddPaths(clip, ptClip, true);
	c.Execute(ctUnion, solution, pftNonZero, pftNonZero);
	
	std::cout << solution << std::endl;
}