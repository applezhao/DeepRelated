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

#include "ImageStruct.h"
#include "PointCloudStruct.h"

using namespace std;


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

		/*for (int y = 0; y < imageBox().size().y; y++)
		{

		}*/
	}

	void getChannelsInfo()
	{
		for (auto it = m_header.channels().begin(); it != m_header.channels().end(); it++)
		{
			const char* chanName = it.name();
			m_channels.push_back(string(chanName));
			BOOST_LOG_TRIVIAL(info) << "DEEP file has channel : " << string(chanName);
		}
		m_imageDataBox = m_header.dataWindow();
		m_imageDisplayBox = m_header.displayWindow();
		m_pc.setImageSize(m_imageDataBox.size().x, m_imageDataBox.size().y);
	}

	void decodeLine(Imf::DeepScanLineInputPart& part, int y)
	{
		const int dataWid = m_header.dataWindow().size().x + 1;
		const int dataX = m_header.dataWindow().min.x;

		std::vector<unsigned> sampleCounts(dataWid, 0);
		unsigned chanCount = m_channels.size();

		Imf::DeepFrameBuffer frameBuffer;
		frameBuffer.insertSampleCountSlice(Imf::Slice(Imf::UINT,
			(char*)(&sampleCounts[0] - dataX),
			sizeof(unsigned), 0));

		std::vector<std::vector<const float*> > samples;
		samples.reserve(chanCount);

		for (int i = 0; i < chanCount; i++)
		{
			samples.push_back(vector<const float*>(dataWid));
			frameBuffer.insert(m_channels[i],
				Imf::DeepSlice(
					Imf::FLOAT, 
					(char*)(&samples.back()[0]-dataX),
					sizeof(const float*),
					0,
					sizeof(float)*chanCount
				));
		}

		std::vector<float> data;
		std::vector<char> deepScanlineBuffer;
		{
			//thread lock
			Imf::Int64 pixSize;
			part.rawPixelData(y, nullptr, pixSize);

			deepScanlineBuffer.resize(pixSize);

			part.rawPixelData(y, &deepScanlineBuffer[0], pixSize);

			BOOST_LOG_TRIVIAL(info) << "DEEP file LINE " << y <<" with data size "<<pixSize <<" == "<<deepScanlineBuffer.size();
		}

		// Read the sample counts from the data buffer.
		part.readPixelSampleCounts(&deepScanlineBuffer[0], frameBuffer, y, y);

		unsigned sampleCountSum = accumulate(sampleCounts.begin(), sampleCounts.end(), 0);

		data.resize(sampleCountSum*chanCount, 0);

		for (int i = 0; i < chanCount; i++)
		{
			const float* ptr = &data[0] + i;
			for (int x = 0; x < dataWid; x++)
			{
				samples[i][x] = ptr;
				ptr += chanCount*sampleCounts[x];
			}
		}

		part.readPixels(&deepScanlineBuffer[0], frameBuffer, y, y);

		for (int x = 0; x < dataWid; x++)
		{
			int sampleCount = sampleCounts[x];
			pixelDeepRGBA pix(sampleCount);
			for (int i = 0; i < sampleCount; i++)
			{
				pix[i].
			}
		}
	}

private:
	string m_filename;
	Imf::MultiPartInputFile *m_file;
	Imf::DeepScanLineInputPart *m_part;
	Imf::Header m_header;
	pointCloud m_pc;
	imagePlane m_ip;
	Imath::Box2i m_imageDataBox;
	Imath::Box2i m_imageDisplayBox;

	vector<string> m_channels;

	//Imf::DeepScanlineBuffer m_deepScanlineBuffer;
};