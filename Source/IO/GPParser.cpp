#include "GPParser.h"
#include <juce_core/juce_core.h>

// ROOT NODE IS GPIF

juce::String GPParser::GetGPVersion(std::unique_ptr<juce::XmlElement>& root) {
    // The value of the node is considered a child
    const juce::XmlElement* versionNode =
        root->getChildByName("GPVersion")->getFirstChildElement();
    return versionNode->getText();
}

std::vector<std::unique_ptr<juce::XmlElement>>
GPParser::GetMasterBars(std::unique_ptr<juce::XmlElement>& root) {
    std::vector<std::unique_ptr<juce::XmlElement>> masterBars;
    const juce::XmlElement* masterBarsNode = root->getChildByName("MasterBars");

    int id = 1;
    for (auto child : masterBarsNode->getChildIterator()) {
        if (child->hasTagName("MasterBar")) {
            juce::XmlElement* truncMasterBar =
                new juce::XmlElement("MasterBar");
            juce::XmlElement* masterBarID = new juce::XmlElement("ID");
            masterBarID->addTextElement(juce::String(id));
            truncMasterBar->addChildElement(masterBarID);
            juce::XmlElement* barsNode = new juce::XmlElement("Bars");
            barsNode->addTextElement(
                child->getChildByName("Bars")->getAllSubText());
            truncMasterBar->addChildElement(barsNode);
            masterBars.push_back(juce::rawToUniquePtr(truncMasterBar));
            printf("Added master bar %d\n", id);
            id++;
        }
    }
    return masterBars;
}
void GPParser::FindTheWeirdElement(std::unique_ptr<juce::XmlElement>& root) {
    const juce::XmlElement* masterBarsNode = root->getChildByName("MasterBars");

    int id = 1;
    for (auto child : masterBarsNode->getChildIterator()) {
        for (auto subchild : child->getChildIterator()) {
            if (subchild->getChildByName("Bars") != nullptr) {
                printf("Weird node: %s parenting bars in masterbar %d\n",
                       subchild->getTagName().toStdString().c_str(), id);
            }
        }
        id++;
    }
    delete masterBarsNode;
}
