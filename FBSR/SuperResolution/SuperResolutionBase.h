#pragma once

#include <opencv2/core/core.hpp>

#include "FrameBuffer/FrameBuffer.h"
#include "../FrameSource/FrameSource.h"
#include "../PropsStruct.h"

using namespace std;
using namespace cv;

class SuperResolutionBase
{
public:
	explicit SuperResolutionBase(int bufferSize = 8);

	bool SetFrameSource(const cv::Ptr<FrameSource>& frameSource);

	void SetProps(double alpha, double beta, double lambda, double P, int maxIterationCount);

	bool Reset();

	void NextFrame(OutputArray outputFrame);

protected:
	void Init(Ptr<FrameSource>& frameSource);

	void UpdateZAndA(Mat& mat, Mat& A, int x, int y, const vector<bool>& index, const vector<Mat>& mats, const int len) const;

	void MedianAndShift(const vector<Mat>& interp_previous_frames, const vector<vector<int>>& current_distances, const Size& new_size, Mat& mat, Mat& mat1) const;

	Mat FastGradientBackProject(const Mat& Xn, const Mat& Z, const Mat& A, const Mat& hpsf);

	Mat GradientRegulization(const Mat& Xn, const double p, const double alpha) const;

	Mat FastRobustSR(const vector<Mat>& interp_previous_frames, const vector<vector<int>>& current_distances, Mat hpsf);

	void Process(Ptr<FrameSource>& frameSource, OutputArray output);

	vector<Mat> NearestInterp2(const vector<Mat>& previousFrames, const vector<vector<double>>& currentDistances) const;

private:
	static vector<vector<double> > RegisterImages(vector<Mat>& frames);

	vector<vector<double> > GetRestDistance(const vector<vector<int>>& distances, int srFactor) const;

	void RoundAndScale(const vector<vector<double>>& distances, vector<vector<int>>& roundedDistance, int srFactor) const;

	void ModAndAddFactor(vector<vector<int>>& distances, int srFactor) const;

	vector<vector<double>> CollectParms(const vector<vector<double>>& distances, vector<vector<int>>& roundedDistance) const;

private:
	Ptr<FrameSource> frameSource;
	Ptr<FrameBuffer> frameBuffer;
	bool isFirstRun;
	int bufferSize;
	Size frameSize;

	int srFactor;
	int psfSize;
	double psfSigma;
	PropsStruct props;
};
