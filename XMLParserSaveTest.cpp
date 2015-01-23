#include "gtest/gtest.h"
#include "XMLParser.h"

class XMLParserSaveTest : public testing::Test {
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
TEST_F(XMLParserSaveTest,Init)
{
	NodeDocument parser;
	NodeDeclearation decl("1.0","UTF-8","");
	NodeComment Comment;
	parser.AddNode(decl.Clone());


	Comment+="Description \n";
	Comment+="Hello Do you Want Build Snow man\n";
	Comment+="Sister Sister Sister!\n\n";
	parser.AddNode(Comment.Clone());
	
	Comment.Clear();
	Comment+="Second Hello\n";
	Comment+="Working?\n\n";

	parser.AddNode(Comment.Clone());

	NodeElement RootElement("BUILDER");

	NodeElement Element("SNOWMAN");
	Element["name"] = "Elena";
	Element.AddNode("Sister")["name"] = "Elsa";
	Element.AddNode("Sister")["name"] = "Rosa";
	Element.AddNode("Sister")["name"] = "Elisa";

	NodeBase::iterator iter = Element.FindNode("Sister");
	for(; iter != Element.LastNode(); iter++)
	{
		NodeElement& Node = (*iter)->ToElement();
		Node.AddNode("skill").AddNode("frozen").AddNode("bridge").AddText("Create frozen Bridge");
		Node("skill").AddNode("fly").AddText("We Fly!!!");
	}
	RootElement.AddNode(Element.Clone());
	parser.AddNode(RootElement.Clone());
	parser.SaveFile("./SNOWMAN.xml");
}