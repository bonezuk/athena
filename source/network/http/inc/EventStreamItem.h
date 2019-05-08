//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_NETWORK_HTTP_EVENTSTREAMITEM_H
#define __ORCUS_NETWORK_HTTP_EVENTSTREAMITEM_H
//-------------------------------------------------------------------------------------------

#include "network/http/inc/Unit.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace network
{
namespace http
{
//-------------------------------------------------------------------------------------------

class HTTP_EXPORT EventStreamItem
{	
	public:
		class Parser;

	public:
		EventStreamItem();
		EventStreamItem(const EventStreamItem &rhs);
		virtual ~EventStreamItem();
		
		static void parseLine(const QString& line, EventStreamItem& out);
		
		const EventStreamItem& operator = (const EventStreamItem& rhs);
		
		virtual bool isEmpty() const;
		
		virtual bool isId() const;
		virtual const int& id() const;
		virtual int& id();
		
		virtual bool isEvent() const;
		virtual const QString& event() const;
		virtual QString& event();
		
		virtual bool isData() const;
		virtual const QString& data() const;
		virtual QString& data();
		
		virtual QString toString() const;
	
	private:
		
		static Parser m_parser;
		
		int m_id;
		QString m_event;
		QString m_data;
		
		virtual void copy(const EventStreamItem& rhs);
};

//-------------------------------------------------------------------------------------------

class HTTP_EXPORT EventStreamItem::Parser
{
	public:
		Parser();
		virtual ~Parser();
		
		virtual bool parseNextLine(const QString& line);
		virtual EventStreamItem& item();
	
	private:
		EventStreamItem m_item;
};

//-------------------------------------------------------------------------------------------
} // namespace http
} // namespace network
} // namespace orcus
//-------------------------------------------------------------------------------------------

Q_DECLARE_METATYPE(orcus::network::http::EventStreamItem);

//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

