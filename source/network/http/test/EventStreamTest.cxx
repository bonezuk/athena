#include "gtest/gtest.h"
#include "network/http/inc/EventStreamItem.h"

using namespace orcus::network::http;

//-------------------------------------------------------------------------------------------

TEST(EventStreamItem, parseEventsAndTransmit)
{
	QString lineA = "id: 42";
	QString lineB = "event: foo";
	QString lineC = "data: This is a piece of data";
	QString lineD = "";
	QString lineE = "id: 43";
	QString lineF = "event: bar";
	QString lineG = "data: This is a piece of the test";
	QString lineH = "data:  with multiple lines.";
	QString lineI = "  ";

	EventStreamItem a;
	
	EventStreamItem::parseLine(lineA, a);
	EXPECT_TRUE(a.isEmpty());
	EventStreamItem::parseLine(lineB, a);
	EXPECT_TRUE(a.isEmpty());
	EventStreamItem::parseLine(lineC, a);
	EXPECT_TRUE(a.isEmpty());
	EventStreamItem::parseLine(lineD, a);
	EXPECT_FALSE(a.isEmpty());
	
	EXPECT_TRUE(a.isId());
	EXPECT_EQ(a.id(), 42);
	EXPECT_TRUE(a.isEvent());
	EXPECT_EQ(a.event(), "foo");
	EXPECT_TRUE(a.isData());
	EXPECT_EQ(a.data(), "This is a piece of data");
	
	QString aExpectStr = "id: 42\r\nevent: foo\r\ndata: This is a piece of data\r\n\r\n";
	EXPECT_EQ(a.toString(), aExpectStr);
	
	EventStreamItem b;
	
	EventStreamItem::parseLine(lineE, b);
	EXPECT_TRUE(b.isEmpty());
	EventStreamItem::parseLine(lineF, b);
	EXPECT_TRUE(b.isEmpty());
	EventStreamItem::parseLine(lineG, b);
	EXPECT_TRUE(b.isEmpty());
	EventStreamItem::parseLine(lineH, b);
	EXPECT_TRUE(b.isEmpty());
	EventStreamItem::parseLine(lineI, b);
	EXPECT_FALSE(b.isEmpty());

	EXPECT_TRUE(b.isId());
	EXPECT_EQ(b.id(), 43);
	EXPECT_TRUE(b.isEvent());
	EXPECT_EQ(b.event(), "bar");
	EXPECT_TRUE(a.isData());
	EXPECT_EQ(b.data(), "This is a piece of the test\r\n with multiple lines.");
	
	QString bExpectStr = "id: 43\r\nevent: bar\r\ndata: This is a piece of the test\r\ndata:  with multiple lines.\r\n\r\n";
	EXPECT_EQ(b.toString(), bExpectStr);
}

//-------------------------------------------------------------------------------------------
