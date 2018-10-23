#pragma once
#include <OpenEXR/ImfMultiPartInputFile.h>
#include <OpenEXR/ImfDeepScanLineInputPart.h>
#include <OpenEXR/ImfDeepFrameBuffer.h>

#include <OpenEXR/ImfPartType.h>
#include <OpenEXR/ImfChannelList.h>

#include <OpenEXR/ImfStringAttribute.h>
#include <OpenEXR/ImfIntAttribute.h>
#include <OpenEXR/ImfFloatAttribute.h>
#include <OpenEXR/ImfVecAttribute.h>
#include <OpenEXR/ImfBoxAttribute.h>
#include <OpenEXR/ImfStringVectorAttribute.h>
#include <OpenEXR/ImfTimeCodeAttribute.h>
#include <OpenEXR/ImfStandardAttributes.h>
#include <OpenEXR/ImfMatrixAttribute.h>

#include <OpenEXR/ImfFramesPerSecond.h>

#include <OpenEXR/ImathBox.h>

#include <iostream>
#include <string>
#include <vector>

#ifdef NDEBUG
#include <boost/shared_ptr.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
# include <boost/log/attributes/named_scope.hpp>
#include <boost/log/attributes/current_thread_id.hpp>

#endif // NDEBUG

#ifdef NDEBUG
static void init_log(void)
{
	boost::log::add_common_attributes();
}
#endif


using namespace std;

struct pointXYDeepRGBA
{
	float x;
	float y;
	float deepFront;
	float deepBack;
	float r;
	float g;
	float b;
	float a;

	void set2dCoord(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
	void setDeep(float front, float back)
	{
		deepFront = front;
		deepBack = back;
	}
	void setColor(float r, float g, float b, float a)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
};
class pointCloud
{
public:
	void addPoint(const pointXYDeepRGBA& p)
	{
		m_points.push_back(p);
	}
	void toGPU(){}
	void backToCPU(){}
	void setImageSize(int w, int h)
	{

	}
private:
	vector<pointXYDeepRGBA> m_points;
	vector< vector<int> > imageIndexes;//start of each pixel in m_points
};
class loadExrAsPointCloud
{
public:
	loadExrAsPointCloud(const string& filename):m_filename(filename){}
	void loadImage()
	{
		int partCounts = -1;
		m_file = new Imf::MultiPartInputFile(m_filename.c_str());
		for (int i = 0; i < m_file->parts(); i++)
		{
			const Imf::Header& header = m_file->header(i);
			if (i == 0)
				m_header = header;
			BOOST_LOG_TRIVIAL(info) << "DEEP file has view : " << header.hasView();

			partCounts = i;
		}
		BOOST_LOG_TRIVIAL(info) << "DEEP file has parts : " << partCounts;

		getChannelsInfo();
		m_part = new Imf::DeepScanLineInputPart(*m_file, partCounts);

		for (int y = 0; y < imageBox().size().y; y++)
		{

		}
	}

	void getChannelsInfo()
	{
		for (auto it = m_header.channels().begin(); it != m_header.channels().end(); it++)
		{
			const char* chanName = it.name();
			m_channels.push_back(string(chanName));
		}
		imageBox = m_header.dataWindow();
		m_pc.setImageSize(imageBox.size().x, imageBox.size().y);
	}

	void decodeLine(Imf::DeepScanLineInputPart& part, int y)
	{
		const int dataWid = m_header.dataWindow().size().x + 1;
		const int dataX = m_header.dataWindow().min.x;

		std::vector<unsigned> sampleCounts(dataWid, 0);

		Imf::DeepFrameBuffer frameBuffer;
		frameBuffer.insertSampleCountSlice(Imf::Slice(Imf::UINT,
			(char*)(&sampleCounts[0] - dataX),
			sizeof(unsigned), 0));

		part.rawPixelData(y, )
	}

private:
	string m_filename;
	Imf::MultiPartInputFile *m_file;
	Imf::DeepScanLineInputPart *m_part;
	Imf::Header m_header;
	pointCloud m_pc;
	Imath::Box2i imageBox;

	vector<string> m_channels;
};