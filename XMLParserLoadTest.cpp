#include "gtest/gtest.h"
#include "XMLParser.h"
class XMLParserLoadTest : public testing::Test {
protected:
	static void SetUpTestCase()
	{
	}
	static void TearDownTestCase()
	{
	}
	virtual void SetUp()
	{
	}

	virtual void TearDown()
	{
	}
};
TEST_F(XMLParserLoadTest,Init)
{
	NodeDocument parser;
	bool result = parser.LoadFile("./SNOWMAN.xml");
	EXPECT_TRUE(result,true);

	parser.accept(NodeVisitor());
	NodeElement& OwnerElem = parser()("BUILDER")("SNOWMAN");
	OwnerElem["name"] = "Anna";
	NodeBase::iterator iter = OwnerElem.FindNode("Sister");
	for(; iter != OwnerElem.LastNode(); iter++)
	{
		NodeElement& SisterElem = (*iter)->ToElement();
		if(SisterElem["name"] == "Elsa"){
			NodeElement& BridgeMake = SisterElem("skill")("frozen")("bridge");
			BridgeMake = "Create FROZEN BRIDGE";
		}
	}
	printf("Save Start!\n");
	parser.SaveFile("./QOS.xml");
}