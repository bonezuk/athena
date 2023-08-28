#include <stdio.h>
#include <stdlib.h>

#include <QImage>
#include <QColor>
#include <QApplication>
#include <QFileInfo>
#include <QDir>

#include "common/inc/BIOStream.h"

#include "gtest/gtest.h"

using namespace omega;

//-------------------------------------------------------------------------------------------

int clampRGB(int c)
{
	if(c < 0)
	{
		c = 0;
	}
	if(c > 255)
	{
		c = 255;
	}
	return c;
}

//-------------------------------------------------------------------------------------------

QColor fromYUV422_YUYV_toRGBA(int y, int Cb, int Cr)
{
	int r, g, b;
	
	r = y + (1.402 * (Cr - 128));
	r = clampRGB(r);
	g = y - (0.344 * (Cb - 128)) - (0.714 * (Cr - 128));
	g = clampRGB(g);
	b = y + (1.772 * (Cb - 128));
	b = clampRGB(b);
	return QColor(r, g, b);
}

//-------------------------------------------------------------------------------------------

QImage convertYUV422_YUYV(const QByteArray& yuvRaw, int width, int height)
{
	int x, y, pos, y1, y2, u, v, r1, g1, b1, r2, g2, b2;
	const unsigned char *raw;
	QColor c1, c2;
	QImage img(width, height, QImage::Format_ARGB32);
	
	raw = reinterpret_cast<const unsigned char *>(yuvRaw.constData());
	
	for(pos = 0, y = 0; y < height; y++)
	{
		for(x = 0; x < width; x+=2, pos+=4)
		{
			if(pos + 3 < yuvRaw.size())
			{
				y1 = static_cast<int>(raw[pos + 0]);
				u  = static_cast<int>(raw[pos + 1]);
				y2 = static_cast<int>(raw[pos + 2]);
				v  = static_cast<int>(raw[pos + 3]);
				c1 = fromYUV422_YUYV_toRGBA(y1, u, v);
				c2 = fromYUV422_YUYV_toRGBA(y2, u, v);
			}
			else
			{
				c1 = QColor(Qt::black);
				c2 = QColor(Qt::black);
			}
			img.setPixelColor(x + 0, y, c1);
			img.setPixelColor(x + 1, y, c2);
		}
	}
	return img;
}

//-------------------------------------------------------------------------------------------

/*
int main(int argc, char *argv[])
{
	int res = -1;

	if(argc < 3)
	{
		fprintf(stdout, "yuv2png <raw Y422 file> <output.png>\n");
		return res;
	}

	QString pluginDir;
	QFileInfo appFile(argv[0]);
	QDir d = appFile.absolutePath();
	d.cd("plugins");
	pluginDir = d.absolutePath();
	pluginDir = QDir::toNativeSeparators(pluginDir);
	QApplication::setLibraryPaths(QStringList(pluginDir));
	
	BIOStream yuvInput(e_BIOStream_FileRead);
	
	if(yuvInput.open(QString::fromUtf8(argv[1])))
	{
		QByteArray yuvArray(yuvInput.size(), '\0');
		
		if(yuvInput.read(yuvArray.data(), yuvArray.size()) == yuvInput.size())
		{
			QImage img(convertYUV422_YUYV(yuvArray, 1920, 1080));
			
			if(img.save(QString(argv[2])))
			{
				fprintf(stdout, "Success convertion! Image written to '%s'\n", argv[2]);
				res = 0;
			}
			else
			{
				fprintf(stdout, "Failed to write image '%s'\n", argv[2]);
			}
		}
		else
		{
			fprintf(stdout, "Failed to read in full contents of yuv422 file '%s'\n", argv[1]);
		}
	}
	else
	{
		fprintf(stdout, "Failed to open raw yuv422 file '%s'\n", argv[1]);
	}
	
	return res;
}
*/

//-------------------------------------------------------------------------------------------


void convert(const QString& inName, const QString& outName, int w, int h)
{
	common::BIOStream yuvInput(common::e_BIOStream_FileRead);
	
	if(yuvInput.open(inName))
	{
		QByteArray yuvArray(yuvInput.size(), '\0');
		
		if(yuvInput.read(yuvArray.data(), yuvArray.size()) == yuvInput.size())
		{
			QImage img(convertYUV422_YUYV(yuvArray, w, h));
			
			if(img.save(outName))
			{
				fprintf(stdout, "Success convertion! Image written to '%s'\n", outName.toUtf8().constData());
			}
			else
			{
				fprintf(stdout, "Failed to write image '%s'\n", outName.toUtf8().constData());
			}
		}
		else
		{
			fprintf(stdout, "Failed to read in full contents of yuv422 file '%s'\n", inName.toUtf8().constData());
		}
	}
	else
	{
		fprintf(stdout, "Failed to open raw yuv422 file '%s'\n", inName.toUtf8().constData());
	}
}

void convert(const char *inName, const char *outName, int width, int height)
{
	common::BIOStream yuvInput(common::e_BIOStream_FileRead);
	
	if(yuvInput.open(QString::fromUtf8(inName)))
	{
		QByteArray yuvArray(yuvInput.size(), '\0');
		
		if(yuvInput.read(yuvArray.data(), yuvArray.size()) == yuvInput.size())
		{
			QImage img(convertYUV422_YUYV(yuvArray, width, height));
			
			if(img.save(QString::fromUtf8(outName)))
			{
				fprintf(stdout, "Success convertion! Image written to '%s'\n", outName);
			}
			else
			{
				fprintf(stdout, "Failed to write image '%s'\n", outName);
			}
		}
		else
		{
			fprintf(stdout, "Failed to read in full contents of yuv422 file '%s'\n", inName);
		}
	}
	else
	{
		fprintf(stdout, "Failed to open raw yuv422 file '%s'\n", inName);
	}
}


TEST(YUV2PNG, run)
{
	const QString baseDir = "/Users/bonez/Development/ActiveSilicon/AP30/firmware_tests/phil_4/1080p30_offset_1";
	const char *fileNames[] = {
		"phil_fcc_async_rec2_1.yuv",
		"phil_fcc_async_rec_1.yuv",
		"phil_fcc_sync_1.yuv",
		NULL};
	
	for(int i = 0; fileNames[i] != NULL; i++)
	{
		QString inName = baseDir + "/";
		inName += fileNames[i];
		QString outName = inName.left(inName.size() - 3) + "png";
		convert(inName.toUtf8().constData(), outName.toUtf8().constData(), 1920, 1080);
	}
}


