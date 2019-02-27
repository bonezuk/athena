//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_VERSIONER_DICTIONARYXMLMAP_H
#define __ORCUS_VERSIONER_DICTIONARYXMLMAP_H
//-------------------------------------------------------------------------------------------

#include <QMap>
#include <QString>

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace versioner
{
//-------------------------------------------------------------------------------------------

class DictionaryXMLMap
{
	public:
		static QMap<QString,QString> read(const QString& fileName);
		static bool write(const QString& fileName,const QMap<QString,QString>& dict);
};

//-------------------------------------------------------------------------------------------
} // namespace versioner
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

