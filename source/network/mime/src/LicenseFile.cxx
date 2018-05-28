#include "network/mime/inc/LicenseFile.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace network
{
//-------------------------------------------------------------------------------------------

LicenseFile::LicenseFile() : m_licenseList()
{}

//-------------------------------------------------------------------------------------------

LicenseFile::~LicenseFile()
{}

//-------------------------------------------------------------------------------------------

int LicenseFile::size() const
{
	return m_licenseList.size();
}

//-------------------------------------------------------------------------------------------

const QByteArray& LicenseFile::block(int idx) const
{
	return m_licenseList.at(idx);
}

//-------------------------------------------------------------------------------------------

bool LicenseFile::read(const QString& name)
{
	common::BIOStream lFile(common::e_BIOStream_FileRead);
	bool res = false;
	
	if(lFile.open(name))
	{
		QByteArray lData(lFile.size(),'\0');
		
		if(lFile.read(lData.data(),lData.size())==lData.size())
		{
			if(read(lData))
			{
				res = true;
			}
		}
		lFile.close();
	}
	return res;
}

//-------------------------------------------------------------------------------------------

bool LicenseFile::read(const QByteArray& lData)
{
	const tchar *x = lData.constData();
	bool res = false;
	
	m_licenseList.clear();
	if(x!=0 && lData.size()>=10)
	{
		tuint32 hdrID = static_cast<tuint32>(common::memory::intFromMemory(&x[0]));
		tint32 totalSize = common::memory::intFromMemory(&x[4]);
		tint noBlocks = static_cast<tint>(common::memory::shortFromMemory(&x[8]));
		if(hdrID==LICENSE_HEADER_ID && totalSize==(lData.length()-8))
		{
			tint i,pos = 10;
			
			res = true;
			for(i=0;i<noBlocks && res;i++)
			{
				if(lData.size()>=(pos + 8))
				{
					tuint32 blockID = static_cast<tuint32>(common::memory::intFromMemory(&x[pos]));
					tint blockSize = static_cast<tint>(common::memory::intFromMemory(&x[pos+4]));
					
					pos += 8;
					if(blockID==LICENSE_BLOCK_ID && lData.size()>=(pos+blockSize))
					{
						QByteArray bArr(&x[pos],blockSize);
						m_licenseList.append(bArr);
						pos += blockSize;
					}
					else
					{
						res = false;
					}
				}
				else
				{
					res = false;
				}
			}
		}
	}
	return res;
}

//-------------------------------------------------------------------------------------------

bool LicenseFile::write(const QString& name,const QVector<QByteArray>& licList)
{
	common::BIOStream lFile(common::e_BIOStream_FileCreate | common::e_BIOStream_FileWrite);
	bool res = false;
	
	if(lFile.open(name))
	{
		QByteArray lData;
		
		if(write(lData,licList))
		{
			if(lFile.write(lData.constData(),lData.size())==lData.size())
			{
				res = true;
			}
		}
		lFile.close();
	}
	return res;
}

//-------------------------------------------------------------------------------------------

bool LicenseFile::write(QByteArray& lData,const QVector<QByteArray>& licList)
{
	tint i;
	tchar mem[10];
	
	lData.clear();
	
	common::memory::intToMemory(LICENSE_HEADER_ID,mem);
	common::memory::intToMemory(0,&mem[4]);
	common::memory::shortToMemory(static_cast<tint16>(licList.size()),&mem[8]);
	lData.append(mem,10);
	
	for(i=0;i<licList.size();i++)
	{
		const QByteArray& cArr = licList.at(i);
		common::memory::intToMemory(LICENSE_BLOCK_ID,mem);
		common::memory::intToMemory(cArr.size(),&mem[4]);
		lData.append(mem,8);
		lData.append(cArr);
	}
	{
		tchar *x = lData.data();
		common::memory::intToMemory(lData.size()-8,&x[4]);
	}
	return true;
}

//-------------------------------------------------------------------------------------------
} // namespace network
} // namespace orcus
//-------------------------------------------------------------------------------------------
