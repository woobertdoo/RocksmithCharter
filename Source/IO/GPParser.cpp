#include "GPParser.h"
#include <bits/stdc++.h>
#include <juce_core/juce_core.h>
#include <memory>
#include <sstream>

// ROOT NODE IS GPIF
//

/*
 * <GPIF>
 *   -- Song Info (Title, etc.)
 *   <Score>
 *      <Title></Title>
 *      <Artist></Artist>
 *      <Album></Album>
 *   </Score>
 *   -- Track Info (name of track)
 *   -- Instrument Info (tuning, number of strings, etc)
 *   <Tracks>
 *     <Track id="x">
 *      <Name><![CDATA[trackname]]></Name>
 *      <Staves>
 *          <Staff>
 *              <Properties>
 *                  <Property name="CapoFret">
 *                      <Fret>x</Fret>
 *                  </Property>
 *                  <Property name="Tuning">
 *                      <!-- Pitches start at 0 = G1 -->
 *                      <Pitches>x x x x x x</Pitches>
 *                  </Property>
 *              </Properties>
 *          </Staff>
 *      </Staves>
 *     </Track>
 *     .
 *     .
 *     .
 *   </Tracks>
 *   <MasterBars>
 *      -- Each master bar refers to ONE BAR.
 *      -- Each bar in the <Bars> tag is for a different TRACK (guitar 1, guitar
 * 2, bass, etc.).
 *      -- If the file has some sort of piano/keyboard, there will be two
 * different BAR TAGS for that track, one for the right hand and one for the
 * left hand.
 *
 *       <MasterBar>
 *          <Time>n/d</Time>
 *          <Repeat start="true/false" end="true/false" count=x/>
 *          <Bars>a b c ... </Bars>
 *          <!-- Which alternate ending of a repeat this uses -->
 *          <AlternateEndings>x</AlternateEndings>
 *      </MasterBar>
 *      .
 *      .
 *      .
 *   </MasterBars>
 *   <Bars>
 *      <Bar id="x">
 *          <!-- Only first voice is important -->
 *          <Voices> n -1 -1 -1 </Voices>
 *      </Bar>
 *      .
 *      .
 *      .
 *   </Bars>
 *
 *   <Voices>
 *      <Voice id="x">
 *      <!-- Note: Beats can repeat (0 1 2 1 2 3, etc.) -->
 *          <Beats>a b c d ... </Beats>
 *      </Voice>
 *   </Voices>
 *
 *  <Beats>
 *      <Beat id="x">
 *          <Rhythm ref="x" />
 *          <Notes>a b c ...</Notes>
 *          <!-- Unsure about arpeggio support atm -->
 *          <Arpeggio>Down/Up</Arpeggio>
 *      </Beat>
 *  </Beats>
 *
 *  <Notes>
 *      <Note id="x">
 *
 *          <Tie origin="true/false" destination="true/false"/>
 *
 *          <Property name="Fret">
 *              <Fret>x</Fret>
 *          </Property>
 *
 *          <!-- Slide Flags
 *
 *          1 = Shift (No LinkNext)
 *          2 = Legato (LinkNext)
 *          4 = Slide Out Down
 *          8 = Slide Out Up
 *          16 = Slide In From Below
 *          32 = Slide In From Above
 *
 *          -->
 *          <Property name="Slide">
 *          <Flags>x</Flags
 *          </Property>
 *
 *          <Property name="String">
 *              <String>x</String>
 *          </Property>
 *      </Note>
 *  </Notes>
 *
 *  <Rhythms>
 *      <Rhythm id="x">
 *          <NoteValue>Whole/Half/Quarter/Eighth/16th/...</NoteValue>
 *          <PrimaryTuplet num="n" den="d"/>
 *          <AugmentationDot count="x"/>
 *      </Rhythm>
 *  </Rhythms>
 * </GPIF>
 *
 *
 * */

juce::String GPParser::GetGPVersion() {
    // The value of the node is considered a child
    const juce::XmlElement* versionNode =
        rootNode->getChildByName("GPVersion")->getFirstChildElement();
    return versionNode->getText();
}

void GPParser::SetRoot(std::unique_ptr<juce::XmlElement>& root) {
    *rootNode = *root;
}

XmlVector GPParser::GetMasterBars() {
    XmlVector masterBars;
    const juce::XmlElement* masterBarsNode =
        rootNode->getChildByName("MasterBars");

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
            id++;
        }

        return masterBars;
    }

    XmlVector GPParser::GetBars(std::unique_ptr<juce::XmlElement> & masterBar) {
        XmlVector bars;
        const juce::XmlElement* barsNode = rootNode->getChildByName("Bars");
        const std::string barsToFind =
            masterBar->getChildByName("Bars")->getAllSubText().toStdString();
        std::vector<int> barIds = splitIds(barsToFind);

        for (auto id : barIds) {
            const juce::XmlElement* foundBar =
                barsNode->getChildByAttribute("id", juce::String(id));
            if (!barsNode) {
                printf("Couldn't find bar with id %d\n", id);
                continue;
            }

            const juce::XmlElement* foundVoices =
                foundBar->getChildByName("Voices");

            if (!foundVoices) {
                printf(
                    "Something went very wrong, this bar (%d) has no voices!\n",
                    id);
                continue;
            }

            juce::XmlElement* truncBar = new juce::XmlElement("Bar");
            juce::XmlElement* voices = new juce::XmlElement("Voices");
            juce::String firstVoiceId =
                foundVoices->getAllSubText().upToFirstOccurrenceOf(" ", false,
                                                                   false);
            voices->addTextElement(firstVoiceId);
            truncBar->setAttribute("id", id);
            truncBar->addChildElement(voices);
            bars.push_back(juce::rawToUniquePtr(truncBar));
        }

        return bars;
    }

    void GPParser::FindTheWeirdElement() {
        const juce::XmlElement* masterBarsNode =
            rootNode->getChildByName("MasterBars");

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

    /* For splitting children IDs to search for them
     *
     * i.e. splits a node <Nodes>1 2 3 4</Nodes>
     * into a vector {1, 2, 3, 4} to search for <Node id="1"> etc..
     * */
    std::vector<int> GPParser::splitIds(std::string ids) {
        std::vector<int> splitVector;
        std::stringstream ss(ids);

        std::string t;

        char del = ' ';

        while (getline(ss, t, del)) {
            int t_int = std::stoi(t);
            splitVector.push_back(t_int);
        }

        return splitVector;
    }
