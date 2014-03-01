import xml.etree.ElementTree as ET

tree = ET.ElementTree(ET.Element("root",{"fuck":"python","much":"syntax"}))
root = tree.getroot()
root.append(ET.Element("el",{"I":"heb","no":"idea","what":"I", "am":"doing"}))

tree.write("test.xml");
