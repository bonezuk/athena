#include "network/http/inc/EventStreamItem.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace network
{
namespace http
{
//-------------------------------------------------------------------------------------------

EventStreamItem::Parser EventStreamItem::m_parser;

//-------------------------------------------------------------------------------------------

EventStreamItem::EventStreamItem() : m_id(-1),
	m_event(),
	m_data()
{}

//-------------------------------------------------------------------------------------------

EventStreamItem::EventStreamItem(const EventStreamItem &rhs) : m_id(-1),
	m_event(),
	m_data()
{
	copy(rhs);
}

//-------------------------------------------------------------------------------------------

EventStreamItem::~EventStreamItem()
{}

//-------------------------------------------------------------------------------------------

const EventStreamItem& EventStreamItem::operator = (const EventStreamItem& rhs)
{
	if(this != &rhs)
	{
		copy(rhs);
	}
	return *this;
}

//-------------------------------------------------------------------------------------------

void EventStreamItem::copy(const EventStreamItem& rhs)
{
	m_id = rhs.m_id;
	m_event = rhs.m_event;
	m_data = rhs.m_data;
}

//-------------------------------------------------------------------------------------------

bool EventStreamItem::isEmpty() const
{
	return (isId() || isEvent() || isData());
}

//-------------------------------------------------------------------------------------------

bool EventStreamItem::isId() const
{
	return (m_id != -1);
}

//-------------------------------------------------------------------------------------------

const int& EventStreamItem::id() const
{
	return m_id;
}

//-------------------------------------------------------------------------------------------

int& EventStreamItem::id()
{
	return m_id;
}

//-------------------------------------------------------------------------------------------

bool EventStreamItem::isEvent() const
{
	return (!m_event.isEmpty());
}

//-------------------------------------------------------------------------------------------

const QString& EventStreamItem::event() const
{
	return m_event;
}

//-------------------------------------------------------------------------------------------

QString& EventStreamItem::event()
{
	return m_event;
}

//-------------------------------------------------------------------------------------------

bool EventStreamItem::isData() const
{
	return (!m_data.isEmpty());
}

//-------------------------------------------------------------------------------------------

const QString& EventStreamItem::data() const
{
	return m_data;
}

//-------------------------------------------------------------------------------------------

QString& EventStreamItem::data()
{
	return m_data;
}

//-------------------------------------------------------------------------------------------

QString EventStreamItem::toString() const
{
	return QString(); // TODO
}

//-------------------------------------------------------------------------------------------

void EventStreamItem::parseLine(const QString& line, EventStreamItem& out)
{
	if(m_parser.parseNextLine(line))
	{
		out = m_parser.item();
		m_parser.item() = EventStreamItem();
	}
}

//-------------------------------------------------------------------------------------------
// EventStreamItem::Parser
//-------------------------------------------------------------------------------------------

EventStreamItem::Parser::Parser() : m_item()
{}

//-------------------------------------------------------------------------------------------

EventStreamItem::Parser::~Parser()
{}

//-------------------------------------------------------------------------------------------

bool EventStreamItem::Parser::parseNextLine(const QString& line)
{
	bool isComplete = false;
	
	if(line.left(4) == "id: ")
	{
		bool ok = false;
		int id = line.mid(4).toInt(&ok);
		if(ok)
		{
			m_item.id() = id;
		}
	}
	else if(line.left(7) == "event: ")
	{
		m_item.event() = line.mid(7);
	}
	else if(line.left(6) == "data: ")
	{
		if(m_item.isData())
		{
			m_item.data() += "\r\n";
		}
		m_item.data() += line.mid(6);
	}
	else if(line.trimmed().isEmpty())
	{
		isComplete = true;
	}
	return isComplete;
}

//-------------------------------------------------------------------------------------------

EventStreamItem& EventStreamItem::Parser::item()
{
	return m_item;
}

//-------------------------------------------------------------------------------------------
} // namespace http
} // namespace network
} // namespace orcus
//-------------------------------------------------------------------------------------------
